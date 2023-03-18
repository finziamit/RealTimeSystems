#include "vxWorks.h"
#include "msgQLib.h"
#include "semLib.h"
#include "stdlib.h"
#include <string.h>
#include "server.h"

#define TASKDELAYTIME 5 

void displayClient (char msg[], char clientNum);

void ClientEntryPoint (MSG_Q_ID clientMQID)
{
    char msgBuf[MAX_MSG_LEN];
    int i;
    for (i=0; i<ITER; i++){  
        char j = '0' + (rand() % N);
        char *task_name = taskName();
        char i = task_name[6];
        while (i == j)
            j = '0' + (rand() % N);
        char msg[MAX_MSG_LEN];
        sprintf(msg,"%c %c %s",i,j,"massage");
        //printf("client msg = %s\n",msg);
        if((msgQSend(serverMQID,msg,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == NULL)
            printf("msgQSend to server in client%c failed\n",i);
        
        printf("Client%c: I sent message to client%c\n",i,j);
        taskDelay(TASKDELAYTIME);
        if (msgQReceive(clientMQID, msgBuf, MAX_MSG_LEN, WAIT_FOREVER) == NULL)
			printf("msgQReceive ack in Client%c failed\n",i);
        printf("msgBug = %s\n",msgBuf);
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
        printf("%s\n",msg);
        printf("Client%c: I got a msg from client%d: %s\n",clientNum,i,sendMassage);
    }
}
