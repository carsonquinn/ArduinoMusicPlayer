#ifndef _TOUCH_H
#define _TOUCH_H

class Touch{

private:
	int tx,ty,state;

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
