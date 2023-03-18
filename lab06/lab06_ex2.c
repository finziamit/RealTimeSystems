/* includes */
#include "vxWorks.h"
#include "msgQLib.h"
#include "semLib.h"
#include "stdlib.h"

/* function prototypes */
void taskOne(int ticks);
void taskTwo(int ticks);
void taskThree(int data);

/* defines */
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LENGTH 50
#define ITER 10
#define STACK_SIZE 2000

/* globals */
MSG_Q_ID mesgQueueId;
SEM_ID semBinary;
int VAR = 0;

void init(void)
{
	/* create message queue */
	if ((mesgQueueId = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO)) == NULL) // if there is no msg queue spawned
	        printf("msgQCreate in failed\n");
	semBinary = semBCreate(SEM_Q_FIFO, SEM_FULL);
    /*create three tasks*/
    int taskIdOne, taskIdTwo, taskIdThree;
    int ticks1 = 10, ticks2 = 8;
	if (taskIdOne = taskSpawn("t1",90,0x100,STACK_SIZE,(FUNCPTR)taskOne,ticks1,0,0,0,0,0,0,0,0,0) == ERROR)
		printf("taskSpawn taskOne failed\n");
	if (taskIdTwo = taskSpawn("t2",90,0x100,STACK_SIZE,(FUNCPTR)taskTwo,ticks2,0,0,0,0,0,0,0,0,0) == ERROR)
		printf("taskSpawn taskTwo failed\n");
	if (taskIdThree = taskSpawn("t3",100,0x100,STACK_SIZE,(FUNCPTR)taskThree,20,0,0,0,0,0,0,0,0,0) == ERROR)
		printf("taskSpawn taskThree failed\n");
    
}

void taskOne(int ticks) 
{
	int i;
	char message[MAX_MESSAGE_LENGTH];
	while(1)
	{
		taskDelay(ticks);
		semTake(semBinary,WAIT_FOREVER); 
		VAR+=2;
		sprintf(message,"t1 %d\n",VAR);
		semGive(semBinary);
		if((msgQSend(mesgQueueId,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR) // try to send message from task one and check if succeed
		        printf("msgQSend in taskOne failed\n");
	}
}

void taskTwo(int ticks)
{
	int i;
	char message[MAX_MESSAGE_LENGTH];
	while(1)
	{
		taskDelay(ticks);
		semTake(semBinary,WAIT_FOREVER); 
		VAR-=1;
		sprintf(message,"t2 %d\n",VAR);
		semGive(semBinary);
		if((msgQSend(mesgQueueId,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR) // try to send message from task one and check if succeed
				printf("msgQSend in taskTwo failed\n");
	}
}

void taskThree(int data)
{
	int i,x ;
	char msgBuf[MAX_MESSAGE_LENGTH];
	char tmp1[10], tmp2[10];
	while(1){
		/* receive message */
		if (msgQReceive(mesgQueueId, msgBuf, MAX_MESSAGE_LENGTH, WAIT_FOREVER)== ERROR)
			printf("msgQReceive in taskThree failed\n");
		else{
			printf("%s\n",msgBuf);
			if (msgBuf[1] == '1'){
				sscanf(msgBuf,"%s %s",tmp1,tmp2);
				x = atoi(tmp2);
				if (x > data*15){
					printf("No winner in this game");
					exit(0);
				}
				else if(x == data){
					printf("T1 is the winner");
					exit(0);
				}
			}
			if (msgBuf[1] == '2'){
				sscanf(msgBuf,"%s %s",tmp1,tmp2);
				x = atoi(tmp2);
				if (x > data*15){
					printf("No winner in this game");
					exit(0);
				}
				else if(x == data){
					printf("T2 is the winner");
					exit(0);
				}
			}
		}
	}
	msgQDelete(mesgQueueId);
}
