#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H
class PlayScreen{
	
public:
 	PlayScreen();
	void drawPlay(int state,Adafruit_ILI9341 tft);
	void drawProgressBar(int progress, Adafruit_ILI9341 tft);
	void drawAlbum(Adafruit_ILI9341 tft);
	void drawInfo(const char* title,int len,Adafruit_ILI9341 tft);
};
#endif