#ifndef STARTUP_MENU_H_
#define STARTUP_MENU_H_

#include "scheduler.h"
#include "base_menu.h"
#include "manual_menu.h"
#include "xbee.h"

extern bool screensaver_allowed;
extern Scheduler sch;
extern XBee xbee;
extern BaseMenu* menu;
extern ManualMenu menuMan;

#define STARTUP_ATTEMPTS 5 //5 number of attempts

struct StartUpMenu : BaseMenu {

	int cnt = STARTUP_ATTEMPTS;

	virtual void onEnter();

};

#endif /* STARTUP_MENU_H_ */
