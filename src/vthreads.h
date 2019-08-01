#pragma once

#include <unistd.h>
#include <pthread.h>

void* thread_function(void* arg)
{
	int32 thread_id = *(int*) arg;
	for (int32 i = 0; i < 4; i++)
	{
		printf("thread running id: %d\n", thread_id);
		sleep(1);
	}
	return NULL;
}

void vthreads_test()
{
	pthread_t thread;
	pthread_attr_t attr;
	
	int32 thread_id = 2;
	pthread_attr_init(&attr);
	int32 error_code = 
		pthread_create(&thread, &attr,
        	thread_function, &thread_id);
    pthread_join(thread, NULL);
        	
}


