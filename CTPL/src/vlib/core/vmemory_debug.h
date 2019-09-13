#pragma once

#include "vtypes.h"

typedef struct allocation_info
{
	uint32 exec_line;
	uint32 allocated_bytes; 
	const char* file_name;
	void* pointer;
} allocation_info;


typedef struct memory_debug_info
{
	uint8 log_on;
	uint32 malloc_counter;
	uint32 free_counter;
	allocation_info* allocation_info_array;
} memory_debug_info;

memory_debug_info g_memory_allocation_info;

void allocation_info_remove_pointer(void* data);

void* vmemory_debug_malloc(uint32 size, const char* file_name, uint32 exec_line);
void  vmemory_debug_free(void* data, const char* file_name, uint32 exec_line);

void vmemory_debug_init(int8 log_on);
void vmemory_debug_print_state(int8 forced_delete_garbage);

void vmemory_debug_test();