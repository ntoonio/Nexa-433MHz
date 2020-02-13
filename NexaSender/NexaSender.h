#ifndef NS_h
#define NS_h

#include <wiringPi.h>
#include <array>

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
	std::array<bool, 32+4> _transmitData;

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

