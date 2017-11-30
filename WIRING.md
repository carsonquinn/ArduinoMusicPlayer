Wiring for ArduinoMusicPlayer
=============================

This document explains how to wire and set up the hardware for Arduino Music
Player. I used DF Robot's mp3 module [DFPlayer](https://www.dfrobot.com/product-1121.html "DFPlayer For Arduino")
to read mp3 files and an [Adafruit ILI9341](https://www.adafruit.com/product/1770 "2.8 TFT LCD with...") touchscreen
breakout to display the GUI. Both the screen and DFPlayer would need separate MicroSDs since DFPlayer's
library doesn't allow for MicroSD reads, it just takes music function requests over serial. I need to read icon files and music tags
to display my GUI and hence the second MicroSD is necessary. It shouldn't be too hard to edit DF Robot's library though
since I believe they're using the standard SD and SPI libraries too but if not, you can implement that.
I'll link all the libraries I used (for touchscreen and DFPlayer) in my [README](https://github.com/dandua98/ArduinoMusicPlayer/blob/master/README.md "README.md"). For my speaker, I used
Gikfun's [5W EK1688](https://www.amazon.ca/Gikfun-Speaker-Stereo-Woofer-Arduino/dp/B0154KLTNM/ref=sr_1_1?ie=UTF8&qid=1511999750&sr=8-1&keywords=gikfun+speaker "Gikfun 2" 8 Ohm 5W Audio..."). DFPlayer's documentation says it can only power speakers less than 3W but this one worked perfectly!

## Hardware

* Arduino Mega 2560<br/>
> _can use an Uno too_
* DF Robot's [DFPlayer mini](https://www.dfrobot.com/product-1121.html "DFPlayer For Arduino") <br/>
>_Could use an amplifier in conjuction with [TMRpcm](https://github.com/TMRh20/TMRpcm "asynchronous playback of PCM/WAV...") library for wav/pcm playback instead, that was my first option but the sound was with mostly static even with 0.25W speakers_<br/>
* Micro SD cards <br/>
>_had to use seperate SDs for the DF Player Mini module and Adafruit's screen_
* A TFT screen for GUI (_I used [Adafruit ILI9341](https://www.adafruit.com/product/1770 "2.8 TFT LCD with...")_)
* Speaker/s, preferably less than 3W
* Breadboard and wires for testing <br/>
>_plan to 3D print an enclosure, DFPlayer also supports headphones although you'd need your own headphone jack_


## Wiring Schematic
![Wiring](https://github.com/dandua98/ArduinoMusicPlayer/raw/master/common/images/wiring.png "Wiring")<br/>
_Sketch made using [Fritzing](http://fritzing.org/home/ "Fritzing") open source schematic software_

#### For DFPlayer Mini:
![Wiring](https://www.dfrobot.com/wiki/images/a/ab/Miniplayer_pin_map.png "DFPlayer Mini")<br/>
_Schematic from DFPlayer Mini wiki_<br/>

| DFPlayer Mini Pin| Arduino Pin|
| -----------------|------------|
| VCC              | 5V         |
| RX               | 18 (TX1)   |
| TX               | 19 (RX1)   |
| SPK_1            | Speaker pin|
| GND              | Arduino GND|
| SPK_2            | Speaker pin|
> If using an Arduino Uno and need additional serial pin or want to use main serial<br/>
only for debugging, use [software serial](https://www.arduino.cc/en/Reference/SoftwareSerial "sofware serial") library to use regular pins as serial pins


#### For Adafruit TFT:
![Wiring](https://cdn-learn.adafruit.com/assets/assets/000/003/037/small360/lcds___displays_controlwire.jpg?1396790524 "Adafruit TFT")<br/>
_image by [Adafruit](https://www.adafruit.com "Adafruit")_

| Adafruit TFT pin| Arduino Pin|
| ----------------|------------|
| GND             | Arduino GND|
| Vin             | 5V         |
| CLK             | 52         |
| MISO            | 50         |
| MOSI            | 51         |
| CS              | 10         |
| D/C             | 9          |
| Y+              | A2         |
| X+              | 4          |
| Y-              | 5          |
| X-              | A3         |
| CCS             | 6          |
> Not using the 8 bit data transfer mode pin

## Authors
* __Danish Dua__ - _Software engineering student at University of Alberta_ - [GitHub](https://github.com/dandua98)
* __Kenta Tellambura__ - _Software engineering student at University of Alberta_ - [GitHub](https://github.com/kenboo98)

## License
The project is licensed under GPL 3.0 - see the [LICENSE](https://github.com/dandua98/ArduinoMusicPlayer/blob/master/LICENSE "LICENSE") file for details
## Acknowledgement
* [Fritzing](http://fritzing.org/home/ "Fritzing") for their open source schematic tool
