#include <stdlib.h>
#include <stdio.h>

#include "vmemory_debug.h"
#include "varray.h"

//FIXME:
void allocation_info_remove_pointer(void* data)
{
	int32 length = varray_len(g_memory_allocation_info.allocation_info_array);
	allocation_info* new_array = malloc((length - 1) * sizeof(new_array));
	for (int32 index = 0, new_index = 0;
		index < length;
		index++, new_index++)
	{
		void* allocation_info_array_data =
			g_memory_allocation_info.allocation_info_array[index].pointer;  
		if (allocation_info_array_data != data)
		{
			new_array[new_index] = 
				g_memory_allocation_info.allocation_info_array[index];
		}
		else
		{
			new_index--;
		}
	}
	if (g_memory_allocation_info.allocation_info_array)
	{
		free(g_memory_allocation_info.allocation_info_array);
		printf("THERE\n");
		g_memory_allocation_info.allocation_info_array = new_array;
	}
} 

void vmemory_debug_init(int8 log_on)
{
	g_memory_allocation_info.log_on = (log_on ? 1 : 0);
	g_memory_allocation_info.free_counter = 0;
	g_memory_allocation_info.malloc_counter = 0;
}

void* vmemory_debug_malloc(uint32 size, const char* file_name, uint32 exec_line)
{
	void* data = malloc(size);
	if (data)
	{
		g_memory_allocation_info.malloc_counter++;
		allocation_info info;
		info.exec_line = exec_line;
		info.allocated_bytes = size; 
		info.file_name = file_name;
		info.pointer = data;
		varray_push(g_memory_allocation_info.allocation_info_array, info);
		return data;
	}
	printf("Error\n");
	return 0;
}

void  vmemory_debug_free(void* data, const char* file_name, uint32 exec_line)
{
	if (data)
	{
		free(data);
		g_memory_allocation_info.free_counter++;
		//FIXME: aborted error
		allocation_info_remove_pointer(data);
		printf("free call: %ld, %s, %d\n", (int64)data, file_name, exec_line);
		data = 0;		
	}
}

#if MEMORY_DEBUG == 0
	#define malloc(size) vmemory_debug_malloc(size, __FILE__, __LINE__);
	#define free(data)   vmemory_debug_free  (data, __FILE__, __LINE__);
#endif

void vmemory_debug_test()
{
	
	vmemory_debug_init(1);

	printf(GREEN("Start testing\n"));
	void* data1 = malloc(10);
	void* data2 = malloc(438);
	void* data3 = malloc(1009);
	void* data4 = malloc(15);
	void* data5 = malloc(14);
	void* data6 = malloc(17);

	free(data1);
	free(data2)
	free(data3);
	//free(data4);
	free(data5);
	free(data6);

	vmemory_debug_print_state(0);

}

#if 1
void vmemory_debug_print_state(int8 forced_delete_garbage)
{
	printf(
		"Malloc calls: "GREEN("%d")"\nFree calls: "GREEN("%d")"\n", 
		g_memory_allocation_info.malloc_counter,
		g_memory_allocation_info.free_counter);

	if (g_memory_allocation_info.malloc_counter > g_memory_allocation_info.free_counter)
	{
		int32 index;
		printf(RED5("There are memory leak!!!\n"));
		for (index = 0; 
			index < varray_len(g_memory_allocation_info.allocation_info_array); 
			index++)
		{
			//if ()
			int64 ptr = (int64) 
			g_memory_allocation_info
				.allocation_info_array[index]
				.pointer;
			printf("pointer: "GREEN("%ld")", exec_line: "GREEN("%d")", file_name: "GREEN("%s")", allocated bytes: "GREEN("%d")"\n", 
				ptr,
				g_memory_allocation_info.allocation_info_array[index].exec_line,
				g_memory_allocation_info.allocation_info_array[index].file_name,
				g_memory_allocation_info.allocation_info_array[index].allocated_bytes);
			
			if (forced_delete_garbage)
			{
				printf("deleting garbaged: "YELLOW("%ld\n"), ptr);
				if (g_memory_allocation_info
					.allocation_info_array[index]
					.pointer)
				{
					free(g_memory_allocation_info
						.allocation_info_array[index]
						.pointer);
					g_memory_allocation_info
						.allocation_info_array[index]
						.pointer = NULL;
				}
			}
		}
	}
}
#endif
