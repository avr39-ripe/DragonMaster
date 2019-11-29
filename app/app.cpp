/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>

// Forward declaration of weekThermostat ugly web responders
void onStateJson(HttpRequest &request, HttpResponse &response);
void onScheduleJson(HttpRequest &request, HttpResponse &response);
void onThermostatsJson(HttpRequest &request, HttpResponse &response);

void AppClass::init()
{
	ApplicationClass::init();
	NtpClient* ntpClient = new NtpClient("pool.ntp.org", 300);

	BinStatesHttpClass* binStatesHttp = new BinStatesHttpClass();
	wsAddBinGetter(binStatesHttp->sysId, WebsocketBinaryDelegate(&BinStatesHttpClass::wsBinGetter,binStatesHttp));
	wsAddBinSetter(binStatesHttp->sysId, WebsocketBinaryDelegate(&BinStatesHttpClass::wsBinSetter,binStatesHttp));

	Wire.pins(4,5);
	lcd.begin(16, 2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
	lcd.backlight();

	const uint8_t onewire_pin{2};
	tempSensor = new TempSensorsOW(ds, 4000);
	ds.begin();
	tempSensor->addSensor();

	BinInClass* inputs[2];
	inputs[0] = new BinInGPIOClass(15,1); // Start button
	inputs[1] = new BinInGPIOClass(16,0); // Stop button

	binInPoller = new BinInPollerClass;
	binInPoller->add(inputs[0]);
	binInPoller->add(inputs[1]);

	outputs[0] = new BinOutGPIOClass(12,1); // Fan
	outputs[1] = new BinOutGPIOClass(13,1); // Pumup
	outputs[2] = new BinOutGPIOClass(3,1); // Gas Caldron

	outputs[0]->state.set(false);
	outputs[1]->state.set(false);
	outputs[2]->state.set(false);

	BinStateHttpClass* fanState = new BinStateHttpClass(webServer, &outputs[0]->state, 0); // Вентилятор
	binStatesHttp->add(fanState);
	BinStateHttpClass* pumpState = new BinStateHttpClass(webServer, &outputs[1]->state, 1); // Насос
	binStatesHttp->add(pumpState);

	thermostats[0] = new ThermostatClass(*tempSensor, 0, ThermostatMode::HEATING, false, false, "Fan"); // Fan thermostat
	thermostats[1] = new ThermostatClass(*tempSensor, 0, ThermostatMode::COOLING, true, false, "Pump"); // Pump thermostat
	thermostats[1]->state.onChange([](uint8_t state){outputs[1]->state.set(state);});
	thermostats[2] = new ThermostatClass(*tempSensor, 0, ThermostatMode::COOLING, true, false, "Pump_cooler"); // Pump cooler thermostat
	thermostats[2]->state.onChange([](uint8_t state){outputs[1]->state.set(state);});

	fan = new FanClass(*tempSensor, *thermostats[0], *outputs[0]); // Fan controller
	inputs[0]->state.onChange([](uint8_t state){fan->_modeStart(state);});
	inputs[1]->state.onChange([](uint8_t state){fan->_modeStop(state);});

	BinHttpButtonClass* webStart = new BinHttpButtonClass(webServer, *binStatesHttp, 0); // Старт
	webStart->state.onChange([](uint8_t state){fan->_modeStart(state);});
	BinHttpButtonClass* webStop = new BinHttpButtonClass(webServer, *binStatesHttp, 1); // Стоп
	webStop->state.onChange([](uint8_t state){fan->_modeStop(state);});


	thermostats[0]->_loadBinConfig();
	thermostats[1]->_loadBinConfig();
	thermostats[2]->_loadBinConfig();
	fan->_loadBinConfig();

	// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(16000);
	tempSensorsHttp->addSensor("http://192.168.31.217/temperature.json?sensor=0");

	weekThermostat = new WeekThermostatClass(*tempSensorsHttp,0,"Дом", 16000);
	weekThermostat->loadStateCfg();


	BinStateHttpClass* weekThermostatState = new BinStateHttpClass(webServer, &weekThermostat->state, 2); // Термостат Дом
	binStatesHttp->add(weekThermostatState);

	fan->setThermostatControlState(true);
	fan->periodicDisable(true);

	thermostats[1]->disable(true);
	thermostats[2]->enable(true);


	fan->active.onChange([](uint8_t state){thermostats[1]->enable(state);});
	fan->active.onChange([](uint8_t state){thermostats[2]->disable(state);});

	//GasHeating
	BinStateClass* gasEnable = new BinStateClass();
	gasEnable->persistent(0);

	BinHttpButtonClass* webGasEnable = new BinHttpButtonClass(webServer, *binStatesHttp, 2, gasEnable); // Газовое отопление
	webGasEnable->state.onChange([gasEnable](uint8_t state){gasEnable->toggle(state);});

	BinStateAndClass* gasCaldron = new BinStateAndClass();
	gasCaldron->addState(gasEnable);
	gasCaldron->addState(&weekThermostat->state);
	gasCaldron->onChange([](uint8_t state){outputs[2]->state.set(state);});

	BinStateHttpClass* gasCaldronState = new BinStateHttpClass(webServer, gasCaldron, 3); // Газовый котел
	binStatesHttp->add(gasCaldronState);


	for(uint8_t i = 0; i< 7; i++)
	{
		weekThermostat->_schedule[i][0].start = 0;
		weekThermostat->_schedule[i][0].targetTemp = 800;
		weekThermostat->_schedule[i][1].start = 360;
		weekThermostat->_schedule[i][1].targetTemp = 1800;
		weekThermostat->_schedule[i][2].start = 540;
		weekThermostat->_schedule[i][2].targetTemp = 1200;
		weekThermostat->_schedule[i][3].start = 720;
		weekThermostat->_schedule[i][3].targetTemp = 1500;
		weekThermostat->_schedule[i][4].start = 1020;
		weekThermostat->_schedule[i][4].targetTemp = 1800;
		weekThermostat->_schedule[i][5].start = 1320;
		weekThermostat->_schedule[i][5].targetTemp = 800;

		weekThermostat->loadScheduleBinCfg();
	}


	webServer.paths.remove("/");
	webServer.paths.set("/",HttpPathDelegate(&AppClass::_httpOnIndex,this));
	webServer.paths.set("/temperature.json",HttpPathDelegate(&TempSensors::onHttpGet,tempSensor));
	webServer.paths.set("/temperatureHome.json",HttpPathDelegate(&TempSensorsHttp::onHttpGet,(TempSensors*)tempSensorsHttp));
	webServer.paths.set("/thermostat.fan",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[0]));
	webServer.paths.set("/thermostat.pump",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[1]));
	webServer.paths.set("/thermostat.pump_cooler",HttpPathDelegate(&ThermostatClass::onHttpConfig,thermostats[2]));
	webServer.paths.set("/fan",HttpPathDelegate(&FanClass::onHttpConfig,fan));
	webServer.paths.set("/state.json", onStateJson);
	webServer.paths.set("/schedule.json", onScheduleJson);
	webServer.paths.set("/thermostats.json", onThermostatsJson);


	Serial.printf(_F("AppClass init done!\n"));
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor->start();
	thermostats[1]->start();
	binInPoller->start();
}

