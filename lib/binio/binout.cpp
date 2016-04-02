/*
 * binout.cpp
 *
 *  Created on: 2 апр. 2016
 *      Author: shurik
 */

#include <binout.h>

// BinOutClass

BinOutClass::BinOutClass(uint8_t unitNumber, uint8_t polarity)
{
	_unitNumber = unitNumber;
	_polarity = polarity;
}

void BinOutClass::setState(uint8_t state)
{
	_state = state ? _polarity : !(_polarity);
	_setUnitState(_state);
}

// BinOutGPIOClass
BinOutGPIOClass::BinOutGPIOClass(uint8_t unitNumber, uint8_t polarity)
:BinOutClass(unitNumber, polarity)
{
	pinMode(_unitNumber, OUTPUT);
}

void BinOutGPIOClass::setUnitNumber(uint8_t unitNumber)
{
	BinOutClass::setUnitNumber(unitNumber);
	pinMode(unitNumber, OUTPUT);
}

void BinOutGPIOClass::_setUnitState(uint8_t state)
{
	digitalWrite(_unitNumber, state);
}
