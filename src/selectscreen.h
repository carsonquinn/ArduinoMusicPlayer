#ifndef _SELECTSCREEN_H
#define _SELECTSCREEN_H

class SelectScreen{

private:
	Adafruit_ILI9341* tft;
  uint8_t current_max;
  uint8_t current_ind;
	String title;
	String artist;
	String album;

public:
 	SelectScreen(Adafruit_ILI9341* tft);

	void drawAlbum(uint8_t index);
	void setAlbums(uint8_t maxIndex);

	void setInfo(uint8_t index);

  uint8_t getIndex();
  void setIndex(uint8_t index);

  uint8_t getMax();
  uint8_t setMax(uint8_t index);

	String getTitle(uint8_t index);
	void setTitle(String title);

	String getArtist(uint8_t index);
	void setArtist(String title);

	String getAlbum(uint8_t index);
	void setAlbum(String title);

};
#endif
