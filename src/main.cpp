#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DFRobotDFPlayerMini.h>
#include "playscreen.h"
#include "selectscreen.h"
#include "boot.h"
#include "touch.h"

// TFT SD includes all the image files for
// icons and music thumbnails, the actual music is
// handled by a seperate sd that goes into DF Mini module
#define TFT_DC 9
#define TFT_CS 10
#define TFT_SDCS 6

// lcd screen and touch screen initialization, can set your own screen here
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Initialize music DFRobotDFPlayerMini music player
DFRobotDFPlayerMini musicPlayer;

void setup(){
	init();

	// DF Mini communicates over serial1
	Serial.begin(9600);
	Serial1.begin(9600);

	// music player setup
	if (!musicPlayer.begin(Serial1)) {
		Serial.println("DFPlayer Fail");
	while(true) { };
	}

	// don't start too loud
	Serial.println("DFPlayer online");
	musicPlayer.volume(14);

	// must come before SD.begin() ...
	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(0xFFFF);

	Serial.print("Initializing SD card...");

	// TFT SD initialization
	if (!SD.begin(TFT_SDCS)) {
		Serial.println("SD fail");
	while (true) { };
	} else {
		Serial.println("Initialized");
	}

}


int main(){
	setup();

  	enum State {BOOT, SELECT_SCREEN, SELECT_TO_PLAY, PLAY, PLAY_TO_SELECT};
  	State state = BOOT;

	// main objects for the screens and touch handler
	// only Boot Screen is Initialized here, the rest are empty Initialized
	SelectScreen ss = SelectScreen(&tft);
	Boot boot = Boot(&tft);
	PlayScreen ps = PlayScreen(&tft);
	Touch touch = Touch();

  	while(true){
		// code for Boot screen
		if (state == BOOT){
			touch.processTouch();
			boot.animate(millis());
			// change state on click anywhere on screen
			if (touch.isButtonUp()){
				state = SELECT_SCREEN;
				ss = SelectScreen(&tft, musicPlayer.readFileCounts()/2);
			}
		}

		// code to run Select Screen
		else if (state == SELECT_SCREEN){
			// get touch, if button is released, handle touch
			touch.processTouch();
			bool move = false;
			if (touch.isButtonUp()){
				move = ss.handleTouch(touch.getX(), touch.getY());
			}
			// the touch handler for class returns true if state change
			// action has been processed
			if (move){
				state = SELECT_TO_PLAY;
			}

		// code to transition to Play Screen
		} else if (state == SELECT_TO_PLAY){
			ps = PlayScreen(&tft, &musicPlayer, ss.getIndex(), musicPlayer.readFileCounts()/2);
			state = PLAY;

		// run PlayScreen code
		} else if (state == PLAY){
			// get touch and pass params to playscreen to handle it
			touch.processTouch();
			bool move = false;
			move = ps.handleTouch(touch.getX(), touch.getY(), touch.getState());
			// the touch handler for class returns true if state change
			// action has been processed
			if (move){
				state = PLAY_TO_SELECT;
			}

		// code to transition back to SelectScreen
		} else if (state == PLAY_TO_SELECT){
			ss = SelectScreen(&tft, musicPlayer.readFileCounts()/2);
			state = SELECT_SCREEN;
		}
  	}

  	return 0;
}
