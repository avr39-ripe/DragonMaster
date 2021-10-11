/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>

namespace std
{
       void __throw_length_error(char const*)
       {
               while (true) {}
       }
}

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

	outputs[0] = new BinOutGPIOClass(2,1); // Холл
	outputs[1] = new BinOutGPIOClass(0,1); // Кухня
	outputs[2] = new BinOutGPIOClass(4,1); // Спальня
	outputs[3] = new BinOutGPIOClass(5,1); // Детская 1
	outputs[4] = new BinOutGPIOClass(13,1); // Детская 2
	outputs[5] = new BinOutGPIOClass(12,1); // Котел
	outputs[6] = new BinOutGPIOClass(14,1);
	outputs[7] = new BinOutGPIOClass(16,1);

	int i = 0;
	for (auto output :outputs)
	{
		output->state.set(false);
		BinHttpButtonClass* httpButton = new BinHttpButtonClass(webServer, *binStatesHttp, i++, &output->state);
		auto togglerFunc = [output](uint8_t state){output->state.toggle(state);};
		httpButton->state.onChange(togglerFunc);
	}


	//1 ec:fa:bc:35:ab:ae
	//2 ec:fa:bc:35:a3:1e
	//3 ec:fa:bc:35:ac:73
	//4 ec:fa:bc:35:a6:f9
	//5 5c:cf:7f:76:60:6e

	// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(16000);
	tempSensorsHttp->addSensor("http://192.168.0.160/temperature.json?sensor=0");
	tempSensorsHttp->addSensor("http://192.168.0.161/temperature.json?sensor=0");
	tempSensorsHttp->addSensor("http://192.168.0.162/temperature.json?sensor=0");
	tempSensorsHttp->addSensor("http://192.168.0.163/temperature.json?sensor=0");
	tempSensorsHttp->addSensor("http://192.168.0.164/temperature.json?sensor=0");


	weekThermostats[0] = new WeekThermostatClass(*tempSensorsHttp,0,"Холл", 16000);
	weekThermostats[1] = new WeekThermostatClass(*tempSensorsHttp,1,"Ванная", 16000);
	weekThermostats[2] = new WeekThermostatClass(*tempSensorsHttp,2,"Спальня", 16000);
	weekThermostats[3] = new WeekThermostatClass(*tempSensorsHttp,3,"Детская 1", 16000);
	weekThermostats[4] = new WeekThermostatClass(*tempSensorsHttp,4,"Детская 2", 16000);

	auto caldron = new BinStateSharedDeferredClass(); // Caldron as shared by thermostats entity with deferred on/of based on delays
	caldron->setTrueDelay(caldronOnDelay);
	caldron->setFalseDelay(0);
	caldron->onChange([](uint8_t state){outputs[5]->state.set(state);}); // Set caldron output state (output[5]) with on/off delay tolerance

	auto caldronSet = [caldron](uint8_t state){caldron->set(state);};

	for (int i=0; i<zonesCount; ++i)
	{
		weekThermostats[i]->state.onChange([i](uint8_t state){outputs[i]->state.set(state);});
		weekThermostats[i]->state.onChange(caldronSet);
	}

//	for (int i=0; i<zonesCount; ++i)
//	{
//		weekThermostats[i]->state.onChange([i](uint8_t state){outputs[i]->state.set(state);});
//	}

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
	webServer.paths.set("/temperature.json",HttpPathDelegate(&TempSensorsHttp::onHttpGet,(TempSensors*)tempSensorsHttp));
	webServer.paths.set("/state.json", onStateJson);
	webServer.paths.set("/schedule.json", onScheduleJson);
	webServer.paths.set("/thermostats.json", onThermostatsJson);


	Serial.printf(_F("AppClass init done!\n"));
}

void AppClass::start()
{
	ApplicationClass::start();
}

void AppClass::_loop()
{
	ApplicationClass::_loop();
	Serial.printf("Free Heap: %d WS count: %d Counter: %d\n", system_get_free_heap_size(), WebsocketConnection::getActiveWebsockets().count(), _counter);
}

void AppClass::userSTAGotIP(IpAddress ip, IpAddress mask, IpAddress gateway)
{
	Serial.printf(_F("AppClass STA GOT IP\n"));
	tempSensorsHttp->start();
//	for (const auto& weekThermostat: weekThermostats)
//	{
//		weekThermostat->start();
//	}
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
	JsonObject root = stream->getRoot();
	for (uint t=0; t < zonesCount; t++)
	{
		root[(String)t] = weekThermostats[t]->getName();

	}
	response.setAllowCrossDomainOrigin("*");
	response.sendDataStream(stream, MIME_JSON);
}
