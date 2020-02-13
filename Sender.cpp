#include <iostream>

#include "NexaSender/NexaSender.h"

int main(int argc, char** argv) {
	
	if (argc < 5) {
		printf("Usage: $ nexasender <clientId> <mode> <group>\n");
		printf("\tmode = 0 - on/off mode <device> <on/off>\n");
		printf("\tmode = 1 - dim mode <device> <dim-level 1-16>\n");
		printf("\tmode = 2 - group mode <on/off>\n");
		printf("\tmode = 3 - fade mode <device> <direction>\n");
		printf("\t\tdirection = 0 - bright -> dark\n");
		printf("\t\tdirection = 1 - dark -> bright\n");
	
		return 1;
	}

	int clientId = atoi(argv[1]);
	int mode = atoi(argv[2]);
	int group = atoi(argv[3]);

	printf("Client id: %i\nMode: %i\nGroup: %i\n", clientId, mode, group);

	NexaSender sender(0, clientId);
	
	if (mode == 0) {
		int device = atoi(argv[4]);
		bool on = atoi(argv[5]);

		printf("Device: %i\nOn: %i\n", device, on);
		printf("Turning %s device %i:%i\n", (on ? "on" : "off"), group, device);

		sender.turnDevice(on, group, device);
	}
	else if (mode == 1) {
		int device = atoi(argv[4]);
		int dimLevel = atoi(argv[5]);

		printf("Device: %i\nDim level: %i\n", device, dimLevel);
		printf("Dimming device %i:%i to dim level %i\n", group, device, dimLevel);

		sender.dimDevice(dimLevel, group, device);
	}
	else if (mode == 2) {
		bool on = atoi(argv[4]);

		printf("On: %i\n", on);
		printf("Turning %s group %i\n", (on ? "on" : "off"), group);

		sender.turnGroup(on, group);
	}
	else if (mode == 3) {
		int device = atoi(argv[4]);
		bool dir = atoi(argv[5]);

		// * Readability is boring *
		printf("Device: %i\nDirection: %i\n", device, dir);
		printf("Fading from %i to %i\n", (dir ? 1 : 16), (dir ? 16 : 1));

		for (int i = (dir ? 1 : 16); (dir ? i <= 16 : i >= 1); (dir ? i++ : i--)) {
			printf("Set dim level to %i\n", i);
			sender.dimDevice(i, group, device);
			delayMicroseconds(2000000);
		}
	}

	return 0;
}
