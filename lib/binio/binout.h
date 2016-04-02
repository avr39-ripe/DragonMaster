/*
 * binout.h
 *
 *  Created on: 2 апр. 2016
 *      Author: shurik
 */

#ifndef LIB_BINIO_BINOUT_H_
#define LIB_BINIO_BINOUT_H_
#include <SmingCore/SmingCore.h>

struct outputData
{
	uint8_t _unitNumber = 0;
	uint8_t _state = 0;
	uint8_t _polarity = 0;
};

class BinOutClass
{
public:
//	BinOutClass(uint16_t refresh = 500) { _refresh = refresh; };
	BinOutClass() {};
	virtual ~BinOutClass() {};
	void addOutput(); // not recommended!!
	void addOutput(uint8_t unitNumber, uint8_t polarity);
	virtual void setUnitNumber(uint8_t outputId, uint8_t unitNumber) { _data[outputId]->_unitNumber = unitNumber; };
	virtual void setUnitNumber(uint8_t unitNumber) { setUnitNumber(0, unitNumber); };
	virtual void setPolarity(uint8_t outputId, uint8_t polarity) { _data[outputId]->_polarity = polarity; };
	virtual void setPolarity(uint8_t polarity) { setPolarity(0, polarity); };
	void setState(uint8_t outputId, uint8_t state);
	void setState(uint8_t state) { setState(0, state); };
	uint8_t getState(uint8_t outputId) { return _data[outputId]->_state; };
	uint8_t getState() { return getState(0); };
//	void start();
//	void stop();
protected:
	Vector<outputData*> _data;
	virtual void _setUnitState(uint8_t unitId, uint8_t state) = 0;
//	uint16_t _refresh;
//	Timer _refreshTimer;
};

class BinOutGPIOClass : public BinOutClass
{
public:
	BinOutGPIOClass() {};
	virtual ~BinOutGPIOClass() {};
	void setUnitNumber(uint8_t outputId, uint8_t unitNumber);
protected:
	virtual void _setUnitState(uint8_t unitId, uint8_t state);
};




#endif /* LIB_BINIO_BINOUT_H_ */
