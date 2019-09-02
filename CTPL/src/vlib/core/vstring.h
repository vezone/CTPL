#pragma once

#include <stdarg.h>
#include "vtypes.h"

#define to_string(x) #x
#define vstring_set(s, x) {int32 vstring_set_string_length=vstring_length(x); \
				s = malloc(vstring_set_string_length*sizeof(char)); \
				for(int32 i=0;i<vstring_set_string_length;i++) { s[i]=x[i]; }}

void vstring_copy(char* dest, const char* src, int32 length);
void vstring_join(char* left_string, const char* right_string);
void vstring_join_character(char* left_string, char character);
char* vstring_format(const char* format, ...);
int32 vstring_length(const char* str);
int32 vstring_compare(const char* left_string, const char* right_string);
char* vstring_inverse(const char* input);
void vstring_set_elements_to(char* input, char value);
void vstring_pushc(char* input, char character);
void vstring_pushs(char* input, const char* string);
char** vstring_split(const char* input, char split_character);
char* vstring_trim_end(const char* input, const char* trim_characters);
char* vstring_catc(const char* input, char character_to_delete);
char* vstring_cats(const char* input, const char* characters_to_delete);
char* vstring_remove(const char* input, int32 start_index);
char* vstring_remove_range(const char* input, int32 start_index, int32 end_index);
char* vstring_with(char character, int32 number_of_splitters);
char* vstring_to_upper(const char* input);
char* vstring_to_lower(const char* input);
int32 vstring_index_of(const char* input, char character);
int32 vstring_last_index_of(const char* input, char character);
char* vstring_replace_character(const char* input, char old_character, char new_character);
char* vstring_replace_string(const char* input, const char* old_string, const char* new_string);
char* vstring_substring(const char* input, int32 start_index);
char* vstring_substring_from(const char* input, int32 start_index, int32 end_index);

int32 vstring_to_int32(const char* string);
char* vint32_to_string(int32 value);
char* vint64_to_string(int64 value);
char* vfloat_to_string(double value);

void vstring_test();