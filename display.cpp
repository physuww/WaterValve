#include "display.h"

void toggleBL() {

	//Read current mode
	uint8_t bit = digitalPinToBitMask(pin_BL);
	uint8_t port = digitalPinToPort(pin_BL);
	volatile uint8_t *reg = portModeRegister(port);

	if(*reg & bit){
		//it is an output, back light is off
		pinMode(pin_BL, INPUT_PULLUP);
	}else{
		pinMode(pin_BL, OUTPUT);
		digitalWrite(pin_BL, LOW);
	}

}

bool isDisplayOn() {

	//Read BL current mode
	uint8_t bit = digitalPinToBitMask(pin_BL);
	uint8_t port = digitalPinToPort(pin_BL);
	volatile uint8_t *reg = portModeRegister(port);

	return !(*reg & bit);

}

void turnOnBL() {

	pinMode(pin_BL, INPUT_PULLUP);

}

void turnOffBL() {

	pinMode(pin_BL, OUTPUT);
	digitalWrite(pin_BL, LOW);
}

void turnOnDisplay() {

	turnOnBL();
	lcd.display();

}

void turnOffDisplay() {

	turnOffBL();
	lcd.noDisplay();

}


