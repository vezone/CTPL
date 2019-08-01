#include <stdlib.h>
#include <stdio.h>

#include "vtree.h"
#include "vstring.h"

tree_t* tree_init(tree_t* root, int64 value_to_add)
{
	root = malloc(sizeof(tree_t));
	root->prev = (void*) 0;
	root->value = value_to_add;
	return root;
}

tree_t* tree_add(tree_t* root, int64 value_to_add)
{
	root->next = (tree_t*) malloc(sizeof(tree_t));
	((tree_t*)root->next)->prev = root;
	((tree_t*)root->next)->value = value_to_add;
	return (tree_t*) root->next;
}

void tree_show_full(tree_t* root)
{
	tree_t* root_pointer = root;
	for (; 
		root_pointer->next != (void*)0; 
		root_pointer = root_pointer->next)
	{
		printf("pointer: %ld, prev: %ld, next: %ld, value: %ld\n", 
			(int64)root_pointer,
			(int64)root_pointer->prev,
			(int64)root_pointer->next,
			root_pointer->value);
	}
	root_pointer = (void*) 0;
}

void tree_visual_show(tree_t* root)
{
	int32 depth = 0;
	for (tree_t* ptr = root; 
		 ptr != nullptr; 
		 ptr = ptr->next)
	{
		char* print_str = nullptr;
		/*
			0 - pointer: 123
			1 - -->pointer: 234
			2 - ---->pointer: 345
			3 - ------>pointer: 456
		*/
		
		if (depth > 1)
		{
			print_str = (vstring_with(' ', (depth-1)*2));
		}
		
		printf("%s%spointer: %ld, value: %ld\n", 
			(print_str != nullptr) ? print_str : "", 
			(depth > 0) ? "-->" : "", 
			(int64) ptr,
			ptr->value);
		
		if (print_str)
		{
			free(print_str);
		}
		
		++depth;
	}
		
}

void tree_test()
{
	tree_t* root;
	root = tree_init(root, 0);
	tree_t* root_pointer = root;
	for (int32 i = 1; i < 10; i++)
	{
		root_pointer = tree_add(root_pointer, i);
	}
	
	//tree_show_full(root);
	tree_visual_show(root);
}
