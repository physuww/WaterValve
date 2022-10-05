#include "test_menu.h"

void TestMenu::onEnter () {

	menu = this;

	lcd.clear();
	if(xbee.n_devices == 0){
		lcd.print("FOUND 0");
		n = 0;
	}else{
		lcd.print(F("PLEASE WAIT..."));
		xbee.startNIDiscovery();
		sch.add(TST_XBEE_TIMEOUT, &genericFunction<TestMenu, &menuTest, &TestMenu::xbeeDiscovery>);
	}

	xbee.triggered = false;
	sch.add(TST_XBEE_CHKTRG, &checkTrig);

}

void TestMenu::onExit() {

	sch.del(genericFunction<TestMenu, &menuTest, &TestMenu::xbeeDiscovery>);
	sch.del(&checkTrig);

}

void TestMenu::xbeeDiscovery() {

	ind = 0;
	n = xbee.device_names.n;

	lcd.clear();
	lcd.print(F("FOUND "));
	lcd.print(n);

	if(n > 0){
		lcd.setCursor(0, 1);
		lcd.print(F("1) "));
		xbee.device_names[0].print(lcd);
	}

}

void TestMenu::changeName() {

	if(n == 0)
		return;
	lcd.setCursor(0, 1);
	lcd.print(ind + 1);
	lcd.print(") ");
	xbee.device_names[ind].print(lcd);
	lcd.print(F("     "));

}

void TestMenu::onUp() {

	if(n == 0)
		return;
	ind = (ind - 1 + n) % n;
	changeName();

}

void TestMenu::onDown() {

	if(n == 0)
		return;
	ind = (ind + 1) % n;
	changeName();

}

void TestMenu::onSelect() {

	onExit();
	menuMan.onEnter();

}

void TestMenu::checkTrig() {

	sch.add(TST_XBEE_CHKTRG, &checkTrig);
	if(!xbee.triggered)
		return;

	lcd.clear();
	lcd.print(F("TRIGGERED BY"));
	lcd.setCursor(0, 1);
	xbee.by.print(lcd);

}


