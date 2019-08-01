#include <stdlib.h>
#include <stdio.h>

#include "vstring_builder.h"
#include "vstring.h"
/*
	int32 -error code
	usage:
	vstring_builder builder;
	vstring_builder_init(&builder);
*/
int32 vstring_builder_init(vstring_builder* builder)
{
	builder->count = 0;
	builder->realloc_size = 500;
	builder->length = 500;
	builder->buffer = malloc(500 * sizeof(char));
	
	if (!builder->buffer)
	{
		return -1;
	}
	
	return 0;
}

int32 vstring_builder_append_character(vstring_builder* builder, char character)
{
	if ((builder->count + 1) <= builder->length)
	{
		builder->buffer[builder->count] = character;
		++builder->count;
	}
	else
	{
		int32 new_length = builder->length + builder->realloc_size;
		char* data = malloc(new_length);
		
		for (int32 i = 0; i < builder->count; i++)
		{
			data[i] = builder->buffer[i];
		}
		data[builder->count] = character;
		++builder->count;
		
		if (builder->buffer)
		{
			free(builder->buffer);
		}
		
		builder->buffer = data;
	}
	return 0;
}

int32 vstring_builder_append_string(vstring_builder* builder, char* string)
{
	int32 string_length = vstring_length(string);
	if ((string_length + builder->count) <= builder->length)
	{
		for (int32 i = 0; i < string_length; i++)
		{ 
			builder->buffer[i + builder->count] = string[i];
		}
		builder->count += string_length;
	}
	else
	{
		int32 new_length = builder->length + builder->realloc_size;
		char* data = malloc(new_length);
		
		for (int32 i = 0; i < builder->count; i++)
		{
			data[i] = builder->buffer[i];
		}
			
		for (int32 i = 0; i < string_length; i++)
		{
			data[i + builder->count] = string[i];
		}
		builder->count += string_length;
		
		if (builder->buffer)
		{
			free(builder->buffer);
		}
		
		builder->buffer = data;
	}
	
	return 0;
}

int32 vstring_builder_append_format(vstring_builder* builder, const char* format, ...)
{
	//va_list valist;
	//va_start(valist, arguments_count)
	//va_arg(valist, double);
	//va_end(valist);
	
	//calculate how many arguments
	int32 state = 0;
	int32 arguments_count = 0;
	va_list valist;
	for (char* ptr = (char*)format; *ptr != '\0'; ptr++)
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
		return -1;
	}
	
	state = 0;
	va_start(valist, format);
	while (*format != '\0')
	{
		
		switch (*format)
		{
			case '%':
			{
				state = 1;
			} break;
			case 'c':
			{
				if (state == 1)
				{
					state = 0;
					char elementc = (char) va_arg(valist, int32);
					vstring_builder_append_character(builder, elementc);
				}
				else
				{
					vstring_builder_append_character(builder, *format);
				}
			} break;
			case 's':
			{
				if (state == 1)
				{
					state = 0;
					const char* elements = va_arg(valist, const char *);
					vstring_builder_append_string(builder, (char*) elements);
				}
				else
				{
					vstring_builder_append_character(builder, *format);
				}
			} break;
			case 'l':
			{
				if (state == 1)
				{
					printf("char: l\n");
					state = 2;
				}
				else
				{
					vstring_builder_append_character(builder, *format);
				}
			} break;
			case 'd':
			{
				if (state == 1)
				{
					int32 elementi32 = va_arg(valist, int32);
					char* elementi32_to_string = vint32_to_string(elementi32);
					vstring_builder_append_string(builder, elementi32_to_string);
					if (elementi32_to_string)
					{ 
						free(elementi32_to_string);
					}
					state = 0;
				}
				else if (state == 2)
				{
					int64 elementi64 = va_arg(valist, int64);
					char* elementi64_to_string = vint64_to_string(elementi64);
					vstring_builder_append_string(builder, elementi64_to_string);
					if (elementi64_to_string)
					{
						free(elementi64_to_string);
					}
					state = 0;
				}
				else
				{
					vstring_builder_append_character(builder, *format);
				}
			} break;
			case 'b':
			{
				if (state == 1)
				{
					int8 elementi8 = (int8) va_arg(valist, int32);
					char* elementi8_to_str = vint32_to_string(elementi8);
					vstring_builder_append_string(builder, elementi8_to_str);
					if (elementi8_to_str)
					{
						free(elementi8_to_str);
					}
					state = 0;
				}
				else
				{
					vstring_builder_append_character(builder, *format);
				}
			} break;
			case 'f':
			{
				if (state == 1)
				{
					double elementd = va_arg(valist, double);
					char* elementd_to_str = vfloat_to_string(elementd);
					vstring_builder_append_string(builder, elementd_to_str);
					if (elementd_to_str)
					{
						free(elementd_to_str);
					}
					state = 0;
				}
				else
				{
					vstring_builder_append_character(builder, *format);
				}
			} break;
			default:
			{
				vstring_builder_append_character(builder, *format);
				state = 0;
			} break;
			
		}
		
		format++;
		
	}
	
	return 0;
}

