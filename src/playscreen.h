#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H

class PlayScreen{

private:
	Adafruit_ILI9341* tft;

public:
 	PlayScreen(Adafruit_ILI9341* tft);
	void drawPlay(int state);
	void drawProgressBar(int progress);
	void drawAlbum();
	void drawInfo(const char* title,int len);
};
#endif
