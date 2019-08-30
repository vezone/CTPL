#pragma once

#define check8(x, b) ((x & ((uint8_t)1 << b)) != 0) ? 1 : 0
#define env8(x, b) (x ^ ((uint8_t)1 << b))
#define set8(x, b) (x | ((uint8_t)1 << b))
#define unset8(x, b) ((check8(x,b) == 1) ? env8(x,b) : 0)

#define check32(x, b) ((x & ((int32_t)1 << b)) != 0) ? 1 : 0
#define env32(x, b) (x ^ ((int32_t)1 << b))
#define set32(x, b) (x | ((int32_t)1 << b))
#define unset32(x, b) ((check32(x,b) == 1) ? env32(x,b) : 0)

#define check64(x, b) ((x & ((int64_t)1 << b)) != 0) ? 1 : 0
#define env64(x, b) (x ^ ((int64_t)1 << b))
#define set64(x, b) (x | ((int64_t)1 << b))
#define unset64(x, b) ((check64(x,b) == 1) ? env64(x,b) : 0)

#define set_null(x, b) (x & 0)


int32_t convert_char4_to_int32(char string_to_convert[4])
{
	int32_t result_bits = 0;
	for (int i = 0; i < 32; i++)
	{
		result_bits = (check32(string_to_convert[i/8], i % 8)) ? set32(result_bits, i) : result_bits;
	}
	
	return result_bits;
}

int64_t convert_char8_to_int64(char string_to_convert[8])
{
	int64_t result_bits = 0;
	for (int i = 0; i < 64; i++)
	{
		result_bits = (check64(string_to_convert[i/8], i % 8)) ? set64(result_bits, i) : result_bits;
	}
	return result_bits;
}

char* convert_int32_to_char4(int32_t int32_to_convert)
{
	int8_t index8;
	int8_t temp;
	int8_t mod;
	char* char4 = (char*)malloc(5*sizeof(char));
	char4[0] = 0; char4[2] = 0; char4[4] = 0;
	char4[1] = 0; char4[3] = 0; 
	for (int8_t i = 0; i < 32; i++)
	{
		index8 = i / 8;
		mod = i % 8;
		temp = char4[index8];
		char4[index8] = (check32(int32_to_convert, i)) ? set8(temp, mod) : temp;
	}
	
	return char4;
}

char* convert_int64_to_char8(int64_t int64_to_convert)
{
	int8_t index8;
	int8_t temp;
	int8_t mod;
	char* char8 = (char*)malloc(9*sizeof(char));
	char8[0] = 0; char8[2] = 0; char8[4] = 0; char8[6] = 0; char8[8] = 0;
	char8[1] = 0; char8[3] = 0; char8[5] = 0; char8[7] = 0;
	
	for (int8_t i = 0; i < 64; i++)
	{
		index8 = i / 8;
		mod = i % 8;
		temp = char8[index8];
		char8[index8] = (check64(int64_to_convert, i)) ? set8(temp, mod) : temp;
	}
	return char8;
}

int64_t add_high_bytes_to_int64(int64_t this_value, int32_t value_to_add)
{
	int64_t result = this_value;
	for (int i = 0; i < 32; i++)
	{
		result = (check32(value_to_add, i)) ? set64(result, (i+32)) : result;
	}
	return result;
}

int32_t get_high_bytes_from_int64(int64_t this_value)
{
	int32_t result = 0;
	for (int i = 32; i < 64; i++)
	{
		result = (check64(this_value, i)) ? set32(result, (i-32)) : result;
	}
	return result;
}

int32_t get_low_bytes_from_int64(int64_t this_value)
{
	int32_t result = 0;
	for (int i = 0; i < 32; i++)
	{
		result = (check64(this_value, i)) ? set32(result, (i)) : result;
	}
	return result;
}

//additional stuff
void print_int8_to_binary(int8_t to_convert)
{
	printf("to_convert = ");
	for (int i = 7; i >= 0; i--)
	{
		printf("%i", check8(to_convert, i));
	}
	printf("\n");
}

void print_int32_to_binary(int32_t to_convert)
{
	printf("to_convert = ");
	for (int i = 31; i >= 0; i--)
	{
		printf("%i", check32(to_convert, i));
	}
	printf("\n");
}