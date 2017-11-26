#ifndef _SELECTSCREEN_H
#define _SELECTSCREEN_H

class SelectScreen{

private:
	Adafruit_ILI9341* tft;
	uint8_t max_ind;
	uint8_t current_max;
	uint8_t current_ind;
	String title;
	String artist;
	String album;

public:
	SelectScreen(Adafruit_ILI9341* tft, uint8_t max_ind);

	// drawAlbum and SetAlbums are called on touch events and hence
	// they handle changing index fields for the object since that's the only
	// time we need to handle that
	void drawAlbum(uint8_t index);
	void setAlbums(int maxIndex);

	uint8_t getIndex();
	void setIndex(uint8_t index);

	String getTitle(uint8_t index);
	void setTitle(String title);

	String getArtist(uint8_t index);
	void setArtist(String title);

	String getAlbum(uint8_t index);
	void setAlbum(String title);

	bool handleTouch(uint16_t tx, uint16_t ty);
	uint8_t handleAlbumTouch(uint16_t tx, uint16_t ty);

};
#endif
