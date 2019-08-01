#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#define wait_for_input() int wait_for_input_number; scanf("%d", &wait_for_input_number);
#define nullptr (void*) 0

#include "vtypes.h"
#include "vstring.h"
#include "vstring_builder.h"

#include "vdynamic_array_i32.h"
#include "vdynamic_table.h"

/*

#include "vtree.h"

*/

/*
	NOTE:
	Конвенция.
	Именование функций:
		* каждая функция и структура начинается с маленькой буквы v, 
		что определяет принадлежность данной функции или структуры к
		данной библиотеке 
			v
		* после буквы v идет структура данных, с которой работает данная функция
		как правило это название должно совпадать с названием файла, в котором 
		располагается функция
			vstring_builder
		* после идет имя операции, которая выполняется над структурой данных
			vstring_builder_append_string
		* первый аргумент такой функции всегда имеет тип структуры данных текущего 
		файла
			vstring_builder_append_string(vstring_builder* builder)
		* следующие параметры идут в независимости от конвенции, но в частных случаях
		название второго параметра вынесено в название функции, как в случае со 
		vstring_builder_append_string, где второй параметр является char*
			vstring_builder_append_string(vstring_builder* builder, char* string);
			
	Именование структур данных:
	
	Именование файлов:
				
			
*/

/*
	TODO:
	*переписать vstring.h, чтобы соблюсти v конвенцию
	**vstring for vstring_builder.h
	**vfloat_to_string() 
	*переписать vdynamic_table, чтобы соблюсти v конвенцию 
	*разделить все .h files на .h + .c files
	*vmemory debug:
	** пофиксить: работает только в main.c
	NOTE: 
		* все дело в scope препроцессоров, нужно создать 
		malloc func pointer / free func pointer и при vmemory_debug_init 
		malloc pointer = vmemory_debug_malloc 
		* убрать pragma once
		* сделать full .h
*/

/*
func pointer:
	void *(*start_routine) (void *)
*/

int main()
{
	
	#if 0
	//tree_test();
	vdynamic_array_i32_test();
	#else
	
	/*
	можно сделать вывод на экран, который будет видоизменяться в зависимости
	от того вывода, который мы будем делать, то есть принцип работы следующий:
		print("root")
		state: 
		root
		
		print("child1  child2");
		state:
			 root
		child1  child2
		
		print("child11  child12  child22");
		state:
		           root
		      child1  child2
		child11  child12  child22
		
	в данной репрезентации, 
	state - это вывод в консоль на момент добавленя потомков к root
	то есть state динамически обновляется в зависимости от длины
	нового output.
	
	в качестве начальной реализации можно создать простую динамическую таблицу.
		
	*/
	/*
	
	vmemory_debug_init();
	vmemory_debug_set_log(0);
	
	MEMDEB(
		void* data = malloc(12);
		data = malloc(120);
		data = malloc(234);
		data = malloc(1024);
		data = malloc(512);
	);
	*/
	
	vstring_builder_test();
	 
	//dynamic_table_test()
	
	//vmemory_debug_print_state(1);
	
	
	#endif	
	
	return 0;
}
