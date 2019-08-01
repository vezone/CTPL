#include <stdlib.h>
#include <stdio.h>

#include "vbinary_tree.h"
#include "vstring.h"

binary_tree_t* binary_tree_init(binary_tree_t* root, int64 value_to_add)
{
	root = malloc(sizeof(binary_tree_t));
	root->prev = (void*) 0;
	root->value = value_to_add;
	return root;
}

void binary_tree_add(binary_tree_t* root, int64 value_to_add)
{
	while (1)
	{
		if (((binary_tree_t*)root->next_left) != nullptr)
		{
			if (((binary_tree_t*)root->next_left)->value > value_to_add)
			{
				((binary_tree_t*)root->next_left)->next_left = (binary_tree_t*) malloc(sizeof(binary_tree_t));
				((binary_tree_t*)root->next_left)->prev = root;
				((binary_tree_t*)root->next_left)->value = value_to_add; 
			}
		}
		else if (((binary_tree_t*)root)->value < value_to_add)
		{
			((binary_tree_t*)root)->next_left = (binary_tree_t*) malloc(sizeof(binary_tree_t));
		}
		else if (((binary_tree_t*)root)->value < value_to_add)
		{
			((binary_tree_t*)root)->next_left = (binary_tree_t*) malloc(sizeof(binary_tree_t));
		}
	}
	/*
	if (value_to_add < root->value)
	{
		root->next_left  = (binary_tree_t*) malloc(sizeof(binary_tree_t));
		((binary_tree_t*)root->next_left)->prev = root;
		((binary_tree_t*)root->next_left)->value = value_to_add;
	}
	else
	{
		root->next_right = (binary_tree_t*) malloc(sizeof(binary_tree_t));
		((binary_tree_t*)root->next_right)->prev = root;
		((binary_tree_t*)root->next_right)->value = value_to_add;
	}
	*/
}

void binary_tree_show_full(binary_tree_t* root)
{
	binary_tree_t* root_pointer = root;
	for (; 
		root_pointer->next_left != (void*)0; 
		root_pointer = root_pointer->next_left)
	{
		printf("pointer: %ld, prev: %ld, next_left: %ld, value: %ld\n", 
			(int64_t)root_pointer,
			(int64_t)root_pointer->prev,
			(int64_t)root_pointer->next_left,
			root_pointer->value);
	}
	root_pointer = (void*) 0;
}

void binary_tree_test()
{
	binary_tree_t* root;
	root = binary_tree_init(root, 5);
	
	printf("pointer: %ld\n", (int64_t) root);
	printf("root\n");
	printf("-->left_child\n");
	printf("  -->left_child\n");
	printf("  -->right_child\n");
	printf("-->right_child\n");
	
	const char* str = vstring_with(' ', 21);
	printf("%s%s%s\n", "begin", vstring_with(' ', 21), "end");	
}
