/***
	Class to handle all the UI elements for PlayScreen mode.
***/

#include "Adafruit_GFX.h"    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include "playscreen.h"

#define BLACK 0x0000
#define BLUE 0xADDAE6
#define RED 0xF800
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define WHITE 0xFFFF
#define LIGHTGREY 0xC638

#define D_WIDTH 320
#define D_HEIGHT 240

// play button data
// X,Y and radius of the play button centered
#define PB_X 240
#define PB_Y 85
#define PB_R 40

// height of the song progress bar
#define PBAR_H 15

// album image size
#define ALBUM_W 150
#define ALBUM_H 150

// distance from the top and left edge of the tft to the album
#define ALBUM_PADDING_TOP 10
#define ALBUM_PADDING_LEFT 10

// position of the text CENTER
#define TEXT_X 160
#define TEXT_Y 215

// characters are 5x8 pixels.The following will change the scale
#define TEXT_SCALE 2

// adafruit_ILI9341 *tft;
PlayScreen::PlayScreen(Adafruit_ILI9341* tft){
	this->tft = tft;
}

// this function takes in the x,y,value of the center of the button and the radius
// also takes in the state which is pressed or not pressed
void PlayScreen::drawPlay(int state){
	// the button will be drawn differently depending on state(pressed or not pressed). NOT YET IMPLEMENTED
	// draw the play button
	tft->fillCircle(PB_X, PB_Y, PB_R, LIGHTGREY);
	tft->fillTriangle(PB_X-PB_R*0.4, PB_Y+PB_R*0.4, PB_X-PB_R*0.4, PB_Y-PB_R*0.4, PB_X+PB_R*0.4, PB_Y,WHITE);
}

void PlayScreen::drawProgressBar(int progress){
	// draws the progress bar for the song. progress is a percentage i.e. a integer from 0 to 100
	tft->fillRect(0, D_HEIGHT-PBAR_H, D_WIDTH*progress/100, PBAR_H, RED);
}

void PlayScreen::drawAlbum(){
	// draws the album cover. currently just draw a square
	tft->fillRect(ALBUM_PADDING_LEFT, ALBUM_PADDING_TOP, ALBUM_W, ALBUM_H, LIGHTGREY);
}

void PlayScreen::drawInfo(const char* title,int len){
	tft->setCursor(TEXT_X-len*5*TEXT_SCALE/2, TEXT_Y-8*TEXT_SCALE/2);
	tft->setTextColor(RED);
	tft->setTextSize(TEXT_SCALE);
	tft->print(title);
}
