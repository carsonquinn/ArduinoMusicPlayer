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
#include <SPI.h>
#include <SD.h>
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
#define PROGBAR_MARGIN 5
#define PROGBAR_Y 225

// album image size
#define ICON_X 10
#define ICON_Y 20
#define ICON_W 150
#define ICON_H 150

#define VOLBAR_Y 8
#define VOLBAR_H 4
#define VOLBAR_UNIT 11

// music info on screen location, TSIDE_W is space left on sides for textbox
#define TEXTBOX_H 50
#define TSIDE_W 30
#define TEXT_MAX 44

#define TITLE_Y 182
#define ARTIST_Y 202
#define ALBUM_Y 214

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

	this->volume = this->musicPlayer->readVolume();
	this->volDelay = 0;

	this->setInfo(index);
	this->draw();
}

PlayScreen::PlayScreen(Adafruit_ILI9341* tft){
	this->tft = tft;
}

// takes the touch position and state (Button up or Pressed) to
// determine which icon was touched and calls touch handler functions
bool PlayScreen::handleTouch(uint16_t tx, uint16_t ty, int state){
	bool checkVar = false;
	// for buttons that perform action on release
	if(state == B_UP){
		// if the play button is pressed, change the image of the button
		if(isTouched(tx, ty, PLAY_X, PLAY_Y, PLAY_W, PLAY_H)){
			onPlayClick();
		}
		// check if the loop button is pressed
		else if(isTouched(tx, ty, LOOP_X, LOOP_Y, LOOP_W, LOOP_H)){
			onLoopClick();
		}
		// move to next song
		else if(isTouched(tx, ty, FRW_X, FRW_Y, FR_W, FR_H)){
			onForwardClick();
		}
		// move to previous song
		else if(isTouched(tx, ty, REV_X, REV_Y, FR_W, FR_H)){
			onReverseClick();
		}
		// move to SelectScreen on icon click
		else if(isTouched(tx, ty, ICON_X, ICON_Y, ICON_W, ICON_H)){
			checkVar = true;
		}
		// scrolling for progress bar
		else if(isTouched(tx, ty, 0, PROGBAR_Y - PROGBAR_MARGIN, SCREEN_W,\
			PROGBAR_H + PROGBAR_MARGIN)){
			onProgressBarClick(((tx)/(SCREEN_W))*100);
		}
	}
	// for buttons that perform action on hold (volume controls)
	else if(state == PRESSED){
		if(isTouched(tx, ty, VOLUP_X, VOLUP_Y, VOL_W, VOL_H)){
			onVolumeUpClick();
		}
		if(isTouched(tx, ty, VOLDOWN_X, VOLDOWN_Y, VOL_W, VOL_H)){
			onVolumeDownClick();
		}
	}
	return checkVar;
}

// set info fields
void PlayScreen::setInfo(uint32_t index){
	String path = "/texts/";
	String ind = "";

	if (index < 9){
		ind = String("000") + String(index+1);
	}else{
		ind = String("00") + String(index+1);
	}

	path = path + ind + String(".txt");

	// convert the path to charArray
	char charBuffer[path.length() + 1];
	path.toCharArray(charBuffer, path.length() + 1);

	File f = SD.open(charBuffer);

	while(f.available() > 0){
		// set data from file
		String f_line = f.readStringUntil('\n');
		int f_break = f_line.indexOf(":");
		String f_setter = f_line.substring(0,f_break);
		String f_variable = f_line.substring(f_break + 1);

		// set field variables accordingly
		if(f_setter == "song"){
			this->title = f_variable;
		}else if(f_setter == "artist"){
			this->artist = f_variable;
		}else if(f_setter == "album"){
			this->album = f_variable;
		}else if(f_setter == "time"){
			char buffer[f_variable.length()];
			f_variable.toCharArray(buffer, f_variable.length());
			this->songLen = atof(buffer);
		}
	}
}

// set title on the screen while handling text centering
void PlayScreen::printTitle(String title){
	this->tft->fillRect(TSIDE_W, SCREEN_H - TEXTBOX_H, SCREEN_W - \
		2*TSIDE_W, TEXTBOX_H - 1, WHITE);

	this->tft->setTextColor(RED);
	this->tft->setTextSize(2);
	if (title.length() > TEXT_MAX/2){
		title = title.substring(0, TEXT_MAX/2 - 3) + String("...");
	}
	this->tft->setCursor(SCREEN_W/2 - (title.length()/2)*12, TITLE_Y);
	this->tft->print(title);
}

// set artist on the screen
void PlayScreen::printArtist(String title){
	this->tft->setTextColor(RED);
	this->tft->setTextSize(1);
	if (title.length()>TEXT_MAX){
		title = title.substring(0, TEXT_MAX - 3) + String("...");
	}
	this->tft->setCursor(SCREEN_W/2 - (title.length()/2)*6, ARTIST_Y);
	this->tft->print(title);
}

