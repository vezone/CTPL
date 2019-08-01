#include "vmath.h"

int32 rank_of(int32 value)
{
	int32 rank = 0;
	for (; ;)
	{
		value /= 10;
		if (value != 0)
		{
			++rank;
		}
		else
		{
			return rank;
		}
	}
}

//TODO: rename this function
int32 get_number_rank_of(int32 number, int32 rank)
{
	if (rank > 0)
	{
		for (int32 i = 0; i < rank; i++)
		{
			number *= 10;
		}
	}
	return number;
}

int32 get_number_of_digit(int64 number, int8 digit)
{
	int32 i;
	
	if (rank_of(number) < digit)
	{
		return 0;	
	}
	
	if (number < 0)
	{ 
		number *= -1;
	}
	
	if (digit == 0)
	{
		return (number % 10);
	}
	
	number %= get_number_rank_of(1, (digit + 1));
	number /= get_number_rank_of(1, digit);
	
	return number;
}
