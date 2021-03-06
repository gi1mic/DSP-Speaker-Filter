# DSP-Speaker-Filter
Teensy DSP audio filter for external speakers


This is a simplified version of the DSP filter designed for use in an external amplified speaker. Please visit https://gi1mic.github.io/ for information on how to setup your development environment and general information that may be helpful in this project.

This version uses a TFT display and a rotary encoder so the operator can change filter properties.

### Usage
- Encoder Short press - toggle between CenterPoint, high and low filter adjustments (displayed in RED on the TFT)
- Encoder Rotation - adjust the selected filter setting
- Encoder Long Press - load a pre-defined filter

### Hardware required:
- A ["green tab 128x128 65K SPI TFT LCD"](https://www.ebay.co.uk/itm/1-44-128x128-65K-SPI-Full-Color-TFT-LCD-Display-Module-replace-OLED-for-Arduino/253219135818?epid=931476969&hash=item3af509614a:g:Rj8AAOSw0kNXg0qF) from eBay. The physical size is up to you but it needs to use a ST7735 driver IC and have a resolution of 128x128. In theory the library supports "red" and "black" tab versions of the displays with a minor code change but I have not tried this.

- A PCB ["rotary encoder arduino"](https://www.ebay.co.uk/itm/Rotary-Encoder-Module-KY-040-Brick-Sensor-Clickable-Switch-Arduino-ARM-Pic-UK/231884393106?hash=item35fd630a92:g:9jQAAOSw2ENW7v67) also from eBay. This provides both rotary control and a push button. Mine has a simple PCB which includes two pull-up resistors but you could just as easily use a bare encoder.

- A ["Teensy 3.2"](https://www.pjrc.com/store/teensy32.html)

- Optional ["Audio Adaptor Board for Teensy"](https://www.pjrc.com/store/teensy3_audio.html)

I am using the Teensy audio shield to simplify audio in/out but settings.h supports using the AD & DAC to reduce costs. This has not been tested it but should work fine. Please see the original project linked above for the audio connections if you want to go down this route. The audio shield has advantages over the AD/DAC in it allows a much larger input/output range with expected the P-P values being adjusted in code plus a built-in headphone amplifier.

### Connections
The connections for the display and encoder can be changed via "settings.h" but if you plan to use the teensy audio adaptor you should stick to the ones provided to avoid conflicts. 

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/display.jpg)

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/encoder.jpg)

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/connections1.jpg)

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/connections2.jpg)
