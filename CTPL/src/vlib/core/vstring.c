#include <stdlib.h>
#include <stdio.h>

#include "vstring.h"
#include "vmath.h"
#include "varray.h"

//to copy memory of char
void vstring_copy(char* dest, const char* src, int32 length)
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
int32 vstring_length(const char* str)
{
	int32 index = 0;
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
int32 vstring_compare(const char* left_string, const char* right_string)
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

char* vstring_inverse(const char* input)
{
	int32 i, j;
	int32 input_length = vstring_length(input);
	char* result_string = malloc(input_length * sizeof(char));
	--input_length;
	for (i = input_length, j = 0; 
		 i >= 0; 
		 i--, j++)
	{
		result_string[j] = input[i];
	}	
	return result_string;
}

void vstring_set_elements_to(char* input, char value)
{
	int32 i;
	int32 input_length = vstring_length(input);
	for (i = 0; i < input_length; i++)
	{
		input[i] = value;
	}
}

void vstring_pushc(char* input, char character)
{
	int32 input_length = vstring_length(input);
	char* output = realloc(input, (input_length + 1) * sizeof(char));
	if (output)
	{
		output[input_length] = character;
		input = output;
	}
}

//NOTE: use it only with malloc strings
void vstring_pushs(char* input, const char* string)
{
	int32 input_length = vstring_length(input);
	int32 string_length = vstring_length(string);
	int32 new_length = input_length + string_length;
	char* output = realloc(input, (new_length + 1) * sizeof(char));
	if (output)
	{
		int32 i;
		for (i = input_length; i < new_length; i++)
		{
			output[i] = string[i - input_length];
		}
		input = output;
		input[new_length] = '\0';
	}
}

//TODO: fix bug with [situation when split_character doesn't exist in input string]
char** vstring_split(const char* input, char split_character)
{
	int32 i;
	int32 input_length = vstring_length(input);
	char* temp = malloc(1*sizeof(char));
	int32 result_length = 0;
	int32 result_capacity = 1;
	char** result = malloc(1 * sizeof(*result));
	for (i = 0; i < input_length; i++)
	{
		char character = input[i];
		if (character == split_character)
		{
			if ((i != 0) && 
				(i +  1) < input_length)
			{
				if (input[i+1] != split_character)
				{
					result[result_length++] = temp;
					temp = malloc(1 * sizeof(char));
					if (result_length >= result_capacity)
					{
						int32 new_result_capacity = 2*result_capacity + 1;
						char** reallocated_result = realloc(result, new_result_capacity * sizeof(*result));
						if (reallocated_result) 
						{
							result = reallocated_result;
							result_capacity = new_result_capacity;
						}
					}
				}
			}
		}
		else
		{
			vstring_pushc(temp, character);
		}
	}
	result[result_length++] = temp;
	char** reallocated_result = realloc(result, (result_capacity+1) * sizeof(*result));
	if (reallocated_result) 
	{
		result = reallocated_result;
		result[result_length] = nullptr;
	}
	return result;
}

//NOTE: we are not deleting space between two words even if 
//this words were fully removed from a string
char* vstring_trim_end(const char* input, const char* trim_characters)
{
	int32 i;
	int32 j;
	int32 flag;
	char temp;
	int32 input_length = vstring_length(input);
	int32 trim_characters_length = vstring_length(trim_characters);
	int32 result_index = 0;
	char* result = malloc(input_length * sizeof(char));
	for (i = 0; i < input_length; i++)
	{
		temp = input[i];
		for (j = 0; j < trim_characters_length; j++)
		{
			if (temp == trim_characters[j])
			{
				flag = 0;
			}	
		}
		if (flag)
		{
			result[result_index++] = temp;
		}
		flag = 1;
	}
	return result;
}

char* vstring_catc(const char* input, char character_to_delete)
{
	char letter;
	int32 count_of_character_to_delete = 0;
	for (const char* input_pointer = input; 
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
	for (const char* input_pointer = input; 
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

char* vstring_cats(const char* input, char* characters_to_delete)
{
	char letter;
	int32 count_of_character_to_delete = 0;
	for (const char* input_pointer = input;
		(letter = *input_pointer) != '\0';
		++input_pointer)
	{
		for (const char* characters_to_delete_pointer = characters_to_delete;
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
	for (const char* input_pointer = input;
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

int32 vstring_to_int32(const char* string)
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

void vstring_test()
{
    char* vstring_inverse_result = vstring_inverse("string to inverse");
	printf("output = %s\n", vstring_inverse_result);
	if (vstring_inverse_result) free(vstring_inverse_result);

	char** vstring_split_result; 
	vstring_split_result = vstring_split("string to split", 'a');

	for (char** ptr = vstring_split_result; *ptr != nullptr; ptr++)
	{
		static int32 index = 0;
		printf("s%d: %s\n", index++, *ptr);
		//free(*ptr);
	}

	char* vstring_trim_end_result = vstring_trim_end("string, with, a, lot, of, commas", ",");
	printf("vstring_trim_end_result: %s\n", vstring_trim_end_result);
	
	char* vstring_pushs_result;
	vstring_set(vstring_pushs_result, "string ");
	vstring_pushs(vstring_pushs_result, "get bigger!");
	printf("vstring_pushs_result: %s\n", vstring_pushs_result);
	

}