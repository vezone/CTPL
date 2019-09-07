#pragma once 

#include "vtypes.h"
#include "varray.h"
#include "vstring.h"

typedef struct vintern
{
    int32 length;
    const char * buffer;
} vintern;

static vintern* interns;

const char* vstring_intern_range(const char* start, const char* end)
{
    int32 i;
    int32 length = end - start;
    for (vintern* it = interns; it != varray_end(interns); it++)
    {
        if (it->length == length && 
            vstring_compare_to(it->buffer, start, length))
        {
            return it->buffer;
        }
    }
    char* new_string = malloc((length+1)*sizeof(char));
    vstring_copy(new_string, start, length);
    new_string[length] = '\0';
    varray_push(interns, ((vintern) {length, new_string}));
    return new_string;
}

const char* vstring_intern(const char* string)
{
    return vstring_intern_range(string, string + vstring_length(string));
}

void vstring_intern_test()
{
    char x[] = "hello, world";
    char y[] = "hello, world";

    printf("x == y [%d]\n", (vstring_intern(x) == vstring_intern(y)));

}