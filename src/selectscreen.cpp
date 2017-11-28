/*
	Class to handle all the UI elements for SelectScreen mode.
*/

/*
	Even indexed songs are in top row, odd index is bottom row
	In total there are 6 song album arts being displayed on screen

	If you're using a different screen, do your calculations first
	On 4:3 aspect,just multiplying everything by the the ratio compared to
	320:240 screen would do it but you would need to chage the numbers on other
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
#include "bmp_disp.h"	// To draw album art
#include "selectscreen.h"

#define SS_BLACK 0x0000
#define SS_BLUE 0xADDAE6
#define SS_RED 0xF800
#define SS_CYAN 0x07FF
#define SS_MAGENTA 0xF81F
#define SS_WHITE 0xFFFF
#define SS_LIGHTGREY 0xC638

#define SS_WIDTH 320
#define SS_HEIGHT 240

#define SS_MARGIN_LR 10
#define SS_MARGIN_TB 15
#define SS_MARGIN_COL 15
#define SS_MARGIN_ROW 20

#define SS_TEXT_BOX 50

#define SS_TEXT_CENTER 160
#define SS_TITLE_Y 192
#define SS_ARTIST_Y 212
#define SS_ALBUM_Y 224

#define SS_TEXT2_MAX 22
#define SS_TEXT1_MAX 44

#define SS_ALBUM_HEIGHT 70
#define SS_ALBUM_WIDTH 70

#define SS_SWITCH_WIDTH 30

// adafruit_ILI9341 *tft;
SelectScreen::SelectScreen(Adafruit_ILI9341* tft, uint32_t max_ind){

	this->tft = tft;
	// -1 since the class is 0 indexed but the songs and album art titles start from 1
	this->max_ind = max_ind - 1;
	this->current_max = 0;
	// current_ind lies between 0 to numSongs - 1
	this->current_ind = 0;
	this->title = "";
	this->artist = "";
	this->album = "";

	// initialization for on screen elements is carried in constructor
	tft->fillScreen(SS_WHITE);
	tft->fillTriangle(0, SS_HEIGHT/2, (SS_SWITCH_WIDTH + SS_MARGIN_LR/2),\
			SS_HEIGHT/2 - (SS_SWITCH_WIDTH + SS_MARGIN_LR)/2,\
			(SS_SWITCH_WIDTH + SS_MARGIN_LR/2), SS_HEIGHT/2 + \
			(SS_SWITCH_WIDTH + SS_MARGIN_LR)/2, SS_LIGHTGREY);
	tft->fillTriangle(SS_WIDTH, SS_HEIGHT/2, SS_WIDTH - (SS_SWITCH_WIDTH \
			+ SS_MARGIN_LR/2), SS_HEIGHT/2 - (SS_SWITCH_WIDTH + SS_MARGIN_LR)/2,\
			SS_WIDTH - (SS_SWITCH_WIDTH + SS_MARGIN_LR/2), SS_HEIGHT/2 + \
			(SS_SWITCH_WIDTH + SS_MARGIN_LR)/2, SS_LIGHTGREY);

	this->setAlbums(this->current_max + 5);
}

// empty initialization to make a global (main) variable
SelectScreen::SelectScreen(Adafruit_ILI9341* tft){
	this->tft = tft;
}

// draw album art corresponding to index, handles getting and drawing album art
// when set albums is called
void SelectScreen::drawAlbum(uint32_t index){

	// set album x and y
	uint16_t album_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((index % 6)/2)\
			*(SS_ALBUM_WIDTH + SS_MARGIN_COL);
	uint16_t album_y = SS_MARGIN_TB;

	if ((index % 2) != 0){
		album_y = album_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
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

	bmpDraw(charBuffer, this->tft, album_x, album_y);
}

// code to transition to new screen on clicks registered at sides
// sets new albums and sets current index to first song on the list
void SelectScreen::setAlbums(int maxIndex){

	// limit it to global max and bottom end to 5
	if (maxIndex > this->max_ind){
		maxIndex = this->max_ind;
	}else if (maxIndex < 0){
		maxIndex = 5;
	}

	// don't change screen on extremeties
	if(!(maxIndex == this->current_max)){
		this->current_max = maxIndex;
		uint8_t lowIndex = maxIndex - (maxIndex % 6);

		for(int i = lowIndex; i<=maxIndex; i++){
			this->drawAlbum(i);
		}

		// in case albums don't fill screen in an extreme case
		if (maxIndex - lowIndex != 5){
			for(int i = maxIndex + 1; (i - lowIndex) <=5; i++){
				// set empty rectangles x and y
				uint16_t rect_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((i% 6)/2)\
						*(SS_ALBUM_WIDTH + SS_MARGIN_COL);
				uint16_t rect_y = SS_MARGIN_TB;

				if ((i % 2) != 0){
					rect_y = rect_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
				}
				this->tft->fillRect(rect_x, rect_y, SS_ALBUM_WIDTH,\
					 	SS_ALBUM_HEIGHT, SS_WHITE);
			}
		}

		this->setIndex(lowIndex);
	}
}

// return current index, is 1 less than music file it correspinds to
uint8_t SelectScreen::getIndex(){
	return this->current_ind;
}

// change current index, set selection rectangles around old and new
// selections and set new music information from mp3 tags
void SelectScreen::setIndex(uint32_t index){

	// set old album (album) x and y (to get rectangle origin) and draw white
	// rectangle around old selection
	uint16_t album_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((this->current_ind % 6)/2) \
			*(SS_ALBUM_WIDTH + SS_MARGIN_COL) - 2;
	uint16_t album_y = SS_MARGIN_TB - 2;

	if ((this->current_ind % 2) != 0){
		album_y = album_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
	}

	this->tft->drawRect(album_x, album_y, SS_ALBUM_WIDTH + 4, SS_ALBUM_HEIGHT + 4, SS_WHITE);

	// set red selection rectangle around new album
	this->current_ind = index;
	album_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((this->current_ind % 6)/2) \
			*(SS_ALBUM_WIDTH + SS_MARGIN_COL) - 2;
	album_y = SS_MARGIN_TB - 2;

	if ((this->current_ind % 2) != 0){
		album_y = album_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
	}

	this->tft->drawRect(album_x, album_y, SS_ALBUM_WIDTH + 4, SS_ALBUM_HEIGHT + 4, SS_RED);
	this->setInfo(index);
	this->printTitle(this->title);
	this->printArtist(this->artist);
	this->printAlbum(this->album);
}

// set info fields
void SelectScreen::setInfo(uint32_t index){
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
			this->song_len = atof(buffer);
		}
	}
}

// get title from the fields
String SelectScreen::getTitle(){
	return this->title;
}

// set title on the screen while handling text centering
void SelectScreen::printTitle(String title){
	this->tft->fillRect(SS_SWITCH_WIDTH, SS_HEIGHT - SS_TEXT_BOX, SS_WIDTH - \
			2*SS_SWITCH_WIDTH, SS_TEXT_BOX - 1, SS_WHITE);
	// uncomment this to set a textbox
	// this->tft->drawRect(SS_SWITCH_WIDTH, SS_HEIGHT - SS_TEXT_BOX, SS_WIDTH - \
	// 		2*SS_SWITCH_WIDTH, SS_TEXT_BOX - 1, SS_RED);
	this->tft->setTextColor(SS_RED);
	this->tft->setTextSize(2);
	if (title.length() > SS_TEXT2_MAX){
		title = title.substring(0, SS_TEXT2_MAX - 3) + String("...");
	}
	this->tft->setCursor(SS_TEXT_CENTER - (title.length()/2)*12, SS_TITLE_Y);
	this->tft->print(title);
}

// get artist from the fields
String SelectScreen::getArtist(){
	return this->artist;
}

// set artist on the screen
void SelectScreen::printArtist(String title){
	this->tft->setTextColor(SS_RED);
	this->tft->setTextSize(1);
	if (title.length()>SS_TEXT1_MAX){
		title = title.substring(0,SS_TEXT1_MAX-3) + String("...");
	}
	this->tft->setCursor(SS_TEXT_CENTER - (title.length()/2)*6, SS_ARTIST_Y);
	this->tft->print(title);
}

// get album from the fields
String SelectScreen::getAlbum(){
	return this->album;
}

// set album on the screen
void SelectScreen::printAlbum(String title){
	this->tft->setTextColor(SS_RED);
	this->tft->setTextSize(1);
	if (title.length()>SS_TEXT1_MAX){
		title = title.substring(0,SS_TEXT1_MAX-3) + String("...");
	}
	this->tft->setCursor(SS_TEXT_CENTER - (title.length()/2)*6, SS_ALBUM_Y);
	this->tft->print(title);
}

// handles touch event and returns true if screen needs to move from
// select to play
bool SelectScreen::handleTouch(uint16_t tx, uint16_t ty){
	// the extremeties are handled inside setAlbums
	if (tx < (SS_SWITCH_WIDTH + SS_MARGIN_LR)){
		int callIndex = this->current_max - (this->current_max % 6) - 1;
		this->setAlbums(callIndex);
		return false;
	} else if (tx > SS_WIDTH - (SS_SWITCH_WIDTH + SS_MARGIN_LR)){
		// the higher than expected extreme would be handles inside setAlbums
		int callIndex = this->current_max + 6;
		this->setAlbums(callIndex);
		return false;
	} else if (ty < SS_HEIGHT - SS_TEXT_BOX){
		// get index of album if an album was touched
		uint8_t touchedIndex = this->handleAlbumTouch(tx, ty);
		if (touchedIndex == this->current_ind){
			return true;
		}else if (touchedIndex <= this->max_ind){
			this->setIndex(touchedIndex);
			// small delay prevents multi-touch
			delay(500);
			return false;
		}else{
			return false;
		}
	}
}

uint8_t SelectScreen::handleAlbumTouch(uint16_t tx, uint16_t ty){
	uint8_t index = this->current_max - (this->current_max % 6);
	// increase index by 1 if it's in bottom row
	if (ty > SS_MARGIN_TB + SS_ALBUM_HEIGHT + SS_MARGIN_ROW/2){
		index += 1;
	}
	tx = tx - SS_SWITCH_WIDTH - SS_MARGIN_LR;
	if (tx > SS_ALBUM_WIDTH + SS_MARGIN_COL/2){
		index += 2;
	}
	if (tx > 2*SS_ALBUM_WIDTH + SS_MARGIN_COL*(3/2)){
		index += 2;
	}
	return index;
};
