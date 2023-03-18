/* includes */
#include "vxWorks.h"
#include "taskLib.h"
#include "semLib.h"

/* function prototypes */
void admin(void);
void philos(int i);


/* globals */
#define ITER 3
#define STACK_SIZE 2000
#define N 3
#define K 2
#define MAX_MSGS 3
#define MAX_MSG_LEN 10
MSG_Q_ID adminMQID;
MSG_Q_ID philosMSG_Q_ID[N];
SEM_ID semCountForForks;
int tidPhilos[N];
int tidAdmin;

void init(void){ /* function to create the three tasks */
	int i;
	semCountForForks = semCCreate(SEM_Q_FIFO, K);
	if ((tidAdmin = taskSpawn ("Admin", 105, 0, STACK_SIZE,(FUNCPTR) admin, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9)) == ERROR){
		printf("Server taskSpawn failed\n");
	}
	for (i=0; i<N; i++){
		if ((tidPhilos[i] = taskSpawn ("Philo", 106, 0, STACK_SIZE,(FUNCPTR) philos, i, 9, 9, 9, 9, 9, 9, 9, 9, 9)) == ERROR){
			printf("Philo taskSpawn failed\n");
		}	
	}
}

void admin(void){
	int j, philo;
	char message[MAX_MSG_LEN];
	if ((adminMQID = msgQCreate(MAX_MSGS,MAX_MSG_LEN,MSG_Q_FIFO)) == NULL){
		printf("Server msgQCreate failed\n");
	}
	for(j=0; j<ITER*N; j++){
		if (msgQReceive(adminMQID, message, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
			printf("adminMQID msgQReceive failed\n");
		philo = message[0] - '0';
		printf("admin recived message from philo %d\n",philo);
		if((msgQSend(philosMSG_Q_ID[philo],"ok",MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR)
			printf("admin ok message to philo failed\n");
		printf("admin sent ok message to philo %d\n",philo);
	}
	msgQDelete(adminMQID);
}

void philos(int i){
	int j;
	char msg1[MAX_MSG_LEN];
	char msg2[MAX_MSG_LEN];
	sprintf(msg1,"%d",i);
	if ((philosMSG_Q_ID[i] = msgQCreate(MAX_MSGS,MAX_MSG_LEN,MSG_Q_FIFO)) == NULL){
		printf("Server msgQCreate failed\n");
	}
	for(j=0; j<ITER; j++){
		if((msgQSend(adminMQID,msg1,MAX_MSG_LEN, WAIT_FOREVER,MSG_PRI_NORMAL)) == ERROR)
			printf("philo eat message to admin failed\n");
		if (msgQReceive(philosMSG_Q_ID[i], msg2, MAX_MSG_LEN, WAIT_FOREVER) == ERROR)
			printf("philo gets ok message failed\n");
		semTake(semCountForForks,WAIT_FOREVER);
		printf("philo %d eating now\n",i);
		taskDelay(10);
		printf("philo %d done eating, now thinking\n",i);
		semGive(semCountForForks);
		taskDelay(20);
	}
	msgQDelete(philosMSG_Q_ID[i]);
}
