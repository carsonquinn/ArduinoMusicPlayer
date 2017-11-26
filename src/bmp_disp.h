/*
  Routine for drawing an image patch from the SD card to the LCD display.
  Code from spitftbitmap example in adafruit library documentations,
  made to fit our purposes
 */

#ifndef _BMP_DISP_H
#define _BMP_DISP_H

// file read functions don't need to declared here,
// they're used in the cpp file anyway
void bmpDraw(char *filename, Adafruit_ILI9341 *tft, int16_t x, int16_t y);

 #endif
