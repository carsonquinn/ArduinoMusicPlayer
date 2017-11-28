#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H

class PlayScreen{

private:
	Adafruit_ILI9341* tft;
	//state of the play button
	bool isPlaying,isLooping;
	uint8_t volume;
	uint32_t index;
	//time since the volume has changed
	int delay;
	void onPlayClick();
	void onForwardClick();
	void onReverseClick();
	void onLoopClick();
	void onVolUpClick();
	void onVolDownClick();
	void onAlbumClick();
	void onProgressBarClick(int percentage);
	void drawVolumeBar();
	void drawProgressBar(int progress);
	void drawAlbum();
	void drawInfo(const char* title,int len);
	bool isTouched(int tx, int ty, int x, int y, int w, int h);
	void draw();
public:
 	PlayScreen(Adafruit_ILI9341* tft);
	void handleTouch(Touch* touch);
	
};
#endif
