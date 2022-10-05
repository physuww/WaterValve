
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "Arduino.h"
#include "constants.h"
#include "LiquidCrystal.h"

extern LiquidCrystal lcd;

void toggleBL();
void turnOnBL();
void turnOffBL();
void turnOnDisplay();
void turnOffDisplay();
bool isDisplayOn();

#endif /* DISPLAY_H_ */
