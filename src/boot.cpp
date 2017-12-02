/*
	Class to handle UI boot screen.
*/

/*
	Space division for boot screen is pretty basic and similar to other screens,
    can be setup for different screens according to screen size

	Space division:
    Note: Screen size is 320x240 pixels
		Width:
			30 pixels on both left and right as margin
            290 pixels used by Textbox in bottom
            150 pixels used by icon (centered), rest is empty space

		Height:
            10 pixels as top margin
            16 pixels for text height
            10 pixels more as margin
			150 pixels for image
            10 pixel margin
			30 pixel text box
            14 pixels empty space in the end
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <SD.h>
#include <SPI.h>
#include "boot.h"
#include "bmpDisplay.h"

// these definitons are for setting different margins and icons on screen.
// They're set for a 320*240 tft screen. Will need to change according
// to your screen, check documentation above for a better description

#define RED 0xF800
#define WHITE 0xFFFF

#define MARGIN 10
#define MARGIN_SIDE 30

// text height and width are for font size 1, scale for other sizes
#define TEXT_H 8
#define TEXTBOX_H 30
#define TEXT_Y 10
#define TITLE_Y 204

#define ICON_X 85
#define ICON_Y 36
#define ICON_W 150
#define ICON_H 150

#define SCREEN_W 320
#define SCREEN_H 240

// text states for animation
#define NO_TEXT 0
#define TEXT 1

// default initialization
Boot::Boot() { }

// adafruit_ILI9341 *tft;
Boot::Boot(Adafruit_ILI9341* tft) : _tft(tft) {
	// last time is the last animation time
    _textState = NO_TEXT;
    _lastTime = 0;

	setScreen();
}

// set screen items
void Boot::setScreen(){
    // set icon
    bmpDraw("/icons/main.bmp", _tft, ICON_X, ICON_Y);

    String title = "Arduino Music Player";

    _tft->setTextSize(2);
    _tft->setTextColor(RED);
    _tft->setCursor(SCREEN_W/2 - (title.length()/2)*12, TITLE_Y);
	_tft->print(title);
}

// set text animation if current time is greater than last animation time
// by 1000 millisecond
void Boot::animate(unsigned long rtime){
    String text = "Touch screen to begin!";

    if (rtime - _lastTime > 1000){
        if (_textState == NO_TEXT){
            _tft->setTextSize(2);
            _tft->setTextColor(RED);
            _tft->setCursor(SCREEN_W/2 - (text.length()/2)*12, TEXT_Y);
        	_tft->print(text);

            // set class fields, set lastTime to current run time
            // set textState to TEXT since we wrote on screen
            _textState = TEXT;
            _lastTime = millis();
        }else{
            // draw empty rectangle to clear text box region
            _tft->fillRect(0, 0, SCREEN_W, ICON_Y, WHITE);
            // set class fields
            _textState = NO_TEXT;
            _lastTime = millis();
        }
    }
}
