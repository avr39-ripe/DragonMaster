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

//OneWire stuff
const uint8_t onewire_pin = 2;
extern OneWire ds;

extern TempSensorsOW tempSensor;
extern LiquidCrystal_I2C lcd;



#endif /* INCLUDE_DRAGONMASTER_H_ */
