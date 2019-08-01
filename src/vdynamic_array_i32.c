#include <stdlib.h>
#include <stdio.h>
#include "vdynamic_array_i32.h"

vdynamic_array_i32 vdynamic_array_i32_init(int32 length)
{
	vdynamic_array_i32 result;
	result.length = length;
	result.count = -1;
	result.buffer = malloc(sizeof(length * sizeof(int32)));
	return result;
}	

void vdynamic_array_i32_add(vdynamic_array_i32* this, int32 value_to_add)
{
	if ((this->count + 1) >= this->length)
	{
		void* reallocated_buffer;
		reallocated_buffer = realloc(this->buffer, (this->length + 1) * sizeof(int32));
		if (reallocated_buffer)
		{
			this->buffer = reallocated_buffer;
			++this->count;
			++this->length;
			this->buffer[this->count] = value_to_add;
		}
		else 
		{
			printf("vdynamic_array_i32 reallocation failed!\nfree pointer:%ld", (int64)this->buffer);
			if (this->buffer)
			{
				free(this->buffer);
				this->buffer = nullptr;
			}
		}
	}
	else
	{
		++this->count;
		this->buffer[this->count] = value_to_add;
	}
}

void vdynamic_array_i32_remove_by_id(vdynamic_array_i32* this, int32 id_to_remove)
{
	if (id_to_remove <= this->count)
	{
		int32* new_buffer = malloc((this->length - 1) * sizeof(int32));
		for (int32 i = 0; i < id_to_remove; i++)
		{
			new_buffer[i] = this->buffer[i];
		}
		
		for (int32 i = (id_to_remove + 1); i < this->length; i++)
		{
			new_buffer[i-1] = this->buffer[i];
		}
		
		if (this->buffer)
		{
			free(this->buffer);
			this->buffer = nullptr;
		}
		
		this->buffer = new_buffer;
		--this->count;
		--this->length;
	}
}

int32 vdynamic_array_i32_get_sum(vdynamic_array_i32* this)
{
	int32 result = 0;
	
	for (int32 index = 0; 
		index <= this->count;
		index++)
	{
		result += this->buffer[index];	
	}
	
	return result;
}

int32 vdynamic_array_i32_get(vdynamic_array_i32* this, int32 index)
{
	if (index >= 0
		&& index <= this->count)
	{
		return this->buffer[index];
	}
	return -1;
}

void vdynamic_array_i32_set(vdynamic_array_i32* this, int32 index, int32 value)
{
	if (index >= 0
		&& index <= this->count)
	{
		this->buffer[index] = value;
	}
	else
	{
		printf("vdynamic_array_i32_set: index out of range\n");	
	}
}

void vdynamic_array_i32_clear(vdynamic_array_i32* this)
{
	for (int32 i = 0; i < this->length; i++)
	{
		this->buffer[i] = 0;
	}
		
	this->count = -1;
}

void vdynamic_array_i32_free(vdynamic_array_i32* this)
{
	if (this->buffer)
	{
		free(this->buffer);
		this->buffer = nullptr;
	}
}

void vdynamic_array_i32_print(vdynamic_array_i32* this)
{
	for (int32 i = 0; i <= this->count; i++)
	{
		printf("%d ", this->buffer[i]);
	}	
	printf("\n");
}

void vdynamic_array_i32_test()
{
	printf("vdynamic_array_i32_test started!\n");
	/*
	NOTE:
	по всей видимости, malloc укладывает аллокацию в маленький 
	кусочек памяти под 3*4 байта, скорее всего, эта область
	памяти располагается в дефрагментированном пространстве
	из-за чего мы имеем дело с ситуацией, когда реалокация может 
	привести нас к используемой области памяти, данную проблему,
	скорее всего, можно решить большой аллокацие и общей кучей.
	NOTE2: НИКОГДА НЕ ЗАБЫВАЙ СМОТРЕТЬ СКОЛЬКО ПАМЯТИ ТЫ ВЫДЕЛЯЕШЬ !!! 
	[Mind should be data oriented]
	*/
	vdynamic_array_i32 max_lengths = vdynamic_array_i32_init(3);
	for (int32 i = 0; i < 10; i++)
	{
		printf("%d\n", i);
		vdynamic_array_i32_add(&max_lengths, i);
	}
	
	vdynamic_array_i32_remove_by_id(&max_lengths, 9);
	
	vdynamic_array_i32_print(&max_lengths);
	
	vdynamic_array_i32_free(&max_lengths);
	
	
}
