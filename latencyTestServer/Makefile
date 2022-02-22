all: latencyTestServer

latencyTestServer: latencyTestServer.cpp  ../socketHandler/socketHandler.c ../socketHandler/socketHandler.h
	g++ latencyTestServer.cpp ../socketHandler/socketHandler.c -O3 -o latencyTestServer

clean:
	-rm latencyTestServer
