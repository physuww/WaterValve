#ifndef TEST_MENU_H_
#define TEST_MENU_H_

#include "base_menu.h"
#include "manual_menu.h"
#include "scheduler.h"
#include "xbee.h"

#define TST_XBEE_TIMEOUT 520 //13s, how long to wait for xbee to get the names of the
							//remotes; should be of the order xbee.n_dev * XBEE_NIDISC_TIMEOUT /
							//XBEE_DISC_GRPSIZE
#define TST_XBEE_CHKTRG 40 //1s, how often to check for a test trigger

struct ManualMenu;
struct TestMenu;

extern Scheduler sch;
extern ManualMenu menuMan;
extern TestMenu menuTest;
extern XBee xbee;

struct TestMenu : BaseMenu {

	int8_t n;
	int8_t ind;

	TestMenu() : n(0), ind(0) {}

	virtual void onEnter();
	virtual void onSelect();
	virtual void onExit();
	virtual void onUp();
	virtual void onDown();

	void xbeeDiscovery();
	static void checkTrig();

	void changeName();

};

template void genericFunction<TestMenu, &menuTest, &TestMenu::xbeeDiscovery> ();

#endif /* TEST_MENU_H_ */
