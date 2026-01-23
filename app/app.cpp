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

	// const uint8_t onewire_pin{2};
	// tempSensor = new TempSensorsOW(ds, 4000);
	// ds.begin();
	// tempSensor->addSensor();

	outputs[0] = new BinOutGPIOClass(14,0); // Осн. Дом
	outputs[1] = new BinOutGPIOClass(12,0); // С. Комната Кухня
	outputs[2] = new BinOutGPIOClass(13,0); // Газовый котел

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

	// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(16000);
	tempSensorsHttp->addSensor("http://192.168.31.217/temperature.json?sensor=0");
	tempSensorsHttp->addSensor("http://192.168.31.216/temperature.json?sensor=0");

	weekThermostats[0] = new WeekThermostatClass(*tempSensorsHttp,0,"Осн. Дом", 16000);
	weekThermostats[1] = new WeekThermostatClass(*tempSensorsHttp,1,"С. Комната Кухня", 16000);


	BinStateHttpClass* weekThermostatStateH = new BinStateHttpClass(webServer, &weekThermostats[0]->state, 2); // Термостат Осн. Дом
	binStatesHttp->add(weekThermostatStateH);

	BinStateHttpClass* weekThermostatStateBR = new BinStateHttpClass(webServer, &weekThermostats[1]->state, 3); // Термостат С. Комната Кухня
	binStatesHttp->add(weekThermostatStateBR);

	//GasHeating
	BinStateClass* gasEnable = new BinStateClass();
	gasEnable->setTrue();
	gasEnable->persistent(0);

	BinHttpButtonClass* webGasEnable = new BinHttpButtonClass(webServer, *binStatesHttp, 2, gasEnable); // Газовое отопление
	webGasEnable->state.onChange([gasEnable](uint8_t state){gasEnable->toggle(state);});

	auto caldron = new BinStateSharedDeferredClass(); // Caldron as shared by thermostats entity with deferred on/of based on delays
	caldron->setTrueDelay(caldronOnDelay);
	caldron->setFalseDelay(caldronOffDelay);

	auto caldronSet = [caldron](uint8_t state){caldron->set(state);};

	auto caldronGate = new BinStateAndClass(); // Gas enable + caldron demand
	caldronGate->addState(gasEnable);
	caldronGate->addState(caldron);
	caldronGate->onChange([](uint8_t state){outputs[2]->state.set(state);}); // Set caldron output state (output[5]) with on/off delay tolerance

	for (int i=0; i<zonesCount; ++i)
	{
		weekThermostats[i]->state.onChange([i](uint8_t state){outputs[i]->state.set(state);});
		weekThermostats[i]->state.onChange(caldronSet);
	}

	BinStateHttpClass* gasCaldronState = new BinStateHttpClass(webServer, caldronGate, 4); // Газовый котел
	binStatesHttp->add(gasCaldronState);

	uint8_t zoneIdx = 0;

	for (const auto& weekThermostat: weekThermostats)
	{
		weekThermostat->loadStateCfg();
		for(uint8_t dayIdx = 1; dayIdx < 6; ++dayIdx) // workdays
		{
			weekThermostat->_schedule[dayIdx][0].start = 0;
			weekThermostat->_schedule[dayIdx][0].targetTemp = zoneIdx == 0 ? 1700 : 1500;
			weekThermostat->_schedule[dayIdx][1].start = 360;
			weekThermostat->_schedule[dayIdx][1].targetTemp = 1900;
			weekThermostat->_schedule[dayIdx][2].start = 540;
			weekThermostat->_schedule[dayIdx][2].targetTemp = zoneIdx == 0 ? 1600 : 1900;
			weekThermostat->_schedule[dayIdx][3].start = 720;
			weekThermostat->_schedule[dayIdx][3].targetTemp = zoneIdx == 0 ? 1600 : 1900;
			weekThermostat->_schedule[dayIdx][4].start = 1050;
			weekThermostat->_schedule[dayIdx][4].targetTemp = 1900;
			weekThermostat->_schedule[dayIdx][5].start = 1320;
			weekThermostat->_schedule[dayIdx][5].targetTemp = 1800;	
		}
		
		for(uint8_t dayIdx: {0, 6}) // weekends
		{
			weekThermostat->_schedule[dayIdx][0].start = 0;
			weekThermostat->_schedule[dayIdx][0].targetTemp = zoneIdx == 0 ? 1700 : 1500;
			weekThermostat->_schedule[dayIdx][1].start = 360;
			weekThermostat->_schedule[dayIdx][1].targetTemp = 1900;
			weekThermostat->_schedule[dayIdx][2].start = 540;
			weekThermostat->_schedule[dayIdx][2].targetTemp = 1900;
			weekThermostat->_schedule[dayIdx][3].start = 720;
			weekThermostat->_schedule[dayIdx][3].targetTemp = 1900;
			weekThermostat->_schedule[dayIdx][4].start = 1050;
			weekThermostat->_schedule[dayIdx][4].targetTemp = 1900;
			weekThermostat->_schedule[dayIdx][5].start = 1320;
			weekThermostat->_schedule[dayIdx][5].targetTemp = 1800;	
		}

		weekThermostat->loadScheduleBinCfg();
		++zoneIdx;
	}

	webServer.paths.remove("/");
	webServer.paths.set("/",HttpPathDelegate(&AppClass::_httpOnIndex,this));
	webServer.paths.set("/temperatureHome.json",HttpPathDelegate(&TempSensorsHttp::onHttpGet,(TempSensors*)tempSensorsHttp));
	webServer.paths.set("/state.json", onStateJson);
	webServer.paths.set("/schedule.json", onScheduleJson);
	webServer.paths.set("/thermostats.json", onThermostatsJson);


	Serial.printf(_F("AppClass init done!\n"));
}

void AppClass::start()
{
	ApplicationClass::start();
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
}

void AppClass::_httpOnIndex(HttpRequest &request, HttpResponse &response)
{
	response.setCache(86400, true); // It's important to use cache for better performance.
	response.sendFile("thermostat.html");
}

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
