/*
 * thermo.h
 *
 *  Created on: 29 дек. 2015 г.
 *      Author: shurik
 */

#ifndef INCLUDE_THERMO_H_
#define INCLUDE_THERMO_H_
#include <SmingCore/SmingCore.h>
#include <tempsensors.h>
#include <binstate.h>

#ifndef ONSTATECHANGEDELEGATE_TYPE_DEFINED
#define ONSTATECHANGEDELEGATE
typedef Delegate<void(uint8_t state)> onStateChangeDelegate;
#endif

namespace WeekThermostatConst
{
	const uint8_t maxInvalidGetTemp = 10; // max unhealthy getTemp before we assume tempsensor lost
	const uint8_t maxProg = 6;
	const uint8_t antiFrozen = 5; // temperature to maintain when thermostat is not _active to prevent system freeze
}

const uint16_t scheduleJsonBufSize = JSON_OBJECT_SIZE(120) + JSON_ARRAY_SIZE(42);
const uint16_t scheduleFileBufSize = 1152;
const uint16_t stateJsonBufSize = JSON_OBJECT_SIZE(10);
const uint16_t stateFileBufSize = 256;

struct SchedUnit
{
	SchedUnit()
	{
		start = 0;
		targetTemp = 0;
	}
	uint16_t start; // scheduled interval start in minutes since 0:00
	uint16_t targetTemp; // target temperature for this interval
};

class WeekThermostatClass
{
public:
	WeekThermostatClass(HttpServer& webServer, TempSensors &tempSensors, uint8_t sensorId, uint8_t uid, String name = "Thermostat", uint16_t refresh = 4000);
	void start();
	void stop();
	void check();
	void onStateCfg(HttpRequest &request, HttpResponse &response);
	uint8_t saveStateCfg();
	uint8_t loadStateCfg();
	void onScheduleCfg(HttpRequest &request, HttpResponse &response);
	uint8_t saveScheduleCfg();
	uint8_t loadScheduleCfg();
	void saveScheduleBinCfg();
	void loadScheduleBinCfg();
	void wsSendStatus(WebSocket& socket, uint8_t sendAll = false);
	void wsBinSetter(WebSocket& socket, uint8_t* data, size_t size);
//	void setManual(bool); // setter for _manual
//	void setSched(uint8_t wDay, uint8_t progNum, uint16_t minutes, float tergetTemp);
//	SchedUnit getSched(uint8_t wDay, uint8_t progNum);
	SchedUnit _schedule[7][WeekThermostatConst::maxProg]; // 7 day X maxProg programs in schedule
//	uint8_t getState() { return _state; };
	uint8_t getUid() { return _uid; };
	String getName() { return _name; };
//	void onStateChange(onStateChangeDelegate delegateFunction);
	BinStateClass state;
	//for binary websocket protocol
	static const uint8_t sysId = 4;
	static const uint8_t scWTGetStatus = 1;
	static const uint8_t scWTSetStatus = 2;
	static const uint8_t scWTGetSchedule = 3;
	static const uint8_t scWTSetDaySchedule = 4;
private:
	String _name; // some text description of thermostat
	uint8_t _active; //thermostat active (true), ON,  works, updates, changes its _state or turned OFF
//	uint8_t _state; // thermostat state on (true) or off (false)
	uint8_t _manual; //thermostat in manual mode (true) or automatic schedule mode (false)
	uint16_t _manualTargetTemp = 2000; //target temperature for manual mode MULTIPLE BY 100
	uint16_t _targetTempDelta = 50; //delta +- for both _targetTemp and manualTargetTemp MULTIPLE BY 100
	uint16_t _refresh; // thermostat update interval
	Timer _refreshTimer; // timer for thermostat update
	TempSensors *_tempSensors;
	uint8_t	_sensorId;
	uint8_t _prevManual = false; // previous state of _manual
	uint8_t _manualProg = 0; // program that was when manual mode turned on, when program change, manual mode will turn off
	onStateChangeDelegate onChangeState = nullptr;
	uint8_t _tempSensorValid = WeekThermostatConst::maxInvalidGetTemp; // if more than zero we STILL trust tempSensor temperature if less zero NOT trust
	void _fillStatusBuffer(uint8_t* buffer);
	uint8_t _uid = 0;
	HttpServer& _webServer;
};

class WeekThermostatsClass
{
public:
	void wsBinGetter(WebSocket& socket, uint8_t* data, size_t size);
	void wsBinSetter(WebSocket& socket, uint8_t* data, size_t size);
	void add(WeekThermostatClass* weekThermostat) { _weekThermostats[weekThermostat->getUid()] = weekThermostat; };
	static const uint8_t sysId = 4;
	static const uint8_t scWTSGetAll = 10;
private:
	HashMap<uint8_t,WeekThermostatClass*> _weekThermostats;
};

#endif /* INCLUDE_THERMO_H_ */
