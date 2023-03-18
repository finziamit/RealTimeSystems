#include "server.h"


void serverInit (MSG_Q_ID clientsMSG_Q_ID[])
{
	int ServerTaskNum;
	if (serverMQID = msgQCreate(MAX_MSGS,MAX_MSG_LEN,MSG_Q_FIFO) == NULL){
		printf("Server msgQCreate failed\n");
	}
	printf("Server created the message queue\n");
	if (ServerTaskNum = taskSpawn ("ServerRead", SERV_PRIO, 0, STACK_SIZE,(FUNCPTR)server, clientsMSG_Q_ID, 9, 9, 9, 9, 9, 9, 9, 9, 9) == ERROR){
		printf("ServerRead taskSpawn failed\n");
	}
	
}

void server (MSG_Q_ID clientsMSG_Q_ID[])
{
	printf("Server starting to read now\n");
	char message[MAX_MSG_LEN];
	int i;
	for (i=1; i<ITER*N; i++){
		if (msgQReceive(serverMQID, message, MAX_MSG_LEN, WAIT_FOREVER) == NULL)
				printf("Server msgQReceive failed\n");
		printf("Server: msg = %s\n",message);
		sendMessages(message,clientsMSG_Q_ID);
	}
	//msgQDelete(serverMQID);
}

void sendMessages (char msg[], MSG_Q_ID clientsMSG_Q_ID[]){
	printf("Server: got msg = %s\n",msg);
	char i[3], j[3];
	sscanf(msg,"%s %s",i,j);
	char *ack = "ack";
	if((msgQSend(clientsMSG_Q_ID[(atoi(i))],ack,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == NULL)
            printf("msgQSend ack to i in serever failed\n");

	if((msgQSend(clientsMSG_Q_ID[(atoi(j))],msg,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == NULL)
            printf("msgQSend to j in serever failed\n");
}
