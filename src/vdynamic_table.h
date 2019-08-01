#pragma once

#include "vtypes.h"
#include "vdynamic_array_i32.h"

typedef struct
{
	/*readonly*/
	int32_t width;
	/*public (we can set it)*/
	//int32_t height;
	char*   content; 
} dynamic_column_t;

typedef struct
{
	int32_t rows_length;
	int32_t columns_count;
	dynamic_column_t* columns;
} dynamic_row_t;

typedef struct
{
	int32 rows_count;
	dynamic_row_t* rows;
	vdynamic_array_i32 columns_max_width;
} dynamic_table_t;

dynamic_column_t* dynamic_column_init(char* content);
dynamic_column_t dynamic_column_init_value(char* content);
void dynamic_column_show(dynamic_column_t* column);
void dynamic_row_add_column(dynamic_row_t* this, 
					 	    dynamic_column_t* value_to_add);
					 	    
dynamic_row_t dynamic_row_init(int32 number_of_columns);
void dynamic_row_show(dynamic_row_t row);
void dynamic_row_free(dynamic_row_t* row);
void dymanic_table_add_row(dynamic_table_t* table);

dynamic_table_t dynamic_table_create(int32 number_of_rows, int32 number_of_columns);
void dynamic_table_set(dynamic_table_t* table, int32 row, int32 column, char* content);
void dynamic_table_set_header(dynamic_table_t* table, int32 column, char* content);
void dynamic_table_add_header(dynamic_table_t *table, char* header_name);
void dynamic_table_show(dynamic_table_t* table);

char* vdynamic_table_to_html_table(dynamic_table_t* table);
void dynamic_table_test();
