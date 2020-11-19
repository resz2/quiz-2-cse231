#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<pthread.h>

sem_t rdr,wrt;
pthread_t tid;
pthread_t writerthread, readerthreads[100];
int test = 0,readercount = 0, tidx[101];
int *integer;

void *reader(void* arg)	{
    sem_wait(&rdr);
    readercount++;
    if(readercount==1)
        sem_wait(&wrt);
    sem_post(&rdr);
    printf("%d reader(s) is/are inside\n", readercount);
	printf("value read: %d\n", *integer);
    sleep(1);
    sem_wait(&rdr);
    readercount--;
    if(readercount==0)	{
        sem_post(&wrt);
    }
    sem_post(&rdr);
    printf("%d reader(s) is/are leaving\n",readercount+1);
}

void *writer(void* arg)	{
    printf("writer trying to enter\n");
    sem_wait(&wrt);
    *integer = test++;
	printf("value written: %d\n", *integer);
    sem_post(&wrt);
    printf("writer leaving\n");
}

int main()	{
    int n;
	key_t key;
	key = ftok("q5.c", 65);
	int shmid;
	shmid = shmget(key, 8, 0666 | IPC_CREAT);
	integer = (int*)shmat(shmid, (void*)0, 0);

    printf("enter number of readers: ");
    scanf("%d",&n);
    printf("\n");
    sem_init(&rdr, 0, 1);
    sem_init(&wrt, 0, 1);

    for(int i=0;i<n;i++)	{
		tidx[i] = i;
        pthread_create(&readerthreads[i], NULL, writer, (void*)&tidx[i+1]);
    }
    pthread_create(&writerthread, NULL, reader, (void*)tidx[0]);
    for(int i=0;i<n;i++)	{
        pthread_join(readerthreads[i],NULL);
    }
    pthread_join(writerthread, NULL);

	shmdt(&integer);
	shmctl(shmid, IPC_RMID, NULL);
	sem_destroy(&rdr);
	sem_destroy(&wrt);
}