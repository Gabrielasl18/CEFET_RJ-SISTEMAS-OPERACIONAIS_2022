//https://medium.com/swlh/the-dining-philosophers-problem-solution-in-c-90e2593f64e8
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define NUM_FILOSOFO 5

sem_t sala;
sem_t talher[NUM_FILOSOFO];

void * philosopher(void *);
void comer(int);
int main()
{
	int i,a[NUM_FILOSOFO];
	pthread_t tid[NUM_FILOSOFO];
	
	sem_init(&sala,0,NUM_FILOSOFO-1);
	
	for(i=0;i<NUM_FILOSOFO;i++)
		sem_init(&talher[i],0,1);
		
	for(i=0;i<NUM_FILOSOFO;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void * philosopher(void * num)
{
	int phil=*(int *)num;

	sem_wait(&sala);
	printf("\nFilosofo %d esta com \033[35mfome\033[m",phil);
    printf("\nFilosofo %d tenta pegar o garfo a esquerda",phil);
	sem_wait(&talher[phil]);
    printf("\nFilosofo %d \033[34mpega\033[m o garfo a esquerda",phil);
    printf("\nFilosofo %d tenta pegar o faca a direita",phil);
	sem_wait(&talher[(phil+1)%NUM_FILOSOFO]);
	printf("\nFilosofo %d \033[34mpega\033[m o faca a direita",phil);

	comer(phil);
	sleep(2);
	printf("\n\033[32mFilosofo %d terminou de comer\033[m",phil);

    printf("\nFilosofo %d libera o faca a direita",phil);
	sem_post(&talher[(phil+1)%NUM_FILOSOFO]);
    printf("\nFilosofo %d libera o garfo a esquerda",phil);
	sem_post(&talher[phil]);
	sem_post(&sala);
}

void comer(int phil)
{
	printf("\n\033[31mFilosofo %d esta comendo\033[m",phil);
}

