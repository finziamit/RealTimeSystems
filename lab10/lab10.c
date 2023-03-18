/* includes */
#include "vxWorks.h"
#include "taskLib.h"
#include "logLib.h"
#include "semLib.h"
/* function prototypes */
void taskOne(void);
void taskTwo(void);
void taskThree(void);
void taskFour(void);
/* globals */
#define ITER 2
#define HIGH 120
#define N 10000000

SEM_ID R1,R2;
void oneTickRoutine();
void twoTickRoutine();
void threeTickRoutine();


void init(void) /* function to create the three tasks */{
		int taskIdOne, taskIdTwo, taskIdThree, taskIdFour;
		printf("\n\n\n\n\n");
		/* semaphores init */ 
		R1 = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
		R2 = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
		/* spawn the four tasks */
		if((taskIdOne = taskSpawn("task1",HIGH,0x100,20000,(FUNCPTR)taskOne,0,0,0,0,0,0,0,0,0,0)) == ERROR)
			printf("taskSpawn taskOne failed\n");
		if((taskIdTwo = taskSpawn("task2",HIGH+1,0x100,20000,(FUNCPTR)taskTwo,0,0,0,0,0,0,0,0,0,0)) == ERROR)
			printf("taskSpawn taskTwo failed\n");
		if((taskIdThree = taskSpawn("task3",HIGH+2,0x100,20000,(FUNCPTR)taskThree,0,0,0,0,0,0,0,0,0,0)) == ERROR)
			printf("taskSpawn taskThree failed\n");
		if((taskIdFour = taskSpawn("task4",HIGH+3,0x100,20000,(FUNCPTR)taskFour,0,0,0,0,0,0,0,0,0,0)) == ERROR)
					printf("taskSpawn taskThree failed\n");
}

void taskOne(void){
	int i;
	for (i=0; i<ITER; i++){
		taskDelay(4);
		twoTickRoutine();
		semTake(R1,WAIT_FOREVER);
		logMsg("R1 Taken\n",0,0,0,0,0,0);
		oneTickRoutine();
		semTake(R2,WAIT_FOREVER);
		logMsg("R2 Taken\n",0,0,0,0,0,0);
		oneTickRoutine();
		semGive(R1);
		semGive(R2);
		logMsg("R1 and R2 Given Back and Task Finished\n",0,0,0,0,0,0);
	}
}

void taskTwo(void){
	int i;
	for (i=0; i<ITER; i++){
		taskDelay(2);
		oneTickRoutine();
		semTake(R2,WAIT_FOREVER);
		logMsg("R2 Taken\n",0,0,0,0,0,0);
		twoTickRoutine();
		semGive(R2);
		logMsg("R2 Given Back\n",0,0,0,0,0,0);
		oneTickRoutine();
		logMsg("Finished\n",0,0,0,0,0,0);
	}
}

void taskThree(void){
	int i;
	for (i=0; i<ITER; i++){
		taskDelay(3);
		threeTickRoutine();
		logMsg("Finished\n",0,0,0,0,0,0);
	}
}

void taskFour(void){
	int i;
	for (i=0; i<ITER; i++){
		oneTickRoutine();
		semTake(R2,WAIT_FOREVER);
		logMsg("R2 Taken\n",0,0,0,0,0,0);
		twoTickRoutine();
		semTake(R1,WAIT_FOREVER);
		logMsg("R1 Taken\n",0,0,0,0,0,0);
		twoTickRoutine();
		semGive(R1);
		semGive(R2);
		logMsg("R1 and R2 Given Back\n",0,0,0,0,0,0);
		oneTickRoutine();
		logMsg("Finished\n",0,0,0,0,0,0);
	}
}

void oneTickRoutine(){
	int i;
	for(i=0; i<N; i++);
}

void twoTickRoutine(){
	int i;
	for(i=0; i<2 ; i++){
		oneTickRoutine();
	}
}

void threeTickRoutine(){
	int i;
	for(i=0; i<3; i++){
		oneTickRoutine();
	}
}
