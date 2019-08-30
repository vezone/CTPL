#include "varray.h"
#include "vtypes.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void* varray_grow(const void* array, size_t new_len, size_t elem_size)
{
	size_t new_cap = MAX(1 + 2 * varray_cap(array), new_len);
	assert(new_len <= new_cap);
	size_t new_size = offsetof(varray_header, array) + new_cap*elem_size;
	varray_header* new_hdr;
	if (array)
	{
		new_hdr = realloc(varray_hdr(array), new_size); 
	}
	else
	{
		new_hdr = malloc(new_size);
		new_hdr->length = 0;
	}
	
	new_hdr->capacity = new_cap;
	return new_hdr->array;
}

typedef struct
{
	int32 age;
	char* name;
} person;

//stretchy buffer test
void varray_test()
{
    int32 i;
	person* arr = NULL;
	person temp;
	temp.age = 25;
	temp.name = "Jack";
	for (i = 0; i < 10; i++)
	{
		varray_push(arr, temp);
		++temp.age;
	}

	for (i = 0; i < 10; i++)
	{
		printf("Jack%d [age = %d, name = %s]\n", i, arr[i].age, arr[i].name);
	}

    varray_free(arr);
}