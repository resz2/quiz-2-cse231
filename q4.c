#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<unistd.h>
#include<wait.h>


int main(void) {
	key_t key1, key2;
	key1 = ftok("para1.txt", 65);
	key2 = ftok("para2.txt", 66);
	int shmid1, shmid2;
	shmid1 = shmget(key1, 1024, 0666 | IPC_CREAT);
	shmid2 = shmget(key2, 1024, 0666 | IPC_CREAT);
	char *string = (char*)shmat(shmid1, (void*)0, 0);
	//string = malloc(20);
	int *integer = (int*)shmat(shmid2, (void*)0, 0);
	sem_t writer, reader, mutex;
	sem_init(&writer, 1, 1);
	sem_init(&reader, 1, 1);
	sem_init(&mutex, 1, 1);
	pid_t pid;

	pid = fork();
	if(pid<0)	{
		perror("fork failed");
		return 1;
	}
	else if(pid>0)	{
		//parent
		//writing to child
		sem_wait(&mutex);
		sem_wait(&writer);
		string = malloc(20);
		strncpy(string, "parent to child", 15);
		*integer = 99;
		sem_post(&writer);
		sem_post(&mutex);

		wait(NULL);

		//reading from child
		sem_wait(&mutex);
		sem_wait(&reader);
		printf("string written by child is : %s\n", string);
		printf("integer written by child is : %d\n", *integer);
		sem_post(&reader);
		sem_post(&mutex);
	}
	else	{
		//child
		//reading from parent
		sem_wait(&mutex);
		sem_wait(&reader);
		printf("string written by parent is : %s\n", string);
		printf("integer written by parent is : %d\n", *integer);
		sem_post(&reader);
		sem_post(&mutex);
		
		//writing to parent
		sem_wait(&mutex);
		sem_wait(&writer);
		string = malloc(20);
		strncpy(string, "child to parent", 15);
		*integer = 1;
		sem_post(&writer);
		sem_post(&mutex);
		exit(0);
	}

	shmctl(shmid1, IPC_RMID, NULL);
	shmctl(shmid2, IPC_RMID, NULL);
	return 0;
}
