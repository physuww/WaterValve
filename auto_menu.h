#ifndef AUTO_MENU_H_
#define AUTO_MENU_H_

#include "base_menu.h"
#include "manual_menu.h"
#include "valve.h"
#include "xbee.h"

struct ManualMenu;

extern LiquidCrystal lcd;
extern Scheduler sch;
extern BaseMenu* menu;
extern ManualMenu menuMan;
extern void (*valveReached) ();
extern bool screensaver_allowed;
extern XBee xbee;

struct AutoMenu : BaseMenu {

	virtual void onEnter();
	virtual void onExit();
	virtual void onSelect();

	static void checkWater();
	static void updateXB();

};



#endif /* AUTO_MENU_H_ */
