/*
 * dragonmaster.h
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */

#pragma once
#include <SmingCore.h>
#include <Libraries/OneWire/OneWire.h>
#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>
#include <JsonObjectStream.h>
#include <tempsensors.h>
#include <binin.h>
#include <binout.h>
#include <binhttpbutton.h>
#include <thermostat.h>
#include <weekthermostat.h>
#include <fan.h>

extern BinOutClass* outputs[3];
extern BinInPollerClass* binInPoller;
extern TempSensorsHttp* tempSensorsHttp;
extern WeekThermostatClass* weekThermostat;
extern ThermostatClass* thermostats[3];
extern TempSensors* tempSensor;
extern LiquidCrystal_I2C lcd;
extern FanClass* fan;
const uint8_t onewire_pin{2};
extern OneWire ds;
