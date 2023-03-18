#include "vxWorks.h"
#include "msgQLib.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>


#define MAX_MSGS 50
#define MAX_MSG_LEN 40
#define SERV_PRIO 108
#define SERVER_SENDER 3
#define N 4
#define ITER 5
#define STACK_SIZE 2000

void ClientEntryPoint ();
void displayClient (char msg[], char clientNum);
void serverInit ();
void server ();
void sendMessages (char msg[]);

MSG_Q_ID serverMQID;
MSG_Q_ID clientsMSG_Q_ID[N];
int tidClients[N];
int tidServer;

void init(void){
	int i;
	if ((serverMQID = msgQCreate(MAX_MSGS,MAX_MSG_LEN,MSG_Q_FIFO)) == NULL){
		printf("Server msgQCreate failed\n");
	}
	if ((tidServer = taskSpawn ("Server", 106, 0, STACK_SIZE,(FUNCPTR) server, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9)) == ERROR){
			printf("Server taskSpawn failed\n");
	}
	for (i=0;i<N;i++){
		if ((clientsMSG_Q_ID[i] = msgQCreate(1,MAX_MSG_LEN,MSG_Q_FIFO)) == NULL){
			printf("client %d msgQCreate failed\n",i);
		}
		char client[10];
		sprintf(client,"client%d",i);
		if ((tidClients[i] = taskSpawn (client, 107, 0, STACK_SIZE, (FUNCPTR)ClientEntryPoint,0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == ERROR){
			printf("Client %d taskSpawn failed\n",i);
		}
	}
}

void server ()
{
	printf("Server starting to read now\n");
	char message[MAX_MSG_LEN];
	int x;
	for (x=0; x<ITER*N; x++){
		if (msgQReceive(serverMQID, message, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
				printf("Server msgQReceive failed\n");
		printf("Server: got msg = %s\n",message);
		sendMessages(message);
	}
}

void sendMessages (char msg[])
{
	char i[3], j[3];
	sscanf(msg,"%s %s",i,j);
	char *ack = "ack";
	if((msgQSend(clientsMSG_Q_ID[(atoi(i))],ack,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR)
            printf("msgQSend ack to i in serever failed\n");

	if((msgQSend(clientsMSG_Q_ID[(atoi(j))],msg,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR)
            printf("msgQSend to j in serever failed\n");
}

void ClientEntryPoint ()
{
	char *task_name = taskName();
	char i = task_name[6];
	int index = i-'0';
    char msgBuf[MAX_MSG_LEN];
    int k;
    for (k=0; k<ITER; k++){  
        char j = '0' + (rand() % N);
        while (i == j)
            j = '0' + (rand() % N);
        char msg[MAX_MSG_LEN];
        sprintf(msg,"%c %c %s",i,j,"massage");
        printf("Client%c: I sent message to client%c\n",i,j);
        if((msgQSend(serverMQID,msg,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR)
            printf("msgQSend to server in client%c failed\n",i);
        if ((msgQReceive(clientsMSG_Q_ID[index],msgBuf, MAX_MSG_LEN, WAIT_FOREVER)) == ERROR)
			printf("msgQReceive ack in Client%c failed\n",i);
        displayClient(msgBuf,i);
    }
}

void displayClient (char msg[], char clientNum)
{
    if(strcmp(msg,"ack") == 0)
        printf("Client%c: I got the ack msg\n",clientNum);
    else{
        int i,j;
        char sendMassage[MAX_MSG_LEN];
        sscanf(msg,"%d %d %s",&i,&j,sendMassage);
        printf("Client%c: I got a msg from client%d: %s\n",clientNum,i,sendMassage);
    }
}
