#include "valve.h"

State getValveState() {
	digitalWrite(pin_stopSwitch, HIGH);
	delay(10);
	if(digitalRead(pin_isOpen) == HIGH){
		digitalWrite(pin_stopSwitch, LOW);
		return open;
	}
	if(digitalRead(pin_isClosed) == HIGH){
		digitalWrite(pin_stopSwitch, LOW);
		return closed;
	}
	digitalWrite(pin_stopSwitch, LOW);
	return undefined;
}

void onIncomplete() {

	pin_MSK = 0;
	digitalWrite(pin_stopSwitch, LOW);
	digitalWrite(pin_pwr, LOW);
	digitalWrite(pin_posSelect, LOW);
	valveReached = nullptr;
	lcd.setCursor(0, 1);
	lcd.print(F("UNDEFINED"));
	PCIFR |= bit(pin_FLG);

}

void onOpened() {

	pin_MSK = 0;
	digitalWrite(pin_stopSwitch, LOW);
	digitalWrite(pin_pwr, LOW);
	digitalWrite(pin_posSelect, LOW);
	valveReached = nullptr;
	lcd.setCursor(0, 1);
	lcd.print(F("OPEN     "));
	PCIFR |= bit(pin_FLG);

	sch.del(&onIncomplete);
}

void openValve() {

	if(getValveState() == open){
		lcd.setCursor(0, 1);
		lcd.print(F("OPEN     "));
		return;
	}

	PCIFR |= bit(pin_FLG); //Clear any pending interrupts by writing
						//logical 1 (according to documentation)
	pin_MSK = bit(pin_isOpen - pin_BSE);
	valveReached = &onOpened;

	lcd.setCursor(0, 1);
	lcd.print(F("OPENING  "));
	digitalWrite(pin_posSelect, HIGH);
	digitalWrite(pin_pwr, HIGH);
	digitalWrite(pin_stopSwitch, HIGH);

	sch.add(VALVE_TIMEOUT, &onIncomplete);

}

void onClosed() {

	pin_MSK = 0;
	digitalWrite(pin_stopSwitch, LOW);
	digitalWrite(pin_pwr, LOW);
	valveReached = nullptr;
	lcd.setCursor(0, 1);
	lcd.print(F("CLOSED   "));
	PCIFR |= bit(pin_FLG);

	sch.del(&onIncomplete);
}

void closeValve() {

	if(getValveState() == closed){
		lcd.setCursor(0, 1);
		lcd.print(F("CLOSED   "));
		return;
	}

	PCIFR |= bit(pin_FLG); //Clear any pending interrupts by writing
						//logical 1 (according to documentation)
	pin_MSK = bit(pin_isClosed - pin_BSE);
	valveReached = &onClosed;

	lcd.setCursor(0, 1);
	lcd.print(F("CLOSING  "));
	digitalWrite(pin_posSelect, LOW);
	digitalWrite(pin_pwr, HIGH);
	digitalWrite(pin_stopSwitch, HIGH);

	sch.add(VALVE_TIMEOUT, &onIncomplete);
}

void stopValve() {

	pin_MSK = 0;
	digitalWrite(pin_stopSwitch, LOW);
	digitalWrite(pin_pwr, LOW);
	valveReached = nullptr;
	lcd.setCursor(0, 1);
	lcd.print(F("STOPPED  "));
	PCIFR |= bit(pin_FLG);

	sch.del(&onIncomplete);

}
