/*
 * The GI1MIC $20 Dollar DSP Audio Filter
 * 
 * Visit https://gi1mic.github.io/ for information on how to use this code.
 * 
 * Email via GI1MIC at outlook.com
 * 
 * Modify "settings.h" to suit your needs 
 * 
 *  
 * This project is intended to be used with an amplified speaker to provide DSP audio filtering
 * mainly for Radio Amateurs or shortwave radio listeners.
 * 
 * The project uses a rotary encoder plus two push buttons (one built into the encoder) for  
 * control plus a SPI display to show a GUI.
 * 
 * 
 * Version 1.0 
 *  Initial release based on my earlier FT-817 Version 2.2 code
 *  but stripped down to a simplified design making it easier to modify
 *  
 */


#include <SerialFlash.h>
#include <Encoder.h>
#include <Bounce.h>
#include "settings.h"
#include "dynamicFilters.h"
#include "dspfilter.h"
#include "display.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define PRESS_QUICK  300
#define PRESS_MEDIUM 550
#define PRESS_LONG   600
#define ENCODER_FREQ_SHIFT 20

short int     filterType;
double        freqLow;
double        freqHigh;
const char*   freqDesc;

unsigned int selected_mode = 0;   // 0 = Center point, 1 = Hi, 2 = Low


Encoder knob(ENCODER1a, ENCODER1b);
long rotaryOldPosition  = 0;
int rotaryDirection = 0;

Bounce knob_select = Bounce(ENCODER1sw, 15); // debounce the filter switching pin

unsigned long sw_startTime;
unsigned long sw_duration = 0;
unsigned long last_time = millis();           // Timer var used for debugging



//---------------------------------------------------------------
void setup() {
  Serial.begin(38400);
  delay(500);

  pinMode(ENCODER1sw, INPUT_PULLUP);

  AudioMemory(12);

  initDisplay();
  displayText("Press encoder switch to cycle through filters. Rotate encode to move center point of filter. Double press encoder in TPF filter to flip between dual filter. Long press encoder switch to reset TPF");
  delay(500);
    
  #ifdef AUDIO_SHIELD
    // Down mix stereo to mono on input
      monoMixer.gain(0, 0.5);                 // Left
      monoMixer.gain(1, 0.5);                 // Right
      monoMixer.gain(2, 0.5);                 // PC Left
      monoMixer.gain(3, 0.5);                 // PC Right
      
      audioControl.enable();                  // Start the SGTL5000
      audioControl.inputSelect(AUDIO_INPUT_LINEIN);
      audioControl.lineInLevel(5);            // Set input level to 1.33V
      audioControl.lineOutLevel(29);          // Set input level to 1.29V
      audioControl.volume(0.8);               // Set headset volume level
  #else
      audioOutput.analogReference(INTERNAL);
  #endif


    // Load default filter
    // myFilter1.begin(FIR_PASSTHRU, 0);                       // passthru mode
    filterType = filterList[0].filterType;
    freqLow = filterList[0].freqLow; 
    freqHigh = filterList[0].freqHigh; 
    freqDesc = filterList[0].desc; 
                  
    audioFilter(fir_active1, 
              NUM_COEFFICIENTS, 
              filterType, 
              W_HAMMING, 
              freqLow, 
              freqHigh );
    myFilter1.begin(fir_active1, NUM_COEFFICIENTS);

    updateDisplay(filterType, freqDesc, freqLow, freqHigh, selected_mode);

    sw_startTime = millis();
    #ifdef DEBUG
        Serial.println("setup done");
    #endif
}


//---------------------------------------------------------------
void loop()
{

  knob_select.update();                            // read the button state

  // On button press start measuring time
  if (knob_select.fallingEdge()) {                 
    sw_startTime = millis();
  }

  // On button release measure duration
  if (knob_select.risingEdge()) {                 
    sw_duration = millis() - sw_startTime;
    #ifdef DEBUG
        Serial.print("SW pressed for:");
        Serial.println(sw_duration);
    #endif
  }

  // Quick button presses toggles between center, top and low band movement
  if ((sw_duration > 0) && (sw_duration <= PRESS_QUICK)) {
    sw_duration = 0;
    selected_mode++;
    if (selected_mode >= 3)
        selected_mode = 0;
    updateDisplay();
    #ifdef DEBUG
       Serial.print("Selected mode:");
       Serial.println(selected_mode);
    #endif
   }

  // Medium duration loads a preset filter
//  if ((sw_duration > PRESS_QUICK) && (sw_duration < PRESS_MEDIUM)) {
    //    sw_duration = 0;
//  }

  // Long press resets
  if (sw_duration > PRESS_LONG) {
    sw_duration = 0;
    loadNextFilter();
    selected_mode = 0;
  }

    if (rotaryChangedState()) {
      switch (selected_mode) {
        case 0: // Centerpoint
            updateCenterpoint();
            updateFilter();
            break;
        case 1:  // High
            updateHigh();
            updateFilter();
            break;
        case 2: // Low
            updateLow();
            updateFilter();
            break;
        default:
            break;
      };
    };
  #ifdef SHOWPERF
    showPerf();
  #endif
}

