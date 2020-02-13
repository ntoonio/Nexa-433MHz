#include "NexaSender.h"

#include <iostream>
#include <array>
#include <algorithm>
#include <wiringPi.h>

NexaSender::NexaSender(int pinId, unsigned long clientId) {
	if (wiringPiSetup() != 0) {
		printf("Couldn't set up wiringPi");
		return;
	}

	_unitLength = 250;
	_longLowLength = 5 * 250;
	_startLowLength = 10 * 250;
	_endLowLength = 40 * 250;

	_dimLevels = {16, 8, 12, 4, 14, 6, 10, 2, 15, 7, 11, 3, 13, 5, 9, 1};

	this->pinId = pinId;
	this->clientId = clientId;

	writeClientId(clientId);
}

void NexaSender::dimDevice(int dimLevel, int group, int device) {
	writeGroupAction(false);
	// The mode is set to "11" physical bits for dim - which is done at transmit
	writeDestination(group, device);
	writeDimLevel(dimLevel);

	transmit(true);
}

void NexaSender::turnDevice(bool mode, int group, int device) {
	writeGroupAction(false);
	writeMode(mode);
	writeDestination(group, device);

	transmit(false);
}

void NexaSender::writeClientId(long clientId) {
	// Convert the client id to binary and add it to the 26 first bits
	int mask = 1;
	for (int i = 0; i < 26; i++) {
		_transmitData[25 - i] = (clientId & mask) >= 1;
		mask <<= 1;
	}
}

void NexaSender::writeGroupAction(bool groupAction) {
	_transmitData[26] = !groupAction;
}

void NexaSender::writeMode(bool mode) {
	_transmitData[27] = !mode;
}

void NexaSender::writeDestination(int group, int device) {
	_transmitData[28] = group & 10;
	_transmitData[29] = group & 1;

	_transmitData[30] = device & 10;
	_transmitData[31] = device & 1;
}

void NexaSender::writeDimLevel(int dimLevel) {
	if (dimLevel < 1 || dimLevel > 16) {
		return;
	}

	// Convert the dim level and att it to the bits 32 - 35
	int mask = 1;
	for (int i = 0; i < 4; i++) {
		_transmitData[35 - i] = ((dimLevel - 1) & mask) >= 1;
		mask <<= 1;
	}
}

void NexaSender::transmit(bool dim) {
	int nrBits = dim ? 36 : 32;
	
	pinMode(pinId, OUTPUT);

	for (int i = 0; i < 4; i++) {
		sendStart();

		for (int i = 0; i < nrBits; i++) {
			if (dim && i == 27) { // Transmit the mode as "11" physical bits for dim
				sendTwo();
			}
			else if (_transmitData[i]) {
				sendOne();
			}
			else {
				sendZero();
			}
		}

		sendEnd();
	}
}

void NexaSender::sendStart() {
	sendPhysicalBit(_startLowLength);
}

void NexaSender::sendEnd() {
	sendPhysicalBit(_endLowLength);
}

// When we want to dim we send the group action as "11"
// Named two just to match naming
void NexaSender::sendTwo() {
	sendPhysicalBit(_unitLength);
	sendPhysicalBit(_unitLength);
}

// "1" is sent as "10"
void NexaSender::sendOne() {
	sendPhysicalBit(_unitLength);
	sendPhysicalBit(_longLowLength);
}

// "0" is sent as "01"
void NexaSender::sendZero() {
	sendPhysicalBit(_longLowLength);
	sendPhysicalBit(_unitLength);
}

void NexaSender::sendPhysicalBit(int length) {
	digitalWrite(pinId, HIGH);
	delayMicroseconds(_unitLength);
	digitalWrite(pinId, LOW);
	delayMicroseconds(length);
}

