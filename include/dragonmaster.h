/*
 * dragonmaster.h
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */

#ifndef INCLUDE_DRAGONMASTER_H_
#define INCLUDE_DRAGONMASTER_H_
#include <SmingCore/SmingCore.h>
//#include <Libraries/OneWire/OneWire.h>
//#include <tempsensors.h>
#include <binout.h>
#include <binhttpbutton.h>
#include <weekthermostat.h>


extern NtpClient* ntpClient;
//OneWire stuff
//const uint8_t onewire_pin = 2;
//extern OneWire ds;

//extern TempSensors* tempSensor;
extern TempSensorsHttp *tempSensorsHttp;
const uint8_t maxWeekThermostats = 1;
extern WeekThermostatClass *weekThermostats[maxWeekThermostats];
extern BinOutClass* output;

#endif /* INCLUDE_DRAGONMASTER_H_ */
