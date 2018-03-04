// Uncomment to enable debug information doe not use with CAT!
// DEBUG mode will also enable the onboard LED to showing tone detection for morse code
#define DEBUG

// Use the Teensy audio shield or ADC/DAC for audio
// NOTE: I am only testing with the shield at the moment.
//       You are on your own if you uncomment this for now :-)
#define AUDIO_SHIELD

// Display the calculated filter coeff in .coe format for import into MATLAB via USB port
// #define SHOWCOEFF

// Number of coefficients. Must be an even number, 4 or higher and no greater than 200
// To compile for a CPU Speed of 24Mhz it will be necessary to reduce this value to a
// value of around 120. Running at 24Mhz reduces power consumption of the Teensy to ~20ma
// Lower values reduce the quality of the filter and will require less CPU effort.
//
// To use more than 200 coefficients you have to change the "#define FIR_MAX_COEFFS" in the
// Teensy audio library source. This is located in 
// C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\filter_fir.h
#define NUM_COEFFICIENTS 200


// Limits
#define FREQ_MAX  5000.00
#define FREQ_MIN   100.00

// Display performance information via USB
// #define SHOWPERF

// Pin assignments for the push buttons
// FIR filter selection
#define SELECT_PIN 3

// Pin assignments for the rotary encoder (used for the twin pass filter)
#define ENCODER1a  0                         // Rotary encoder (a) attached to pin
#define ENCODER1b  1                         // Rotary encoder (b) attached to pin
#define ENCODER1sw 2                         // Rotary encoder (sw) attached to pin

// Pin assignments for the SPI display
// This Teensy3 native optimized version of the ST7735 library requires specific pins
//
#define sclk 14  // SCLK can also use pin 14 if you tell the driver to use software mode (see lib examples)
#define mosi 7  // MOSI can also use pin 7 if you tell the driver to use software mode (see lib examples)
#define cs   20  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define dc   21   //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define rst  8   // RST can use any pin
#define sdcs 4   // CS for SD card, can use any pin
