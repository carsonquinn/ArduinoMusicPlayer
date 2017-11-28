/*
	Class to handle all the UI elements for PlayScreen mode.
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

#define PS_WIDTH 320
#define PS_HEIGHT 240
// All the coordinates for the buttons are top left
// play button data
// X,Y of the play button centered
#define PB_X 200
#define PB_Y 20
#define PB_W 80
#define PB_H 80
// location of reverse and forward
#define PS_REVERSE_X 160
#define PS_REVERSE_Y 100
#define PS_REVERSE_W 80
#define PS_REVERSE_H 30

#define PS_FORWARD_X 240
#define PS_FORWARD_Y 100
#define PS_FORWARD_W 80
#define PS_FORWARD_H 30
// location of the volume controls and loop controls
#define PS_VOLDOWN_X 160
#define PS_VOLDOWN_Y 140
#define PS_VOLUP_X 270
#define PS_VOLUP_Y 140
#define PS_VOL_W 50
#define PS_VOL_H 30

#define PS_LOOP_X 210
#define PS_LOOP_Y 140
#define PS_LOOP_W 60
#define PS_LOOP_H 30
// height of the song progress bar
#define PBAR_H 10
#define PBAR_Y 225

// album image size
#define ALBUM_W 150
#define ALBUM_H 150

// distance from the top and left edge of the tft to the album
#define ALBUM_PADDING_TOP 20
#define ALBUM_PADDING_LEFT 10

#define VOLUMEBAR_Y 8
#define VOLUMEBAR_H 4
#define VOLUMEBAR_UNIT_WIDTH 11
#define VOLUMEBAR_DELAY 200

// position of the text CENTER
#define TEXT_X 160
#define TEXT_Y 215

// characters are 6x8 pixels.The following will change the scale
#define TEXT_SCALE 2

// the state of the touch will always be one of the following
// changes will be made on button up (B_UP) only
#define NO_TOUCH 0
#define B_DOWN 1
#define PRESSED 2
#define B_UP 3

// adafruit_ILI9341 *tft;
PlayScreen::PlayScreen(Adafruit_ILI9341* tft, DFRobotDFPlayerMini* musicPlayer){
	this->tft = tft;
	this->musicPlayer = musicPlayer;
	this->isPlaying = true;
	this->isLooping = false;
	this->index = index;
	this->delay = 0;
	
	this->draw();
}

PlayScreen::PlayScreen(Adafruit_ILI9341* tft){
	this->tft = tft;
}

// this function takes in the x,y,value of the center of the button and the radius
// also takes in the state which is pressed or not pressed
void PlayScreen::handleTouch(uint16_t tx, uint16_t ty, int state){
	//only do something if touch was detected
	if(state == B_UP){
		//if the play button is touched, change the image of the button
		if(isTouched(tx,ty,PB_X,PB_Y,PB_W,PB_H)){
			onPlayClick();
		}
		//check if the looping button is pressed
		else if(isTouched(tx,ty,PS_LOOP_X,PS_LOOP_Y,PS_LOOP_W,PS_LOOP_H)){
			onLoopClick();
		}
		else if(isTouched(tx,ty,PS_FORWARD_X,PS_FORWARD_Y,PS_FORWARD_W,PS_FORWARD_H)){
			onForwardClick();
		}
		else if(isTouched(tx,ty,PS_REVERSE_X,PS_REVERSE_Y,PS_REVERSE_W,PS_REVERSE_H)){
			onReverseClick();
		}
		else if(isTouched(tx,ty,ALBUM_PADDING_LEFT,ALBUM_PADDING_TOP,ALBUM_W,ALBUM_H)){
			onAlbumClick();
		}
		else if(isTouched(tx,ty,0,PBAR_Y,PS_WIDTH,PBAR_H)){
			onProgressBarClick((tx)/(PS_WIDTH)*100);
		}


	}
	//for the volume button, we do not want to wait for the buttonUp event
	//simply pressing the screen should change the volume
	else if(state == PRESSED){
		if(isTouched(tx,ty,PS_VOLUP_X,PS_VOLUP_Y,PS_VOL_W,PS_VOL_H)){
			onVolUpClick();
		}
		if(isTouched(tx,ty,PS_VOLDOWN_X,PS_VOLDOWN_Y,PS_VOL_W,PS_VOL_H)){
			onVolDownClick();
		}
	}
	drawProgressBar(5);

}

void PlayScreen::drawProgressBar(int progress){
	// draws the progress bar for the song. progress is a percentage i.e. a integer from 0 to 100
	this->tft->fillRect(0,PBAR_Y, PS_WIDTH*progress/100, PBAR_H, RED);
}

void PlayScreen::drawAlbum(){
	// draws the album cover. currently just draw a square
	this->tft->fillRect(ALBUM_PADDING_LEFT, ALBUM_PADDING_TOP, ALBUM_W, ALBUM_H, GREY);
}

void PlayScreen::drawInfo(const char* title,int len){
	this->tft->setCursor(TEXT_X-len*6*TEXT_SCALE/2, TEXT_Y-8*TEXT_SCALE/2);
	this->tft->setTextColor(RED);
	this->tft->setTextSize(TEXT_SCALE);
	this->tft->print(title);
}
void PlayScreen::drawVolumeBar(){
	this->tft->fillRect(0,VOLUMEBAR_Y,PS_WIDTH,VOLUMEBAR_H,WHITE);
	this->tft->fillRect(0,VOLUMEBAR_Y,VOLUMEBAR_UNIT_WIDTH*(this->volume),VOLUMEBAR_H,GREY);
}
void PlayScreen::draw(){
	//does all the drawing at the start
	this->tft->fillScreen(WHITE);
	this->drawProgressBar(0);
	this->drawInfo("Taylor Swift - Mine",19);
	this->drawAlbum();
	//draw all the buttons
	bmpDraw("/icons/pause.bmp",this->tft,PB_X,PB_Y);
	bmpDraw("/icons/repeat.bmp",this->tft,PS_LOOP_X,PS_LOOP_Y);
	bmpDraw("/icons/forward.bmp",this->tft,PS_FORWARD_X,PS_FORWARD_Y);
	bmpDraw("/icons/reverse.bmp",this->tft,PS_REVERSE_X,PS_FORWARD_Y);
	bmpDraw("/icons/volup.bmp",this->tft,PS_VOLUP_X,PS_VOLUP_Y);
	bmpDraw("/icons/voldown.bmp",this->tft,PS_VOLDOWN_X,PS_VOLDOWN_Y);
}
bool PlayScreen::isTouched(int tx, int ty, int x, int y, int w, int h){
	//touch collision check
	return (tx> x&& tx<(x+w))&&(ty>y && ty<(y+h));
}
//all of the different on click functions below
void PlayScreen::onPlayClick(){
	if(this->isPlaying){
				//if music was currently playing, we want to change the state to paused
				this->isPlaying = false;
				bmpDraw("/icons/pause.bmp",this->tft,PB_X,PB_Y);
			}else{
				//if music was paused, unpause and redraw
				this->isPlaying = true;
				bmpDraw("/icons/play.bmp",this->tft,PB_X,PB_Y);
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
			bmpDraw("/icons/repeat.bmp",this->tft,PS_LOOP_X,PS_LOOP_Y);
		}else{
			this->isLooping = true;
			bmpDraw("/icons/repeat1.bmp",this->tft,PS_LOOP_X,PS_LOOP_Y);

		}
}
void PlayScreen::onVolUpClick(){
	//if the time since the last press is long enough, change volume
	if(millis()-(this->delay) > VOLUMEBAR_DELAY){
		this->delay = millis();
		if(this->volume < 30){
		(this->volume)++;
		}
		this->drawVolumeBar();
	}

}
void PlayScreen::onVolDownClick(){
	if(millis()-(this->delay) > VOLUMEBAR_DELAY){
		this->delay = millis();
		if(this->volume > 0){
			(this->volume)--;
		}
		this->drawVolumeBar();
	}

}
void PlayScreen::onAlbumClick(){
	Serial.print("Album");
	Serial.flush();
}
void PlayScreen::onProgressBarClick(int percentage){
	Serial.print("progress");
	Serial.flush();
}
