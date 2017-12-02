#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H

class PlayScreen{

private:
	Adafruit_ILI9341* _tft;
	DFRobotDFPlayerMini* _musicPlayer;

	// state of the buttons
	bool _isPlaying, _isLooping;

	// music variables
	uint8_t _volume;

	// song variables
	uint32_t _index;
	uint32_t _maxIndex;
	String _title;
	String _artist;
	String _album;
	float _songLen;

	// time since the volume has changed
	unsigned long _volDelay;
	// song start time and pause Time counter
	unsigned long _startTime;
	unsigned long _pauseTime;
	unsigned long _lastAnimate;

	void onPlayClick();
	void onForwardClick();
	void onReverseClick();
	void onLoopClick();
	void onVolumeUpClick();
	void onVolumeDownClick();

	void drawScreen();
	void drawIcon(uint32_t index);
	void drawVolumeBar(uint8_t volume);
	void drawProgressBar(float progress);
	void resetProgressBar();

	void setInfo(uint32_t index);
	void printTitle(String title);
	void printArtist(String title);
	void printAlbum(String title);

	bool isTouched(int tx, int ty, int x, int y, int w, int h);

public:
 	PlayScreen( );
	PlayScreen(Adafruit_ILI9341* tft, DFRobotDFPlayerMini* musicPlayer,\
		 uint32_t index, uint32_t maxIndex);

	bool handleTouch(int tx, int ty, int state);
	void animate( );

};
#endif
