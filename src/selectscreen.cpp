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

	The rectangle around selected item will start and end 2x2 pixels from album art
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
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

#define SS_ALBUM_HEIGHT 70
#define SS_ALBUM_WIDTH 70

#define SS_SWITCH_WIDTH 30

// adafruit_ILI9341 *tft;
SelectScreen::SelectScreen(Adafruit_ILI9341* tft, uint8_t max_ind){
	this->tft = tft;
	// -1 since the class is 0 indexed but the songs and album art titles start from 1
	this->max_ind = max_ind - 1;
  this->current_max = 0;
  this->current_ind = 0;
  this->title = "";
	this->artist = "";
	this->album = "";

	tft->drawRect(SS_SWITCH_WIDTH, SS_HEIGHT - SS_TEXT_BOX, SS_WIDTH - 2*SS_SWITCH_WIDTH, SS_TEXT_BOX - 1, SS_RED);
	this->setAlbums(this->current_max + 5);
	this->setIndex(this->current_ind);
}

// draw album according to index, just handles getting and drawing album art
// when set albums is called
void SelectScreen::drawAlbum(uint8_t index){

	// set album x and y
	uint16_t album_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((index % 6)/2)*(SS_ALBUM_WIDTH + SS_MARGIN_COL);
	uint16_t album_y = SS_MARGIN_TB;

	if ((index % 2) != 0){
		album_y = album_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
	}

	// the bitmaps for albums are stored in Sd card under ssbamps directory
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

// code to transition to new screen (scroll left or right for more options)
// on clicks registered at sides
void SelectScreen::setAlbums(uint8_t maxIndex){

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

		this->setIndex(lowIndex);
	}
}

uint8_t SelectScreen::getIndex(){
	return this->current_ind;
}

// setting index involves changing current index, redrawing selection rectangles
// (old and new ones) and then getting information from mp3 tags and calling
// setters. On second click on same index, transition (to PlayScreen) code is called
void SelectScreen::setIndex(uint8_t index){

	// set old album x and y (to get rectangle origin) and draw white rectangle around
	// old selection
	uint16_t oalbum_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((this->current_ind % 6)/2)*(SS_ALBUM_WIDTH + SS_MARGIN_COL) - 2;
	uint16_t oalbum_y = SS_MARGIN_TB - 2;

	if ((this->current_ind % 2) != 0){
		oalbum_y = oalbum_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
	}

	this->tft->drawRect(oalbum_x, oalbum_y, SS_ALBUM_WIDTH + 4, SS_ALBUM_HEIGHT + 4, SS_WHITE);

	// do the something except with the new selection and red rectangle
	this->current_ind = index;
	uint16_t album_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((this->current_ind % 6)/2)*(SS_ALBUM_WIDTH + SS_MARGIN_COL) - 2;
	uint16_t album_y = SS_MARGIN_TB - 2;

	if ((this->current_ind % 2) != 0){
		album_y = album_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
	}

	this->tft->drawRect(album_x, album_y, SS_ALBUM_WIDTH + 4, SS_ALBUM_HEIGHT + 4, SS_RED);
}
