CXXFLAGS=-DRPI -Wall -g

sender: Sender.o NexaSender/NexaSender.o
	$(CXX) $(CXXFLAGS) -o sender Sender.o NexaSender/NexaSender.o -lwiringPi

"NexaSender/NexaSender.o": NexaSender/NexaSender.cpp
	$(CXX) $(CXXFLAGS) -c NexaSender/NexaSender.cpp

Sender.o: Sender.cpp
	$(CXX) $(CXXFLAGS) -c Sender.cpp

