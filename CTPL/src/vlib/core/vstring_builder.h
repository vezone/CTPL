#pragma once

#include <stdarg.h>
#include "vtypes.h"

typedef struct
{
	int32 count;
	int32 realloc_size;
	int32 length;
	char* buffer;
} vstring_builder;

int32 vstring_builder_init(vstring_builder* builder);
int32 vstring_builder_append_character(vstring_builder* builder, char character);
int32 vstring_builder_append_string(vstring_builder* builder, char* string);
int32 vstring_builder_append_format(vstring_builder* builder, const char* format, ...);
void vstring_builder_replace(vstring_builder* builder, char old_character, char new_character);
void vstring_builder_to_upper_case(vstring_builder* builder);
void vstring_builder_to_lower_case(vstring_builder* builder);
char* vstring_builder_to_string(vstring_builder* builder);
void vstring_builder_test();
