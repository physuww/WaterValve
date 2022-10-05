#ifndef BASE_MENU_H_
#define BASE_MENU_H_

#include "Arduino.h"
#include "display.h"
#include "scheduler.h"
#include "constants.h"

struct BaseMenu;

extern Scheduler sch;
extern BaseMenu* menu;
extern bool button_pressed;
extern bool screensaver_allowed;

struct BaseMenu {

	virtual void onEnter() {};
	virtual void onExit() {};

	virtual void onSelect() {};
	virtual void onLeft() {};
	virtual void onRight() {};
	virtual void onUp() {};
	virtual void onDown() {};

	static void onButton() {
		auto btn = analogRead(pin_input);
		if(btn > none){
			button_pressed = false;
			sch.add(BUTTON_POLL, &onButton);
			return;
		}
		sch.add(BUTTON_DEBOUNCE, &onButton);
		if(screensaver_allowed){
			sch.del(&turnOffDisplay);
			sch.add(DISPLAY_OFF_DELAY, &turnOffDisplay);
		}
		if(!isDisplayOn()){
			turnOnDisplay();
			return;
		}
		if(button_pressed)
			return;
		else
			button_pressed = true;

		if(btn > select){
			menu->onSelect();
		}else if(btn > left){
			menu->onLeft();
		}else if(btn > down){
			menu->onDown();
		}else if(btn > up){
			menu->onUp();
		}else{
			menu->onRight();
		}
	}

};



#endif /* BASE_MENU_H_ */
