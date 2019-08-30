#pragma once

#include "vtypes.h"

#define to_string(x) #x
#define vstring_set(s, x) int32 string_length=vstring_length(x); \
				s = malloc(string_length*sizeof(char)); \
				for(int32 i=0;i<string_length;i++) { s[i]=x[i]; }

void vstring_copy(char* dest, const char* src, int32 length);
int32 vstring_length(const char* str);
int32 vstring_compare(const char* left_string, const char* right_string);
char* vstring_inverse(const char* input);
void vstring_set_elements_to(char* input, char value);
void vstring_pushc(char* input, char character);
void vstring_pushs(char* input, const char* string);
char** vstring_split(const char* input, char split_character);
char* vstring_trim_end(const char* input, const char* trim_characters);
char* vstring_catc(const char* input, char character_to_delete);
char* vstring_cats(const char* input, char* characters_to_delete);
char* vstring_with(char character, int32 number_of_splitters);
int32 vstring_to_int32(const char* string);

char* vint32_to_string(int32 value);
char* vint64_to_string(int64 value);
char* vfloat_to_string(double value);

void vstring_test();