/*
  Routine for drawing an image patch from the SD card to the LCD display.
  Code from spitftbitmap example in adafruit library documentations,
  made to fit our purposes
 */

#ifndef _BMP_DISP_H
#define _BMP_DISP_H

 // uint32_t read32(File &f);
 // uint16_t read16(File &f);
 void bmpDraw(char *filename, Adafruit_ILI9341 *tft, int16_t x, int16_t y);

 #endif
