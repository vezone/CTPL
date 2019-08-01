#pragma once

#include "vtypes.h"

#define to_string(x) #x

void vstring_copy(char* dest, char* src, int32 length);
int32 vstring_length(char* str);
int32 vstring_compare(char* left_string, char* right_string);
char* vstring_catc(char* input, char character_to_delete);
char* vstring_cats(char* input, char* characters_to_delete);
char* vstring_with(char character, int32 number_of_splitters);
int32 vstring_to_int32(char* string);
char* vint32_to_string(int32 value);
char* vint64_to_string(int64 value);
char* vfloat_to_string(double value);
