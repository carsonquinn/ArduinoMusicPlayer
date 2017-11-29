/*
	Class to handle all the UI elements for PlayScreen mode.
*/

/*
	Space division for PlayScreen. The icon/button definitons are self
	explanatory in macros, this explains how the screen is divided
	Space division:
		Width:

*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <DFRobotDFPlayerMini.h>
#include "bmp_disp.h"
#include "playscreen.h"


#define BLACK 0x0000
#define RED 0xF800
#define WHITE 0xFFFF
#define GREY 0xC638

#define SCREEN_W 320
#define SCREEN_H 240

// location of play button
#define PLAY_X 200
#define PLAY_Y 20
#define PLAY_W 80
#define PLAY_H 80

// location of reverse and forward buttons
#define REV_X 160
#define REV_Y 100
#define FRW_X 240
#define FRW_Y 100
#define FR_W 80
#define FR_H 30

// location of the volume controls
#define VOLDOWN_X 160
#define VOLDOWN_Y 140
#define VOLUP_X 270
#define VOLUP_Y 140
#define VOL_W 50
#define VOL_H 30

// location of loop button
#define LOOP_X 210
#define LOOP_Y 140
#define LOOP_W 60
#define LOOP_H 30

// height of the song progress bar
#define PROGBAR_H 10
#define PROGBAR_Y 225

// album image size
#define ICON_X 10
#define ICON_Y 20
#define ICON_W 150
#define ICON_H 150

#define VOLBAR_Y 8
#define VOLBAR_H 4
#define VOLBAR_UNIT 11

// music info on screen location
#define TEXTBOX_H 50
#define TEXT_MAX 44

#define TITLE_Y 192
#define ARTIST_Y 212
#define ALBUM_Y 224

// the state of the touch will always be one of the following
// changes will be made on button up (B_UP) only
#define NO_TOUCH 0
#define B_DOWN 1
#define PRESSED 2
#define B_UP 3

// adafruit_ILI9341 *tft;
PlayScreen::PlayScreen(Adafruit_ILI9341* tft, DFRobotDFPlayerMini* musicPlayer,\
	uint32_t index){

	this->tft = tft;
	this->musicPlayer = musicPlayer;

	this->isPlaying = true;
	this->isLooping = false;

	this->index = index;
	this->title = "";
	this->artist = "";
	this->album = "";
	this->songLen = 0;

	this->volume = this->musicPlayer.readVolume();
	this->volDelay = 0;

	this->draw();
}

PlayScreen::PlayScreen(Adafruit_ILI9341* tft){
	this->tft = tft;
}

// this function takes in the x,y,value of the center of the button and the radius
// also takes in the state which is pressed or not pressed
bool PlayScreen::handleTouch(uint16_t tx, uint16_t ty, int state){
	bool checkVar = false;
	// only do something if touch was detected
	if(state == B_UP){
		// if the play button is touched, change the image of the button
		if(isTouched(tx, ty, PLAY_X, PLAY_Y, PLAY_W, PLAY_H)){
			onPlayClick();
		}
		// check if the looping button is pressed
		else if(isTouched(tx, ty, LOOP_X, LOOP_Y, LOOP_W, LOOP_H)){
			onLoopClick();
		}
		else if(isTouched(tx, ty, FRW_X, FRW_Y, FR_W, FR_H)){
			onForwardClick();
		}
		else if(isTouched(tx, ty, REV_X, REV_Y, FR_W, FR_H)){
			onReverseClick();
		}
		else if(isTouched(tx, ty, ICON_X, ICON_Y, ICON_W, ICON_H)){
			onIconClick();
			checkVar = true;
		}
		else if(isTouched(tx,ty,0,PROGBAR_Y,SCREEN_W,PROGBAR_H)){
			onProgressBarClick((tx)/(SCREEN_W)*100);
		}


	}
	// for the volume button, we do not want to wait for the buttonUp event
	// simply pressing the screen should change the volume
	else if(state == PRESSED){
		if(isTouched(tx,ty,VOLUP_X,VOLUP_Y,VOL_W,VOL_H)){
			onVolUpClick();
		}
		if(isTouched(tx,ty,VOLDOWN_X,VOLDOWN_Y,VOL_W,VOL_H)){
			onVolDownClick();
		}
	}
	drawProgressBar(5);
	return checkVar;

}

void PlayScreen::drawProgressBar(int progress){
	// draws the progress bar for the song respective to current play time
	this->tft->fillRect(0,PROGBAR_Y, SCREEN_W*progress/100, PROGBAR_H, RED);
}

// draw the album art
void PlayScreen::drawIcon(){
	this->tft->fillRect(ICON_X, ICON_Y, ICON_W, ICON_H, GREY);
}


void PlayScreen::drawVolumeBar(){
	this->tft->fillRect(0,VOLBAR_Y,SCREEN_W,VOLBAR_H,WHITE);
	this->tft->fillRect(0,VOLBAR_Y,VOLBAR_UNIT*(this->volume),VOLBAR_H,GREY);
}

// sets buttons at initialization
void PlayScreen::draw(){
	this->tft->fillScreen(WHITE);
	this->drawProgressBar(0);
	this->drawIcon();

	// draw all the buttons
	bmpDraw("/icons/pause.bmp", this->tft, PLAY_X, PLAY_Y);
	bmpDraw("/icons/repeat.bmp", this->tft, LOOP_X, LOOP_Y);
	bmpDraw("/icons/forward.bmp", this->tft, FRW_X, FRW_Y);
	bmpDraw("/icons/reverse.bmp", this->tft, REV_X, FRW_Y);
	bmpDraw("/icons/volup.bmp", this->tft, VOLUP_X, VOLUP_Y);
	bmpDraw("/icons/voldown.bmp", this->tft, VOLDOWN_X, VOLDOWN_Y);

	this->setInfo();
}

// checks for touch for various buttons/icons using button/icon location
bool PlayScreen::isTouched(int tx, int ty, int x, int y, int w, int h){
	return (tx> x && tx < (x+w))&&(ty > y && ty < (y+h));
}
//all of the different on click functions below
void PlayScreen::onPlayClick(){
	if(this->isPlaying){
				//if music was currently playing, we want to change the state to paused
				this->isPlaying = false;
				bmpDraw("/icons/pause.bmp",this->tft,PLAY_X,PLAY_Y);
			}else{
				//if music was paused, unpause and redraw
				this->isPlaying = true;
				bmpDraw("/icons/play.bmp",this->tft,PLAY_X,PLAY_Y);
			}
}
void PlayScreen::onForwardClick(){

}
void PlayScreen::onReverseClick(){
	Serial.print("reverse");
	Serial.flush();
}
void PlayScreen::onLoopClick(){
	if(this->isLooping){
			this->isLooping = false;
			bmpDraw("/icons/repeat.bmp",this->tft,LOOP_X,LOOP_Y);
		}else{
			this->isLooping = true;
			bmpDraw("/icons/repeat1.bmp",this->tft,LOOP_X,LOOP_Y);

		}
}
void PlayScreen::onVolUpClick(){
	//if the time since the last press is long enough, change volume
	if(millis() - (this->volDelay) > 250){
		if(this->volume < 30){
		(this->volume)++;
		}
		this->drawVolumeBar();
		this->volDelay = millis();
	}

}
void PlayScreen::onVolDownClick(){
	if(millis()-(this->volDelay) > 250){
		if(this->volume > 0){
			(this->volume)--;
		}
		this->drawVolumeBar();
		this->volDelay = millis();
	}

}
void PlayScreen::onIconClick(){
	Serial.print("Album");
	Serial.flush();
}
void PlayScreen::onProgressBarClick(int percentage){
	Serial.print("progress");
	Serial.flush();
}
