#ifndef _SELECTSCREEN_H
#define _SELECTSCREEN_H

class SelectScreen{

private:
	Adafruit_ILI9341* tft;
	uint32_t maxIndex;
	uint32_t currentMax;
	uint32_t index;
	String title;
	String artist;
	String album;
	float songLen;

	// drawAlbum and SetAlbums are called on touch events and hence
	// they handle changing index fields for the object since that's the only
	// time we need to handle that
	void drawAlbum(uint32_t index);
	void setAlbums(int maxIndex);

	void setIndex(uint32_t index);

	void printTitle(String title);
	void printArtist(String title);
	void printAlbum(String title);

	void setInfo(uint32_t index);
	uint8_t handleAlbumTouch(uint16_t tx, uint16_t ty);

public:
	SelectScreen(Adafruit_ILI9341* tft, uint32_t maxIndex);
	SelectScreen(Adafruit_ILI9341* tft);

	String getTitle();
	String getArtist();
	String getAlbum();
	uint32_t getIndex();

	bool handleTouch(uint16_t tx, uint16_t ty);
};
#endif
