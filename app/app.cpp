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
void onStateJson(HttpRequest &request, HttpResponse &response);
void onScheduleJson(HttpRequest &request, HttpResponse &response);
void onThermostatsJson(HttpRequest &request, HttpResponse &response);

void AppClass::init()
{
	ApplicationClass::init();
	ntpClient = new NtpClient("pool.ntp.org", 300);

	BinStatesHttpClass* binStatesHttp = new BinStatesHttpClass();
	wsAddBinGetter(binStatesHttp->sysId, WebSocketBinaryDelegate(&BinStatesHttpClass::wsBinGetter,binStatesHttp));
	wsAddBinSetter(binStatesHttp->sysId, WebSocketBinaryDelegate(&BinStatesHttpClass::wsBinSetter,binStatesHttp));

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
//	output[0] = new BinOutGPIOClass(12,1); // Fan
//	output[1] = new BinOutGPIOClass(13,1); // Pumup
//	output[2] = new BinOutGPIOClass(14,1); // O3
	output[0] = new BinOutGPIOClass(12,0); // Fan
	output[1] = new BinOutGPIOClass(14,0); // Pumup
	output[2] = new BinOutGPIOClass(13,0); // O3
#else
	output[0] = new BinOutMCP23S17Class(*mcp001,1,0); // Fan
	output[1] = new BinOutMCP23S17Class(*mcp001,2,0); // Pumup
	output[2] = new BinOutMCP23S17Class(*mcp001,3,0); // O3
#endif
	output[0]->state.set(false);
	output[1]->state.set(false);
	output[2]->state.set(false);

	BinStateHttpClass* fanState = new BinStateHttpClass(webServer, &output[0]->state, "Вентилятор", 0);
	binStatesHttp->add(fanState);

	BinStateHttpClass* pumpState = new BinStateHttpClass(webServer, &output[1]->state, "Насос", 1);
	binStatesHttp->add(pumpState);


//	input[0]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[0]));
//	input[1]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[1]));

	thermostats[0] = new ThermostatClass(*tempSensor, ThermostatMode::HEATING, false, false, "Fan"); // Fan thermostat

//	thermostats[0]->onStateChange(onStateChangeDelegate(&BinOutGPIOClass::setState, output[2]));

	thermostats[1] = new ThermostatClass(*tempSensor, ThermostatMode::COOLING, true, false, "Pump"); // Pump thermostat
	thermostats[1]->state.onChange(onStateChangeDelegate(&BinStateClass::set, &output[1]->state));

	thermostats[2] = new ThermostatClass(*tempSensor, ThermostatMode::COOLING, true, false, "Pump_safety"); // Pump thermostat
	thermostats[2]->state.onChange(onStateChangeDelegate(&BinStateClass::set, &output[1]->state));

	fan = new FanClass(*tempSensor, *thermostats[0], *output[0]); // Fan controller
	input[0]->state.onChange(onStateChangeDelegate(&FanClass::_modeStart, fan));
	input[1]->state.onChange(onStateChangeDelegate(&FanClass::_modeStop, fan));

	BinHttpButtonClass* webStart = new BinHttpButtonClass(webServer, *binStatesHttp, 0, "Старт");
	webStart->state.onChange(onStateChangeDelegate(&FanClass::_modeStart, fan));

	BinHttpButtonClass* webStop = new BinHttpButtonClass(webServer, *binStatesHttp, 1, "Стоп");
	webStop->state.onChange(onStateChangeDelegate(&FanClass::_modeStop, fan));

	ds.begin();
	tempSensor->addSensor();

	thermostats[0]->_loadBinConfig();
	thermostats[1]->_loadBinConfig();
	thermostats[2]->_loadBinConfig();
	fan->_loadBinConfig();

// http tempsensors + Week Thermostat
//	tempSensorsHttp = new TempSensorsHttp(4000);
//	tempSensorsHttp->addSensor("http://10.2.113.116/temperature.json?sensor=0"); // House tempsensor

//	weekThermostats[0] = new WeekThermostatClass(*tempSensorsHttp,0,"House", 4000);

//	BinStateHttpClass* weekThermostatState = new BinStateHttpClass(webServer, &weekThermostats[0]->state, "Термостат Дом", 2);
//	binStatesHttp->add(weekThermostatState);
//	weekThermostats[0]->state.onChange(onStateChangeDelegate(&FanClass::setThermostatControlState, fan));
//	weekThermostats[0]->state.onChange(onStateChangeDelegate(&FanClass::periodicDisable, fan));
//	weekThermostats[0]->state.onChange(onStateChangeDelegate(&ThermostatClass::enable, thermostats[1]));
//	weekThermostats[0]->state.onChange(onStateChangeDelegate(&ThermostatClass::disable, thermostats[2]));
	fan->setThermostatControlState(true);
	fan->periodicDisable(true);
	thermostats[1]->enable(true);
	thermostats[2]->disable(true);



//	for(uint8_t i = 0; i< 7; i++)
//	{
//		for (auto _thermostat: weekThermostats)
//		{
//			_thermostat->_schedule[i][0].start = 0;
//			_thermostat->_schedule[i][0].targetTemp = 800;
//			_thermostat->_schedule[i][1].start = 360;
//			_thermostat->_schedule[i][1].targetTemp = 1800;
//			_thermostat->_schedule[i][2].start = 540;
//			_thermostat->_schedule[i][2].targetTemp = 1200;
//			_thermostat->_schedule[i][3].start = 720;
//			_thermostat->_schedule[i][3].targetTemp = 1500;
//			_thermostat->_schedule[i][4].start = 1020;
//			_thermostat->_schedule[i][4].targetTemp = 1800;
//			_thermostat->_schedule[i][5].start = 1320;
//			_thermostat->_schedule[i][5].targetTemp = 800;
//
//			_thermostat->loadStateCfg();
//			_thermostat->loadScheduleBinCfg();
//		}
//	}
	webServer.addPath("/temperature.json",HttpPathDelegate(&TempSensors::onHttpGet,tempSensor));
	webServer.addPath("/thermostat.fan",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[0]));
	webServer.addPath("/thermostat.pump",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[1]));
	webServer.addPath("/thermostat.pump_safety",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[2]));
	webServer.addPath("/fan",HttpPathDelegate(&FanClass::onHttpConfig,fan));
	webServer.addPath("/monitor",monitor);
