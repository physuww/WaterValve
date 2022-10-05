#include "auto_menu.h"

void AutoMenu::onEnter () {

	menu = this;

	lcd.clear();
	lcd.print(F("AUTO XB:?"));

	State st = getValveState();
	lcd.setCursor(0, 1);
	if(st == open)
		lcd.print(F("OPEN"));
	else if(st == closed)
		lcd.print(F("CLOSED"));
	else
		lcd.print(F("UNDEFINED"));

	sch.add(WATER_POLL, &checkWater);

	xbee.triggered = false;
	updateXB();

}

void AutoMenu::updateXB() {

	sch.add(MAN_XBEE_UPDATE, &updateXB);
	if(xbee.available){
		lcd.setCursor(8, 0);
		lcd.write('0' + xbee.n_devices);
	}else{
		lcd.setCursor(8, 0);
		lcd.write('-');
	}
	xbee.is_available();

}

void AutoMenu::onExit() {

	noTone(pin_buzzer);
	sch.del(&checkWater);
	sch.del(&updateXB);
	screensaver_allowed = true;
	if(sch.remains(&turnOffDisplay) == 0)
		sch.add(DISPLAY_OFF_DELAY, &turnOffDisplay);

}

void AutoMenu::onSelect() {

	if(valveReached)
		return;
	onExit();
	menuMan.onEnter();

}

void AutoMenu::checkWater() {

	auto level = analogRead(pin_detectWater);
	if(level < WATER_THRESHOLD){
		sch.del(&turnOffDisplay);
		screensaver_allowed = false;
		if(!isDisplayOn())
			turnOnDisplay();
		closeValve();
		lcd.home();
		lcd.print("TRIGGED (WIRE)");
		tone(pin_buzzer, 400);
	}else if(xbee.triggered){
		sch.del(&turnOffDisplay);
		screensaver_allowed = false;
		if(!isDisplayOn())
			turnOnDisplay();
		closeValve();
		lcd.home();
		lcd.print("TRIGGED (");
		xbee.by.print(lcd);
		lcd.print(")");
		tone(pin_buzzer, 400);
	}else
		sch.add(WATER_POLL, &checkWater);

}

