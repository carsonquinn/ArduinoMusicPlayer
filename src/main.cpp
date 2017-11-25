#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <DFRobotDFPlayerMini.h>
#include "Adafruit_GFX.h"
#include "playscreen.h"

#define JOY_Y A1
#define JOY_X A0
#define JOY_SEL 2
#define JOY_THRESHOLD 128

// TFT SD includes all the image files for
// icons and music thumbnails, the actual music is
// handled by a seperate sd that goes into DF Mini module
#define TFT_DC 9
#define TFT_CS 10
#define TFT_SDCS 6

#define MUSIC_FILES 30

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// lcd screen and touch screen initialization, can set your own screen here
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Initialize music DFRobotDFPlayerMini music player
DFRobotDFPlayerMini music_player;

// initialize the current screen. The PlayScreen class controls all the UI elements
// for PlayScreen mode. Takes the arduino screen as the argument
PlayScreen ps = PlayScreen(&tft);

File root;

// to scroll through music
String titles[MUSIC_FILES];
String currentTitle;


void setup(){
  init();

  // DF Mini communicates over serial1
  Serial.begin(9600);
  Serial1.begin(9600);

  // music player setup
  if (!music_player.begin(Serial1)) {  //Use softwareSerial to communicate with mp3.
    Serial.println("DFPlayer Fail");
    while(true) { };
  }
  Serial.println("DFPlayer online");
  music_player.volume(30);

  // must come before SD.begin() ...
  tft.begin();
  tft.setRotation(3);

  Serial.print("Initializing SD card...");

  // TFT SD initialization
  if (!SD.begin(TFT_SDCS)) {
    Serial.println("SD fail");
    while (true) { };
  } else {
    Serial.println("Initialized");
  }

}

// get music file titles
void getMusicFiles(File dir, String *titlesArr) {
  // to keep count
  int i = 0;

  while (true) {

    File entry =  dir.openNextFile();
    // no more files
    if (!entry) break;

    // store in array if the file is of type .wav
    if (!entry.isDirectory()) {
      String name = entry.name();

      if (name.endsWith(".WAV") && !name.startsWith("_")){
        String song = name.substring(0, name.length() - 4);
        *(titlesArr + i) = song;
        Serial.println(song);
        i++;
      }

    }
    entry.close();
  }
}

int main(){
  setup();

  // go into SD card main directory and get titles
  // getMusicFiles(SD.open("/"), &titles[0]);
  tft.fillScreen(0xFFFF);

  ps.drawPlay(1);
  ps.drawAlbum();
  int progress = 0;
  ps.drawInfo("Africa - Toto\0",13);

  music_player.play(1);

  while(1){
    delay(500);
    ps.drawProgressBar(progress);
    progress++;
  }
  return 0;
}
