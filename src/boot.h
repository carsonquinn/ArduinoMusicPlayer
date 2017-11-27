#ifndef _BOOTSCREEN_H
#define _BOOTSCREEN_H

class Boot{

private:
    Adafruit_ILI9341* tft;
    int text_state;
    int last_time;

public:
    Boot(Adafruit_ILI9341* tft);

    void setScreen();
    void animate(int rtime);
};

#endif
