/*
	Class to handle all the UI elements for SelectScreen mode.
*/

/*
	Even indexed songs are in top row, odd index is bottom row
	In total there are 6 song album arts being displayed on screen

	If you're using a different screen, do your calculations first
	On 4:3 aspect,just multiplying everything by the the ratio compared to
	320:240 screen would do it but you would need to change the numbers on other
	screens

	Space division:
		Width:
			30 pixels on both left and right are reserved for switch screens
			10 pixels on left and right as margin
			15 pixels between the columns

		Height:
			50 pixels reserved for text box in the bottom
			15 pixel as margin on top and bottom
			20 pixels between the two rows

		Note: 10 pixels is larger than I expected so you should decrease the
		margins by a bit. Required me to resize all my bitmaps and was too
		lazy for that.

	The rectangle around selected item will start and end 2x2 pixels from album art
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "bmpDisplay.h"
#include "selectscreen.h"

#define BLACK 0x0000
#define RED 0xF800
#define WHITE 0xFFFF
#define GREY 0xC638

#define SCREEN_W 320
#define SCREEN_H 240

// margins at sides and top bottom
#define MARGIN_LR 10
#define MARGIN_TB 15

// margins between icon columns and row
#define MARGIN_COL 15
#define MARGIN_ROW 20

// music info on screen location
#define TEXTBOX_H 50
#define TEXT_MAX 44

#define TITLE_Y 192
#define ARTIST_Y 212
#define ALBUM_Y 224

// Side switch icon points, Y1 and Y2 are for side points
// the icons are triangles that make it look like arrows
// Height is for the triangle (goes L->R), Triangle base is 40 pixels
#define SIDE_W 30
#define TRI_H 35
#define TRI_B 40
#define TRI_Y1 100 // SCREEN_H/2 - triangle base/2
#define TRI_Y2 140 // SCREEN_H/2 + triangle base/2
#define TRI_RX 285 // SCREEN_W - TRI_H (right side icon x)

// Icon X and Y refer to 0th index (first on screen) icon
#define ICON_H 70
#define ICON_W 70
#define ICON_ZX 40 // SIDE_W + MARGIN_LR
#define ICON_ZY 15 // MARGIN_TB

// adafruit_ILI9341 *tft;
SelectScreen::SelectScreen(Adafruit_ILI9341* tft, uint32_t maxIndex) :
	_tft(tft), _maxIndex(maxIndex - 1) {

	_currentMax = _index = 0;

	// sets to empty string anyway
	_title = _artist = _album = "";

	// initialization for on screen elements is carried in constructor
	_tft->fillScreen(WHITE);
	_tft->fillTriangle(0, SCREEN_H/2, TRI_H, TRI_Y1, TRI_H, TRI_Y2, GREY);
	_tft->fillTriangle(SCREEN_W, SCREEN_H/2, TRI_RX, TRI_Y1, TRI_RX, TRI_Y2, GREY);

	drawAlbumScreen(_currentMax + 5);
}

// default initialization
SelectScreen::SelectScreen( ) { }

// draw album art corresponding to index, handles getting and drawing album art
// when set albums is called
void SelectScreen::drawAlbum(uint32_t index){

	// set album x and y
	uint16_t iconX = ICON_ZX + ((index % 6)/2)*(ICON_W + MARGIN_COL);
	uint16_t iconY = ICON_ZY;

	if ((index % 2) != 0){
		iconY += MARGIN_ROW + ICON_H;
	}

	// the bitmaps for albums are stored in SD card under ssbamps directory
	String path = "/ssbmaps/";
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

	bmpDraw(charBuffer, _tft, iconX, iconY);
}

// code to transition to new screen on clicks registered at sides
// draws new albums on screen and sets current index to first song on the list
void SelectScreen::drawAlbumScreen(int maxIndex){

	// limit it to global max and bottom end to 5
	if (maxIndex > (int)_maxIndex){
		maxIndex = _maxIndex;
	} else if (maxIndex < 0){
		maxIndex = 5;
	}

	// don't change screen on extremeties
	if(!(maxIndex == (int)_currentMax)){
		_currentMax = maxIndex;
		uint8_t lowIndex = maxIndex - (maxIndex % 6);

		for(int i = lowIndex; i <= maxIndex; i++){
			drawAlbum(i);
		}

		// in case albums don't fill screen in an extreme case
		if (maxIndex - lowIndex != 5){
			for (int i = maxIndex + 1; (i - lowIndex) <= 5; i++){
				// set empty rectangles to replace current album art
				uint16_t rectX = ICON_ZX + ((i % 6)/2)*(ICON_W + MARGIN_COL);
				uint16_t rectY = ICON_ZY;

				if ((i % 2) != 0){
					rectY = rectY + MARGIN_ROW + ICON_H;
				}
				_tft->fillRect(rectX, rectY, ICON_W, ICON_H, WHITE);
			}
		}
		setIndex(lowIndex);
	}
}

// return current index, is 1 less than music file it correspinds to
uint32_t SelectScreen::getIndex(){
	return _index;
}

// change current index, set selection rectangles around old and new
// selections and set new music information from mp3 tags
void SelectScreen::setIndex(uint32_t index){

	// set old album (album) x and y (to get rectangle origin) and draw white
	// rectangle around old selection
	uint16_t iconX = ICON_ZX + ((_index % 6)/2)*(ICON_W + MARGIN_COL) - 2;
	uint16_t iconY = ICON_ZY - 2;

	if ((_index % 2) != 0){
		iconY += MARGIN_ROW + ICON_H;
	}

	_tft->drawRect(iconX, iconY, ICON_W + 4, ICON_H + 4, WHITE);

	// set red selection rectangle around new album
	_index = index;
	iconX = ICON_ZX + ((_index % 6)/2)*(ICON_W + MARGIN_COL) - 2;
	iconY = ICON_ZY - 2;

	if ((_index % 2) != 0){
		iconY += MARGIN_ROW + ICON_H;
	}

	_tft->drawRect(iconX, iconY, ICON_W + 4, ICON_H + 4, RED);
	setInfo(index);
	printTitle(_title);
	printArtist(_artist);
	printAlbum(_album);
}

// set info fields
void SelectScreen::setInfo(uint32_t index){
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

// get title from the fields
String SelectScreen::getTitle(){
	return _title;
}

// set title on the screen while handling text centering
void SelectScreen::printTitle(String title){
	_tft->fillRect(SIDE_W, SCREEN_H - TEXTBOX_H, SCREEN_W - \
		2*SIDE_W, TEXTBOX_H - 1, WHITE);

	_tft->setTextColor(RED);
	_tft->setTextSize(2);
	if (title.length() > TEXT_MAX/2){
		title = title.substring(0, TEXT_MAX/2 - 3) + String("...");
	}
	_tft->setCursor(SCREEN_W/2 - (title.length()/2)*12, TITLE_Y);
	_tft->print(title);
}

// get artist from the fields
String SelectScreen::getArtist(){
	return _artist;
}

// set artist on the screen
void SelectScreen::printArtist(String title){
	_tft->setTextColor(RED);
	_tft->setTextSize(1);
	if (title.length()>TEXT_MAX){
		title = title.substring(0, TEXT_MAX - 3) + String("...");
	}
	_tft->setCursor(SCREEN_W/2 - (title.length()/2)*6, ARTIST_Y);
	_tft->print(title);
}

// get album from the fields
String SelectScreen::getAlbum(){
	return _album;
}

// set album on the screen
void SelectScreen::printAlbum(String title){
	_tft->setTextColor(RED);
	_tft->setTextSize(1);
	if (title.length()>TEXT_MAX){
		title = title.substring(0,TEXT_MAX-3) + String("...");
	}
	_tft->setCursor(SCREEN_W/2 - (title.length()/2)*6, ALBUM_Y);
	_tft->print(title);
}

// handles touch event and returns true if screen needs to move from
// select to play
bool SelectScreen::handleTouch(int tx, int ty){
	bool checkVar = false;
	// the extremeties are handled inside drawAlbumScreen
	// left icon clicked
	if (tx < (ICON_ZX)){
		int callIndex = _currentMax - (_currentMax % 6) - 1;
		drawAlbumScreen(callIndex);

	// the higher than expected extreme would be handles inside drawAlbumScreen
	// right icon clicked
	} else if ((tx > SCREEN_W - ICON_ZX) && (ty < SCREEN_H)){
		int callIndex = _currentMax + 6;
		drawAlbumScreen(callIndex);

	} else if ((ty < SCREEN_H - TEXTBOX_H) && (tx < SCREEN_W)){
		// get index of album if an album was touched
		uint32_t touchedIndex = handleAlbumTouch(tx, ty);
		if (touchedIndex == _index){
			checkVar = true;

		}else if (touchedIndex <= _maxIndex){
			setIndex(touchedIndex);
			// small delay prevents multi-touch
			delay(500);
		}
	}
	return checkVar;

}

uint32_t SelectScreen::handleAlbumTouch(int tx, int ty){
	uint32_t index = _currentMax - (_currentMax % 6);
	// increase index by 1 if it's in bottom row
	if (ty > MARGIN_TB + ICON_H + MARGIN_ROW/2){
		index += 1;
	}
	tx = tx - SIDE_W - MARGIN_LR;
	if (tx > ICON_W + MARGIN_COL/2){
		index += 2;
	}
	if (tx > 2*ICON_W + MARGIN_COL*(3/2)){
		index += 2;
	}
	return index;
};
