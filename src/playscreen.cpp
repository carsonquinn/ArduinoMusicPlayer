/*
	Class to handle all the UI elements for PlayScreen mode.
*/

/*
	Space division for PlayScreen. The icon/button definitons are self
	explanatory in macros, this explains how the screen is divided
	Space division:
		Width:
			20 pixels margin on left
			150 pixels for album icons
			150 pixels on the right are for icons (arranged differently in 3 layers)
			Note - the bottom layer below icon has a textbox space to display music info

		Height:
			16 pixels (8 margin) for volume bar
			150 pixels for album icon
			Note - on the right of album art, the 150 pixels are divided into
			3 layers of button, an 80 pixel play button, 30 pixels forward and
			reverse and 30 picels volume and loop and the rest margins
			50 pixels for textbox
			15 pixels for progress bar (5 margin)
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <SD.h>
#include "bmpDisplay.h"
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

// music info on screen location, TSIDE_W is space left on sides
// for textbox (not drawn just imaginary boundary)
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
	uint32_t index, uint32_t maxIndex) : _tft(tft), _musicPlayer(musicPlayer),
	_index(index), _maxIndex(maxIndex - 1) {

	_isPlaying = true;
	_isLooping = false;

	// sets to empty string anyway
	_title = _artist = _album = "";
	_songLen = 0;

	_volume = _musicPlayer->readVolume();
	// would sync on volume press anyway
	_volDelay = millis();

	setInfo(index);
	drawScreen();

	// music index is 2*index + 1 plus screen index
	_musicPlayer->play(2*index + 1);
	_startTime = _lastAnimate = millis();
}

// default initialization
PlayScreen::PlayScreen() { }

// takes the touch position and state (Button up or Pressed) to
// determine which icon was touched and calls touch handler functions
bool PlayScreen::handleTouch(int tx, int ty, int state){
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
	}
	// for buttons that perform action on hold (volume controls)
	else if(state == PRESSED){
		if(isTouched(tx, ty, VOLUP_X, VOLUP_Y, VOL_W, VOL_H)){
			onVolumeUpClick();
		}
		else if(isTouched(tx, ty, VOLDOWN_X, VOLDOWN_Y, VOL_W, VOL_H)){
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
	} else if (index < 99){
		ind = String("00") + String(index+1);
	} else if (index < 999){
		ind = String("0") + String(index+1);
	} else {
		ind = String(index+1);
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
			_title = f_variable;
		}else if(f_setter == "artist"){
			_artist = f_variable;
		}else if(f_setter == "album"){
			_album = f_variable;
		}else if(f_setter == "time"){
			char buffer[f_variable.length()];
			f_variable.toCharArray(buffer, f_variable.length());
			_songLen = atof(buffer);
		}
	}
}

// set title on the screen while handling text centering
void PlayScreen::printTitle(String title){
	_tft->fillRect(TSIDE_W, TITLE_Y, SCREEN_W - 2*TSIDE_W, TEXTBOX_H, WHITE);

	_tft->setTextColor(RED);
	_tft->setTextSize(2);
	if (title.length() > TEXT_MAX/2){
		title = title.substring(0, TEXT_MAX/2 - 3) + String("...");
	}
	_tft->setCursor(SCREEN_W/2 - (title.length()/2)*12, TITLE_Y);
	_tft->print(title);
}

// set artist on the screen
void PlayScreen::printArtist(String title){
	_tft->setTextColor(RED);
	_tft->setTextSize(1);
		if (title.length()>TEXT_MAX){
			title = title.substring(0, TEXT_MAX - 3) + String("...");
		}
	_tft->setCursor(SCREEN_W/2 - (title.length()/2)*6, ARTIST_Y);
	_tft->print(title);
}

// set album on the screen
void PlayScreen::printAlbum(String title){
	_tft->setTextColor(RED);
	_tft->setTextSize(1);
		if (title.length()>TEXT_MAX){
			title = title.substring(0,TEXT_MAX-3) + String("...");
		}
	_tft->setCursor(SCREEN_W/2 - (title.length()/2)*6, ALBUM_Y);
	_tft->print(title);
}

// draws the progress bar for the song respective to current play time
// progress is in percent and is calculated when the function is called
// at animate
void PlayScreen::drawProgressBar(float progress){
	_tft->fillRect(0, PROGBAR_Y, (SCREEN_W*progress)/100, PROGBAR_H, RED);
}

// resetting progress bar to 0
void PlayScreen::resetProgressBar(){
	_tft->fillRect(0, PROGBAR_Y, SCREEN_W, PROGBAR_H, WHITE);
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

	bmpDraw(charBuffer, _tft, ICON_X, ICON_Y);
}

// draw volume bar corresponding to current volume
void PlayScreen::drawVolumeBar(uint8_t volume){
	_tft->fillRect(0, VOLBAR_Y, SCREEN_W, VOLBAR_H, WHITE);
	_tft->fillRect(0, VOLBAR_Y, VOLBAR_UNIT*volume, VOLBAR_H, GREY);
}

// sets buttons at initialization
void PlayScreen::drawScreen(){
	_tft->fillScreen(WHITE);
	resetProgressBar();
	drawVolumeBar(_volume);
	drawIcon(_index);

	// draw all the buttons
	bmpDraw("/icons/pause.bmp", _tft, PLAY_X, PLAY_Y);
	bmpDraw("/icons/repeat.bmp", _tft, LOOP_X, LOOP_Y);
	bmpDraw("/icons/forward.bmp", _tft, FRW_X, FRW_Y);
	bmpDraw("/icons/reverse.bmp", _tft, REV_X, FRW_Y);
	bmpDraw("/icons/volup.bmp", _tft, VOLUP_X, VOLUP_Y);
	bmpDraw("/icons/voldown.bmp", _tft, VOLDOWN_X, VOLDOWN_Y);

	printTitle(_title);
	printArtist(_artist);
	printAlbum(_album);
}

// moves progress bar
void PlayScreen::animate( ){
	if (_isPlaying && (millis() - _lastAnimate) > 1000){

		unsigned long runTime = millis() - _startTime;
		float progress = (float)runTime/(float)(_songLen*1000);

		if (progress >= 0.99 && _isLooping){
			resetProgressBar();
			progress = 0;
			_startTime = millis();

		} else if (progress >= 1){
			progress = 1;
		}
		drawProgressBar(progress*100);
		_lastAnimate = millis();
	}
}

// checks for touch for various buttons/icons using button/icon location
bool PlayScreen::isTouched(int tx, int ty, int x, int y, int w, int h){
	return (tx > x && tx < (x+w)) &&  (ty > y && ty < (y+h));
}

// on play click is used to play/pause music
void PlayScreen::onPlayClick(){
	if(_isPlaying){
		// if music was currently playing, we want to change the state to paused
		_isPlaying = false;
		_musicPlayer->pause();
		_pauseTime = millis();
		bmpDraw("/icons/play.bmp", _tft, PLAY_X, PLAY_Y);
	} else {
		//if music was paused, unpause and redraw
		_isPlaying = true;
		_musicPlayer->start();
		_startTime += millis() - _pauseTime;
		bmpDraw("/icons/pause.bmp", _tft, PLAY_X, PLAY_Y);
	}
}

// move to next song, reset loop, play, icon and titles
void PlayScreen::onForwardClick(){
	if (_index < _maxIndex){
		_musicPlayer->pause();

		_index++;
		_isPlaying = true;
		_isLooping = false;
		bmpDraw("/icons/repeat.bmp", _tft, LOOP_X, LOOP_Y);

		// music info and drawing
		setInfo(_index);
		resetProgressBar();
		drawIcon(_index);
		printTitle(_title);
		printArtist(_artist);
		printAlbum(_album);

		// play new song
		_musicPlayer->play(2*_index + 1);
		_startTime = millis();
	}
}

// move to previous song, reset loop, play, icon and titles
void PlayScreen::onReverseClick(){
	if (_index > 0){
		_musicPlayer->pause();

		_index--;
		_isPlaying = true;
		_isLooping = false;
		bmpDraw("/icons/repeat.bmp", _tft, LOOP_X, LOOP_Y);

		// music info and drawing
		setInfo(_index);
		resetProgressBar();
		drawIcon(_index);
		printTitle(_title);
		printArtist(_artist);
		printAlbum(_album);

		// play new song
		_musicPlayer->play(2*_index + 1);
		_startTime = millis();
	}

}

// set/disable loop for current song
void PlayScreen::onLoopClick(){
	if(_isLooping){
			_isLooping = false;
			_musicPlayer->disableLoop();
			bmpDraw("/icons/repeat.bmp", _tft, LOOP_X, LOOP_Y);
		}else{
			// sets red looping icon on setting loop to true
			_isLooping = true;
			_musicPlayer->loop(2*_index + 1);
			resetProgressBar();
			_startTime = millis();
			bmpDraw("/icons/repeatR.bmp", _tft, LOOP_X, LOOP_Y);

		}
}

// volume click works on pressed instead of release so we have to
// check for time since hold every iteration to slow it down a bit
void PlayScreen::onVolumeUpClick(){
	if(millis() - (_volDelay) > 200){
		if(_volume < 30){
		_volume += 2;
		_musicPlayer->volume(_volume);
		drawVolumeBar(_volume);
		_volDelay = millis();
		}
	}
}

// same as previous but to decrease volume
void PlayScreen::onVolumeDownClick(){
	if(millis() - (_volDelay) > 200){
		if(_volume > 0){
			_volume -= 2;
			_musicPlayer->volume(_volume);
			drawVolumeBar(_volume);
			_volDelay = millis();
		}
	}
}
