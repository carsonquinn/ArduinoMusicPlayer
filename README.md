# Arduino Music Player

CMPUT-274 (Intro to Tangible Computing) final project with @kenboo98<br/>
An Arduino Music Player built using DFRobot's [DF Player](https://www.dfrobot.com/product-1121.html "DF Player A mini...") mp3 module and Adafruit industries's [2.8" TFT](https://www.adafruit.com/product/1770 "2.8in TFT...") Touch Shield.

## Getting Started
### Hardware
<img src="https://github.com/dandua98/ArduinoMusicPlayer/raw/master/common/images/setup.jpg" width="500"><br/>
_Speaker missing in this image_<br/>
To set up your hardware and wire it up, check [WIRING.md](https://github.com/dandua98/ArduinoMusicPlayer/blob/master/WIRING.md) for a detailed explanation and schematics.

### Environment setup
The makefile for arduino sketches being used can be found at: https://github.com/sudar/Arduino-Makefile. You'll need to download and setup this one or make your own makefile for uploading the code to your Arduino. Also, change the TAG to your arduino, use mega2560 for arduino mega<br/>
The makefile in src ([src/Makefile](https://github.com/dandua98/ArduinoMusicPlayer/blob/master/src/Makefile "Makefile")) extends this makefile. Edit line 32 to  your Makefile location:

```Makefile
# Default install location of Arduino Makefile
include /usr/share/arduino/Arduino.mk
```
Also, you will need to edit your default arduino directory and arduino libraries directory.
```Makefile

# line 13
# Arduino UA Directory
ifndef ARDUINO_UA_DIR
ARDUINO_UA_DIR = $(HOME)/arduino-ua
endif

# ------------------------------------------

# line 28
# User Installed Library Location
ifndef USER_LIB_PATH
USER_LIB_PATH = $(ARDUINO_UA_DIR)/libraries
endif

```

I used DFRobot's DFPlayer to handle mp3 playing, you can use an amplifier circuit and [TMRpcm](https://github.com/TMRh20/TMRpcm "TMRpcm") to play wav files instead (didn't work out for me). It also supports ID3 tag reading (check Deployment for more details) which would make reading music data much more cleaner and would only require one SD card (explained in Deployment). The libraries I used are listed in next section. You'll need to download and add them to your default arduino libraries location.

#### Libraries
##### Arduino core libraries
* [Arduino SD](https://www.arduino.cc/en/Reference/SD "SD library") library - The default arduino library for reading/writing SD cards
* [Arduino SPI](https://www.arduino.cc/en/Reference/SPI "SPI library") library - The default arduino library to communicate with SPI devices, needed to read SD card<br/>

##### Other Libraries
* [DFRobotDFPlayerMini](https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299 "DFPlayer") - DF Robot's official library for DF Player, link can be found in wiki under sample code<br/>
* [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library "Adafruit GFX") - Adafruit's core graphics library for displays
* [Adafruit ILI9341](https://github.com/adafruit/Adafruit_ILI9341 "Adafruit ILI9341") - Hardware specific library, can find your display specific library at Adafruit's [Github](https://github.com/adafruit?utf8=✓ "Adafruit Github") page

## Deployment
As explained in wiring, DF Robot's module doesn't have any direct SD read options since the module handles that itself so you would need another SD in your tft screen with the same data under a music folder. To display music information, I wanted to directly read ID3 tags from mp3 files on the SD card in the tft screen but couldn't find any library that worked with the Arduino.
> The way ID3 tags work is depending on tag version, either the first 128 or the last 128 bytes of an mp3 file are music tag information. Simply offsetting 128 bytes from a file and reading for keyword TAG and then reading the tags should work but all libraries use fstream for that and arduino doesn't support fstream. You could try reading 128 bytes from a file using SD library and keep on reading until the keyword TAG in the end but that's extremely slow. For my information check: https://en.wikipedia.org/wiki/ID3<br/>

I just made a python script that reads ID3 tags on my computer and makes a folder with txt files corresponding to every mp3 file with the same name as mp3 file and contains song title, artist, album and song length. The program then reads this data from a folder labelled texts.<br/>
> Example txt file:<br/>
song:Elephant<br/>
artist:Tame Impala<br/>
album:Lonerism<br/>
time:211.252244898


For album arts, I made a python script that converts png to the bitmaps of the size specified using [Pillow](https://pillow.readthedocs.io/en/4.3.x/ "Pillow"), a Python imaging library. You would need two different folders for different size bitmaps for selectScreen (70x70) and PlayScreen (150x150).<br/>
Also, your music files should be labelled as 0001.mp3, 0002.mp3 and so on since that's how DFPlayer reads data. So your final file structure should look like:
```
DF Player SD:
    0001.mp3
    0002.mp3
    0003.mp3
    ...

TFT Player SD:

    icons // for boot screen and buttons bitmaps, check common/icons
        forward.bmp
        reverse.bmp
        pause.bmp
        ...

    texts
        0001.txt
        0002.txt
        0003.txt
        ...

    ssbmaps // small size bitmaps (70x70) for selectScreen
        01.bmp
        02.bmp
        03.bmp
        ...

    psbmaps // large size bitmaps (150x1500) for playScreen
        01.bmp
        02.bmp
        03.bmp
        ...
```

## Run and Usage

The screen being used was 320x240 and to make it easier to edit icon locations and margins for different screens, the locations and sizes are defined in different classes. You can easily edit them to fit you screen. <br/>
For example:<br/>
_from selectscreen.cpp_
```C++
// Icon X and Y refer to 0th index (first on screen) icon
#define ICON_H 70
#define ICON_W 70
#define ICON_ZX 40 // SIDE_W + MARGIN_LR
#define ICON_ZY 15 // MARGIN_TB
```

Also, the pixel distribution has been explained in each file.<br/>
>Note:  If you are using Mac OS X to copy the mp3, the file system will automatically add hidden files like: "._0001.mp3" for index, which this module will handle as valid mp3 files. That's why index for music play and loop in code is 2*index + 1 (to ignore these files). If using windows, please change it to index + 1 or run __dot_clean /Volumes/SDVolumeName__ to delete these files

```C++
// example on line 120 in playscree.cpp
// music index is 2*index + 1 plus screen index
_musicPlayer->play(2*index + 1);
```

### Usage
<img src="https://github.com/dandua98/ArduinoMusicPlayer/raw/master/common/images/boot.jpg" width="280" height = "200"> <img src="https://github.com/dandua98/ArduinoMusicPlayer/raw/master/common/images/boot2.jpg" width="280" height = '200'> <br/>
__1.__ Simply touch anywhere on screen to get to the selectScreen, there's a flashing text animation for that!<br/>
<img src="https://github.com/dandua98/ArduinoMusicPlayer/raw/master/common/images/selectscreen.jpg" width="350"><br/>
__2.__ Clicking on an album art on select screen would display it's details. Another click on the same album would take you to play screen. Use left and right arrows (the whole side region would work) to navigate. When returning to select screen from play screen, the music continues playing unless paused.<br/>
<img src="https://github.com/dandua98/ArduinoMusicPlayer/raw/master/common/images/playscreen.jpg" width="350"><br/>
__3.__ If you selected a song, you'll see this screen, also called the play screen. Clicking on album art takes you back to the menu. The pause button turns to play on click and vice-versa. The plus and minus buttons are used for volume (the grey line is the volume bar). The red bar at the bottom is the progress bar and the forward and backwards arrows take you to next or last song. The loop button turns red on being activated.<br/>

## Authors
* __Danish Dua__ - _Software engineering student at University of Alberta_ - [GitHub](https://github.com/dandua98)
* __Kenta Tellambura__ - _Software engineering student at University of Alberta_ - [GitHub](https://github.com/kenboo98)

## License
The project is licensed under GPL 3.0 - see the [LICENSE](https://github.com/dandua98/ArduinoMusicPlayer/blob/master/LICENSE "LICENSE") file for details

## Acknowledgement
* [Fritzing](http://fritzing.org/home/ "Fritzing") for their open source schematic tool
* [Prof. Zachary Friggstad](https://www.ualberta.ca/science/about-us/contact-us/faculty-directory/zachary-friggstad "Contact the faculty of science...") and anyone else responsible for CMPUT-274 course and the [Makefile](https://github.com/dandua98/ArduinoMusicPlayer/blob/master/src/Makefile "Makefile") developed for class
