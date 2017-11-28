#ifndef _SELECTSCREEN_H
#define _SELECTSCREEN_H

class SelectScreen{

private:
	Adafruit_ILI9341* tft;
	uint32_t max_ind;
	uint32_t current_max;
	uint32_t current_ind;
	String title;
	String artist;
	String album;
	float song_len;

public:
	SelectScreen(Adafruit_ILI9341* tft, uint32_t max_ind);
	SelectScreen(Adafruit_ILI9341* tft);

	// drawAlbum and SetAlbums are called on touch events and hence
	// they handle changing index fields for the object since that's the only
	// time we need to handle that
	void drawAlbum(uint32_t index);
	void setAlbums(int maxIndex);

	uint8_t getIndex();
	void setIndex(uint32_t index);

	String getTitle(uint32_t index);
	void printTitle(String title);

	String getArtist(uint32_t index);
	void printArtist(String title);

	String getAlbum(uint32_t index);
	void printAlbum(String title);

	void setInfo(uint32_t index);

	bool handleTouch(uint16_t tx, uint16_t ty);
	uint8_t handleAlbumTouch(uint16_t tx, uint16_t ty);

};
#endif
