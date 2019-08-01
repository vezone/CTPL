#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "vtypes.h"
#include "vdynamic_table.h"
#include "vdynamic_array_i32.h"
#include "vstring_builder.h"
#include "vstring.h"

#define MEMORY_DEBUG 1
//#include "vmemory_debug.h"

dynamic_column_t* dynamic_column_init(char* content)
{
	dynamic_column_t* column = malloc(sizeof(dynamic_column_t));
	int32 content_length = vstring_length(content);
	column->width = content_length;
	column->content = malloc(content_length * sizeof(char));
	vstring_copy(column->content, content, content_length);
	return column;
}

dynamic_column_t dynamic_column_init_value(char* content)
{
	dynamic_column_t column;
	int32 content_length = vstring_length(content);
	column.width = content_length;
	column.content = malloc(content_length * sizeof(char));
	vstring_copy(column.content, content, content_length);
	return column;
}

void dynamic_column_show(dynamic_column_t* column)
{
	/*
	printf("-------");
	printf("|width|");
	printf("-------");
	*/
	
	if (column != nullptr)
	{
		char* top_bot_border = vstring_with('-', (column->width + 2));
		printf("%s\n|%s|\n%s\n", top_bot_border, column->content, top_bot_border);
		
		if (top_bot_border)
		{
			free(top_bot_border);
		}
	}
	else
	{
		printf(RED("dynamic_column_show: column == null\n"));
	}	
}

void dynamic_row_add_column(dynamic_row_t* this, 
					 	    dynamic_column_t* value_to_add)
{
	int32 c;
	if (value_to_add->content != nullptr)
	{
		if (this->columns >= 0)
		{
			dynamic_column_t* columns = malloc((this->columns_count + 1) * sizeof(dynamic_column_t));
			if (columns != nullptr)
			{
				for (c = 0;
					 c < this->columns_count;
					 c++)
				{
					columns[c] = dynamic_column_init_value(this->columns[c].content);
				}
				
				columns[this->columns_count] = dynamic_column_init_value(value_to_add->content);
				
				if (this->columns)
				{
					free(this->columns);
				}
				
				this->columns = columns;
				++this->columns_count;
				this->rows_length += value_to_add->width + 1;
			}
			else
			{
				printf(RED("dynamic_row_add_column: malloc error!\n"));
			}
		}
	}
}

dynamic_row_t dynamic_row_init(int32 number_of_columns)
{
	dynamic_row_t row;
	row.columns_count = 0;
	row.rows_length = 0;
	row.columns = nullptr;
	
	int32 c;
	for (c = 0; 
		 c < number_of_columns;
		 c++)
	{
		dynamic_column_t* column_temp = dynamic_column_init("");
		dynamic_row_add_column(&row, column_temp);
		
		if (column_temp)
		{
			free(column_temp);
		}
	}
	
	return row;
}


void dynamic_row_show(dynamic_row_t row)
{
	char* top_bot_border = vstring_with('-', row.rows_length);
	
	printf("%s\n", top_bot_border);
	for (int32_t index = 0;
		 index < row.columns_count;
		 index++)
	{
		printf("|%s|", row.columns[index].content);	 
	}
	printf("\n%s\n", top_bot_border);

	if (top_bot_border)
	{
		free(top_bot_border);
	}
}

void dynamic_row_free(dynamic_row_t* row)
{
	for (int i = 0; i < row->columns_count; i++)
	{
		if (row->columns[i].content != nullptr)
		{
			//printf("free: %ld\n", (int64)row->columns[i].content);
			free(row->columns[i].content);
		}
	}
	
	if (row->columns)
	{
		free(row->columns);
		row->columns = nullptr;
	}
}

void dymanic_table_add_row(dynamic_table_t* table)
{
	if (table)
	{
		if (table->rows)
		{
			void* new_row_pointer = realloc(table->rows, (table->rows_count + 1) * sizeof(dynamic_row_t));
			if (new_row_pointer)
			{
				free(table->rows);	
				table->rows = new_row_pointer;
				table->rows[table->rows_count] = dynamic_row_init(table->rows->columns_count);
				++table->rows_count;
			}
			else
			{
				printf(RED("reallocation error!\n"));
				return;
			}
		}
		else
		{
			printf(RED("table->rows == nullptr for some reason :(\n"));
			return;
		}	
	}
	else
	{
		printf(RED("it's can't be true but table == nullptr :(\n"));
		return;
	}
}

