/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>

void std::__throw_length_error(char const*) { while (true) {} }

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

	outputs[0] = new BinOutGPIOClass(14,0); // Fan
	outputs[1] = new BinOutGPIOClass(12,0); // Pumup
	outputs[2] = new BinOutGPIOClass(13,0); // Gas Caldron

	outputs[0]->state.set(false);
	outputs[1]->state.set(false);
	outputs[2]->state.set(false);

	int i = 0;
	for (auto output :outputs)
	{
		output->state.set(false);
		BinHttpButtonClass* httpButton = new BinHttpButtonClass(webServer, *binStatesHttp, i++, &output->state);
		auto togglerFunc = [output](uint8_t state){output->state.toggle(state);};
		httpButton->state.onChange(togglerFunc);
	}
	// BinStateHttpClass* fanState = new BinStateHttpClass(webServer, &outputs[0]->state, 0, outputs[0]->state); // Вентилятор
	// binStatesHttp->add(fanState);
	// BinStateHttpClass* pumpState = new BinStateHttpClass(webServer, &outputs[1]->state, 1, outputs[1]->state); // Насос
	// binStatesHttp->add(pumpState);

	// BinHttpButtonClass* webStart = new BinHttpButtonClass(webServer, *binStatesHttp, 0); // Старт
	// webStart->state.onChange([](uint8_t state){outputs[0]->state.toggle(state);});
	// BinHttpButtonClass* webStop = new BinHttpButtonClass(webServer, *binStatesHttp, 1); // Стоп
	// webStop->state.onChange([](uint8_t state){outputs[1]->state.toggle(state);});

	// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(16000);
	tempSensorsHttp->addSensor("http://192.168.31.217/temperature.json?sensor=0");
	tempSensorsHttp->addSensor("http://192.168.31.216/temperature.json?sensor=0");

	// weekThermostat = new WeekThermostatClass(*tempSensorsHttp,0,"Дом", 16000);
	// weekThermostat->loadStateCfg();

	weekThermostats[0] = new WeekThermostatClass(*tempSensorsHttp,0,"Осн. Дом", 16000);
	weekThermostats[1] = new WeekThermostatClass(*tempSensorsHttp,1,"С. Комната Кухня", 16000);


	BinStateHttpClass* weekThermostatStateH = new BinStateHttpClass(webServer, &weekThermostats[0]->state, 2); // Термостат Осн. Дом
	binStatesHttp->add(weekThermostatStateH);

	BinStateHttpClass* weekThermostatStateBR = new BinStateHttpClass(webServer, &weekThermostats[1]->state, 3); // Термостат С. Комната Кухня
	binStatesHttp->add(weekThermostatStateBR);

	//GasHeating
	BinStateClass* gasEnable = new BinStateClass();
	gasEnable->persistent(0);

	BinHttpButtonClass* webGasEnable = new BinHttpButtonClass(webServer, *binStatesHttp, 2, gasEnable); // Газовое отопление
	webGasEnable->state.onChange([gasEnable](uint8_t state){gasEnable->toggle(state);});

	// BinStateAndClass* gasCaldron = new BinStateAndClass();
	// gasCaldron->addState(gasEnable);
	// gasCaldron->addState(&weekThermostats[0]->state);
	// gasCaldron->addState(&weekThermostats[1]->state);
	// gasCaldron->onChange([](uint8_t state){outputs[2]->state.set(state);});

	auto caldron = new BinStateSharedDeferredClass(); // Caldron as shared by thermostats entity with deferred on/of based on delays
	caldron->setTrueDelay(caldronOnDelay);
	caldron->setFalseDelay(0);
	caldron->onChange([](uint8_t state){outputs[2]->state.set(state);}); // Set caldron output state (output[5]) with on/off delay tolerance

	auto caldronSet = [caldron](uint8_t state){caldron->set(state);};

	for (int i=0; i<zonesCount; ++i)
	{
		weekThermostats[i]->state.onChange([i](uint8_t state){outputs[i]->state.set(state);});
		weekThermostats[i]->state.onChange(caldronSet);
	}

	BinStateHttpClass* gasCaldronState = new BinStateHttpClass(webServer, caldron, 4); // Газовый котел
	binStatesHttp->add(gasCaldronState);


	// for(uint8_t i = 0; i< 7; i++)
	// {
	// 	weekThermostat->_schedule[i][0].start = 0;
	// 	weekThermostat->_schedule[i][0].targetTemp = 800;
	// 	weekThermostat->_schedule[i][1].start = 360;
	// 	weekThermostat->_schedule[i][1].targetTemp = 1800;
	// 	weekThermostat->_schedule[i][2].start = 540;
	// 	weekThermostat->_schedule[i][2].targetTemp = 1200;
	// 	weekThermostat->_schedule[i][3].start = 720;
	// 	weekThermostat->_schedule[i][3].targetTemp = 1500;
	// 	weekThermostat->_schedule[i][4].start = 1020;
	// 	weekThermostat->_schedule[i][4].targetTemp = 1800;
	// 	weekThermostat->_schedule[i][5].start = 1320;
	// 	weekThermostat->_schedule[i][5].targetTemp = 800;

	// 	weekThermostat->loadScheduleBinCfg();
	// }

	for (const auto& weekThermostat: weekThermostats)
	{
		weekThermostat->loadStateCfg();
		for(uint8_t i = 0; i< 7; i++)
		{
			weekThermostat->_schedule[i][0].start = 0;
			weekThermostat->_schedule[i][0].targetTemp = 1800;
			weekThermostat->_schedule[i][1].start = 360;
			weekThermostat->_schedule[i][1].targetTemp = 2100;
			weekThermostat->_schedule[i][2].start = 540;
			weekThermostat->_schedule[i][2].targetTemp = 2100;
			weekThermostat->_schedule[i][3].start = 720;
			weekThermostat->_schedule[i][3].targetTemp = 2100;
			weekThermostat->_schedule[i][4].start = 1020;
			weekThermostat->_schedule[i][4].targetTemp = 2100;
			weekThermostat->_schedule[i][5].start = 1320;
			weekThermostat->_schedule[i][5].targetTemp = 1800;

			weekThermostat->loadScheduleBinCfg();
		}
	}

	webServer.paths.remove("/");
	webServer.paths.set("/",HttpPathDelegate(&AppClass::_httpOnIndex,this));
	webServer.paths.set("/temperature.json",HttpPathDelegate(&TempSensors::onHttpGet,tempSensor));
	webServer.paths.set("/temperatureHome.json",HttpPathDelegate(&TempSensorsHttp::onHttpGet,(TempSensors*)tempSensorsHttp));
	webServer.paths.set("/state.json", onStateJson);
	webServer.paths.set("/schedule.json", onScheduleJson);
	webServer.paths.set("/thermostats.json", onThermostatsJson);


	Serial.printf(_F("AppClass init done!\n"));
}

