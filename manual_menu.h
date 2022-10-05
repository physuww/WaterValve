
#ifndef MANUAL_MENU_H_
#define MANUAL_MENU_H_

#include "base_menu.h"
#include "valve.h"
#include "auto_menu.h"
#include "test_menu.h"
#include "scheduler.h"
#include "xbee.h"

struct AutoMenu;
struct TestMenu;

extern Scheduler sch;
extern AutoMenu menuAuto;
extern TestMenu menuTest;
extern void (*valveReached)();
extern XBee xbee;

struct ManualMenu : BaseMenu {

	virtual void onEnter();
	virtual void onSelect();
	virtual void onLeft();
	virtual void onRight();
	virtual void onExit();
	virtual void onUp();
	virtual void onDown();

	static void updateXB();

};


#endif /* MANUAL_MENU_H_ */
