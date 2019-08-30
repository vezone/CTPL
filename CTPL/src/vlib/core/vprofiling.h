#pragma once

#include <time.h>

#include "vtypes.h"

int32 start;
int32 end;
double cpu_time_used;

#define profiling_check_time(x) start = clock(); x; end = clock() 

/*
	1 second = 1000 millisecond
	1 millisecond = 1000 microsecond
	1 microsecond = 1000 nanosecond 
*/
void profiling_begin()
{
    start = clock();
}

void profiling_end()
{
    end = clock();
}

double profiling_get_time() 
{
    return (double) (end - start); //microseconds 
}

void profiling_test()
{
    profiling_begin();
	{
		int32 i;
		double sum = 1.0, sum2 = 1.0;
		for (i = 0; i < MB(50); i++)
		{
			sum += (sum * i) + (sum2 / ((sum2*1024) - 15)) * sum;
			sum2 += sum / (14 * sum);  
		}
		int32 k;
	}
	profiling_end();

	double time = profiling_get_time();
	printf("vstring_builder_test() takes %f ms\n", time/1000);
}