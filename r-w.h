/*
******************************************************************************
*Create date : 2022/5/31
*Last modify date : 2022/5/31
*Usage : 
*	Reader writer header
******************************************************************************
*/
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define MAXSIZE 100
#define THREAD_NUM 100
#define NITERS 1000000

int message;
int readers;

sem_t mutex;
sem_t roomEmpty;
