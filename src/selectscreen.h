#ifndef _SELECTSCREEN_H
#define _SELECTSCREEN_H

class SelectScreen{

private:
	Adafruit_ILI9341* tft;
  uint32_t current_max;
  uint32_t current_ind;

public:
 	SelectScreen(Adafruit_ILI9341* tft);
	void drawAlbum(uint32_t index);
	void drawInfo();
	void setInfo(uint32_t index);
  uint32_t getIndex();
  void setIndex(uint32_t index);
  uint32_t getMax();
  uint32_t setMax(uint32_t index);
	void drawInfo(const char* title,int len);
};
#endif
