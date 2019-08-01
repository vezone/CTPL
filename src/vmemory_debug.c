#include <stdlib.h>
#include <stdio.h>

#include "vmemory_debug.h"


void mdi_init(memory_debug_info* this, int32 length)
{
	this->init = 1;
	this->log_on = 0;
	this->malloc_counter = 0;
	this->free_counter = 0;
	
	this->length = length;
	this->count = 0;
	this->allocation_info = malloc(length * sizeof(allocation_info_t));
}

void mdi_get_copy(memory_debug_info* copy_to, memory_debug_info* copy_from)
{
	int32 index;
	mdi_init(copy_to, copy_from->length);
	copy_to->length = copy_from->length;
	copy_to->count  = copy_from->count;
	for (index = 0; 
		 index < copy_from->length; 
		 index++)
	{
		copy_to->allocation_info[index] = copy_from->allocation_info[index];
	}
}

void mdi_get_bigger_copy(memory_debug_info* copy_to, memory_debug_info* copy_from)
{
	int32 index;
	mdi_init(copy_to, copy_from->length + 1);
	copy_to->length = copy_from->length + 1;
	copy_to->count  = copy_from->count;
	for (index = 0; 
		 index < copy_from->length; 
		 index++)
	{
		copy_to->allocation_info[index] = copy_from->allocation_info[index];
	}
}

void mdi_get_except_copy(memory_debug_info* copy_to, memory_debug_info* copy_from, int32 id)
{
	int32 less_length = copy_from->length - 1;
	if (less_length >= 0)
	{
		int32 index;
		if (less_length == 0)
		{ 
			mdi_init(copy_to, 1);
		}
		else
		{
			mdi_init(copy_to, less_length);
		}
		
		copy_to->length = less_length;
		copy_to->count = copy_from->count - 1;

		for (index = 0; 
			 index < id; 
			 index++)
		{
			copy_to->allocation_info[index] = copy_from->allocation_info[index];
		}

		for (index = (id + 1); 
			 index < copy_from->length; 
			 index++)
		{
			copy_to->allocation_info[index] = copy_from->allocation_info[index];
		}
	}
	else
	{
		printf("mdi_get_except_copy: (copy_from->length - 1) < 0 \n");
	}
}

int32 mdi_search_pointer(memory_debug_info* this, void* pointer)
{
	int32 index;
	for (index = 0; 
		 index < this->count; 
		 index++)
	{
		if (this->allocation_info[index].pointer == pointer)
		{
			return index;
		}
	}
	return -1;
}

void mdi_add(memory_debug_info* this, allocation_info_t value_to_add)
{
	if ((this->count) >= (this->length))
	{
		memory_debug_info result;
		mdi_get_bigger_copy(&result, this);
		result.allocation_info[result.count] = value_to_add;
		++result.count;
		this = &result;
	}
	else
	{
		this->allocation_info[this->count] = value_to_add;
		++this->count;
	}
}

void mdi_remove_by_id(memory_debug_info* this, int32 id)
{
	if ((id < this->count) &&
		 id >= 0)
	{
		memory_debug_info result;
		mdi_get_except_copy(&result, this, id);
		this = &result;
	}
}

void mdi_remove_by_pointer(memory_debug_info* this, void* allocated_chunk_of_memory)
{
	if (this)
	{
		if (allocated_chunk_of_memory)
		{
			int32 id = mdi_search_pointer(this, allocated_chunk_of_memory);
			if (id != -1)
			{
				int32 less_length = this->length - 1;
				if (less_length >= 0)
				{
					int32 index;
					memory_debug_info result;
					result.length = less_length;
					result.count = this->count - 1;
					
					if (less_length == 0)
					{
						mdi_init(&result, 1);
					}
					else
					{
						mdi_init(&result, less_length);
					}
					
					for (index = 0; 
						 index < id; 
						 index++)
					{
						result.allocation_info[index] = this->allocation_info[index];
					}

					for (index = (id + 1); 
						 index < this->length; 
						 index++)
					{
						result.allocation_info[index - 1] = this->allocation_info[index];
					}
					
					this = &result;
				}
			}
		}
	}
	else
	{
		printf("vmemory_debug_darray_get_lesser_copy: (copy_from->length - 1) < 0 \n");	
	}
}

