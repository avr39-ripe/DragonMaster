/*
 * binaryinput.h
 *
 *  Created on: 1 апр. 2016 г.
 *      Author: shurik
 */

#ifndef LIB_BINIO_BINARYINPUT_H_
#define LIB_BINIO_BINARYINPUT_H_
#include <SmingCore/SmingCore.h>

typedef Delegate<void(uint8_t state)> onStateChangeDelegate;

struct inputData
{
	uint8_t _unitNumber = 0;
	uint8_t _state = 0;
	uint8_t _polarity = 0;
	onStateChangeDelegate _onChangeState = nullptr;
};

class BinInClass
{
public:
	BinInClass(uint16_t refresh = 500) { _refresh = refresh; };
	virtual ~BinInClass() {};
	void addInput(); // not recommended!!
	void addInput(uint8_t unitNumber, uint8_t polarity);
	virtual void setUnitNumber(uint8_t inputId, uint8_t unitNumber) { _data[inputId]->_unitNumber = unitNumber; };
	virtual void setUnitNumber(uint8_t unitNumber) { setUnitNumber(0, unitNumber); };
	virtual void setPolarity(uint8_t inputId, uint8_t polarity) { _data[inputId]->_polarity = polarity; };
	virtual void setPolarity(uint8_t polarity) { setPolarity(0, polarity); };
	uint8_t getState(uint8_t inputId) { return _data[inputId]->_state; };
	uint8_t getState() { return getState(0); };
	void onStateChange(uint8_t inputId, onStateChangeDelegate delegateFunction);
	void onStateChange(onStateChangeDelegate delegateFunction) { onStateChange(0, delegateFunction); };
	void start();
	void stop();
protected:
	Vector<inputData*> _data;
	uint16_t _refresh;
	Timer _refreshTimer;
	void _readState();
	virtual uint8_t _readUnit(uint8_t unitId) = 0;
};

class BinInGPIOClass : public BinInClass
{
public:
	BinInGPIOClass(uint16_t refresh = 500) :BinInClass(refresh) {};
	virtual ~BinInGPIOClass() {};
	void setUnitNumber(uint8_t inputId, uint8_t unitNumber);
protected:
	virtual uint8_t _readUnit(uint8_t unitId);
};

#endif /* LIB_BINIO_BINARYINPUT_H_ */
