#include <stdlib.h>
#include <stdio.h>

#include "vstring.h"
#include "vmath.h"
#include "varray.h"

#define free(x) 

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

void vstring_join(char* left_string, const char* right_string)
{
	int32 left_string_length  = vstring_length(left_string);
	int32 right_string_length = vstring_length(right_string);
	int32 result_length = left_string_length + right_string_length;
	char* result = realloc(left_string, (result_length + 2) * sizeof(char));
	if (result)
	{
		vstring_copy((result + left_string_length), right_string, right_string_length);
		result[result_length + 1] = '\0';
	}
}

void vstring_join_character(char* left_string, char character)
{
	int32 left_string_length = vstring_length(left_string);
	char* result = realloc(left_string, (left_string_length + 2) * sizeof(char));
	if (result)
	{
		result[left_string_length] = character;
		result[left_string_length + 1] = '\0';
	}
}

//TODO: fix bug
char* vstring_format(const char* format, ...)
{
	int32 state = 0;
	int32 arguments_count = 0;
	va_list valist;
	char* result;
	vstring_set(result, "\0");

	for (const char* ptr = format; 
		(*ptr != '\0'); 
		ptr++)
	{
		if (*ptr == '%')
		{
			state = 1;
		}
		else if (*ptr == 'c' ||
			     *ptr == 's' || 
			     *ptr == 'l' ||
			     *ptr == 'd' ||
			     *ptr == 'b' ||
			     *ptr == 'f')
		{
			state = 0;
			++arguments_count;
		}
		else 
		{
			state = 0;
		}
	}
	
	if (arguments_count <= 0)
	{
		printf("arguments_count = 0\n");
		return "vstring_format: error";
	}
	
	state = 0;
	va_start(valist, format);
	while (*format != '\0')
	{
		switch (*format)
		{
			case '%':
			{
				if (state == 0)
				{
					state = 1;
				}
				else if (state == 1)
				{
					state = 0;
					vstring_join_character(result, (*format));
				}
				break;
			}
			case 'c':
			{
				if (state == 1)
				{
					char elementc = (char) va_arg(valist, int32);
					vstring_join_character(result, elementc);
				}
				else
				{
					vstring_join_character(result, (*format));
				}
				state = 0;
				break;
			} 
			case 's':
			{
				if (state == 1)
				{
					const char* elements = va_arg(valist, const char *);
					vstring_join(result, elements);

					if (elements)
					{
						free(elements);
					}
				}
				else
				{
					vstring_join_character(result, (*format));
				}
				state = 0;
				break;
			}
			case 'l':
			{
				if (state == 1)
				{
					state = 2;
				}
				else
				{
					vstring_join_character(result, (*format));
					state = 0;
				}
				break;
			} 
			case 'd':
			{
				if (state == 1)
				{
					int32 elementi32 = va_arg(valist, int32);
					char* elementi32_to_string = vint32_to_string(elementi32);
					if (elementi32_to_string)
						vstring_join(result, elementi32_to_string);
					
					if (elementi32_to_string)
					{ 
						free(elementi32_to_string);
					}
				}
				else if (state == 2)
				{
					int64 elementi64 = va_arg(valist, int64);
					char* elementi64_to_string = vint64_to_string(elementi64);
					vstring_join(result, elementi64_to_string);
					if (elementi64_to_string)
					{
						free(elementi64_to_string);
					}
				}
				else
				{
					vstring_join_character(result, (*format));
				}
				state = 0;
				break;
			}
			case 'b':
			{
				if (state == 1)
				{
					int32 elementi8 = va_arg(valist, int32);
					char* elementi8_to_str = vint32_to_string(elementi8);
					vstring_join(result, elementi8_to_str);
					if (elementi8_to_str)
					{
						free(elementi8_to_str);
					}
				}
				else
				{
					vstring_join_character(result, (*format));
				}
				state = 0;
				break;
			}
			case 'f':
			{
				if (state == 1)
				{
					double elementd = va_arg(valist, double);
					char* elementd_to_str = vfloat_to_string(elementd);
					vstring_join(result, elementd_to_str);
					if (elementd_to_str)
					{
						free(elementd_to_str);
					}
				}
				else
				{
					vstring_join_character(result, (*format));
				}
				state = 0;
				break;
			}
			default:
			{
				vstring_join_character(result, (*format));
				state = 0;
				break;
			}
		}
		format++;
	}

#if 1
	int32 result_length = vstring_length(result);
	char* ptr = realloc(result, (result_length + 1));
	if (ptr)
	{
		result[result_length] = '\0';
	}
#endif
	return result;
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

int32 vstring_compare_to(const char* left_string, const char* right_string, int32 length)
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
			 right_string_length < length;
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
	char* output = realloc(input, (input_length + 2) * sizeof(char));
	if (output)
	{
		output[input_length] = character;
		output[input_length+1] = '\0';
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

char** vstring_split(const char* input, char split_character)
{
	int32 i;
	int32 input_length = vstring_length(input);
	char* temp = malloc(1 * sizeof(char));
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

//NOTE: > then vstring_trim_end()
char* vstring_cats(const char* input, const char* characters_to_delete)
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
		for (const char* characters_to_delete_pointer = characters_to_delete;
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

char* vstring_remove(const char* input, int32 start_index)
{
	int32 input_length = vstring_length(input);
	if (start_index == 0)     { return ""; }
	else if (start_index < 0) { return "vstring_remove: ERROR"; }
	char* result = malloc((start_index + 1) * sizeof(char));
	vstring_copy(result, input, start_index);
	result[start_index] = '\0';
	return result;
}

//NOTE: not tested
//TODO: fix bug
char* vstring_remove_range(const char* input, int32 start_index, int32 end_index)
{
	//0 1 2 3 4 5 6 7 8 9
	//      3     6
	int32 input_length = vstring_length(input);
	int32 new_length = input_length - (end_index - start_index + 1);
	if ((start_index < 0) && (end_index < 0) &&
		(start_index >= end_index) && (end_index >= input_length)) 
	{ return "vstring_remove: ERROR"; }
	char* result = malloc((new_length + 1) * sizeof(char));
	vstring_copy(result, input, start_index);
	vstring_copy((result + start_index), (input + end_index + 1), (input_length - end_index));
	result[new_length] = '\0';
	return result;
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

char* vstring_to_upper(const char* input)
{
	int32 i;
	int32 input_length = vstring_length(input);
	char* result = malloc((input_length + 1) * sizeof(char));
	for (i = 0; i < input_length; i++)
	{
		char element = input[i];
		if (element >= 'a' && element <= 'z')
		{
			result[i] = element - 'a' + 'A';
		}
		else
		{
			result[i] = element;
		} 
	}
	result[input_length] = '\0';
	return result;
}

char* vstring_to_lower(const char* input)
{
	int32 i;
	int32 input_length = vstring_length(input);
	char* result = malloc((input_length + 1) * sizeof(char));
	for (i = 0; i < input_length; i++)
	{
		char element = input[i];
		if (element >= 'A' && element <= 'Z')
		{
			result[i] = element - 'A' + 'a';
		}
		else
		{
			result[i] = element;
		}
	}
	result[input_length] = '\0';
	return result;
}

int32 vstring_index_of(const char* input, char character)
{
	int32 i;
	int32 input_length = vstring_length(input);
	for (i = 0; i < input_length; i++)
	{
		if (input[i] == character)
		{
			return i;
		}
	}
	return -1;
}

int32 vstring_last_index_of(const char* input, char character)
{
	int32 i;
	int32 input_length = vstring_length(input) - 1;
	for (i = input_length; i >= 0; i--)
	{
		if (input[i] == character)
		{
			return i;
		}
	}
	return -1;
}

char* vstring_replace_character(const char* input, char old_character, char new_character)
{
	int32 i;
	int32 input_length = vstring_length(input);
	char* result = malloc((input_length + 1) * sizeof(char));
	vstring_copy(result, input, input_length);
	for (i = 0; i < input_length; i++)
	{
		if (result[i] == old_character)
		{
			result[i] = new_character;
		}
	}
	result[input_length] = '\0';
	return result;
}

//NOTE: not tested
char* vstring_replace_string(const char* input, const char* old_string, const char* new_string)
{
	int8 temp;
	int32 i;
	int32 j;
	int32 input_length = vstring_length(input);
	int32 old_string_length = vstring_length(old_string);
	int32 new_string_length = vstring_length(new_string);
	char* result;
	if (input_length >= old_string_length &&
		input_length >= new_string_length)
	{
		result = malloc((input_length + 1) * sizeof(char));
		for (i = 0; i < input_length; i++)
		{
			if (input[i] == old_string[0])
			{
				if (old_string_length > 1)
				{
					temp = 1;
					for (j = 1; j < old_string_length; j++)
					{
						if (input[i + j] != old_string[j])
						{
							temp = 0;	
						}
					}
				}
				else 
				{
					temp = 1;
				}
				if (temp == 1)
				{
					if (new_string_length == old_string_length)
					{
						vstring_copy(&result[i], new_string, new_string_length);
						return result;
					}
					else if (new_string_length > old_string_length)
					{
						int32 new_length = input_length + (new_string_length - old_string_length);
						char* ptr = realloc(result, 
							(new_length + 1) * sizeof(char));
						if (ptr)
						{
							vstring_copy(&result[i], new_string, new_string_length);
							result[new_length] = '\0';
							return result;
						}
					}
					else if (new_string_length < old_string_length)
					{
						vstring_copy(&result[i], new_string, new_string_length);
						int32 new_length = input_length - (new_string_length - old_string_length);
						char* ptr = realloc(result, (new_length + 1) * sizeof(char));
						if (ptr)
						{
							result[new_length] = '\0';
							return result;
						}	
					}
				}
				else if (temp == 0)
				{
					result[i] = input[i];
				}
			}
			else 
			{
				result[i] = input[i];
			}
		}
	}
	else
	{
		if (result)
		{
			free(result);
		}
		result = (char*)input;
	}
	return result;
}

char* vstring_substring(const char* input, int32 start_index)
{
	int32 input_length = vstring_length(input);
	if ((start_index < input_length) && (start_index >= 0))
	{
		int32 new_length = input_length - start_index + 1;
		char* result = malloc(new_length * sizeof(char));
		int32 i;
		for (i = start_index; i < input_length; i++)
		{
			result[i - start_index] = input[i];
		}
		return result;
	}
	return "vstring_substring: ERROR";
}

char* vstring_substring_range(const char* input, int32 start_index, int32 end_index)
{
	int32 input_length = vstring_length(input);
	if ((start_index < input_length) && 
		(start_index >= 0) && 
		(end_index < input_length) &&
		(end_index > start_index))
	{
		int32 i;
		int32 new_length = end_index - start_index + 1;
		char* result = malloc(new_length * sizeof(char));
		for (i = start_index; i <= end_index; i++)
		{
			result[i - start_index] = input[i];
		}
		return result;
	}
	return "vstring_substring: ERROR";
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
	result[value_length] = '\0';
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
	int32 vstring_length_result = vstring_length("string ");
	printf("vstring_length_result: %d\n", vstring_length_result);

	char* vstring_join_result;
	vstring_set(vstring_join_result, "string ");
	vstring_join(vstring_join_result, "to add");
	vstring_join(vstring_join_result, "!");
	printf("vstring_join_result: %s\n", vstring_join_result);

	char* vstring_joinc;
	vstring_set(vstring_joinc, "string to ");
	vstring_join_character(vstring_joinc, 'a');
	vstring_join_character(vstring_joinc, 'd');
	vstring_join_character(vstring_joinc, 'd');
	printf("vstring_joinc: %s\n", vstring_joinc);

    char* vstring_inverse_result = vstring_inverse("string to inverse");
	printf("output = %s\n", vstring_inverse_result);
	if (vstring_inverse_result) free(vstring_inverse_result);

	char** vstring_split_result; 
	vstring_split_result = vstring_split("string to split", ' ');

	for (char** ptr = vstring_split_result; *ptr != nullptr; ptr++)
	{
		static int32 index = 0;
		printf("split[%d]: %s\n", index++, *ptr);
		if (*ptr) free(*ptr);
	}

	char* vstring_trim_end_result = vstring_trim_end("string, with, a, lot, of, commas", ",");
	printf("vstring_trim_end_result: %s\n", vstring_trim_end_result);
	
	char* vstring_cats_result = vstring_cats("string, with, a, lot, of, commas", ",o");
	printf("vstring_cats_result: %s\n", vstring_cats_result);
	
	const char* vstring_remove_string = "Alex, Artem, Anton";
	char* vstring_remove_result = vstring_remove(
		vstring_remove_string, 
		vstring_last_index_of(vstring_remove_string, 'A')-2);
	printf("vstring_remove_result: %s\n", vstring_remove_result);
	
	char* vstring_remove_range_result = vstring_remove_range("abc---def", 3, 9);
	printf("vstring_remove_range_result: %s\n", vstring_remove_range_result);

	char* vstring_pushs_result;
	vstring_set(vstring_pushs_result, "string ");
	vstring_pushs(vstring_pushs_result, "get");
	vstring_pushs(vstring_pushs_result, " bigger");
	vstring_pushs(vstring_pushs_result, "!");
	printf("vstring_pushs_result: %s\n", vstring_pushs_result);
	
	char* vstring_to_upper_result = vstring_to_upper("String to upper");
	printf("vstring_to_upper_result: %s\n", vstring_to_upper_result);

	char* vstring_to_lower_result = vstring_to_lower("StrIng tO LOWER");
	printf("vstring_to_lower_result: %s\n", vstring_to_lower_result);

	int32 vstring_index_of_result = vstring_index_of("matrix A * B = C A", 'A');
	printf("vstring_index_of_result: %d\n", vstring_index_of_result);

	int32 vstring_last_index_of_result = vstring_last_index_of("matrix A * B = C A", 'A');
	printf("vstring_last_index_of_result: %d\n", vstring_last_index_of_result);

	char* vstring_replace_character_result = 
		vstring_replace_character("new.string.with.replaced.dot", '.', ' ');
	printf("vstring_replace_character_result: %s\n", vstring_replace_character_result);

	char* vstring_replace_string_result = 
		vstring_replace_string("Hello, Mike! It's Alex", "Alex", "Joe");
	printf("vstring_replace_string_result: %s\n", vstring_replace_string_result);
	
	char* vstring_substring_result = vstring_substring("one two three", 4);
	printf("vstring_substring_result: %s\n", vstring_substring_result);

	char* vstring_substring_from_result = 
		vstring_substring_range("one two three", 0, 2);
	printf("vstring_substring_from_result: %s\n", vstring_substring_from_result);
	
	const char* vstring_format_name = "Kris";
	int32 age = 20;
	char* vstring_format_result = 
	vstring_format(
		"Hello, my name is %s! I'm %d years old.", 
		vstring_format_name,
		age);

}