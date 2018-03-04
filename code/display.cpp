#include "settings.h"
#include "display.h"
#include "dynamicFilters.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#define DIS_WIDTH  FREQ_MAX
#define DIS_OFFSET (FREQ_MAX - 3000)/2

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, mosi, sclk, rst);


//---------------------------------------------------------------
void initDisplay(){
  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab (128x128 resolution)
}

//---------------------------------------------------------------
void displayText(const char *text) {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print(text);

}

//---------------------------------------------------------------
void updateDisplay(const int &TYPE, const char *desc, double freqLow, double freqHigh, unsigned int selected_mode) {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.setTextColor(ST7735_WHITE);

  // Title
  tft.setCursor(10, 8);
  tft.setTextSize(2);
  tft.print(desc);

  // Graphic

  switch (TYPE) {
      case ID_LOWPASS:
                  freqHigh = freqLow;
                  freqLow = FREQ_MIN;
                  break;
      case ID_HIGHPASS:
                  freqHigh = FREQ_MAX;
                  freqLow = freqLow;
                  break;
      case ID_BANDPASS:
                  break;
      case ID_BANDSTOP:
                  break;
      default:
                  break;
  }                    
  
  if (freqHigh < freqLow) {
    double temp = freqLow;
    freqLow = freqHigh;
    freqHigh = temp;
  }

  
  double scale = DIS_WIDTH/(tft.width()-1);
  double low = freqLow/scale;
  double high = freqHigh/scale;
  tft.drawLine(0, 100, low - 10, 100, ST7735_BLUE);
  tft.drawLine(low - 10, 100, low, 50, ST7735_BLUE);
  tft.drawLine(low, 50, high, 50, ST7735_BLUE);
  tft.drawLine(high, 50, high + 10, 100, ST7735_BLUE);    
  tft.drawLine(high + 10, 100, tft.width()-1, 100, ST7735_BLUE);

  // Footer
  tft.drawLine(0, 115, tft.width()-1, 115, ST7735_WHITE);

  tft.setTextSize(1);
  tft.setCursor(10, 120);

  if ((selected_mode == 0) || (selected_mode == 2))  
      tft.setTextColor(ST7735_RED);
  else
      tft.setTextColor(ST7735_WHITE);

  tft.print(freqLow);

  tft.setCursor(80, 120);
  if ((selected_mode == 0) || (selected_mode == 1)) 
      tft.setTextColor(ST7735_RED);
  else
      tft.setTextColor(ST7735_WHITE);
  tft.print(freqHigh);

}
