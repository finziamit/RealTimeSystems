#include "vxWorks.h"
#include "msgQLib.h"
#include "semLib.h"
#include "stdlib.h"

#define MAX_MSGS 50
#define MAX_MSG_LEN 40
#define SERV_PRIO 108
#define SERVER_SENDER 3
#define N 4
#define ITER 5
#define STACK_SIZE 2000
#define ERROR NULL

static MSG_Q_ID serverMQID;

void serverInit (MSG_Q_ID clientsMSG_Q_ID[]);
void server (MSG_Q_ID clientsMSG_Q_ID[]);
void sendMessages (char msg[], MSG_Q_ID clientsMSG_Q_ID[]);
