#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define n 5

sem_t canEat;
sem_t forks[n];

void * philosopher(void *num)	{
	int phil=*(int *)num;

	sem_wait(&canEat);
	printf("philosopher %d is ready to eat\n",phil+1);
	sem_wait(&forks[phil]);
	sem_wait(&forks[(phil+1)%n]);
	printf("P%d receives F%d, F%d\n", phil+1, phil+1, (phil+1)%n+1);

	sleep(2);
	printf("philosopher %d has finished eating\n",phil+1);

	sem_post(&forks[phil]);
	sem_post(&forks[(phil+1)%n]);
	sem_post(&canEat);
}

int main()	{
	int i,philos[n];
	pthread_t tid[n];
	
	sem_init(&canEat,0,n-1);
	
	for(i=0;i<n;i++)	{
		sem_init(&forks[i],0,1);
	}

	for(i=0;i<n;i++)	{
		philos[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&philos[i]);
	}

	for(i=0;i<n;i++)	{
		pthread_join(tid[i],NULL);
	}
}