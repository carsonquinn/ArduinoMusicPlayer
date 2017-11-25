#include <Arduino.h>
#include <TouchScreen.h>
#include "touch.h"
// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 5   // can be a digital pin
#define XP 4   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
//X and Y are flipped when 
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
//the state of the screen will always be one of the following
//BDOWN and BUP will only be true for one loop. This way,we can
//call events just once without repeating changes
#define NOTOUCH 1
#define BDOWN 2
#define PRESSED 3
#define BUP 4

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Touch::Touch(){
	//this constructor sets it to the default state
	this->state = NOTOUCH;
}
void Touch::processTouch(){
	
	TSPoint p = ts.getPoint();
	if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
		//this is called when the user is not touching the screen
			if(this->state == BDOWN){
				this->state = BUP;
			}else if(this->state == BUP){
				this->state = NOTOUCH;
			}else if(state == PRESSED){
				this->state = BUP;
			}
			return; 
			//exit loop if there is no touch
		}
	if(this->state == NOTOUCH){
		this->state = BDOWN;
	}else if(this->state == BDOWN){
		this->state = PRESSED;
	}else if(this->state == BUP){
		this->state = BDOWN;
	}

	// Scale from ~0->1000 to tft.width using the calibration #'s

	p.x = map(p.x, TS_MINX, TS_MAXX, 0, DISPLAY_HEIGHT);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, DISPLAY_WIDTH);
	//px and py are the coordinates of the touch if the screen was in vertical orientation.
	//Convert the coordinates to horizontal coordinates for our purpose
	this->tx = -(p.y - DISPLAY_WIDTH);
	this->ty = p.x;
}
bool Touch::isPressed(){
	//whether or not the touchscreen is being constantly being held. will repeat
	return this->state==PRESSED;
}
bool Touch::isButtonDown(){
	//whether or not the touchscreen was just touched. will not repeat
	return this->state==BDOWN;
}
bool Touch::isButtonUp(){
	//wherther or not the touchscreen was just released. will not repeat
	return this->state==BUP;
}
int Touch::getX(){
	return this->tx;
}
int Touch::getY(){
	return this->ty;
}