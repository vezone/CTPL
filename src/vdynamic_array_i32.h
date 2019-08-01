#pragma once

#include "vtypes.h"

typedef struct 
{
	int32 length;
	int32 count;	
	int32* buffer;
} vdynamic_array_i32;

vdynamic_array_i32 vdynamic_array_i32_init(int32 length);
void vdynamic_array_i32_add(vdynamic_array_i32* this, int32 value_to_add);
void vdynamic_array_i32_remove_by_id(vdynamic_array_i32* this, int32 id_to_remove);
int32 vdynamic_array_i32_get_sum(vdynamic_array_i32* this);
int32 vdynamic_array_i32_get(vdynamic_array_i32* this, int32 index);
void vdynamic_array_i32_set(vdynamic_array_i32* this, int32 index, int32 value);
void vdynamic_array_i32_clear(vdynamic_array_i32* this);
void vdynamic_array_i32_free(vdynamic_array_i32* this);
void vdynamic_array_i32_print(vdynamic_array_i32* this);
void vdynamic_array_i32_test();