void mdi_free(memory_debug_info* this)
{
	int32 i;
	for (i = 0; i < this->count; i++)
	{
		if (this->allocation_info[i].pointer)
		{
			if (gmemory_allocation_info.log_on)
			{
				printf("free {%ld}\n", (int64)this->allocation_info[i].pointer);
			}
			free(this->allocation_info[i].pointer);
		}
	}
}

/*
	typedef struct
	{
		int8 init;
		int8 log_on;
		int32 malloc_counter;
		int32 free_counter;
		
		int32 length;
		int32 count;
		allocation_info_t* allocation_info;
		
		allocation_info_array_t* allocations;
	} memory_debug_info;
*/
void mdi_print(memory_debug_info* this)
{
	if (this)
	{
		int32 i;
		for (i = 0; i < this->count; i++)
		{
			printf("pointer: %ld, file: %s, line: %u\n",
				(int64) this->allocation_info[i].pointer,
					 	this->allocation_info[i].file_name,
					 	this->allocation_info[i].exec_line);
		}
	}
}

void* vmemory_debug_malloc(uint32 size, const char* file_name, uint32 exec_line)
{
	if (gmemory_allocation_info.init != 1)
	{
		printf("ERROR: gmemory_allocation_info.init != 1\n");
		return NULL;
	}
	
	if (gmemory_allocation_info.log_on)
	{
		printf(BLUE("Malloc_call:")" File: %s, Line: %i\n", file_name, exec_line);
	}
	
	++gmemory_allocation_info.malloc_counter;
	
	void* data = malloc(size);
	allocation_info_t allocation_info;
	allocation_info.pointer = data;
	allocation_info.file_name = file_name;
	allocation_info.exec_line = exec_line;
	allocation_info.allocated_bytes = size;
	
	mdi_add(&gmemory_allocation_info, allocation_info);
	
	return data;
}

void vmemory_debug_free(void* data, const char* file_name, uint32 exec_line)
{
	if (data)
	{
		++gmemory_allocation_info.free_counter;
		
		int32 index = mdi_search_pointer(&gmemory_allocation_info, data);
				
		if (index != -1)
		{
			mdi_remove_by_id(&gmemory_allocation_info, index);
		}
		
		if (gmemory_allocation_info.log_on)
			printf(GREEN("Free_call")": File: %s, Line: %i\n", file_name, exec_line);
			
		free(data);
	}
}

void vmemory_debug_init()
{
	gmemory_allocation_info.init = 1;
	gmemory_allocation_info.log_on = 0;
	mdi_init(&gmemory_allocation_info, 10);
	gmemory_allocation_info.free_counter = 0;
	gmemory_allocation_info.malloc_counter = 0;
}

/*TODO*/
void vmemory_debug_print_state(int32 forced_delete_garbage)
{
	printf(
		"Malloc calls: "GREEN("%d")"\nFree calls: "GREEN("%d")"\n", 
		gmemory_allocation_info.malloc_counter,
		gmemory_allocation_info.free_counter);

	if (gmemory_allocation_info.malloc_counter > gmemory_allocation_info.free_counter)
	{
		int32 index;
		printf(RED5("There are memory leak!!!\n"));
		for (index = 0; 
			index < gmemory_allocation_info.count; 
			index++)
		{
			int64 ptr = (int64) gmemory_allocation_info
				.allocation_info[index]
				.pointer;
			printf("pointer: "GREEN("%ld")", exec_line: "GREEN("%d")", file_name: "GREEN("%s")", allocated bytes: "GREEN("%d")"\n", 
				ptr,
				gmemory_allocation_info.allocation_info[index].exec_line,
				gmemory_allocation_info.allocation_info[index].file_name,
				gmemory_allocation_info.allocation_info[index].allocated_bytes);
			
			if (forced_delete_garbage)
			{
				printf("deleting garbaged: "YELLOW("%ld\n"), ptr);
				if (gmemory_allocation_info
					.allocation_info[index]
					.pointer)
				{
					free(gmemory_allocation_info
						.allocation_info[index]
						.pointer);
					gmemory_allocation_info
						.allocation_info[index]
						.pointer = NULL;
				}
			}
		}
	}
}

int32 vmemory_debug_get_log()
{
	return gmemory_allocation_info.log_on;
}

void vmemory_debug_set_log(int8 value)
{
	gmemory_allocation_info.log_on = value;
}
