#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <TMRpcm.h>

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

TMRpcm tmr;

// lcd screen and touch screen initialization
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

File root;

void printDir(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDir(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void setup(){
  init();

  // Audio input for LM 386
  tmr.speakerPin = 11;
  // to debug
  Serial.begin(9600);

  pinMode(JOY_SEL, INPUT_PULLUP);

  // must come before SD.begin() ...
  tft.begin();

  Serial.print("Initializing SD card...");

  // SD initialization
  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("SD fail");
    while (true) { };
  } else {
    Serial.println("Initialized");
  }

  root = SD.open("/");
  printDir(root, 0);

  tmr.setVolume(MAX_VOL);
  tmr.quality(NORMAL_Q);
}

int main(){
  setup();

  // main loop
  while(true){

    int selVal = digitalRead(JOY_SEL);

    if (selVal == LOW){
      Serial.println("low");
      tmr.play("nothot.wav", 30);
      delay(100);
    }

  }

  return 0;
}
