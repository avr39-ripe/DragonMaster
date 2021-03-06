/*
 * FanClass.h
 *
 *  Created on: 3 апр. 2016
 *      Author: shurik
 */

#ifndef LIB_FAN_FAN_H_
#define LIB_FAN_FAN_H_
#include <SmingCore/SmingCore.h>
#include <tempsensors.h>
#include <binin.h>
#include <binout.h>
#include <thermostat.h>

namespace FanMode
{
	const uint8_t IDLE=0;
	const uint8_t START=1;
	const uint8_t RUN=2;
	const uint8_t PERIODIC=3;
	const uint8_t STOP=4;
}

//const uint8_t _maxLowTempCount = 3;

class FanClass
{
public:
	FanClass(TempSensors &tempSensor, ThermostatClass &thermostat, BinInClass &startButton, BinInClass &stopButton, BinOutClass &fanRelay);
	virtual ~FanClass() {};
	void start();
	void stop();
	uint8_t getMode() const { return _mode;	}
	void setMode(uint8_t mode) { _mode = mode; }
	uint16_t getStartDuration() const { return _startDuration; }
	void setStartDuration(uint16_t startDuration) { _startDuration = startDuration; }
	uint16_t getStopDuration() const { return _stopDuration; };
	void setStopDuration(uint16_t stopDuration) { _stopDuration = stopDuration; };
	uint16_t getPeriodicInterval() const { return _periodicInterval; };
	void setPeriodicInterval(uint16_t periodicInterval) { _periodicInterval = periodicInterval; };
	uint16_t getPeriodicDuration() const { return _periodicDuration; };
	void setPeriodicDuration(uint16_t periodicDuration) { _periodicDuration = periodicDuration; };
	uint16_t getPeriodicTempDelta() const { return _periodicTempDelta; };
	void setPeriodicTempDelta(uint16_t periodicTempDelta) { _periodicTempDelta = periodicTempDelta; };
	void onHttpConfig(HttpRequest &request, HttpResponse &response);
	void _saveBinConfig();
	void _loadBinConfig();
private:
	void _modeStart(uint8_t state);
	void _modeStartEnd();
	void _modeStop(uint8_t state);
	void _modeStopEnd();
	void _pereodic();
	void _pereodicEnd();
	uint16_t _timerInterval = 0;
	Timer _fanTimer;
	TempSensors* _tempSensor;
	BinInClass* _startButton;
	BinInClass* _stopButton;
	BinOutClass* _fanRelay;
	ThermostatClass* _thermostat;
	uint8_t _mode = FanMode::IDLE;
	uint16_t _startDuration = 1; // turn Fan on for this duration when START button pressed, Minutes
	uint16_t _stopDuration = 1; // turn Fan on for this duration when STOP button pressed, Minutes
	uint16_t _periodicInterval = 9; // interval to turn on Fan in RUN mode, Minutes
	uint16_t _periodicDuration = 2; // duration to turn on Fan in RUN mode, Minutes
	float _periodicStartTemp = 0; // Temperature at periodic start, will be compared to temperature at periodic and
	uint16_t _periodicTempDelta = 300; // minimum temperature difference after periodic fan turn on, if less go to IDLE mode
	uint8_t _maxLowTempCount = 3;
	uint8_t _periodicCounter = _maxLowTempCount; // if 0 o to IDLE mode;
};
#endif /* LIB_FAN_FAN_H_ */
