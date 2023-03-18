#include "vxWorks.h"
#include "msgQLib.h"
#include "semLib.h"
#include "stdlib.h"
#include <stdio.h>
#include "client.c"


void init(void){
	int i;
	MSG_Q_ID clientsMSG_Q_ID[N];
	int tidClients[N];
	int tidServer;
	if (tidServer = taskSpawn ("Server", 105, 0, STACK_SIZE,(FUNCPTR) serverInit, clientsMSG_Q_ID, 9, 9, 9, 9, 9, 9, 9, 9, 9) == ERROR){
		printf("Server taskSpawn failed\n");
	}
	
	for (i=0;i<N;i++){
		if (clientsMSG_Q_ID[i] = msgQCreate(1,MAX_MSG_LEN,MSG_Q_FIFO) == NULL){
			printf("client %d msgQCreate failed\n",i);
		}
		char client[10];
		sprintf(client,"client%d",i);
		if (tidClients[i] = taskSpawn (client, 107, 0, STACK_SIZE, (FUNCPTR)ClientEntryPoint,clientsMSG_Q_ID[i], 0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR){
			printf("Client %d taskSpawn failed\n",i);
		}
	}
}
