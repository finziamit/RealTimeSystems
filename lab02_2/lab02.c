#define ITERATIONS 10
#define STACK_SIZE 2000

void print(int k, int ticks);

spawn_ten() /* Subroutine to perform the spawning */
{
    int ticks1=20, ticks2=40, taskId1,taskId2;
    taskId1 = taskSpawn("t1",120,0x100,STACK_SIZE,print,1,ticks1,0,0,0,0,0,0,0,0);
    taskId2 = taskSpawn("t2",120,0x100,STACK_SIZE,print,2,ticks2,0,0,0,0,0,0,0,0);
}

void print(int k, int ticks) /* Subroutine to be spawned */
{
    int i;
    for(i=0;i<ITERATIONS;i++){
        taskDelay(ticks);
        printf("Hello - I am Task%d. ID=%d\n",k,taskIdSelf());
    }
}