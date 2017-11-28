#ifndef _TOUCH_H
#define _TOUCH_H

class Touch{

private:
	uint16_t tx,ty;
	int state;

public:
	Touch();

	void processTouch();
	bool isPressed();
	bool isButtonDown();
	bool isButtonUp();

	int getX();
	int getY();
};

#endif
