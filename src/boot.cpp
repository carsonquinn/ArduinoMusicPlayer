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
            150 pixels used by icon, rest is empty space

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
#include "bmp_disp.h"

#define BT_RED 0xF800
#define BT_WHITE 0xFFFF

#define BT_MARGIN 10
#define BT_MARGIN_LR 30

#define BT_TEXT_HEIGHT 8
#define BT_TEXT_WIDTH 5
#define BT_TEXT_CENTER 160
#define BT_TEXTBOX 30

#define BT_ICON_WIDTH 150
#define BT_ICON_HEIGHT 150

#define BT_WIDTH 320
#define BT_HEIGHT 240

#define NO_TEXT 0
#define TEXT 1

// adafruit_ILI9341 *tft;
Boot::Boot(Adafruit_ILI9341* tft){
	this->tft = tft;
    this->text_state = NO_TEXT;
    this->last_time = 0;

    this->setScreen();
}

// set screen items
void Boot::setScreen(){
    // set icon
    bmpDraw("/icons/main.bmp", this->tft, (BT_WIDTH - BT_ICON_WIDTH)/2, \
            2*BT_MARGIN + BT_TEXT_HEIGHT*2);

    // draw text box, uncomment this to draw it
    // this->tft->drawRect(BT_MARGIN_LR, 3*BT_MARGIN + 2*BT_TEXT_HEIGHT + BT_ICON_HEIGHT,\
    //         BT_WIDTH - 2*BT_MARGIN_LR, BT_TEXTBOX, BT_RED);

    String title = "Arduino Music Player";

    this->tft->setTextSize(2);
    this->tft->setTextColor(BT_RED);
    this->tft->setCursor(BT_TEXT_CENTER - (title.length()/2)*12, \
            3*BT_MARGIN + 2*BT_TEXT_HEIGHT + BT_ICON_HEIGHT + BT_TEXTBOX/4);
	this->tft->print(title);

}

// set text animation if current time is greater than last animation time
// by 1000 millis
void Boot::animate(int rtime){
    String msg = "Touch screen to begin!";
    if (rtime - this->last_time > 1000){
        if (this->text_state == NO_TEXT){
            this->tft->setTextSize(2);
            this->tft->setTextColor(BT_RED);
            this->tft->setCursor(BT_TEXT_CENTER - (msg.length()/2)*12, BT_MARGIN);
        	this->tft->print(msg);

            // set class fields, set last_time to current run time
            // set text_state to TEXT since we wrote on screen
            this->text_state = TEXT;
            this->last_time = millis();
        }else{
            // draw empty rectangle
            this->tft->fillRect(0, 0,\
                    BT_WIDTH, 2*(BT_MARGIN + BT_TEXT_HEIGHT), BT_WHITE);
            // set class fields
            this->text_state = NO_TEXT;
            this->last_time = millis();
        }
    }
}
