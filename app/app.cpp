/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>

//AppClass
void monitor(HttpRequest &request, HttpResponse &response); // Monitor via json some important params

void AppClass::init()
{
	ntpClient = new NtpClient("pool.ntp.org", 300);
	//TODO: add config param for TZ!
	SystemClock.setTimeZone(3);

	Wire.pins(5,4);
	lcd.begin(16, 2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
	lcd.backlight();

	tempSensor = new TempSensorsOW(ds, 4000);
#ifdef MCP23S17 //use MCP23S17
	mcp001 = new MCP(0x001, mcp23s17_cs);
#endif

#ifndef MCP23S17 //use GPIO
	input[0] = new BinInGPIOClass(15,1); // Start button
	input[1] = new BinInGPIOClass(16,0); // Stop button
#else
	input[0] = new BinInMCP23S17Class(*mcp001,1,0); // Start button
	input[1] = new BinInMCP23S17Class(*mcp001,2,0); // Stop button
#endif
	binInPoller.add(input[0]);
	binInPoller.add(input[1]);

#ifndef MCP23S17 //use GPIO
	output[0] = new BinOutGPIOClass(12,0); // Fan
	output[1] = new BinOutGPIOClass(14,0); // Pumup
	output[2] = new BinOutGPIOClass(13,0); // O3
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
	thermostats[1]->state.onChange(onStateChangeDelegate(&BinOutClass::setState, output[1]));

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
	webServer.addPath("/monitor",monitor);
//	Serial.printf("AppClass init done!\n");
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor->start();
	thermostats[1]->start();
	binInPoller.start();
#ifdef MCP23S17 //use MCP23S17
	mcp001->begin();
	mcp001->pinMode(0xFF00); // Set PORTA to OUTPUT 0x00, PORTB to INPUT 0xFF
	mcp001->pullupMode(0xFF00); // turn on internal pull-up for PORTB 0xFF
	mcp001->digitalWrite(0x00FF); //Set all PORTA to 0xFF for simple relay which is active LOW
#endif
//	Serial.printf("AppClass start done!\n");
}

void AppClass::_loop()
{
	DateTime nowTime = SystemClock.now();

	lcd.clear();
	ApplicationClass::_loop();
//	Serial.printf("AppClass loop\n");
//	Serial.printf("GPIO 15: %d GPIO 16: %d\n", input[0]->getState(), input[1]->getState());
	Serial.printf("%s - Fan: %d Pump: %d\n", nowTime.toShortTimeString(true).c_str(), thermostats[0]->state.get(), thermostats[1]->state.get());
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
//	lcd.print(_counter);
	lcd.print(nowTime.toShortTimeString(true).c_str());
}

void monitor(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	json["fan"] = output[0]->getState();
	json["pump"] = output[1]->getState();
	json["mode"] = fan->getMode();


	response.setHeader("Access-Control-Allow-Origin", "*");
	response.sendJsonObject(stream);
}