void vstring_builder_replace(vstring_builder* builder, char old_character, char new_character)
{
	int32 index;
	for (index = 0; index < builder->count; index++)
	{
		if (builder->buffer[index] == old_character)
		{
			builder->buffer[index] = new_character;
		}
	}
}

//'A' == 65
//'a' == 97
void vstring_builder_to_upper_case(vstring_builder* builder)
{
	char temp;
	int32 index;
	for (index = 0; index < builder->count; index++)
	{
		temp = builder->buffer[index];
		if (temp >= 'a' && temp <= 'z')
		{
			builder->buffer[index] = temp - 'a' + 'A';
		}
	}
}

void vstring_builder_to_lower_case(vstring_builder* builder)
{
	char temp;
	int32 index;
	for (index = 0; index < builder->count; index++)
	{
		temp = builder->buffer[index];
		if (temp >= 'A' && temp <= 'Z')
		{
			builder->buffer[index] = temp - 'A' + 'a';
		}
	}
}

int32 vstring_builder_contains(vstring_builder* builder, const char* part)
{
	int32 index;
	int32 equals;
	int32 c;
	char* temp = (char*) part;
	int32 temp_length = vstring_length(temp);
	for (index = 0; index < builder->count; index++)
	{
		equals = 1;
		for (c = 0; c < temp_length; c++)
		{
			if (builder->buffer[c + index] != temp[c])
			{
				equals = 0;
			} 
		}
		
		if (equals)
		{
			return equals;
		}
	}
	
	return equals;
}

char* vstring_builder_to_string(vstring_builder* builder)
{
	return builder->buffer;
}

void vstring_builder_test()
{
	
	vstring_builder builder;
	int32 errcode = vstring_builder_init(&builder);
	
	#if 0
	vstring_builder_append_string(&builder, "Hello");
	vstring_builder_append_string(&builder, " ");
	vstring_builder_append_string(&builder, "my");
	vstring_builder_append_string(&builder, " ");
	vstring_builder_append_string(&builder, "name");
	vstring_builder_append_string(&builder, " ");
	vstring_builder_append_string(&builder, "is");
	vstring_builder_append_string(&builder, " ");
	//vstring_builder_append_string(&builder, "Vova");
	vstring_builder_append_character(&builder, 'V');
	vstring_builder_append_character(&builder, 'o');
	vstring_builder_append_character(&builder, 'v');
	vstring_builder_append_character(&builder, 'a');
	vstring_builder_append_string(&builder, "!");
	printf("%s\n", builder.buffer);
	#endif
	vstring_builder_append_format(&builder, 
		"character: %c, int8: %d, int: %d, float: %f, double: %f, string: %s, int: %d", 
		'g', 8, 100, 12.3f, 1.03204, "string", 100500);
	vstring_builder_append_string(&builder, " Hello");
	printf("%s\n", builder.buffer);
	
	
	vstring_builder_to_upper_case(&builder);
	printf("%s\n", builder.buffer);
	vstring_builder_to_lower_case(&builder);
	printf("%s\n", builder.buffer);
	
	
	int32 cont = vstring_builder_contains(&builder, "dou");
	if (cont)
		printf("contains\n");
	else
		printf("does not\n");
	
}


