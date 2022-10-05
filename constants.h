#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "Arduino.h"

#define pin_RS 8
#define pin_EN 9
#define pin_d4 4
#define pin_d5 5
#define pin_d6 6
#define pin_d7 7
#define pin_BL 10 //corresponds to PB2 on the ATMega 328P

#define pin_input A0

#define pin_pwr 11
#define pin_posSelect 12
#define pin_stopSwitch 3
#define pin_buzzer A4
#define pin_isOpen A1
#define pin_isClosed A2
#define pin_detectWater A3

/*
 * Pin change interrupts registers corresponding
 * to pin_isOpen and pin_isClosed
 */
#define pin_MSK PCMSK1
#define pin_FLG PCIF1
#define pin_BSE A0

#define WATER_THRESHOLD 900
#define DISPLAY_OFF_DELAY 800 //20sec
#define BUTTON_POLL 4 //100ms
#define BUTTON_DEBOUNCE 16 //400ms
#define WATER_POLL	40 //1sec
#define VALVE_TIMEOUT 400 //10sec, time before turning off valve motor
#define MAN_XBEE_UPDATE 400 //10sec, used by menuMan to check if xbee is available

enum State {open, closed, undefined};
enum Button {none = 900, select = 600, left = 400, down = 240, up = 90, right = 0};


#endif /* CONSTANTS_H_ */
