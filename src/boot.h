#ifndef _BOOTSCREEN_H
#define _BOOTSCREEN_H

class Boot{

private:
    Adafruit_ILI9341* _tft;
    int _textState;
    unsigned long _lastTime;

public:
    Boot();
    Boot(Adafruit_ILI9341* tft);

    void setScreen();
    void animate(unsigned long rtime);
};

#endif
