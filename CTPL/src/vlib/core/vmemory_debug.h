#pragma once

#include "vtypes.h"

/*
	TODO:
	* rewrite all piece of potato stuff 
*/

typedef struct
{
	uint32 exec_line;
	uint32 allocated_bytes; 
	const char* file_name;
	void* pointer;
} allocation_info_t;

typedef struct 
{
	int32 length;
	int32 count;
	allocation_info_t* allocation_info;
} allocation_info_array_t;

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

memory_debug_info gmemory_allocation_info;

void mdi_init(memory_debug_info* this, int32 length);
void mdi_get_copy(memory_debug_info* copy_to, memory_debug_info* copy_from);
void mdi_get_bigger_copy(memory_debug_info* copy_to, memory_debug_info* copy_from);
void mdi_get_except_copy(memory_debug_info* copy_to, memory_debug_info* copy_from, int32 id);
int32 mdi_search_pointer(memory_debug_info* this, void* pointer);
void mdi_add(memory_debug_info* this, allocation_info_t value_to_add);
void mdi_remove_by_id(memory_debug_info* this, int32 id);
void mdi_remove_by_pointer(memory_debug_info* this, void* allocated_chunk_of_memory);
void mdi_free(memory_debug_info* this);
void mdi_printf(memory_debug_info* this);

void* vmemory_debug_malloc(uint32 size, const char* file_name, uint32 exec_line);
void  vmemory_debug_free(void* data, const char* file_name, uint32 exec_line);

#if MEMORY_DEBUG == 1
	#define malloc(size) vmemory_debug_malloc(size, __FILE__, __LINE__);
	#define free(data)   vmemory_debug_free  (data, __FILE__, __LINE__);
#endif

void vmemory_debug_init();
void vmemory_debug_print_state(int32 forced_delete_garbage);
int32 vmemory_debug_get_log();
void vmemory_debug_set_log(int8 value);

#define MEMDEB(log, x) vmemory_debug_init();vmemory_debug_set_log(log);x;vmemory_debug_print_state(0)

