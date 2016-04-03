/*
 * dragonmaster.h
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */

#ifndef INCLUDE_DRAGONMASTER_H_
#define INCLUDE_DRAGONMASTER_H_
#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>
#include <Libraries/OneWire/OneWire.h>
#include <tempsensors.h>
#include <binin.h>
#include <binout.h>
#include <thermostat.h>
#include <fan.h>

//OneWire stuff
const uint8_t onewire_pin = 2;
extern OneWire ds;

extern TempSensorsOW* tempSensor;
extern LiquidCrystal_I2C lcd;

extern BinInClass* input[2];
extern BinInPollerClass binInPoller;

extern BinOutClass* output[3];

extern ThermostatClass* thermostats[2];
extern FanClass* fan;

#endif /* INCLUDE_DRAGONMASTER_H_ */
