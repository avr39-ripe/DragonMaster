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
	mcp001 = new MCP(0x001, mcp23s17_cs);

	input[0] = new BinInGPIOClass(15,1); // Start button
	input[1] = new BinInGPIOClass(16,0); // Stop button

	binInPoller.add(input[0]);
	binInPoller.add(input[1]);

#ifndef MCP23S17 //use GPIO
	output[0] = new BinOutGPIOClass(12,1); // Fan
	output[1] = new BinOutGPIOClass(13,1); // Pumup
	output[2] = new BinOutGPIOClass(14,1); // O3
#else
	output[0] = new BinOutMCP23S17Class(*mcp001,1,0); // Fan
	output[1] = new BinOutMCP23S17Class(*mcp001,2,0); // Pumup
	output[2] = new BinOutMCP23S17Class(*mcp001,3,0); // O3
#endif
	output[0]->setState(false);
	output[1]->setState(false);
	output[2]->setState(false);
//	input[0]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[0]));
//	input[1]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[1]));

	thermostats[0] = new ThermostatClass(*tempSensor, ThermostatMode::HEATING, false, false, "Fan"); // Fan thermostat

//	thermostats[0]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[2]));

	thermostats[1] = new ThermostatClass(*tempSensor, ThermostatMode::COOLING, true, false, "Pump"); // Pump thermostat
	thermostats[1]->onStateChange(onStateChangeDelegate(&BinOutClass::setState, output[1]));

	fan = new FanClass(*tempSensor, *thermostats[0], *input[0], *input[1], *output[0]); // Fan controller

	ds.begin();
	tempSensor->addSensor();

	ApplicationClass::init();
	thermostats[0]->_loadBinConfig();
	thermostats[1]->_loadBinConfig();
	fan->_loadBinConfig();

	webServer.addPath("/temperature.json",HttpPathDelegate(&TempSensors::onHttpGet,tempSensor));
	webServer.addPath("/thermostat.fan",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[0]));
	webServer.addPath("/thermostat.pump",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[1]));
	webServer.addPath("/fan",HttpPathDelegate(&FanClass::onHttpConfig,fan));
//	Serial.printf("AppClass init done!\n");
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor->start();
	thermostats[1]->start();
	binInPoller.start();
	mcp001->begin();
	mcp001->pinMode(0xFF00); // Set PORTA to output
	mcp001->digitalWrite(0x00FF); //Set all PORTA to 0xFF for simple relay which is active LOW
//	Serial.printf("AppClass start done!\n");
}

void AppClass::_loop()
{
	lcd.clear();
	ApplicationClass::_loop();
//	Serial.printf("AppClass loop\n");
//	Serial.printf("GPIO 15: %d GPIO 16: %d\n", input[0]->getState(), input[1]->getState());
	Serial.printf("Free Heap: %d\r\n", system_get_free_heap_size());
	lcd.setCursor(0,0);
	switch (fan->getMode())
	{
	case FanMode::IDLE:
		lcd.print("IDLE ");
		break;
	case FanMode::START:
		lcd.print("START");
		break;
	case FanMode::RUN:
		lcd.print("RUN  ");
		break;
	case FanMode::PERIODIC:
		lcd.print("PERID");
		break;
	case FanMode::STOP:
		lcd.print("STOP ");
		break;
	};
	lcd.printf(" F:%d", output[0]->getState());
	lcd.printf(" P:%d", output[1]->getState());
	lcd.setCursor(0,1);
	lcd.print(tempSensor->getTemp());
	lcd.setCursor(7,1);
	lcd.print(_counter);
}
