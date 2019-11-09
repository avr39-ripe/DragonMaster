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

	BinOutClass* outputs[8]
				{
					new BinOutGPIOClass(2,0),
					new BinOutGPIOClass(0,0),
					new BinOutGPIOClass(4,0),
					new BinOutGPIOClass(5,0),
					new BinOutGPIOClass(13,0),
					new BinOutGPIOClass(12,0),
					new BinOutGPIOClass(14,0),
					new BinOutGPIOClass(16,0),
				};
	int i = 0;
	for (auto output :outputs)
	{
		output->state.set(false);
		BinHttpButtonClass* httpButton = new BinHttpButtonClass(webServer, *binStatesHttp, i++, &output->state);
		auto togglerFunc = [output](uint8_t state){output->state.toggle(state);};
		httpButton->state.onChange(togglerFunc);
	}




	// http tempsensors + Week Thermostat
	tempSensorsHttp = new TempSensorsHttp(4000);
	tempSensorsHttp->addSensor("http://10.2.113.118/temperature.json?sensor=0");



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
//	for (auto _thermostat: weekThermostats)
//		_thermostat->start();
}
