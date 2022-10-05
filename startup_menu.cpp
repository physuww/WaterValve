#include "startup_menu.h"

void StartUpMenu::onEnter() {

	menu = this;

	lcd.clear();
	lcd.print(F("STARTING... "));
	lcd.print(cnt);
	xbee.init();
	while(!xbee.available && cnt != 0){
		-- cnt;
		lcd.setCursor(12, 0);
		lcd.print(cnt);
		xbee.init();
	}
	screensaver_allowed = true;
	sch.add(DISPLAY_OFF_DELAY, &turnOffDisplay);
	menuMan.onEnter();
}
