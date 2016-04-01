/*
 * binaryinput.cpp
 *
 *  Created on: 1 апр. 2016 г.
 *      Author: shurik
 */

#include <binaryinput.h>

// BinaryInputClass

void BinaryInputClass::start()
{
	_refreshTimer.initializeMs(_refresh, TimerDelegate(&BinaryInputClass::_readState, this)).start(true);
}

void BinaryInputClass::stop()
{
	_refreshTimer.stop();
}

void BinaryInputClass::addInput()
{
	auto newInputData = new inputData;
	_data.add(newInputData);
}

void BinaryInputClass::addInput(uint8_t unitNumber, uint8_t polarity)
{
	auto newInputData = new inputData;
	_data.add(newInputData);
	uint8_t lastElementId = _data.count() - 1;
	setUnitNumber(lastElementId, unitNumber);
	setPolarity(lastElementId, polarity);
}

//BinaryInputGPIOClass

void BinaryInputGPIOClass::setUnitNumber(uint8_t inputId, uint8_t unitNumber)
{
	BinaryInputClass::setUnitNumber(inputId, unitNumber);
	pinMode(unitNumber, INPUT);
}

void BinaryInputGPIOClass::_readState()
{
	for (uint8_t id=0; id < _data.count(); id++)
	{
		_data[id]->_state = digitalRead(_data[id]->_unitNumber) ? _data[id]->_polarity : !(_data[id]->_polarity);
	}
}
