#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#define wait_for_input() int wait_for_input_number; scanf("%d", &wait_for_input_number);

#include "vlib/core/vtypes.h"
#include "vlib/core/vmath.h"
#include "vlib/core/vstring.h"
#include "vlib/core/vstring_builder.h"

#include "vlib/core/vdynamic_array_i32.h"
#include "vlib/core/vdynamic_table.h"
#include "vlib/core/varray.h"

#include "vlib/core/vbase64.h"

#include "vlib/core/vprofiling.h"


#include "vlib/lexer/programming_language.h"

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
	FIXME
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


typedef void (*func_pointer) (int number);

//declare
//void (*fun) (int) = func;
void real_func(int number)
{
	printf("number = %d\n", number);
}

void funcpointer_test(func_pointer function, int number)
{
	function(number);
}

int main(int argc, char** argv)
{
	
	#if 0

	//

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
	
	//last
	//vstring_builder_test();
	
	//programming_language_main();

	//varray
	//varray_test();

	//vdynamic_array
	//vdynamic_array_i32_test();
	
	//vbase64_test();
	
	//time_check();

	//dynamic_table_test()
	
	//vmemory_debug_print_state(1);
	
	//func pointer
	//funcpointer_test(real_func, 10);

#if 1
	//vstring_test();
	//vmath_test();
	programming_language_main();

#if 0
	//for faster memcpy
	char* str_ptr =  "abcdabcd";
	int64 int_ptr = *((int64*) str_ptr);
	char* new_str = malloc(9*sizeof(char));
	new_str = ((char*) (int64*)&int_ptr);
	new_str[8] = 0;
	printf("new_str: %s\n", new_str);

#endif

#else

	char* ptr = "string to ptr bla";
	
	printf("string = %s\n", (ptr+5));
#endif

	#endif	
	
	return 0;
}
