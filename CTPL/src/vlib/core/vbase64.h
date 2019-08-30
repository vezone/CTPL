#pragma once

//  TODO: завершить транслирование 4 в 3 байтов
//  для всей строки использовать vbase64_complete_string
//  и для vbase64_decomplete_string

#include <stdlib.h>
#include "vbits_operations.h"
#include "vstring.h"
#include "vtypes.h"

int32 counter;
char vbase64_dictionary[65];

// '0' 'A' = 65 'a' = 97
void vbase64_dictionary_init()
{
    char i;
    counter = 0;
    
    vbase64_dictionary[counter] = ' ';
    counter++;

    for (i = '0'; i < '9'; i++)
    {
        vbase64_dictionary[counter] = i;
        counter++;
    }
    
    for (i = 'A'; i < 'Z'; i++)
    {
        vbase64_dictionary[counter] = i;
        counter++;
    }

    for (i = 'a'; i < 'z'; i++)
    {
        vbase64_dictionary[counter] = i;
        counter++;
    }

    vbase64_dictionary[counter] = '\0';
}

void vbase64_dictionary_show()
{
    int32 i;
    printf("Dictionary: ");
    for (i = 0; i < counter; i++)
    {
        printf("%c ", vbase64_dictionary[i]);
    }
    printf("\n");
}

uint8* vbase64_translate_char_to_dictionary(char* string)
{
    char character;
    int32 i;
    int32 string_length = vstring_length(string);
    uint8* decompressed_array = malloc(string_length * sizeof(uint8)); 
    for (i = 0; i < string_length; i++)
    {
        character = string[i];
        if (character == ' ')
        {
            decompressed_array[i] = 0;
        }
        else if (character >= '0' && 
                 character <= '9')
        {
            decompressed_array[i] = (uint8)(character - '0' + 1);
        }
        else if (character >= 'A' && 
                 character <= 'Z')
        {
            decompressed_array[i] = (uint8)(character - 'A' + 11);
        }
        else if (character >= 'a' && 
                 character <= 'z')
        {
            decompressed_array[i] = (uint8)(character - 'a' + 11 + 26);
        }
    }

    return decompressed_array;
}

/*
    сжатие 8 битов в 6 битов
    Linux:
    [little endian]
    0 1 2 3 4 5 6 7
    1 1 1 1 1 1 0 0

    converting large string
    "abcdefji blabla bla string to string char int8 int16"
     01234567 ...
     ******
    "abcdefj "
    .    
    .
    .
*/
uint8* vbase64_compress(uint8* input)
{
    //8 8 8 8
    uint8* output = malloc(3 * sizeof(uint8));
    output[0] = (check8(input[0], 0) == 1) ? set8(output[0], 0) : output[0];
    output[0] = (check8(input[0], 1) == 1) ? set8(output[0], 1) : output[0];
    output[0] = (check8(input[0], 2) == 1) ? set8(output[0], 2) : output[0];
    output[0] = (check8(input[0], 3) == 1) ? set8(output[0], 3) : output[0];
    output[0] = (check8(input[0], 4) == 1) ? set8(output[0], 4) : output[0];
    output[0] = (check8(input[0], 5) == 1) ? set8(output[0], 5) : output[0];
    
    output[0] = (check8(input[1], 0) == 1) ? set8(output[0], 6) : output[0];
    output[0] = (check8(input[1], 1) == 1) ? set8(output[0], 7) : output[0];
    output[1] = (check8(input[1], 2) == 1) ? set8(output[1], 0) : output[1];
    output[1] = (check8(input[1], 3) == 1) ? set8(output[1], 1) : output[1];
    output[1] = (check8(input[1], 4) == 1) ? set8(output[1], 2) : output[1];
    output[1] = (check8(input[1], 5) == 1) ? set8(output[1], 3) : output[1];
   
    output[1] = (check8(input[2], 0) == 1) ? set8(output[1], 4) : output[1];
    output[1] = (check8(input[2], 1) == 1) ? set8(output[1], 5) : output[1];
    output[1] = (check8(input[2], 2) == 1) ? set8(output[1], 6) : output[1];
    output[1] = (check8(input[2], 3) == 1) ? set8(output[1], 7) : output[1];
    output[2] = (check8(input[2], 4) == 1) ? set8(output[2], 0) : output[2];
    output[2] = (check8(input[2], 5) == 1) ? set8(output[2], 1) : output[2];
  
    output[2] = (check8(input[3], 0) == 1) ? set8(output[2], 2) : output[2];
    output[2] = (check8(input[3], 1) == 1) ? set8(output[2], 3) : output[2];
    output[2] = (check8(input[3], 2) == 1) ? set8(output[2], 4) : output[2];
    output[2] = (check8(input[3], 3) == 1) ? set8(output[2], 5) : output[2];
    output[2] = (check8(input[3], 4) == 1) ? set8(output[2], 6) : output[2];
    output[2] = (check8(input[3], 5) == 1) ? set8(output[2], 7) : output[2];

    return output;
}

