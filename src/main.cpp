#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <TMRpcm.h>
#include "Adafruit_GFX.h"
#include "playscreen.h"

#define JOY_Y A1
#define JOY_X A0
#define JOY_SEL 2
#define JOY_THRESHOLD 128

#define TFT_DC 9
#define TFT_CS 10
#define SD_CHIP_SELECT 6

#define MAX_VOL 7
#define OVERSAMPLING_Q 1
#define NORMAL_Q 0

#define MUSIC_FILES 30

#define THUMB_GUI 0
#define MUSIC_GUI 1

#define MUSIC_VOL 0
#define MUSIC_REW 1
#define MUSIC_PLAY 2
#define MUSIC_FOR 3
#define MUSIC_LOOP 4
#define MUSIC_TIME 5

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// library to play sounds
TMRpcm tmr;

// lcd screen and touch screen initialization
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
PlayScreen ps = PlayScreen();
File root;

// to scroll through music
String titles[MUSIC_FILES];
String currentTitle;

using namespace std;

void setup(){
  init();

  // Audio input for LM 386
  tmr.speakerPin = 11;
  // to debug
  Serial.begin(9600);

  pinMode(JOY_SEL, INPUT_PULLUP);

  // must come before SD.begin() ...
  tft.begin();
  tft.setRotation(3);

  Serial.print("Initializing SD card...");

  // SD initialization
  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("SD fail");
    while (true) { };
  } else {
    Serial.println("Initialized");
  }

  // basic audio setup
  tmr.setVolume(MAX_VOL);
  tmr.quality(NORMAL_Q);
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
  root = SD.open("/");
  getMusicFiles(root, &titles[0]);
  tft.fillScreen(0xFFFF);

  ps.drawPlay(1,tft);
  ps.drawAlbum(tft);
  int progress=0;
  ps.drawInfo("Africa - Toto\0",13,tft);
  while(1){
    delay(500);
    ps.drawProgressBar(progress,tft);
    progress++;
  }
  return 0;
}
