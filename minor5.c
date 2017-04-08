/*
 * Brandon Reid
 * CSCE 3600
 * Oct 23, 2016
 * minor5.c - using producer-consumer paradigm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NITEMS 10		// number of items in shared buffer

// shared variables
char shared_buffer[NITEMS];	// echo buffer
int shared_count;		// item count

pthread_cond_t prod, cons;	// cond vars
pthread_mutex_t mutex;		// pthread mutex

unsigned int prod_index = 0; 	// producer index into shared buffer
unsigned int cons_index = 0; 	// consumer index into shard buffer

// function prototypes
void * producer(void *arg);
void * consumer(void *arg);

int main() 
{ 
	pthread_t prod_tid, cons_tid1, cons_tid2; 

	// initialize pthread cond variables
	pthread_cond_init(&prod, NULL);
	pthread_cond_init(&cons, NULL);

	// initialize pthread variables
	pthread_mutex_init(&mutex, NULL);
	
	// start producer thread
	pthread_create(&prod_tid, NULL, producer, NULL);

	// start consumer threads
	pthread_create(&cons_tid1, NULL, consumer, NULL);
	pthread_create(&cons_tid2, NULL, consumer, NULL);
	
	// wait for threads to finish
	pthread_join(prod_tid, NULL);
	pthread_join(cons_tid1, NULL);
	pthread_join(cons_tid2, NULL);
			
	// clean up
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&prod);
	pthread_cond_destroy(&cons);
	
	return 0;
}

// producer thread executes this function
void * producer(void *arg)
{
	char key;

	while (1)
	{
		// read input key
		scanf("%c", &key);

		while (1)
		{
			// acquire mutex lock
			pthread_mutex_lock(&mutex);

			// if buffer is full, release mutex lock and check again
			if (shared_count == NITEMS)
			{
				// wait for consumer
				pthread_cond_wait(&prod, &mutex);
				pthread_mutex_unlock(&mutex);
			}
			else
				break;
		}

		// store key in shared buffer
		shared_buffer[prod_index] = key;

		// update shared count variable
		shared_count++;

		// update producer index
		if (prod_index == NITEMS - 1)
			prod_index = 0;
		else
			prod_index++;

		// signal for new items to be consumed		
		pthread_cond_signal(&cons);

		// release mutex lock
		pthread_mutex_unlock(&mutex); 
	}

	return NULL;
}

// consumer thread executes this function
void * consumer(void *arg)
{
	char key;

	int id = (int)pthread_self();

	while (1)
	{
		while (1)
		{
			// acquire mutex lock
			pthread_mutex_lock(&mutex);

			// if buffer is empty, release mutex lock and check again
			if (shared_count == 0)
			{
				// wait for producer
				pthread_cond_wait(&cons, &mutex);
				pthread_mutex_unlock(&mutex);
			}
			else
				break;
		}

		// read key from shared buffer
		key = shared_buffer[cons_index];
		
		// echo key
		printf("consumer %d %c\n", id, key);

		// update shared count variable
		shared_count--;

		// update consumer index
		if (cons_index == NITEMS - 1)
			cons_index = 0;
		else
			cons_index++;

		// signal that items are ready to be produced	
		pthread_cond_signal(&prod);

		// release mutex lock
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}
