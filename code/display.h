#ifndef DISPLAY_H
#define DISPLAY_H

// Function prototypes
void initDisplay();
void displayText(const char *text);
void updateDisplay(const int &TYPE, const char *desc, double freqLow, double freqHigh, unsigned int selected_mode);

#endif
