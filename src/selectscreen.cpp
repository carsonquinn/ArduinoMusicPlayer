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
SelectScreen::SelectScreen(Adafruit_ILI9341* tft){
	this->tft = tft;
  this->current_max = 5;
  this->current_ind = 0;
  this->title = "";
	this->artist = "";
	this->album = "";

	tft->drawRect(SS_SWITCH_WIDTH, SS_HEIGHT - SS_TEXT_BOX, SS_WIDTH - 2*SS_SWITCH_WIDTH, SS_TEXT_BOX - 1, SS_RED);
}

void SelectScreen::drawAlbum(uint8_t index){

	// set album x and y
	uint16_t album_x = SS_SWITCH_WIDTH + SS_MARGIN_LR + ((index % 6)/2)*(SS_ALBUM_WIDTH + SS_MARGIN_COL);
	uint16_t album_y = SS_MARGIN_TB;

	if ((index % 2) != 0){
		album_y = album_y + SS_MARGIN_ROW + SS_ALBUM_HEIGHT;
	}
}
