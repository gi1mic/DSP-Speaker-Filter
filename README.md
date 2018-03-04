# DSP-Speaker-Filter
Teensy DSP audio filter for external speakers


This is a simplified version of the DSP filter designed for use in an external amplified speaker. Please visit https://gi1mic.github.io/ for information on how to setup your development environment and general information that may be helpful in this project.

This version uses a TFT display and a rotary encoder so the operator can change filter properties.

### Usage
- Encoder Rotation - change selected filter setting (displayed in RED on the TFT)
- Encoder Short press - toggle between CenterPoint, high, low filter adjustments
- Encoder Long Press - load pre-defined filter

### Hardware required:
- A [Links with title](https://www.ebay.co.uk/itm/1-44-128x128-65K-SPI-Full-Color-TFT-LCD-Display-Module-replace-OLED-for-Arduino/253219135818?epid=931476969&hash=item3af509614a:g:Rj8AAOSw0kNXg0qF"green tab 128x128 65K SPI TFT LCD") from eBay. The size is up to you but it needs to use a ST7735 driver IC. In theory the library supports "red" and "black" tab versions but I have not tried them.

- A PCB [Links with title](https://www.ebay.co.uk/itm/Rotary-Encoder-Module-KY-040-Brick-Sensor-Clickable-Switch-Arduino-ARM-Pic-UK/231884393106?hash=item35fd630a92:g:9jQAAOSw2ENW7v67"rotary encoder arduino") also from eBay. This provides both rotary control and a push button. The one in my position has a simple PCB which includes two pull-up resistors but you could just as easily use a bare encoder.

- A [Links with title](https://www.pjrc.com/store/teensy32.html"Teensy 3.2")

- Optional [Links with title](https://www.pjrc.com/store/teensy3_audio.html"Audio Adaptor Board for Teensy")

Note I am using the Teensy audio shield to simplify audio in/out for this but the code does support using the AD & DAC to reduce costs. I have not tested it but should work fine. Please see the original project linked above for the audio connections should you go down this route.

### Connections
![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/display.jpg)

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/encoder.jpg)

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/connections1.jpg)

![](https://github.com/gi1mic/DSP-Speaker-Filter/blob/master/images/connections2.jpg)
