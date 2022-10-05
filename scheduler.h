#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Arduino.h"

#define N_SLOTS 12
#define N_INSTA_SLOTS 0
#define PERIOD 0x0187 //25msec
#define HALF_PERIOD 0xc4

struct Scheduler {

	uint16_t elapsed;

	uint16_t t[N_SLOTS];
	uint16_t insta_t[N_INSTA_SLOTS];
	void (*func[N_SLOTS])();
	void (*insta_func[N_INSTA_SLOTS])();

	void init();
	void add(uint16_t, void (*)(), bool = false);
	void del(void (*)());
	uint16_t remains(void (*)());
	void onInterrupt();
	void run();

};



#endif /* SCHEDULER_H_ */
