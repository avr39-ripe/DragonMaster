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

void BinaryInputClass::onStateChange(uint8_t inputId, onStateChangeDelegate delegateFunction)
{
	_data[inputId]->_onChangeState = delegateFunction;
}

void BinaryInputClass::_readState()
{
	uint8_t prevState;

	for (uint8_t id=0; id < _data.count(); id++)
	{
		prevState = _data[id]->_state;
		_data[id]->_state = _readUnit(_data[id]->_unitNumber) ? _data[id]->_polarity : !(_data[id]->_polarity);
		if (prevState != _data[id]->_state && _data[id]->_onChangeState)
		{
			Serial.printf("onChangeState Delegate/CB called!\n");
			_data[id]->_onChangeState(_data[id]->_state);
		}
	}
}
//BinaryInputGPIOClass

void BinaryInputGPIOClass::setUnitNumber(uint8_t inputId, uint8_t unitNumber)
{
	BinaryInputClass::setUnitNumber(inputId, unitNumber);
	pinMode(unitNumber, INPUT);
}

uint8_t BinaryInputGPIOClass::_readUnit(uint8_t unitId)
{
	return digitalRead(unitId);
}