dynamic_table_t dynamic_table_create(int32 number_of_rows, int32 number_of_columns)
{
	dynamic_table_t table;
	table.rows_count = number_of_rows;
	table.rows = malloc(number_of_rows * sizeof(dynamic_row_t));
	table.columns_max_width = vdynamic_array_i32_init(number_of_columns);
	
	int32 r;
	for (r = 0;
		 r < number_of_rows;
		 r++)
	{
		table.rows[r] = dynamic_row_init(number_of_columns);
		vdynamic_array_i32_add(&table.columns_max_width, 0);
	}
	
	return table;
}

void dynamic_table_set(dynamic_table_t* table, int32 row, int32 column, char* content)
{
	if (row > 0 
		&& column >= 0 
		&& (row < table->rows_count) 
		&& (column < table->rows->columns_count))
	{
		int32 content_length = vstring_length(content);
		int32 this_column_max_width = vdynamic_array_i32_get(&table->columns_max_width, column);
		if (this_column_max_width != -1)
		{
			int32 this_column_length = 0;
			if (table->rows[row].columns[column].content)
			{
				this_column_length = vstring_length(table->rows[row].columns[column].content);
			}
			
			if (content_length > this_column_max_width)
			{
				vdynamic_array_i32_set(&table->columns_max_width, column, content_length);
			}
			else if (content_length < this_column_max_width 
				  && this_column_length == this_column_max_width)
			{
				int32 r;
				int32 max_content_length = 0;
				int32 current_column_length;
				for (r = 0; r < table->rows_count; r++)
				{
					current_column_length = vstring_length(table->rows[r].columns[column].content);
					if (current_column_length > max_content_length)
					{
						if (current_column_length != this_column_max_width)
						{
							max_content_length = current_column_length;
						}
					}
				}
				
				if (max_content_length > 0)
				{
					vdynamic_array_i32_set(&table->columns_max_width, column, max_content_length);	  
				}
			}
					
		}
		
		if (table->rows[row].columns[column].content)
		{
			free(table->rows[row].columns[column].content);
		}
		
		table->rows[row].columns[column].content = malloc(content_length * sizeof(char) + 1);
		vstring_copy(table->rows[row].columns[column].content, content, content_length+1);
		
	}
	else 
	{
		printf(RED("Index out of range\n"));
	}
}

void dynamic_table_set_header(dynamic_table_t* table, int32 column, char* content)
{
	int32 content_length;
	int32 temp;
	if (column >= 0 
		&& (column < table->rows->columns_count))
	{
		if (table->rows[0].columns[column].content)
		{
			free(table->rows[0].columns[column].content);
		}
		
		content_length = vstring_length(content);
		table->rows[0].columns[column].content = malloc(content_length * sizeof(char) + 1);
		vstring_copy(table->rows[0].columns[column].content, content, (content_length + 1));
		
		temp = vdynamic_array_i32_get(&table->columns_max_width, column);
		if (temp != -1)
		{
			if (content_length > temp)
			{
				vdynamic_array_i32_set(&table->columns_max_width, column, content_length);
			}
		}
	}
	else 
	{
		printf(RED("Index out of range\n"));
	}
}

void dynamic_table_add_header(dynamic_table_t *table, char* header_name)
{
	if (table->rows_count > 0)
	{
		
		int32 compare;
		int32 c;
		int32 r;
		for (c = 0;
			 c < table->rows->columns_count;
			 c++)
		{
			int32 compare = vstring_compare(table->rows[0].columns[c].content, header_name);
			if (compare == 1)
			{
				printf(BRIGHTRED("Table already contains this header name %s\n"), header_name);
				return;
			}
		}
		
		int32 header_name_length = vstring_length(header_name);
		dynamic_column_t* header = dynamic_column_init(header_name);
		dynamic_column_t* temp_column = dynamic_column_init("");
		
		vdynamic_array_i32_add(&table->columns_max_width, header_name_length);
		dynamic_row_add_column(&table->rows[0], header); 
		
		for (r = 1;
			 r < table->rows_count;
			 r++)
		{
			dynamic_row_add_column(&table->rows[r], temp_column);
		}
		
		if (temp_column)
		{
			free(temp_column);
		}	
	}	
}