//---------------------------------------------------------------
// Load the next filter
void loadNextFilter() {
    #ifdef DEBUG
        Serial.println("Loading next filter");
    #endif
    filterIndex++;
    if (filterIndex >= (sizeof(filterList) / sizeof(filter)))
          filterIndex = 0;                          // if end of array, then loop to begining

    filterType = filterList[filterIndex].filterType;
    freqLow = filterList[filterIndex].freqLow; 
    freqHigh = filterList[filterIndex].freqHigh; 
    freqDesc = filterList[filterIndex].desc; 

    updateFilter();    
}

//---------------------------------------------------------------
// Move the filter center point if encoder is rotated
void updateFilter(){
    #ifdef DEBUG
              Serial.print("Moving low = "); Serial.print(freqLow); 
              Serial.print(" Center = "); Serial.print(freqHigh - freqLow);
              Serial.print(" Moving high = "); Serial.println(freqHigh);
    #endif
    audioFilter(fir_active1, 
               NUM_COEFFICIENTS, 
               filterList[filterIndex].filterType, 
               filterList[filterIndex].window, 
               freqLow, 
               freqHigh);
    updateDisplay();
}


//---------------------------------------------------------------
void updateDisplay() {
      updateDisplay(filterList[filterIndex].filterType, 
                 filterList[filterIndex].desc,
                 freqLow, 
                 freqHigh, 
                 selected_mode);
}

//---------------------------------------------------------------
// return if encoder changed state
bool rotaryChangedState() {
  long rotaryNewPosition  = 0;

  rotaryNewPosition = knob.read();
  if (rotaryNewPosition == 0)
    return false;
  rotaryDirection = rotaryNewPosition;

  knob.write(0);
  if (verifyLimits() == false)
                return false; 
  return true;
}

//---------------------------------------------------------------
void updateCenterpoint() {
           if (rotaryDirection >= 1) {
                freqLow  += ENCODER_FREQ_SHIFT;
                freqHigh += ENCODER_FREQ_SHIFT;
            }
            if (rotaryDirection <= -1) {
                freqLow  -= ENCODER_FREQ_SHIFT;
                freqHigh -= ENCODER_FREQ_SHIFT;              
            };
}

//---------------------------------------------------------------
void updateHigh() {
            if (rotaryDirection >= 1)  freqHigh += ENCODER_FREQ_SHIFT;
            if (rotaryDirection <= -1) freqHigh -= ENCODER_FREQ_SHIFT;
}

//---------------------------------------------------------------
void updateLow() {
            if (rotaryDirection >= 1)  freqLow += ENCODER_FREQ_SHIFT;
            if (rotaryDirection <= -1) freqLow -= ENCODER_FREQ_SHIFT;
}

//---------------------------------------------------------------
bool verifyLimits() {
    if (freqLow < FREQ_MIN) {
           freqLow = FREQ_MIN;
           return false;
       };
    if (freqHigh > FREQ_MAX) {
           freqHigh = FREQ_MAX;
           return false;
       };
    if (freqLow >= freqHigh) 
           freqLow = freqHigh;
    if (freqHigh <= freqLow) 
           freqHigh = freqLow;
    return true;
}
//---------------------------------------------------------------
// Print resource usage to USB debug port
void showPerf() {
    if (millis() - last_time >= 2500) {
      if ( audioInputPeak.available() ) {
          Serial.print("Peak input level = ");
          Serial.print(audioInputPeak.readPeakToPeak());
          Serial.print(", ");    
      }
      Serial.print("Proc = ");
      Serial.print(AudioProcessorUsage());
      Serial.print(" (");    
      Serial.print(AudioProcessorUsageMax());
      Serial.print("),  Mem = ");
      Serial.print(AudioMemoryUsage());
      Serial.print(" (");    
      Serial.print(AudioMemoryUsageMax());
      Serial.println(")");
      last_time = millis();
    }
}