int8* vbase64_decompress(uint8* input)
{
    uint8* decompressed_array = malloc(4);

    decompressed_array[0] = (check8(input[0], 0) == 1) ? set8(decompressed_array[0], 0) : decompressed_array[0];
    decompressed_array[0] = (check8(input[0], 1) == 1) ? set8(decompressed_array[0], 1) : decompressed_array[0];
    decompressed_array[0] = (check8(input[0], 2) == 1) ? set8(decompressed_array[0], 2) : decompressed_array[0];
    decompressed_array[0] = (check8(input[0], 3) == 1) ? set8(decompressed_array[0], 3) : decompressed_array[0];
    decompressed_array[0] = (check8(input[0], 4) == 1) ? set8(decompressed_array[0], 4) : decompressed_array[0];
    decompressed_array[0] = (check8(input[0], 5) == 1) ? set8(decompressed_array[0], 5) : decompressed_array[0];
    
    decompressed_array[1] = (check8(input[0], 6) == 1) ? set8(decompressed_array[1], 0) : decompressed_array[1];
    decompressed_array[1] = (check8(input[0], 7) == 1) ? set8(decompressed_array[1], 1) : decompressed_array[1];
    decompressed_array[1] = (check8(input[1], 0) == 1) ? set8(decompressed_array[1], 2) : decompressed_array[1];
    decompressed_array[1] = (check8(input[1], 1) == 1) ? set8(decompressed_array[1], 3) : decompressed_array[1];
    decompressed_array[1] = (check8(input[1], 2) == 1) ? set8(decompressed_array[1], 4) : decompressed_array[1];
    decompressed_array[1] = (check8(input[1], 3) == 1) ? set8(decompressed_array[1], 5) : decompressed_array[1];
    
    decompressed_array[2] = (check8(input[1], 4) == 1) ? set8(decompressed_array[2], 0) : decompressed_array[2];
    decompressed_array[2] = (check8(input[1], 5) == 1) ? set8(decompressed_array[2], 1) : decompressed_array[2];
    decompressed_array[2] = (check8(input[1], 6) == 1) ? set8(decompressed_array[2], 2) : decompressed_array[2];
    decompressed_array[2] = (check8(input[1], 7) == 1) ? set8(decompressed_array[2], 3) : decompressed_array[2];
    decompressed_array[2] = (check8(input[2], 0) == 1) ? set8(decompressed_array[2], 4) : decompressed_array[2];
    decompressed_array[2] = (check8(input[2], 1) == 1) ? set8(decompressed_array[2], 5) : decompressed_array[2];

    decompressed_array[3] = (check8(input[2], 2) == 1) ? set8(decompressed_array[3], 0) : decompressed_array[3];
    decompressed_array[3] = (check8(input[2], 3) == 1) ? set8(decompressed_array[3], 1) : decompressed_array[3];
    decompressed_array[3] = (check8(input[2], 4) == 1) ? set8(decompressed_array[3], 2) : decompressed_array[3];
    decompressed_array[3] = (check8(input[2], 5) == 1) ? set8(decompressed_array[3], 3) : decompressed_array[3];
    decompressed_array[3] = (check8(input[2], 6) == 1) ? set8(decompressed_array[3], 4) : decompressed_array[3];
    decompressed_array[3] = (check8(input[2], 7) == 1) ? set8(decompressed_array[3], 5) : decompressed_array[3];

    return (int8*) decompressed_array;
}

char* vbase64_complete_string(char* array)
{
    int32 array_length = vstring_length(array);
    int32 mod = array_length % 4;
    if (mod != 0)
    {
        int32 new_array_length = (array_length + 4 - mod);
        char* new_array = malloc(new_array_length * sizeof(char));

        for (int32 i = 0; i < array_length; i++)
        {
            new_array[i] = array[i];
        } 

        {
            int32 i;
            for (i = array_length; i < new_array_length; i++)
            {
                new_array[i] = '+';
            }

        }

        return new_array;
    }
    else
    {
        printf("mod == 0\n");
        return array;
    }
}

char* vbase64_decomplete_string(char* array)
{
    int32 i;
    int32 array_length = vstring_length(array);
    int32 new_length = array_length;

    for (i = 0; i < array_length; i++)
    {
        if (array[i] == '+')
        {
            new_length = i;
            break;  
        }
    }

    char* new_array = malloc(new_length * sizeof(char));
    for (i = 0; i < new_length; i++)
    {
        new_array[i] = array[i];
    }

    return new_array;
}

void vbase64_test()
{
    printf(RED5("vbase64 test\n"));
    vbase64_dictionary_init();
    //vbase64_dictionary_show();

    char* input = "A12";
    printf("Input: %s\nInput in i32: ", input);
    for (int32 i = 0; i < 4; i++)
        printf("%d ", (int32)input[i]);
    printf("\n");

    char* output = vbase64_complete_string(input);
    printf("Output complete string: %s\n", output);
    
    uint8* comp = vbase64_compress(output);
    printf("compressed: ");  
    for (int32 i = 0; i < 3; i++)
    {
        printf("%d ", comp[i]);
    }
    printf("\n");    

    uint8* decomp = vbase64_decompress(comp);
    printf("decompressed: ");
    for (int32 i = 0; i < 4; i++)
    {
        printf("%d ", decomp[i]);
    }
    printf("\n");    

    output = vbase64_decomplete_string(decomp);
    printf("Decomplete string: %s\nDecompete in i32: ", output);
    for (int32 i = 0; i < vstring_length(output); i++)
        printf("%d ", (int32)output[i]);
    printf("\n");    

}