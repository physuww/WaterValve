#include "xerial.h"

Xerial::Xerial() : rx_head(0), rx_tail(0), tx_head(0), tx_tail(0),
			api_flg(false) {}

void Xerial::begin() {

	UCSR0A = bit(TXC0); //Clear U2X0 bit since it is not necessary for 9600 baud.
	UCSR0C = bit(UCSZ01) | bit(UCSZ00); //8 bit characters, 1 stop bit, and no parity check
	UBRR0H = 0;
	UBRR0L = 103; //per ATMEGA328P documentation

	UCSR0B = bit(RXCIE0) | bit(RXEN0) | bit(TXEN0);

}

void Xerial::end() {

	UCSR0B = 0;
	rx_head = rx_tail;
	tx_head = tx_tail;

}

/* Unlike the arduino Serial, here rx_head is the oldest data in the FIFO.
 * The buffers are circular in nature and start to override the oldest
 * data when full--when rx_tail becomes equal to rx_head, the latter
 * is incremented by 1. This means the effective size of rx_buf is 255
 * and the effective size of tx_buf is 127.
 */
int Xerial::read() {

	uint8_t ch;

	if(rx_head == rx_tail)
		return XERIAL_EMPTY;

	cli();
	if(rx_buf[rx_head] != 0x7d){
		ch = rx_buf[rx_head ++];
		sei();
		if(ch == 0x7e)
			api_flg = true;
		return ch;
	}

	if(rx_tail - rx_head == 1U){
		sei();
		return XERIAL_EMPTY;
	}

	ch = rx_buf[++ rx_head];
	++ rx_head;
	sei();

	ch ^= 0x20;
	if(ch != 0x7e && ch != 0x7d && ch != 0x31 && ch != 0x11)
		return XERIAL_ESCFAULT;
	else
		return ch;

}

int Xerial::peek() {

	uint8_t ch;

	if(rx_head == rx_tail)
		return XERIAL_EMPTY;

	cli();
	if(rx_buf[rx_head] != 0x7d){
		ch = rx_buf[rx_head];
		sei();
		return ch;
	}

	if(rx_tail - rx_head == 1U){
		sei();
		return XERIAL_EMPTY;
	}

	ch = rx_buf[rx_head + 1];
	sei();

	ch ^= 0x20;
	if(ch != 0x7e && ch != 0x7d && ch != 0x31 && ch != 0x11)
		return XERIAL_ESCFAULT;
	else
		return ch;

}

int Xerial::timedRead(){

	int c;
	unsigned long start_millis = millis();

	do{
		c = read();
		if(c >= 0 || c == XERIAL_ESCFAULT)
			return c;
	} while(millis() - start_millis < XERIAL_TIMEOUT);

	return XERIAL_EMPTY;
}

int Xerial::timedPeek(){

	int c;
	unsigned long start_millis = millis();

	do{
		c = peek();
		if(c >= 0 || c == XERIAL_ESCFAULT)
			return c;
	} while(millis() - start_millis < 5);

	return XERIAL_EMPTY;
}

/*
 * If 0x7e has been read, the function immediately returns so that
 * the next api frame can get processed. The functions in xbee.cpp
 * that call this function must also immediately return.
 */
uint8_t Xerial::readBytes(uint8_t* buf, size_t cnt) {

	uint8_t i = 0;
	while(i < cnt){

		auto c = timedRead();
		if(c < 0)
			return i;
		buf[i] = c;
		++ i;
		if(api_flg)
			return i;

	}

	return i;

}

uint8_t Xerial::available() {

	cli();
	uint8_t res = rx_tail - rx_head;
	sei();
	return res;

}

void Xerial::write(uint8_t ch) {

	if(ch == 0x7e || ch == 0x7d || ch == 0x31 || ch == 0x11){
		cli();
		tx_buf[tx_tail] = 0x7d;
		tx_tail = (tx_tail + 1) & 0x8f;
		if(tx_tail == tx_head)
			tx_head = (tx_head + 1) & 0x8f;
		tx_buf[tx_tail] = ch ^ 0x20;
		tx_tail = (tx_tail + 1) & 0x8f;
		if(tx_tail == tx_head)
			tx_head = (tx_head + 1) & 0x8f;
		sei();
	}else{
		cli();
		tx_buf[tx_tail] = ch;
		tx_tail = (tx_tail + 1) & 0x8f;
		if(tx_tail == tx_head)
			tx_head = (tx_head + 1) & 0x8f;
		sei();
	}

	if((UCSR0B & bit(UDRIE0)) == 0)
		UCSR0B |= bit(UDRIE0);

}

void Xerial::write(const char* s) {

	uint8_t i = 0;
	while(s[i] != 0){
		write(s[i]);
		++ i;
	}

}

void Xerial::writeBytes(uint8_t* buf, size_t cnt) {

	for(uint8_t i = 0; i < cnt; ++ i)
		write(buf[i]);

}

void Xerial::rx_irq() {

	while(UCSR0A & bit(RXC0)){

		if(UCSR0A & (bit(UPE0) | bit(FE0))){
			UDR0;
			continue;
		}
		rx_buf[rx_tail ++] = UDR0;
		if(rx_tail == rx_head)
			++ rx_head;

	}

}

void Xerial::tx_irq() {

	if(tx_tail == tx_head){
		UCSR0B &= ~bit(UDRIE0);
		return;
	}

	UDR0 = tx_buf[tx_head];
	tx_head = (tx_head + 1) & 0x8f;

}

void Xerial::flushTx() {

	cli();
	tx_head = tx_tail;
	UCSR0B &= ~bit(UDRIE0);
	sei();

}

void Xerial::flushRx() {

	cli();
	rx_head = rx_tail;
	while(UCSR0A & bit(RXC0))
		UDR0;
	sei();

}

int Xerial::bareRead() {

	uint8_t ch;

	if(rx_head == rx_tail)
		return XERIAL_EMPTY;

	cli();
	ch = rx_buf[rx_head ++];
	sei();
	return ch;


}

void Xerial::bareWrite(uint8_t ch) {

	cli();
	tx_buf[tx_tail] = ch;
	tx_tail = (tx_tail + 1) & 0x8f;
	if(tx_tail == tx_head)
		tx_head = (tx_head + 1) & 0x8f;
	sei();

	if((UCSR0B & bit(UDRIE0)) == 0)
		UCSR0B |= bit(UDRIE0);

}

void Xerial::bareWrite(const char* s) {

	uint8_t i = 0;
	while(s[i] != 0){
		bareWrite(s[i]);
		++ i;
	}

}

