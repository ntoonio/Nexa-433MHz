# RPi-433mhz
A c++ library for controlling Nexa devices with the "self-learning" protocol.

The library can turn on and off a device or a whole group, as well as dim it to a specified value.

## Usage
  #include "NexaSender/NexaSender.h"

	int main() {
		// Set up the sender class
		
		int pinId = 0;
		long clientId = 12345678;
		
		NexaSender sender(pinId, clientId);
		
		// Turn on device 1 in group 1
		
		int group = 1;
		int device = 1;
		bool onoff = true;
		
		sender.turnDevice(onoff, group, device);
		
		// Dim device 1 in group 1 to 50%
		
		int dim = 8; // Value between 1-16, where 16 is bright
		
		sender.dimDevice(dim, group, device);
		
		// Turn on whole group 1
		
		// TODO: Not implemented!
		// sender.turnGroup(mode, group);
	}

## License
No license. Go nuts. Don't hold me liable please - the code is provided "as is", as all the licenses likes to say :)
