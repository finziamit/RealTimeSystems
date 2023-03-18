#include "vxWorks.h"
#include "taskLib.h"
#include "semLib.h"
#include "stdio.h"
#define ITER 10
#define STACK_SIZE 2000
/* function prototypes */
void taskOne(int);
void taskTwo(int);
void taskThree(int);
void taskFour();

/* globals */
SEM_ID semBinary;
SEM_ID semFour;

int var = 0;
void binary(void)
{
	int taskIdOne, taskIdTwo, taskIdThree, taskIdFour;
	int ticks=15,ticks3=20;
	/* create semaphore with semaphore available and queue tasks on PRIORITY basis */
	semBinary = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	semFour = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	
	/* spawn the two tasks */
	printf("Start\n");
	taskIdOne = taskSpawn("t1",105,0x100,STACK_SIZE,(FUNCPTR)taskOne,ticks,0,0,0,0,0,0,0,0,0);
	printf("First Task spawned\n");
	taskIdTwo = taskSpawn("t2",105,0x100,STACK_SIZE,(FUNCPTR)taskTwo,ticks,0,0,0,0,0,0,0,0,0);
	printf("Second task spawned\n");
	taskIdThree = taskSpawn("t3",102,0x100,STACK_SIZE,(FUNCPTR)taskThree,ticks3,0,0,0,0,0,0,0,0,0);
	printf("Third task Spawned\n");
	taskIdFour = taskSpawn("t4",101,0x100,STACK_SIZE,(FUNCPTR)taskFour,0,0,0,0,0,0,0,0,0,0);
	
}

void taskOne(int ticks)
{
	int i;
	for (i=0; i < ITER; i++)
	{
		taskDelay(ticks);
		semTake(semBinary,WAIT_FOREVER); 
		
		var+=3;
		printf("Task 1. VAR = %d\n",var);
		semGive(semBinary);
	}
}

void taskTwo(int ticks)
{
	int i;
	for (i=0; i < ITER; i++)
	{
		taskDelay(ticks);
		semTake(semBinary,WAIT_FOREVER); 
		
		var+=5;
		printf("Task 2. VAR = %d\n",var);
		semGive(semBinary); 
	}
	semTake(semFour,WAIT_FOREVER);
}

void taskThree(int ticks3)
{
	int i;
	for(i=0;i<ITER;i++)
	{
		taskDelay(ticks3);
		semTake(semBinary, WAIT_FOREVER);
		
		printf("Task 3\n");
		if(var%3==0 || var%5==0){
			semGive(semFour);
		}

		semGive(semBinary);
	}
}

void taskFour (){
	int i;
	semGive(semBinary);
	for(i=0;i<ITER;i++){
		
		printf("Task 4\n");
		
		semTake(semFour,WAIT_FOREVER);
		if(var%3==0){
			printf("VAR is divided by 3\n");
		}
		if(var%5==0){
			printf("VAR is divided by 5\n");
		}
	}
	
}
