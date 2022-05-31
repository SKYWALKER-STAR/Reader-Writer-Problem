/*
******************************************************************************
*Create date : 2022/5/31
*Last modify date : 2022/5/31
*Usage : 
*	Reader writer problem
******************************************************************************
*/
#include "r-w.h"

static void *reader(void *);
static void *writer(void *);

/*
******************************************************************************
* @brief	: initialize related resources
******************************************************************************
*/
static void initialization()
{
	readers = 0;
	message = 0;
	
	errno = 0;
	sem_init(&mutex,0,1);
	if (errno) {
		perror("main.c:18:8:error");
		exit(-1);
	}
	sem_init(&roomEmpty,0,1);
	if (errno) {
		perror("main.c:23:8:error");
		exit(-1);
	}
}

/*
******************************************************************************
* @brief	: deinitialize
******************************************************************************
*/

static void deinitialization()
{
	readers = 0;
	errno = 0;
	sem_destroy(&mutex);
	if (errno) {
		perror("main.c:34:8:error");
		exit(-1);
	}
	sem_destroy(&roomEmpty);
	if (errno) {
		perror("main.c:39:8:error");
		exit(-1);
	}
}

/*
******************************************************************************
* @brief	: create readers and writers
******************************************************************************
*/
static void create_threads()
{	
	int i;
	
	for (i = 0;i < THREAD_NUM;i++) {
		pthread_t *readertid = 
				(pthread_t *)malloc(sizeof(pthread_t));
		pthread_t *writertid = 
				(pthread_t *)malloc(sizeof(pthread_t));
	
		pthread_create(readertid,NULL,reader,NULL);
		if (errno) {
			perror("main.c:56:16:error");
			exit(-1);
		}
		pthread_create(writertid,NULL,writer,NULL);
		if (errno) {
			perror("main.c:61:16:error");
			exit(-1);
		}
	}
}

/*
******************************************************************************
* @brief	: reader
******************************************************************************
*/
static void *reader(void *argv)
{
	int res;
	sem_wait(&mutex);
	readers += 1;
	if (readers == 1) 
		sem_wait(&roomEmpty);
	sem_post(&mutex);

	/* critical code for readers begain */
	res = message;
	/* critical code for readers end */
	
	sem_wait(&mutex);
	readers -= 1;
	if (readers == 0)
		sem_post(&roomEmpty);
	sem_post(&mutex);
	
	return NULL;
}
/*
******************************************************************************
* @brief	: writer
******************************************************************************
*/

static void *writer(void *argv)
{
	int i;
	sem_wait(&roomEmpty);
	/* critical code for writers begain */
	for (i = 0;i < NITERS;i++)
		message++;
	/* critical code for readers end */
	sem_post(&roomEmpty);
	
	
	return NULL;
}

int test1()
{
	initialization();
	pthread_t tid1;
	pthread_t tid2;
	
	pthread_create(&tid1,NULL,writer,NULL);
	pthread_create(&tid2,NULL,writer,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	if (message != (2*NITERS))
		printf("BOOM!%d\n",message);
	else
		printf("It is right!%d\n",message);
	deinitialization();
}

int main()
{
	initialization();
	create_threads();
	
	sleep(1);
	
	if (message != (100 * NITERS))
		printf("Boom!%d\n",message);
	else
		printf("OK!%d\n",message);
}
