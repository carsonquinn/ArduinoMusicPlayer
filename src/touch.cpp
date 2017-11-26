#include <Arduino.h>
#include <TouchScreen.h>
#include "touch.h"

// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 5   // can be a digital pin
#define XP 4   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define TH_WIDTH 320
#define TH_HEIGHT 240

// the state of the screen will always be one of the following
// changes will be made on button up (B_UP) only
#define NO_TOUCH 0
#define B_DOWN 1
#define PRESSED 2
#define B_UP 3

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Touch::Touch(){
	// this constructor sets it to the default state
	this->state = NO_TOUCH;
}

void Touch::processTouch(){

	TSPoint p = ts.getPoint();

	if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
		// this is called when the user is not touching the screen
			if(this->state == B_DOWN){
				this->state = B_UP;
			}else if(this->state == B_UP){
				this->state = NO_TOUCH;
			}else if(this->state == PRESSED){
				this->state = B_UP;
			}
			return;
			// exit loop if there is no touch
	}

	if(this->state == NO_TOUCH){
		this->state = B_DOWN;
	}else if(this->state == B_DOWN){
		this->state = PRESSED;
	}else if(this->state == B_UP){
		this->state = B_DOWN;
	}

	// Scale from 0->1000 to tft.width using the calibration #'s
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, TH_HEIGHT);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, TH_WIDTH);

	// px and py are the coordinates of the touch if the screen was in vertical orientation.
	// Convert the coordinates to horizontal coordinates for our purpose
	this->tx = -(p.y - TH_WIDTH);
	this->ty = p.x;
}

bool Touch::isPressed(){
	// whether or not the touchscreen is being constantly being held. Will repeat
	return this->state == PRESSED;
}

bool Touch::isButtonDown(){
	// whether or not the touchscreen was just touched. Will not repeat
	return this->state == B_DOWN;
}

bool Touch::isButtonUp(){
	// whether or not the touchscreen was just released. Will not repeat
	return this->state == B_UP;
}

int Touch::getX(){
	return this->tx;
}

int Touch::getY(){
	return this->ty;
}