void AppClass::start()
{
	ApplicationClass::start();
	tempSensor->start();
	binInPoller->start();
	for (const auto& weekThermostat: weekThermostats)
	{
		weekThermostat->start();
	}
}

void AppClass::_loop()
{
	ApplicationClass::_loop();

	DateTime nowTime = SystemClock.now();

	Serial.printf(_F("Free Heap: %d WS count: %d Counter: %d\n"), system_get_free_heap_size(), WebsocketConnection::getActiveWebsockets().count(), _counter);
}

void AppClass::userSTAGotIP(IpAddress ip, IpAddress mask, IpAddress gateway)
{
	Serial.printf(_F("AppClass STA GOT IP\n"));
	tempSensorsHttp->start();
	//weekThermostat->start();
}

void AppClass::_httpOnIndex(HttpRequest &request, HttpResponse &response)
{
	response.setCache(86400, true); // It's important to use cache for better performance.
	response.sendFile("thermostat.html");
}

// void onStateJson(HttpRequest &request, HttpResponse &response)
// {
// 	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
// 	weekThermostat->onStateCfg(request,response);
// }

// void onScheduleJson(HttpRequest &request, HttpResponse &response)
// {
// 	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
// 	weekThermostat->onScheduleCfg(request,response);
// }

// void onThermostatsJson(HttpRequest &request, HttpResponse &response)
// {
// 	JsonObjectStream* stream = new JsonObjectStream();
// 	JsonObject& root = stream->getRoot();
// 	root[(String)0] = weekThermostat->getName();

// 	response.setAllowCrossDomainOrigin("*");
// 	response.sendDataStream(stream, MIME_JSON);
// }

void onStateJson(HttpRequest &request, HttpResponse &response)
{
	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
	weekThermostats[currThermostat]->onStateCfg(request,response);
}

void onScheduleJson(HttpRequest &request, HttpResponse &response)
{
	uint8_t currThermostat = request.getQueryParameter("thermostat").toInt();
	weekThermostats[currThermostat]->onScheduleCfg(request,response);
}

void onThermostatsJson(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& root = stream->getRoot();
	for (uint8_t t=0; t < zonesCount; t++)
	{
		root[(String)t] = weekThermostats[t]->getName();

	}
	response.setAllowCrossDomainOrigin("*");
	response.sendDataStream(stream, MIME_JSON);
}