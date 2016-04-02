/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>

//AppClass

void AppClass::init()
{
	Wire.pins(5,4);
	lcd.begin(16, 2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
	lcd.backlight();

	input[0] = new BinInGPIOClass(15,1);
//	input[0]->setUnitNumber(15);
//	input[0]->setPolarity(1);
	input[1] = new BinInGPIOClass(16,0);
//	input[1]->setUnitNumber(16);
//	input[1]->setPolarity(0);

	binInPoller.add(input[0]);
	binInPoller.add(input[1]);

//	pinMode(12, OUTPUT);
//	pinMode(13, OUTPUT);
//	pinMode(14, OUTPUT);

	ds.begin();
	tempSensor.addSensor();

	ApplicationClass::init();
//	Serial.printf("AppClass init done!\n");
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor.start();
	binInPoller.start();
//	Serial.printf("AppClass start done!\n");
}

void AppClass::_loop()
{
	lcd.clear();
	ApplicationClass::_loop();
//	Serial.printf("AppClass loop\n");
//	Serial.printf("GPIO 15: %d GPIO 16: %d\n", digitalRead(15), digitalRead(16));
	Serial.printf("GPIO 15: %d GPIO 16: %d\n", input[0]->getState(), input[1]->getState());
	lcd.setCursor(0,0);
	lcd.print(_counter);
	lcd.setCursor(0,1);
	lcd.print(tempSensor.getTemp());
	digitalWrite(12, state);
	state = !state;
	Serial.printf("State: , %d\n", state);
}
