#ifndef _BOOTSCREEN_H
#define _BOOTSCREEN_H

class Boot{

private:
    Adafruit_ILI9341* tft;
    int textState;
    unsigned long lastTime;

public:
    Boot(Adafruit_ILI9341* tft);

    void setScreen();
    void animate(unsigned long rtime);
};

#endif
