/***
	Class to handle all the UI elements for SelectScreen mode.
***/

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

#include "Adafruit_GFX.h"    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include "selectscreen.h"

#define BLACK 0x0000
#define BLUE 0xADDAE6
#define RED 0xF800
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define WHITE 0xFFFF
#define LIGHTGREY 0xC638

#define SS_WIDTH 320
#define SS_HEIGHT 240

#define SS_MARGIN_LR 10
#define SS_MARGIN_TB 15
#define SS_MARGIN_COL 15
#define SS_MARGIN_ROW 20

#define SS_TEXT_BOX 50

#define SS_ALBUM_HEIGHT 70
#define SS_ALBUM_WIDTH 70

#define SS_SWITCH_WIDTH 30