//	webServer.addPath("/state.json", onStateJson);
//	webServer.addPath("/schedule.json", onScheduleJson);
//	webServer.addPath("/thermostats.json", onThermostatsJson);
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
	lcd.printf(" F:%d", output[0]->state.get());
	lcd.printf(" P:%d", output[1]->state.get());
	lcd.setCursor(0,1);
	lcd.print(tempSensor->getTemp());
	lcd.setCursor(7,1);
//	lcd.print(_counter);
	lcd.print(nowTime.toShortTimeString(true).c_str());
}

void AppClass::userSTAGotIP(IPAddress ip, IPAddress mask, IPAddress gateway)
{
//	tempSensorsHttp->start();
//	for (auto _thermostat: weekThermostats)
//		_thermostat->start();
}
void monitor(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	json["fan"] = output[0]->state.get();
	json["pump"] = output[1]->state.get();
	json["mode"] = fan->getMode();


	response.setHeader("Access-Control-Allow-Origin", "*");
	response.sendJsonObject(stream);
}

//void onStateJson(HttpRequest &request, HttpResponse &response)
//{
//	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
//	weekThermostats[currThermostat]->onStateCfg(request,response);
//}
//
//void onScheduleJson(HttpRequest &request, HttpResponse &response)
//{
//	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
//	weekThermostats[currThermostat]->onScheduleCfg(request,response);
//}
//
//void onThermostatsJson(HttpRequest &request, HttpResponse &response)
//{
//	DynamicJsonBuffer jsonBuffer;
//	JsonObject& root = jsonBuffer.createObject();
//	for (uint t=0; t < maxWeekThermostats; t++)
//	{
//		root[(String)t] = weekThermostats[t]->getName();
//
//	}
//	char buf[scheduleFileBufSize];
//	root.printTo(buf, sizeof(buf));
//
//	response.setHeader("Access-Control-Allow-Origin", "*");
//	response.setContentType(ContentType::JSON);
//	response.sendString(buf);
//}
