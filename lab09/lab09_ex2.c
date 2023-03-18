/* includes */
#include "vxWorks.h"
#include "taskLib.h"
#include "logLib.h"
#include "kernelLib.h"

/* function prototypes */
void taskOne2(void);
void taskTwo2(void);
void taskThree2(void);

/* globals */
#define ITER1 100
#define ITER2 1
#define LONG_TIME 3000000
#define HIGH 100 /* high priority */
#define MID 101 /* medium priority */
#define LOW 102 /* low priority */
#define TICKS (int)sysClkRateGet()/10 /* number of ticks */


void sched2(void) /* function to create the three tasks */{
	int taskIdOne, taskIdTwo, taskIdThree;
	printf("\n\n\n\n\n");
	/* spawn the three tasks */
	if((taskIdOne = taskSpawn("task1",MID,0x100,20000,(FUNCPTR)taskOne2,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskOne failed\n");
	if((taskIdTwo = taskSpawn("task2",LOW,0x100,20000,(FUNCPTR)taskTwo2,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskTwo failed\n");
	if((taskIdThree = taskSpawn("task3",LOW,0x100,20000,(FUNCPTR)taskThree2,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskThree failed\n");
}

void taskOne2(void){
	int i,j;
	for (i=0; i < ITER1; i++){
		for (j=0; j < ITER2; j++)
			logMsg("\n",0,0,0,0,0,0);
		for (j=0; j < LONG_TIME; j++);
	}
	logMsg("\n~~~~~~~~~~~~\n",0,0,0,0,0,0);
}

void taskTwo2(void){
	int i,j;
	for (i=0; i < ITER1; i++){
		kernelTimeSlice(TICKS);
		for (j=0; j < ITER2; j++)
			logMsg("\n",0,0,0,0,0,0);
		for (j=0; j < LONG_TIME; j++);
	}
}

void taskThree2(void){
	int i,j;
	for (i=0; i < ITER1; i++){
		kernelTimeSlice(TICKS);
		for (j=0; j < ITER2; j++)
			logMsg("\n",0,0,0,0,0,0);
		for (j=0; j < LONG_TIME; j++);
	}
}
