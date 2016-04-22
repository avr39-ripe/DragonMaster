/*
 * dragonmaster.h
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */

#ifndef INCLUDE_DRAGONMASTER_H_
#define INCLUDE_DRAGONMASTER_H_
#include <SmingCore/SmingCore.h>
#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>
#include <Libraries/OneWire/OneWire.h>
#include <Libraries/MCP23S17/MCP23S17.h>
#include <tempsensors.h>
#include <binin.h>
#include <binout.h>
#include <thermostat.h>
#include <fan.h>
#include <weekthermostat.h>

//output mode selector: GPIO or MCP23S17
#define MCP23S17

extern NtpClient* ntpClient;
//OneWire stuff
const uint8_t onewire_pin = 2;
extern OneWire ds;

const uint8_t mcp23s17_cs = 15;
extern MCP* mcp001;

extern TempSensors* tempSensor;
extern LiquidCrystal_I2C lcd;

extern BinInClass* input[2];
extern BinInPollerClass binInPoller;

extern BinOutClass* output[3];

extern ThermostatClass* thermostats[2];
extern FanClass* fan;

#endif /* INCLUDE_DRAGONMASTER_H_ */
