/*
 * binstate.cpp
 *
 *  Created on: 18 апр. 2016 г.
 *      Author: shurik
 */
#include <binstate.h>


void BinStateClass::setState(uint8_t state, uint8_t forceDelegatesCall)
{
	uint8_t prevState = _state;
	_state = state;
//	Serial.printf("Thermostat %s: %s\n", _name.c_str(), _state ? "true" : "false");
	if (_state != prevState || forceDelegatesCall)
	{
		_callOnStateChangeDelegates();
	}
}

void BinStateClass::onStateChange(onStateChangeDelegate delegateFunction, uint8_t directState)
{
	if (directState)
	{
		_onChangeState.add(delegateFunction);
	}
	else
	{
		_onChangeStateInverse.add(delegateFunction);
	}

}

void BinStateClass::_callOnStateChangeDelegates()
{
	for (uint8_t i = 0; i < _onChangeState.count(); i++)
	{
		_onChangeState[i](_state);
	}

	for (uint8_t i = 0; i < _onChangeStateInverse.count(); i++)
	{
		_onChangeStateInverse[i](!_state);
	}
}