// set album on the screen
void PlayScreen::printAlbum(String title){
	this->tft->setTextColor(RED);
	this->tft->setTextSize(1);
	if (title.length()>TEXT_MAX){
		title = title.substring(0,TEXT_MAX-3) + String("...");
	}
	this->tft->setCursor(SCREEN_W/2 - (title.length()/2)*6, ALBUM_Y);
	this->tft->print(title);
}

void PlayScreen::drawProgressBar(int progress){
	// draws the progress bar for the song respective to current play time
	this->tft->fillRect(0, PROGBAR_Y, SCREEN_W*progress/100, PROGBAR_H, RED);
}

// draw the album art
void PlayScreen::drawIcon(uint32_t index){
	// the bitmaps for playscreen are stored in SD card under psbamps directory
	String path = "/psbmaps/";
	String ind = "";

	if (index < 9){
		ind = String(0) + String(index+1);
	}else{
		ind = String(index+1);
	}

	path = path + ind + String(".bmp");

	// convert the path to charArray
	char charBuffer[path.length() + 1];
	path.toCharArray(charBuffer, path.length() + 1);

	bmpDraw(charBuffer, this->tft, ICON_X, ICON_Y);
}


void PlayScreen::drawVolumeBar(uint8_t volume){
	this->tft->fillRect(0, VOLBAR_Y, SCREEN_W, VOLBAR_H, WHITE);
	this->tft->fillRect(0, VOLBAR_Y, VOLBAR_UNIT*volume, VOLBAR_H, GREY);
}

// sets buttons at initialization
void PlayScreen::draw(){
	this->tft->fillScreen(WHITE);
	this->drawProgressBar(0);
	this->drawVolumeBar(this->volume);
	this->drawIcon(this->index);

	// draw all the buttons
	bmpDraw("/icons/pause.bmp", this->tft, PLAY_X, PLAY_Y);
	bmpDraw("/icons/repeat.bmp", this->tft, LOOP_X, LOOP_Y);
	bmpDraw("/icons/forward.bmp", this->tft, FRW_X, FRW_Y);
	bmpDraw("/icons/reverse.bmp", this->tft, REV_X, FRW_Y);
	bmpDraw("/icons/volup.bmp", this->tft, VOLUP_X, VOLUP_Y);
	bmpDraw("/icons/voldown.bmp", this->tft, VOLDOWN_X, VOLDOWN_Y);

	this->printTitle(this->title);
	this->printArtist(this->artist);
	this->printAlbum(this->album);
}

// checks for touch for various buttons/icons using button/icon location
bool PlayScreen::isTouched(int tx, int ty, int x, int y, int w, int h){
	return (tx > x && tx < (x+w)) &&  (ty > y && ty < (y+h));
}
//all of the different on click functions below
void PlayScreen::onPlayClick(){
	if(this->isPlaying){
				//if music was currently playing, we want to change the state to paused
				this->isPlaying = false;
				bmpDraw("/icons/pause.bmp", this->tft, PLAY_X, PLAY_Y);
			}else{
				//if music was paused, unpause and redraw
				this->isPlaying = true;
				bmpDraw("/icons/play.bmp", this->tft, PLAY_X, PLAY_Y);
			}
}

void PlayScreen::onForwardClick(){
	Serial.print("forward");
	Serial.flush();
}

void PlayScreen::onReverseClick(){
	Serial.print("reverse");
	Serial.flush();
}

void PlayScreen::onLoopClick(){
	if(this->isLooping){
			this->isLooping = false;
			bmpDraw("/icons/repeat.bmp", this->tft, LOOP_X, LOOP_Y);
		}else{
			// sets red looping icon on setting loop to true
			this->isLooping = true;
			bmpDraw("/icons/repeat1.bmp", this->tft, LOOP_X, LOOP_Y);

		}
}

void PlayScreen::onVolumeUpClick(){
	//if the time since the last press is long enough, change volume
	if(millis() - (this->volDelay) > 250){
		if(this->volume < 30){
		this->volume += 2;
		}
		this->drawVolumeBar(this->volume);
		this->volDelay = millis();
	}
}

void PlayScreen::onVolumeDownClick(){
	if(millis()-(this->volDelay) > 250){
		if(this->volume > 0){
			this->volume -= 2;
			this->musicPlayer->volume(this->volume);
		}
		this->drawVolumeBar(this->volume);
		this->volDelay = millis();
	}
}

void PlayScreen::onProgressBarClick(int percentage){
	Serial.print("progress");
	Serial.flush();
}
