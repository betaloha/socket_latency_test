#include <string.h>
#include <iostream>
using namespace std;
#include "../socketHandler/socketHandler.h"
#include <assert.h>



int main(int argc, char* argv[]){
    int serverPort = 0;
    const char* useErrorMsg = "Use: ./redisServerAdaptor PMSwitch_port\n";
    if(argc>1){
        if(argv[1][0]>'9'||argv[1][0]<'0'){
            cerr << useErrorMsg << endl;
            exit(1);
        }
        serverPort = atoi(argv[1]);
    }

    int serverSock = socketHandler_listen(serverPort, DATAGRAM, BLOCKING);
    int ret=0;
    char recvBuff[8192];
    char sendBuff[8192];
    int seqNumber = 0;
    while(1){
        struct sockaddr_in addressStruct;
        size_t addr_struct_Size = sizeof(addressStruct);
        size_t recvSize = 0;
        size_t sendSize = 0;
        ret = socketHandler_recv_bytes_from(serverSock, recvBuff, sizeof(recvBuff), &addressStruct, &addr_struct_Size);
        if(ret==0){
            cerr << "Socket closed, exiting";
            exit(0);
        }
        recvSize = ret;
        int port = ntohs(addressStruct.sin_port);
        char src_ip[64];
	//memcpy(sendBuff, recvBuff, recvSize);
	sendSize = recvSize;
        inet_ntop(AF_INET, (void*)&addressStruct.sin_addr.s_addr, src_ip, sizeof(src_ip));
        ret = socketHandler_send_bytes_to(serverSock, recvBuff, sendSize, (const char*)src_ip, port);
	assert(ret>0 && "Send Error");	
    }


}
