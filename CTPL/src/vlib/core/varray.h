#pragma once

/*
bitwise: all the C data structures you'll ever need
1. Stretchy buffers
2. Pointer/uintptr hash tables  (uintptr -> uintptr key-value mapping)
3. String intern table
*/

#include <stddef.h>

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

typedef struct varray_header
{
    size_t length;
    size_t capacity;
    char array[0]; // [0] C99 only feature
} varray_header;

/*
 	The macro offsetof() returns the offset of the field 
	member from the start of the structure type
	NOTE it's offset because we take 0 address memory 
	convert it to size_t type 
	#define offset(type, mem) (size_t) &(((type*)0)->mem)
*/

void* varray_grow(const void* array, size_t new_len, size_t elem_size);
void varray_test();

#define varray_hdr(b) ((varray_header*) ((char*)b - offsetof(varray_header, array)))
#define varray_len(b) ((b) ? varray_hdr(b)->length : 0)
#define varray_cap(b) ((b) ? varray_hdr(b)->capacity : 0)
#define varray_fits(b, n) (varray_len(b) + (n) <= varray_cap(b))
#define varray_fit(b, n) (varray_fits(b, n) ? 0 : ((b) = varray_grow((b), varray_len(b) + (n), sizeof(*b))))

#define varray_push(b, v) (varray_fit(b, 1), b[varray_len(b)] = (v), varray_hdr(b)->length++)
#define varray_free(b) ((b) ? free(varray_hdr(b)) : ((b) = NULL))