void AppClass::_loop()
{
	ApplicationClass::_loop();

	DateTime nowTime = SystemClock.now();

	lcd.clear();

	Serial.printf(_F("%s - Fan: %d Pump: %d\n"), nowTime.toShortTimeString(true).c_str(), thermostats[0]->state.get(), thermostats[1]->state.get());

	lcd.setCursor(0,0);
	switch (fan->getMode())
	{
	case FanMode::IDLE:
		lcd.print(_F("IDLE "));
		break;
	case FanMode::START:
		lcd.print(_F("START"));
		break;
	case FanMode::RUN:
		lcd.print(_F("RUN  "));
		break;
	case FanMode::PERIODIC:
		lcd.print(_F("PERID"));
		break;
	case FanMode::STOP:
		lcd.print(_F("STOP "));
		break;
	};
	lcd.printf(_F(" F:%d"), outputs[0]->state.get());
	lcd.printf(_F(" P:%d"), outputs[1]->state.get());
	lcd.setCursor(0,1);
	lcd.print(tempSensor->getTemp());
	lcd.setCursor(7,1);
	lcd.print(nowTime.toShortTimeString(true).c_str());

	Serial.printf(_F("Free Heap: %d WS count: %d Counter: %d\n"), system_get_free_heap_size(), WebsocketConnection::getActiveWebsockets().count(), _counter);
}

void AppClass::userSTAGotIP(IpAddress ip, IpAddress mask, IpAddress gateway)
{
	Serial.printf(_F("AppClass STA GOT IP\n"));
	tempSensorsHttp->start();
	weekThermostat->start();
}

void AppClass::_httpOnIndex(HttpRequest &request, HttpResponse &response)
{
	response.setCache(86400, true); // It's important to use cache for better performance.
	response.sendFile("thermostat.html");
}

void onStateJson(HttpRequest &request, HttpResponse &response)
{
	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
	weekThermostat->onStateCfg(request,response);
}

void onScheduleJson(HttpRequest &request, HttpResponse &response)
{
	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
	weekThermostat->onScheduleCfg(request,response);
}

void onThermostatsJson(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& root = stream->getRoot();
	root[(String)0] = weekThermostat->getName();

	response.setAllowCrossDomainOrigin("*");
	response.sendDataStream(stream, MIME_JSON);
}