void dynamic_table_show(dynamic_table_t* table)
{
	int32 row_length;
	row_length = vdynamic_array_i32_get_sum(&table->columns_max_width) + (table->columns_max_width.count + 1);
	char* top_bot_border = vstring_with('-', row_length);
	int32 r;
		
	for (r = 0; 
		r < table->rows_count;
		r++)
	{	
		printf("%s\n", top_bot_border);
		for (int32_t index = 0;
			 index < table->rows[r].columns_count;
			 index++)
		{
			int32 content_length = vstring_length(table->rows[r].columns[index].content);
			int32 temp = vdynamic_array_i32_get(&table->columns_max_width, index);
			if (temp != -1)
			{
				if (content_length < temp)
				{
					int32 difference = temp - content_length;
					
					if ((difference % 2) == 0)
					{
						char* offset = vstring_with(' ', (difference) / 2);
						printf("|%s%s%s", offset, table->rows[r].columns[index].content, offset);
						
						if (offset)
						{
							free(offset);
						}	
					}
					else
					{
						int32 left_offset_value = difference / 2;
						int32 rigth_offset_value = difference - left_offset_value;
						char* left_offset  = vstring_with(' ', left_offset_value);
						char* rigth_offset = vstring_with(' ', rigth_offset_value);
						printf("|%s%s%s", 
								left_offset, 
								table->rows[r].columns[index].content, 
								rigth_offset);
						
						if (left_offset)
						{
							free(left_offset);
						}
						
						if (rigth_offset)
						{
							free(rigth_offset);
						}
						
					}
				}
				else
				{
					printf("|%s", table->rows[r].columns[index].content);
				}
			}	 
		}
		printf("|\n");
	}
	printf("%s\n", top_bot_border);
	
	if (top_bot_border)
	{
		free(top_bot_border);
	}
}

/*	
	##################################
	##################################
	##################################
	####						  ####
	#### ADDITIONAL FUNCTIONALITY ####
	####						  ####
	##################################
	##################################
	##################################
*/

char* vdynamic_table_to_html_table(dynamic_table_t* table)
{	
	int32 r;
	int32 c;
	vstring_builder builder;
	vstring_builder_init(&builder);
	vstring_builder_append_string(&builder, "<table>\n");
	
	vstring_builder_append_string(&builder, "<tr>\n");
	for (c = 0; c < table->rows->columns_count; c++)
	{
		vstring_builder_append_format(&builder, "<th scope=\"column\">%s</th>\n", table->rows->columns[c].content);
	}
	vstring_builder_append_string(&builder, "</tr>\n");
	
	for (r = 1; r < table->rows_count; r++)
	{
		vstring_builder_append_string(&builder, "<tr>\n");
		for (c = 0; c < table->rows->columns_count; c++)
		{
			vstring_builder_append_format(&builder, "<td>%s</td>\n", table->rows[r].columns[c].content);
		}
		vstring_builder_append_string(&builder, "</tr>\n");
	}
	vstring_builder_append_string(&builder, "</table>");
	
	return vstring_builder_to_string(&builder);
}



void dynamic_table_test()
{
	
	dynamic_table_t table = dynamic_table_create(4, 4);
	
	dynamic_table_set_header(&table, 0, "header1");
	dynamic_table_set_header(&table, 1, "header2");
	dynamic_table_set_header(&table, 2, "header3");
	dynamic_table_set_header(&table, 3, "header4");

	dynamic_table_add_header(&table, "header5");
	dynamic_table_add_header(&table, "header6");
	
	dynamic_table_add_header(&table, "header5");
	dynamic_table_add_header(&table, "header5");
	dynamic_table_add_header(&table, "header5");
	dynamic_table_add_header(&table, "header5");
	
	dynamic_table_set(&table, 1, 3, "lala3");
	dynamic_table_set(&table, 1, 2, "lala2");
	dynamic_table_set(&table, 1, 1, "lala1");
	dynamic_table_set(&table, 1, 0, "lala0");
	
	dynamic_table_set(&table, 2, 3, "lala3");
	dynamic_table_set(&table, 2, 2, "lala2");
	dynamic_table_set(&table, 2, 1, "lala1");
	dynamic_table_set(&table, 2, 0, "lala0");
	
	dynamic_table_set(&table, 3, 3, "lala3");
	dynamic_table_set(&table, 3, 2, "lala2");
	dynamic_table_set(&table, 3, 1, "lala1");
	dynamic_table_set(&table, 3, 0, "lala00000000000");
	
	dynamic_table_set(&table, 1, 4, "+792131231231");
	dynamic_table_set(&table, 2, 4, "+213123123");
	dynamic_table_set(&table, 1, 5, "+892131231231");
	
	//dynamic_row_show(row);
	
	dymanic_table_add_row(&table);
	
	
	dynamic_table_set(&table, 4, 1, "lala1");
	dynamic_table_set(&table, 4, 2, "lala1");
	dynamic_table_set(&table, 4, 3, "lala1");
	dynamic_table_set(&table, 4, 4, "lala1");
	
	dynamic_table_show(&table);

	dynamic_table_set(&table, 3, 0, "1");
	dynamic_table_show(&table);
	
	dynamic_table_set(&table, 3, 0, "00123111231231");
	dynamic_table_show(&table);
	
	dynamic_table_set_header(&table, 0, "header1222222222222222222222");
	dynamic_table_show(&table);
	
	char* to_html = vdynamic_table_to_html_table(&table);
	printf("%s\n", to_html);
}
