/*
 * dragonmaster.h
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */

#pragma once
#include <SmingCore.h>
#include <Libraries/OneWire/OneWire.h>
#include <JsonObjectStream.h>
#include <tempsensors.h>
#include <binin.h>
#include <binout.h>
#include <binhttpbutton.h>
#include <weekthermostat.h>

const uint8_t zonesCount{2}; // Zones quantity
const uint8_t caldronOnDelay{5}; //Caldron ON delay in minutes
const uint8_t caldronOffDelay{0}; //Caldron OFF delay in minutes
extern BinOutClass* outputs[3];
extern TempSensorsHttp* tempSensorsHttp;
extern WeekThermostatClass* weekThermostats[zonesCount];
// extern TempSensors* tempSensor;
// const uint8_t onewire_pin{2};
// extern OneWire ds;
