#pragma once

#include "vtypes.h"

typedef struct
{
	void* prev;
	void* next;
	int64 value;
} tree_t;

tree_t* tree_init(tree_t* root, int64 value_to_add);
tree_t* tree_add(tree_t* root, int64 value_to_add);
void tree_show_full(tree_t* root);
void tree_visual_show(tree_t* root);
void tree_test();
