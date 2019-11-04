/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>

void AppClass::init()
{
	ApplicationClass::init();
	NtpClient* ntpClient = new NtpClient("pool.ntp.org", 300);

	BinStatesHttpClass* binStatesHttp = new BinStatesHttpClass();
	wsAddBinGetter(binStatesHttp->sysId, WebsocketBinaryDelegate(&BinStatesHttpClass::wsBinGetter,binStatesHttp));
	wsAddBinSetter(binStatesHttp->sysId, WebsocketBinaryDelegate(&BinStatesHttpClass::wsBinSetter,binStatesHttp));

	auto zone1Out = new BinOutGPIOClass(12,0);
	auto zone2Out = new BinOutGPIOClass(13,0);
	auto zone3Out = new BinOutGPIOClass(14,0);
	auto caldronOut = new BinOutGPIOClass(15,0);

	zone1Out->state.set(false);
	zone2Out->state.set(false);
	zone3Out->state.set(false);
	caldronOut->state.set(false);

	BinStateHttpClass* zone1State = new BinStateHttpClass(webServer, &zone1Out->state, /*"Зона 1",*/ 0);
	binStatesHttp->add(zone1State);

	BinStateHttpClass* zone2State = new BinStateHttpClass(webServer, &zone2Out->state, /*"Зона 2",*/ 1);
	binStatesHttp->add(zone2State);

	BinStateHttpClass* zone3State = new BinStateHttpClass(webServer, &zone3Out->state, /*"Зона 3",*/ 2);
	binStatesHttp->add(zone3State);


	// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(4000);
	tempSensorsHttp->addSensor("http://10.2.113.125/temperature.json?sensor=0");

	auto thermostatZone1 = new ThermostatClass(*tempSensorsHttp, 0, ThermostatMode::HEATING, true, false, "Zone1"); // Pump thermostat
	thermostatZone1->state.onChange([=](uint8_t state){zone1Out->state.set(state);});
	thermostatZone1->_loadBinConfig();
	thermostatZone1->enable(true);

	auto thermostatZone2 = new ThermostatClass(*tempSensorsHttp, 0, ThermostatMode::HEATING, true, false, "Zone2"); // Pump thermostat
	thermostatZone2->state.onChange([=](uint8_t state){zone2Out->state.set(state);});
	thermostatZone2->_loadBinConfig();
	thermostatZone2->disable(true);

	auto thermostatZone3 = new ThermostatClass(*tempSensorsHttp, 0, ThermostatMode::HEATING, true, false, "Zone3"); // Pump thermostat
	thermostatZone3->state.onChange([=](uint8_t state){zone3Out->state.set(state);});
	thermostatZone2->_loadBinConfig();
	thermostatZone2->disable(true);

	auto caldron = new BinStateSharedDeferredClass();
	caldron->setTrueDelay(0);
	caldron->setFalseDelay(0);
	caldron->onChange([=](uint8_t state){caldronOut->state.set(state);});

	BinHttpButtonClass* webStart = new BinHttpButtonClass(webServer, *binStatesHttp, 0/*, "Старт"*/);
//	webStart->state.onChange([=](uint8_t state){fan->_modeStart(state);});
	BinHttpButtonClass* webStop = new BinHttpButtonClass(webServer, *binStatesHttp, 1/*, "Стоп"*/);
//	webStop->state.onChange([=](uint8_t state){fan->_modeStop(state);});

	//GasHeating
	BinStateClass* gasEnable = new BinStateClass();
	gasEnable->persistent(0);

	BinHttpButtonClass* webGasEnable = new BinHttpButtonClass(webServer, *binStatesHttp, 2, /*"Газовое отопление",*/ gasEnable);
	//webGasEnable->state.onChange(std::bind(&BinStateClass::toggle, gasEnable, std::placeholders::_1));
	webGasEnable->state.onChange([=](uint8_t state){gasEnable->toggle(state); Serial.printf("LAMBDA!\n");});

	BinStateAndClass* gasCaldron = new BinStateAndClass();
	gasCaldron->addState(gasEnable);
//	gasCaldron->addState(&weekThermostats[0]->state);
//	gasCaldron->onChange([=](uint8_t state){output[2]->state.set(state);});

	BinStateHttpClass* gasCaldronState = new BinStateHttpClass(webServer, gasCaldron, /*"Газовый котел",*/ 3);
	binStatesHttp->add(gasCaldronState);
	//GasHeating

//	webServer.paths.set("/temperature.json",HttpPathDelegate(&TempSensors::onHttpGet,tempSensor));
	webServer.paths.set("/temperatureHome.json",HttpPathDelegate(&TempSensorsHttp::onHttpGet,(TempSensors*)tempSensorsHttp));
//	webServer.paths.set("/thermostat.fan",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[0]));
//	webServer.paths.set("/thermostat.pump",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[1]));
//	webServer.paths.set("/thermostat.pump_cooler",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[2]));
	Serial.printf(_F("AppClass init done!\n"));
}

void AppClass::start()
{
	ApplicationClass::start();
//	tempSensor->start();
//	thermostats[1]->start();
//	binInPoller.start();
#ifdef MCP23S17 //use MCP23S17
	mcp001->begin();
	mcp001->pinMode(0xFF00); // Set PORTA to OUTPUT 0x00, PORTB to INPUT 0xFF
	mcp001->pullupMode(0xFF00); // turn on internal pull-up for PORTB 0xFF
	mcp001->digitalWrite(0x00FF); //Set all PORTA to 0xFF for simple relay which is active LOW
#endif
//
}

void AppClass::_loop()
{
	DateTime nowTime = SystemClock.now();

//	lcd.clear();
	ApplicationClass::_loop();
//	Serial.printf(_F("AppClass loop\n"));
//	Serial.printf("GPIO 15: %d GPIO 16: %d\n", input[0]->getState(), input[1]->getState());
//	Serial.printf("%s - Fan: %d Pump: %d\n", nowTime.toShortTimeString(true).c_str(), thermostats[0]->state.get(), thermostats[1]->state.get());
	Serial.printf("Free Heap: %d WS count: %d Counter: %d\n", system_get_free_heap_size(), WebsocketConnection::getActiveWebsockets().count(), _counter);
/*	lcd.setCursor(0,0);
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
	*/
}

void AppClass::userSTAGotIP(IpAddress ip, IpAddress mask, IpAddress gateway)
{
	Serial.printf(_F("AppClass STA GOT IP\n"));
	tempSensorsHttp->start();
//	for (auto _thermostat: weekThermostats)
//		_thermostat->start();
}
