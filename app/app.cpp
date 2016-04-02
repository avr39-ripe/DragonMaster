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

	tempSensor = new TempSensorsOW(ds, 4000);

	input[0] = new BinInGPIOClass(15,1);
	input[1] = new BinInGPIOClass(16,0);

	binInPoller.add(input[0]);
	binInPoller.add(input[1]);

	output[0] = new BinOutGPIOClass(12,0);
	output[1] = new BinOutGPIOClass(13,0);
	output[2] = new BinOutGPIOClass(14,0);

	output[0]->setState(false);
	output[1]->setState(false);
	output[2]->setState(false);

	input[0]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[0]));
	input[1]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[1]));

	thermostats[0] = new ThermostatClass(*tempSensor);
	thermostats[0]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[2]));

	ds.begin();
	tempSensor->addSensor();

	ApplicationClass::init();
//	Serial.printf("AppClass init done!\n");
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor->start();
	thermostats[0]->start();
	binInPoller.start();
//	Serial.printf("AppClass start done!\n");
}

void AppClass::_loop()
{
	lcd.clear();
	ApplicationClass::_loop();
//	Serial.printf("AppClass loop\n");
	Serial.printf("GPIO 15: %d GPIO 16: %d\n", input[0]->getState(), input[1]->getState());
	lcd.setCursor(0,0);
	lcd.print(_counter);
	lcd.setCursor(0,1);
	lcd.print(tempSensor->getTemp());
}
