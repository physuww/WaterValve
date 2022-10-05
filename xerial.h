#ifndef XERIAL_H_
#define XERIAL_H_

#include "Arduino.h"

#define XERIAL_EMPTY -1
#define XERIAL_ESCFAULT -2
#define XERIAL_TIMEOUT 5 //5 msec

struct Xerial {

	uint8_t rx_buf[256];
	uint8_t tx_buf[128];
	uint8_t rx_head;
	uint8_t rx_tail;
	uint8_t tx_head;
	uint8_t tx_tail;

	bool api_flg;

	Xerial();

	void begin();
	void end();
	int read();
	int peek();
	int timedPeek();
	int timedRead();
	uint8_t readBytes(uint8_t*, size_t);
	uint8_t available();
	void write(uint8_t);
	void write(const char*);
	void writeBytes(uint8_t*, size_t);

	void rx_irq();
	void tx_irq();

	void flushRx();
	void flushTx();

	/*
	 * We need this for command mode communication with
	 * no escaping.
	 */
	int bareRead();
	void bareWrite(uint8_t);
	void bareWrite(const char*);

};



#endif /* XERIAL_H_ */
