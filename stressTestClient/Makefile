all: stressTestClient

stressTestClient: stressTestClient.cpp ../socketHandler/socketHandler.c ../socketHandler/socketHandler.h
	g++ stressTestClient.cpp ../socketHandler/socketHandler.c -O3 -o stressTestClient


clean:
	-rm stressTestClient
