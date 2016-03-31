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

	pinMode(15,INPUT);
	pinMode(16,INPUT);

	ds.begin();
	tempSensor.addSensor();

	ApplicationClass::init();
	Serial.printf("AppClass init done!\n");
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor.start();
	Serial.printf("AppClass start done!\n");
}

void AppClass::_loop()
{
	lcd.clear();
	ApplicationClass::_loop();
	Serial.printf("AppClass loop\n");
	Serial.printf("GPIO 15: %d GPIO 16: %d\n", digitalRead(15), digitalRead(16));
	lcd.setCursor(0,0);
	lcd.print(_counter);
	lcd.setCursor(0,1);
	lcd.print(tempSensor.getTemp());
}
