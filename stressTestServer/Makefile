all: stressTestServer

stressTestServer: stressTestServer.cpp  ../socketHandler/socketHandler.c ../socketHandler/socketHandler.h
	g++ stressTestServer.cpp ../socketHandler/socketHandler.c -O3 -o stressTestServer

clean:
	-rm stressTestServer
