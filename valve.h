#ifndef VALVE_H_
#define VALVE_H_

#include "Arduino.h"
#include "LiquidCrystal.h"
#include "constants.h"
#include "scheduler.h"

extern LiquidCrystal lcd;
extern void (*valveReached) ();
extern Scheduler sch;

State getValveState();
void onIncomplete();
void onOpened();
void openValve();
void onClosed();
void closeValve();
void stopValve();



#endif /* VALVE_H_ */
