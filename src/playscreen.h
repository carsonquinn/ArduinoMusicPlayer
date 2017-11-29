#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H

class PlayScreen{

private:
	Adafruit_ILI9341* tft;
	DFRobotDFPlayerMini* musicPlayer;

	// state of the buttons
	bool isPlaying, isLooping;

	// music variables
	uint8_t volume;

	// song variables
	uint32_t index;
	String title;
	String artist;
	String album;
	float songLen;

	//time since the volume has changed
	int volDelay;

	void onPlayClick();
	void onForwardClick();
	void onReverseClick();
	void onLoopClick();
	void onVolumeUpClick();
	void onVolumeDownClick();
	void onProgressBarClick(int percentage);

	void draw();
	void drawVolumeBar(uint8_t volume);
	void drawProgressBar(int progress);
	void drawIcon(uint32_t index);

	void setInfo(uint32_t index);
	void printTitle(String title);
	void printArtist(String title);
	void printAlbum(String title);

	bool isTouched(int tx, int ty, int x, int y, int w, int h);

public:
 	PlayScreen(Adafruit_ILI9341* tft);
	PlayScreen(Adafruit_ILI9341* tft, DFRobotDFPlayerMini* musicPlayer, uint32_t index);

	bool handleTouch(uint16_t tx, uint16_t ty, int state);

};
#endif
