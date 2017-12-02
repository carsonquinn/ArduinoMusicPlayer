#ifndef _SELECTSCREEN_H
#define _SELECTSCREEN_H

class SelectScreen{

private:
	Adafruit_ILI9341* _tft;
	uint32_t _maxIndex;
	uint32_t _currentMax;
	uint32_t _index;
	String _title;
	String _artist;
	String _album;
	float _songLen;

	// drawAlbum and SetAlbums are called on touch events and hence
	// they handle changing index fields for the object since that's the only
	// time we need to handle that
	void drawAlbum(uint32_t index);
	void drawAlbumScreen(int maxIndex);

	void setIndex(uint32_t index);

	void printTitle(String title);
	void printArtist(String title);
	void printAlbum(String title);

	void setInfo(uint32_t index);
	uint32_t handleAlbumTouch(int tx, int ty);

public:
	SelectScreen(Adafruit_ILI9341* tft, uint32_t maxIndex);
	SelectScreen( );

	String getTitle();
	String getArtist();
	String getAlbum();
	uint32_t getIndex();

	bool handleTouch(int tx, int ty);
};
#endif
