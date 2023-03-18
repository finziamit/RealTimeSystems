/* includes */
#include "vxWorks.h"
#include "msgQLib.h"

/* function prototypes */
void taskOne(void);
void taskTwo(void);

/* defines */
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LENGTH 50

/* globals */
MSG_Q_ID mesgQueueId;

void message(void) /* function to create the message queue and two tasks */
{
    /*
    the function identify us if anything fails to create
    */
    int taskIdOne, taskIdTwo;
    /* create message queue */
    if ((mesgQueueId = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))
        == NULL) // if there is no msg queue spawned
        printf("msgQCreate in failed\n");
    /* spawn the two tasks that will use the message queue */
    if((taskIdOne = taskSpawn("t1",90,0x100,2000,(FUNCPTR)taskOne,0,0,0,0,0,0,0,
        0,0,0)) == ERROR) // if the first task doesnt spawn
        printf("taskSpawn taskOne failed\n");
    if((taskIdTwo = taskSpawn("t2",90,0x100,2000,(FUNCPTR)taskTwo,0,0,0,0,0,0,0,
        0,0,0)) == ERROR) //if the second task doesnt spawn
        printf("taskSpawn taskTwo failed\n");
}

void taskOne(void) /* task that writes to the message queue */
{
    char message[] = "Received message from taskOne";
    char msgBuf[MAX_MESSAGE_LENGTH]; //edited
    /* send message */
    if((msgQSend(mesgQueueId,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER,
        MSG_PRI_NORMAL)) == ERROR) // try to send message from task one and check if succeed
        printf("msgQSend in taskOne failed\n");

    /* the whole part coming from here is edited*/
    if(msgQReceive(mesgQueueId,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER)
        == ERROR)
        printf("msgQReceive in taskOne failed\n");
    else
        printf("%s\n",msgBuf);
    msgQDelete(mesgQueueId);
}

void taskTwo(void) /* tasks that reads from the message queue */
{
    char message[] = "Received message from taskTwo"; //edited
    char msgBuf[MAX_MESSAGE_LENGTH];
    /* receive message */
    if(msgQReceive(mesgQueueId,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER)
        == ERROR) // try to recive the message task one sent. if fails => (print fail)
        printf("msgQReceive in taskTwo failed\n");
    else //if succeed => (add message to queue and print it)
        printf("%s\n",msgBuf);
    msgQDelete(mesgQueueId); /* delete message queue */

    /* the whole part coming from here is edited*/
    if((msgQSend(mesgQueueId,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER,
        MSG_PRI_NORMAL)) == ERROR)
        printf("msgQSend in taskTwo failed\n");

}