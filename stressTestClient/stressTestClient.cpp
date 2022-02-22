/*
unoptimized old version

Korakit Seemakhupt
korakit@virginia.edu
*/

#include <iostream>
#include <fstream>
using namespace std;
#include "../socketHandler/socketHandler.h"
#include <errno.h>
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <chrono>
// Constants

// Enable per-request timing
//#define TIMING

#define REMOTE_ADDRESS "192.168.1.1"

int downStreamUDPSock = 0;
int numOps = 100000;
int payloadSize = 128;
char sendBuff[8192];
char recvBuff[8192];
int port_no = 0;
uint32_t* timeArray;
int dumpTiming = 0;


void initializePayload(){
    int i;
    int* randomPayload_intPtr = (int*)sendBuff;
    for(i=0;i<sizeof(sendBuff)/sizeof(int);i++){
        randomPayload_intPtr[i] = rand();
    }
}

int runTest(){
    int ctr = 0;
    int numWrite = 0;
    auto start_time = chrono::high_resolution_clock::now();
    auto lastReqEnd_time = start_time;
    auto thisReqEnd_time = start_time;
    while(ctr<numOps){
        int ret;
        ret = socketHandler_send_bytes(downStreamUDPSock, sendBuff, payloadSize);
        assert(ret==payloadSize && "Send error");
        ret = socketHandler_recv_bytes(downStreamUDPSock, recvBuff, sizeof(recvBuff));
	assert(ret && "Recv error");
        //Timing
#ifdef TIMING
        thisReqEnd_time = chrono::high_resolution_clock::now();
        timeArray[ctr] = (uint32_t)chrono::duration_cast<chrono::microseconds>(thisReqEnd_time - lastReqEnd_time).count();
        lastReqEnd_time = thisReqEnd_time;
#endif
        ctr++;
    }
    ofstream statFile;
    

    
    auto end_time = chrono::high_resolution_clock::now();
#ifndef TIMING
    cerr << "Iterations: " << ctr << ", PayloadSize: " << payloadSize << ", " ;
    uint32_t totalTime = (uint32_t)chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
    cerr << "TotalTime: " << totalTime << ", ";
    cerr << "AvgTime: " << ((double)totalTime)/ctr << endl;
#else
    // timeArray
    std::vector<uint32_t>timeVectorMicroFull(timeArray, &timeArray[ctr]);
    std::vector<uint32_t>timeVectorMicro(&timeArray[ctr*10/100], &timeArray[ctr*95/100]);
    int64_t totalTimeMicro = 0;
    int dataPtsCount = (ctr*95/100) - (ctr*10/100);
    for(int k=0;k<timeVectorMicro.size();k++){
        totalTimeMicro += timeVectorMicro[k];
    }
    cerr << "totalTime: " << totalTimeMicro << endl;
    cerr << "AvgTime: " << (double)totalTimeMicro/dataPtsCount << endl;
    // std::sort(timeVectorMicro.begin(), timeVectorMicro.end());
    // cout << "LowestTime " << timeVectorMicro[0] << ", Longest Time " << timeVectorMicro.back() << endl; 
    // cout << "P95: " << timeVectorMicro[timeVectorMicro.size()*95/100] << ", P99: " << timeVectorMicro[timeVectorMicro.size()*99/100] << endl;
    if(dumpTiming){
        ofstream dumpFile;
        dumpFile.open ("distribution_pmSwitch_size_" + to_string(payloadSize) + ".txt", ostream::trunc);
        for(int itr=0;itr<timeVectorMicro.size();itr++){
            dumpFile <<timeVectorMicro[itr] << endl;
        }
        dumpFile.close();
        return 0;
    }
#endif
    return 0;
}




int main(int argc, char* argv[]){
    // Parse parameters
    const char* useErrorMsg = "Use: ./stressTestClient numOps payloadSize port dumpTiming\n";
    if(argc>1){
        if(argv[1][0]>'9'||argv[1][0]<'0'){
            cerr << useErrorMsg << endl;
            exit(1);
        }
        numOps = atoi(argv[1]);
    }
    if(argc>2){
        if(argv[2][0]>'9'||argv[2][0]<'0'){
            cerr << useErrorMsg << endl;
            exit(1);
        }
        payloadSize = atoi(argv[2]);
    }
    if(argc>3){
        if(argv[3][0]=='-'){
            ;
        }else{
            if(argv[3][0]>'9'||argv[3][0]<'0'){
                cerr << useErrorMsg << endl;
                exit(1);
            }else{
                port_no = atoi(argv[3]);
            }
        }
    }
    if(argc>4){
        if(argv[4][0]>'9'||argv[4][0]<'0'){
            cerr << useErrorMsg << endl;
            exit(1);
        }
        dumpTiming = atoi(argv[4]);
    }
    assert(((port_no>0) && (port_no<65536)) && "port No error");
    // Prepare space for timing.
#ifdef TIMING
    timeArray = (uint32_t*)malloc(numOps*sizeof(uint32_t));
#endif

    // Initialize payload.
    initializePayload();
    downStreamUDPSock = socketHandler_connect(REMOTE_ADDRESS, port_no, DATAGRAM, BLOCKING);
    assert(downStreamUDPSock);
    //cerr << "Initialized" << endl;
    runTest();

}
