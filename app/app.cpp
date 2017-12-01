/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>
extern "C" int fb_test();
//AppClass
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

//	tempSensor = new TempSensorsOW(ds, 4000);


	output = new BinOutGPIOClass(13,0); // Gas Caldron Control
	output->state.set(false);


// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(4000);
	tempSensorsHttp->addSensor("http://10.2.113.125/temperature.json?sensor=0"); // House tempsensor

	weekThermostats[0] = new WeekThermostatClass(*tempSensorsHttp,0,"House", 4000);

	BinStateHttpClass* weekThermostatState = new BinStateHttpClass(webServer, &weekThermostats[0]->state, "Термостат Дом", 2);
	binStatesHttp->add(weekThermostatState);

	//GasHeating
	BinStateClass* gasEnable = new BinStateClass();
	gasEnable->persistent(0);

	BinHttpButtonClass* webGasEnable = new BinHttpButtonClass(webServer, *binStatesHttp, 2, "Газовое отопление", gasEnable);
	webGasEnable->state.onChange(onStateChangeDelegate(&BinStateClass::toggle, gasEnable));

	BinStateAndClass* gasCaldron = new BinStateAndClass();
	gasCaldron->addState(gasEnable);
	gasCaldron->addState(&weekThermostats[0]->state);
	gasCaldron->onChange(onStateChangeDelegate(&BinStateClass::set, &output->state));

	BinStateHttpClass* gasCaldronState = new BinStateHttpClass(webServer, gasCaldron, "Газовый котел", 3);
	binStatesHttp->add(gasCaldronState);
	//GasHeating

	for (auto _thermostat: weekThermostats)
	{
		_thermostat->loadStateCfg();
		for(uint8_t i = 0; i< 7; i++)
		{
			_thermostat->_schedule[i][0].start = 0;
			_thermostat->_schedule[i][0].targetTemp = 800;
			_thermostat->_schedule[i][1].start = 360;
			_thermostat->_schedule[i][1].targetTemp = 1800;
			_thermostat->_schedule[i][2].start = 540;
			_thermostat->_schedule[i][2].targetTemp = 1200;
			_thermostat->_schedule[i][3].start = 720;
			_thermostat->_schedule[i][3].targetTemp = 1500;
			_thermostat->_schedule[i][4].start = 1020;
			_thermostat->_schedule[i][4].targetTemp = 1800;
			_thermostat->_schedule[i][5].start = 1320;
			_thermostat->_schedule[i][5].targetTemp = 800;

			_thermostat->loadScheduleBinCfg();
		}
	}
//	webServer.addPath("/temperature.json",HttpPathDelegate(&TempSensors::onHttpGet,tempSensor));
	webServer.addPath("/temperatureHome.json",HttpPathDelegate(&TempSensorsHttp::onHttpGet,(TempSensors*)tempSensorsHttp));
	webServer.addPath("/state.json", onStateJson);
	webServer.addPath("/schedule.json", onScheduleJson);
	webServer.addPath("/thermostats.json", onThermostatsJson);

	for(uint8_t i=0; i<50; i++)
	{
		Serial.printf("Pre FB Free Heap: %d\n", system_get_free_heap_size());
		fb_test();
		Serial.printf("Post FB Free Heap: %d\n", system_get_free_heap_size());
		//Serial.printf("fb_test CALLED!\n");
	}
}

void AppClass::start()
{
	ApplicationClass::start();
//	tempSensor->start();
}

void AppClass::_loop()
{
	ApplicationClass::_loop();
}

void AppClass::userSTAGotIP(IPAddress ip, IPAddress mask, IPAddress gateway)
{
	tempSensorsHttp->start();
	for (auto _thermostat: weekThermostats)
		_thermostat->start();
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
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	for (uint t=0; t < maxWeekThermostats; t++)
	{
		root[(String)t] = weekThermostats[t]->getName();

	}
	char buf[scheduleFileBufSize];
	root.printTo(buf, sizeof(buf));

	response.setHeader("Access-Control-Allow-Origin", "*");
	response.setContentType(ContentType::JSON);
	response.sendString(buf);
}
