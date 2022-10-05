#include "scheduler.h"

void Scheduler::init() {

	for(uint8_t i = 0; i < N_SLOTS; ++ i){
		t[i] = 0;
		func[i] = nullptr;
	}
	for(uint8_t i = 0; i < N_INSTA_SLOTS; ++ i){
		insta_t[i] = 0;
		insta_func[i] = nullptr;
	}

	/*
	 * The 16 bit registers of timer1 provide atomic
	 * read/write access by copying the high byte
	 * into a temp register when accessing the low byte.
	 */
	TCCR1A = 0; //Arduino sets the timer in fast PWM mode, cannot set OCR1AH in this mode
	TCCR1B = 0; //stop the timer
	TIFR1 = 0xff; //clear any pending interrupts
	TIMSK1 = bit(OCIE1A); //enable interrupts from this timer
	TCNT1H = 0;
	TCNT1L = 0;
	OCR1AH = highByte(PERIOD);
	OCR1AL = lowByte(PERIOD);
	TCCR1B = bit(WGM12) | bit(CS12) | bit(CS10);//start the timer with
												//1024x prescaler

	elapsed = 0;
}

void Scheduler::add(uint16_t new_time, void (*new_func)(), bool is_insta) {

	uint16_t elps = TCNT1L;
	uint16_t ub = TCNT1H;
	elps += ub << 8;

	if(new_time > 1 && elps > HALF_PERIOD)
		-- new_time;

	if(is_insta){
		uint8_t n = 0;
		while(n < N_INSTA_SLOTS && t[n] > 0)
			++ n;
		if(n == N_INSTA_SLOTS)
			return;
		insta_t[n] = new_time;
		insta_func[n] = new_func;
	}else{
		uint8_t n = 0;
		while(n < N_SLOTS && t[n] > 0)
			++ n;
		if(n == N_SLOTS)
			return;
		t[n] = new_time;
		func[n] = new_func;
	}

}

void Scheduler::del(void (*del_func)()) {

	uint8_t n = 0;
	while(n < N_SLOTS && func[n] != del_func)
		++ n;
	if(n == N_SLOTS){
		n = 0;
		while(n < N_INSTA_SLOTS && func[n] != del_func)
			++ n;
		if(n == N_INSTA_SLOTS)
			return;
		insta_t[n] = 0;
		insta_func[n] = nullptr;
		return;
	}
	t[n] = 0;
	func[n] = nullptr;

}

uint16_t Scheduler::remains(void (*r_func)()) {

	for(uint8_t i = 0; i < N_SLOTS; ++ i)
		if(func[i] == r_func)
			return t[i];

	for(uint8_t i = 0; i < N_INSTA_SLOTS; ++ i)
		if(insta_func[i] == r_func)
			return insta_t[i];

	return 0;

}

void Scheduler::onInterrupt() {

	++ elapsed;

	for(uint8_t i = 0; i < N_INSTA_SLOTS; ++ i)
		if(insta_t[i] > 0){
			-- insta_t[i];
			if(insta_t[i] == 0){
				auto exec = insta_func[i];
				insta_func[i] = nullptr;
				exec();
			}
		}
}

void Scheduler::run() {

	cli();
	auto celapsed = elapsed;
	elapsed = 0;
	sei();

	if(celapsed > 0){
		for(uint8_t i = 0; i < N_SLOTS; ++ i)
			if(t[i] == 0)
				continue;
			else if(t[i] > celapsed)
				t[i] -= celapsed;
			else{
				t[i] = 0;
				auto exec = func[i];
				func[i] = nullptr;
				exec();
			}
	}

	SMCR = bit(SE);
	__asm__ ("sleep\n");
}
