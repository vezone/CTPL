#include <stdlib.h>
#include <stdio.h>

#include "vstring.h"
#include "vmath.h"

#define to_string(x) #x

//to copy memory of char
void vstring_copy(char* dest, char* src, int32 length)
{
	int32 i;
	for (i = 0; 
		 i < length; 
		 i++)
	{
		dest[i] = src[i];
	}
}

/*for null-terminated string*/
int32 vstring_length(char* str)
{
	int32_t index = 0;
	for (char* ptr = (char*)str; 
		 *ptr != '\0'; 
		 ptr++, index++);
	return index;
}

/*
	compare logic:
		*  0 - means left != rigth
		*  1 - means left == rigth
*/
int32 vstring_compare(char* left_string, char* right_string)
{
	int32 left_string_length = vstring_length(left_string);
	int32 right_string_length = vstring_length(right_string);
	
	if ((left_string_length == 0) 
		||
		(right_string_length == 0))
	{	
		return 0;	
	}
	else if ((left_string_length > right_string_length)
		||
		(left_string_length < right_string_length))
	{
		return 0;
	}
	else 
	{
		for (right_string_length = 0;
			 right_string_length < left_string_length;
			 right_string_length++)
		{
			if (left_string[right_string_length] != right_string[right_string_length])
			{
				return 0;
			}
		}
	}
	
	return 1;
}

char* vstring_catc(char* input, char character_to_delete)
{
	char letter;
	int32 count_of_character_to_delete = 0;
	for (char* input_pointer = input; 
		(letter = *input_pointer) != '\0'; 
		++input_pointer)
	{
		if (letter == character_to_delete)
		{
			++count_of_character_to_delete;
		}
	}

	int32 temp_length = vstring_length(input) - count_of_character_to_delete + 1;
	char* result_string = (char*)malloc(temp_length);
	char* result_string_pointer = result_string;
	for (char* input_pointer = input; 
		(letter = *input_pointer) != '\0'; 
		++input_pointer)
	{
		if (letter != character_to_delete)
		{
			*result_string_pointer = letter;
			++result_string_pointer;
		}
	}

	result_string[temp_length - 1] = '\0';

	return result_string;
}

char* vstring_cats(char* input, char* characters_to_delete)
{
	char letter;
	int32 count_of_character_to_delete = 0;
	for (char* input_pointer = input;
		(letter = *input_pointer) != '\0';
		++input_pointer)
	{
		for (char* characters_to_delete_pointer = characters_to_delete;
			*characters_to_delete_pointer != '\0';
			++characters_to_delete_pointer)
		{
			if (letter == *characters_to_delete_pointer)
			{
				++count_of_character_to_delete;
			}
		}
	}

	int32 temp_length = vstring_length(input) - count_of_character_to_delete + 1;
	char* result_string = (char*)malloc(temp_length);
	char* result_string_pointer = result_string;
	for (char* input_pointer = input;
		(letter = *input_pointer) != '\0';
		++input_pointer)
	{
		int32 temp_counter = 0;
		for (char* characters_to_delete_pointer = characters_to_delete;
			*characters_to_delete_pointer != '\0';
			++characters_to_delete_pointer)
		{
			if (letter == *characters_to_delete_pointer)
			{
				++temp_counter;
			}
		}
		if (temp_counter == 0)
		{
			*result_string_pointer = letter;
			++result_string_pointer;
		}
		temp_counter = 0;
	}

	result_string[temp_length - 1] = '\0';

	return result_string;
}

char* vstring_with(char character, int32 number_of_splitters)
{
	if (number_of_splitters > 0)
	{
		char* string = malloc((number_of_splitters + 1) * sizeof(char));
		*(string + (number_of_splitters * sizeof(char))) = '\0';
		char* last = (string + (number_of_splitters * sizeof(char)));
		for (char* ptr = string; ptr != last; ptr++)
		{
			*ptr = character;
		}
		last = nullptr;
		return string;
	}
	else
	{
		printf("number_of_splitters <= 0\n");
		return (void*)0;
	}
}

int32 vstring_to_int32(char* string)
{
	char c;
	int32 i = 0;
	int32 minus = 1;
	int32 result = 0;
	int32 string_length = vstring_length(string);
	
	if (string[0] == '-')
	{
		minus = -1;
		i = 1;
	}
	else if (string[0] == '+')
	{
		i = 1;
	}
	
	for (; i < string_length; i++)
	{
		c = string[i];
		if ((c < '0' || c > '9'))
		{
			return -1;
		}
		
		result += (c - 48) * get_number_rank_of(1, (string_length - 1 - i));
	}
	
	return minus * result;
}

/*
	int32: 10023
	int32: -1009
*/
char* vint32_to_string(int32 value)
{
	int32 i;
	int8 is_negative = ((value < 0) ? 1 : 0);
	int32 rank = rank_of(value);
	int32 value_length = rank + is_negative + 1;
	char* result = malloc((value_length + 1) * sizeof(char));
	
	if (is_negative)
	{
		result[0] = '-';
	}
	
	for (i = is_negative; i < value_length; i++)
	{
		result[i] = get_number_of_digit(value, rank) + 48;
		--rank;
	}
	
	return result;
}

char* vint64_to_string(int64 value)
{
	int32 i;
	int8 is_negative = ((value < 0) ? 1 : 0);
	int32 rank = rank_of(value);
	int32 value_length = rank + is_negative + 1;
	char* result = malloc((value_length + 1) * sizeof(char));
	
	if (is_negative)
	{
		result[0] = '-';
	}
	
	for (i = is_negative; i < value_length; i++)
	{
		result[i] = get_number_of_digit(value, rank) + 48;
		--rank;
	}
	
	return result;
}


char* vfloat_to_string(double value)
{
	char* string = malloc(30);
	string[29] = '\0';
	sprintf(string, "%g", value);
	return string;
}


