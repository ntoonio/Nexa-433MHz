#include "NexaSender.h"

#include <array>
#include <wiringPi.h>

NexaSender::NexaSender(int pinId, unsigned long clientId) {
	if (wiringPiSetup() != 0) {
		throw std::runtime_error("Wiring Pi couldn't be set up");
	}

	_highLength = 250;
	_longLowLength = 5 * 250;
	_startLowLength = 10 * 250;
	_endLowLength = 40 * 250;

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

void NexaSender::turnGroup(bool mode, int group) {
	writeGroupAction(true);
	writeMode(mode);
	writeDestination(group, 1); // Device doesn't matter

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
	_transmitData[26] = !groupAction; // Invert for transmit
}

void NexaSender::writeMode(bool mode) {
	_transmitData[27] = !mode; // Invert for transmit
}

void NexaSender::writeDestination(int group, int device) {
	// Input must be 1-4, but is transmitted as 0-3

	if (group < 1 || group > 4 || device < 1 || device > 4) {
		throw std::out_of_range("Forbidden group or device value, must be 1-4");
	}

	// Invert for transmit
	group = 4 - group;
	device = 4 - device;

	_transmitData[28] = group & 10;
	_transmitData[29] = group & 1;

	_transmitData[30] = device & 10;
	_transmitData[31] = device & 1;
}

void NexaSender::writeDimLevel(int dimLevel) {
	// Input must be 1-16, but is transmitted as 0-15

	if (dimLevel < 1 || dimLevel > 16) {
		throw std::out_of_range("Forbidden dim value. Must be 1-16");
	}
	
	// Invert for transmit
	int dim = 16 - dimLevel;

	// Convert the dim level and att it to the bits 32 - 35
	int mask = 1;
	for (int i = 0; i < 4; i++) {
		_transmitData[35 - i] = (dim & mask) >= 1;
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
	sendPhysicalBit(_highLength);
	sendPhysicalBit(_highLength);
}

// "1" is sent as "10"
void NexaSender::sendOne() {
	sendPhysicalBit(_highLength);
	sendPhysicalBit(_longLowLength);
}

// "0" is sent as "01"
void NexaSender::sendZero() {
	sendPhysicalBit(_longLowLength);
	sendPhysicalBit(_highLength);
}

void NexaSender::sendPhysicalBit(int length) {
	digitalWrite(pinId, HIGH);
	delayMicroseconds(_highLength);
	digitalWrite(pinId, LOW);
	delayMicroseconds(length);
}

