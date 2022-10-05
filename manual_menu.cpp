#include "manual_menu.h"

void ManualMenu::onEnter () {

	menu = this;

	lcd.clear();
	lcd.print(F("MAN XB:?   O<->C"));

	State st = getValveState();
	lcd.setCursor(0, 1);
	if(st == open)
		lcd.print(F("OPEN     "));
	else if(st == closed)
		lcd.print(F("CLOSED   "));
	else
		lcd.print(F("UNDEFINED"));

	updateXB();

}

void ManualMenu::updateXB() {

	sch.add(MAN_XBEE_UPDATE, &updateXB);
	if(xbee.available){
		lcd.setCursor(7, 0);
		lcd.write('0' + xbee.n_devices);
	}else{
		lcd.setCursor(7, 0);
		lcd.write('-');
	}
	xbee.is_available();

}

void ManualMenu::onExit() {

	sch.del(&updateXB);

}

void ManualMenu::onSelect () {

	if(valveReached)
		return;
	onExit();
	menuAuto.onEnter();

}

void ManualMenu::onLeft () {

	if(valveReached)
		stopValve();

	openValve();

}

void ManualMenu::onRight () {

	if(valveReached)
		stopValve();

	closeValve();

}

void ManualMenu::onUp () {

	if(valveReached || !xbee.available)
		return;

	onExit();
	menuTest.onEnter();

}

void ManualMenu::onDown () {

	if(valveReached || !xbee.available)
		return;

	onExit();
	menuTest.onEnter();

}
