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
#include <thermostat.h>
#include <weekthermostat.h>

const uint8_t zonesCount = 5; // Zones quantity
const uint8_t caldronOnDelay = 1; //Caldron ON delay in minutes
extern BinOutClass* outputs[8];
extern TempSensorsHttp* tempSensorsHttp;
extern WeekThermostatClass* weekThermostats[zonesCount];
