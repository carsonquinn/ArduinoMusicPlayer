/***
	Class to handle all the UI elements for SelectScreen mode.
***/

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

#define SS_PADDING 10

#define SS_ALBUM_HEIGHT 65
#define SS_ALBUM_WIDTH 65

#define SS_CHANGE_WIDTH 40
