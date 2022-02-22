all: latencyTestClient

latencyTestClient: latencyTestClient.cpp ../socketHandler/socketHandler.c ../socketHandler/socketHandler.h
	g++ latencyTestClient.cpp ../socketHandler/socketHandler.c -O3 -o latencyTestClient


clean:
	-rm latencyTestClient
