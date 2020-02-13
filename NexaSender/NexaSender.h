#ifndef NS_h
#define NS_h

#if defined(RPI)
#define ON_RPI 1
#define TRANSMIT_DATA_DEF std::array<bool, 36> _transmitData

#include <wiringPi.h>
#include <array>
#elif defined(ARDUINO) && ARDUINO >= 100
#define ON_ARDUINO 1
#define TRANSMIT_DATA_DEF bool _transmitData[36]

#include "arduino.h"
#endif

class NexaSender {
public:
	NexaSender(int pinId, unsigned long clientId);
	void turnDevice(bool mode, int group, int device);
	void dimDevice(int level, int group, int device);
	void turnGroup(bool mode, int group);

private:
	int _highLength;
	int _longLowLength;
	int _startLowLength;
	int _endLowLength;

	int pinId;
	unsigned long clientId;
	TRANSMIT_DATA_DEF;

	void writeClientId(long clientId);
	void writeGroupAction(bool groupAction);
	void writeMode(bool mode);
	void writeDestination(int group, int device);
	void writeDimLevel(int dimLevel);

	void transmit(bool dim);

	void sendStart();
	void sendEnd();
	void sendTwo();
	void sendOne();
	void sendZero();
	void sendPhysicalBit(int length);
};

#endif
