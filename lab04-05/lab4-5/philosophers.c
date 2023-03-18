#ifndef _PHILOSOPHERS_
#define _PHILOSOPHERS_

#include "vxWorks.h"
#include "taskLib.h"
#include "semLib.h"
#include "stdio.h"
#include "lab3.c"
#include "lab2.c"

#define ITER 10
#define STACK_SIZE 2000
#define PRIO_PHILS 120 // Philosother priority
#define NUM_OF_PHILS 5 // NumberOf Philosophers
//#define TAKE_FOREVER 10 // Take Semaphore forever
#define EAT 30
#define THINK 30

void PhilEntryPoint (int num); // Procedure for each philosopher ( think, take forks, eat)
void philInit (void); // Create philosopher tasks and create semaphors
void displayPhilosophers (int num); // Monitoring task to display the current system state

SEM_ID semForks[NUM_OF_PHILS];
SEM_ID semBinaryForks;
SEM_ID semMaxEatingPhilo;
int philo[NUM_OF_PHILS];
int philoEATcount[NUM_OF_PHILS]={0};
int currentEatingcount = 0;
int maxPhiloEating = NUM_OF_PHILS/2;
int lab2Task, lab3Task;

void philInit (void){
	int i;
	char taskName[6] = "phil0";
	semBinaryForks = semMCreate(SEM_Q_FIFO);
	semMaxEatingPhilo = semCCreate(SEM_Q_FIFO,maxPhiloEating);
	for (i=0; i<NUM_OF_PHILS; i++){
		semForks[i] = semMCreate(SEM_Q_FIFO);
	}
	for (i=0; i<NUM_OF_PHILS; i++){
		taskName[4] = (char)(i);
		philo[i] = taskSpawn(taskName,PRIO_PHILS,0x100,STACK_SIZE,(FUNCPTR)PhilEntryPoint,i,0,0,0,0,0,0,0,0,0);
	}
	lab2Task = taskSpawn("lab2Task",110,0x100,STACK_SIZE,(FUNCPTR)spawn_ten,0,0,0,0,0,0,0,0,0,0);
	lab3Task = taskSpawn("lab3Task",110,0x100,STACK_SIZE,(FUNCPTR)binary,0,0,0,0,0,0,0,0,0,0);
	
}

void PhilEntryPoint(int num){
	int i;
	for (i=0; i<ITER; i++){
		printf("Philo %d is thinking now\n",num);
		taskDelay(THINK);
		
		semTake(semMaxEatingPhilo,WAIT_FOREVER); //Avoiding deadlock
		semTake(semForks[(num)%NUM_OF_PHILS],WAIT_FOREVER); //Taking fork 1
		taskDelay(20); //In this way each philosopher will take one fork and make dedadlock
		semTake(semForks[(num+1)%NUM_OF_PHILS],WAIT_FOREVER); //Taking fork 2
		
		semTake(semBinaryForks,WAIT_FOREVER);
		currentEatingcount++;
		semGive(semBinaryForks);
		
		philoEATcount[num]++;
		//printf("Philo %d is eating now\n",num);
		printf("Current eating pilosophers counter = %d\n",currentEatingcount);
		taskDelay(EAT);
		
		semTake(semBinaryForks,WAIT_FOREVER);
		currentEatingcount--;
		semGive(semBinaryForks);
		
		semGive(semForks[(num)%NUM_OF_PHILS]); //Giving back fork 1
		semGive(semForks[(num+1)%NUM_OF_PHILS]); //Giving back fork 2
		semGive(semMaxEatingPhilo); //Avoiding deadlock
	}
	printf("Pilosopher %d EATING counter = %d \n",num,philoEATcount[num]);
}


#endif
