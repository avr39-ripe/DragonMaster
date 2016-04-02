/*
 * binout.cpp
 *
 *  Created on: 2 апр. 2016
 *      Author: shurik
 */

#include <binout.h>

// BinOutClass

void BinOutClass::addOutput()
{
	auto newOutputData = new outputData;
	_data.add(newOutputData);
}

void BinOutClass::addOutput(uint8_t unitNumber, uint8_t polarity)
{
	auto newOutputData = new outputData;
	_data.add(newOutputData);
	uint8_t lastElementId = _data.count() - 1;
	setUnitNumber(lastElementId, unitNumber);
	setPolarity(lastElementId, polarity);
}

void BinOutClass::setState(uint8_t outputId, uint8_t state)
{
	_data[outputId]->_state = state ? _data[outputId]->_polarity : !(_data[outputId]->_polarity);
	_setUnitState(_data[outputId]->_unitNumber, _data[outputId]->_state);
}

// BinOutGPIOClass

void BinOutGPIOClass::setUnitNumber(uint8_t outputId, uint8_t unitNumber)
{
	BinOutClass::setUnitNumber(outputId, unitNumber);
	pinMode(unitNumber, OUTPUT);
}

void BinOutGPIOClass::_setUnitState(uint8_t unitId, uint8_t state)
{
	digitalWrite(unitId, state);
}
