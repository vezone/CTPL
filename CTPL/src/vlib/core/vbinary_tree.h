#pragma once

#include "vtypes.h"

typedef struct
{
	void* prev;
	void* next_left;
	void* next_right;
	int64_t value;
} binary_tree_t;

binary_tree_t* binary_tree_init(binary_tree_t* root, int64 value_to_add);
void binary_tree_add(binary_tree_t* root, int64 value_to_add);
void binary_tree_show_full(binary_tree_t* root);
void binary_tree_test();
