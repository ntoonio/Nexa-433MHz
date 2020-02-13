#include "NexaSender.h"

void setup() {
	// put your setup code here, to run once:

	NexaSender sender(10, 61086057);

	sender.turnDevice(true, 1, 1);
}

void loop() {
	// put your main code here, to run repeatedly:

}
