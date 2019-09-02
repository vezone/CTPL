#pragma once

#include <ctype.h>

#include "../core/vtypes.h"
#include "../core/varray.h"

// lexing: translating char stream to toke stream
typedef enum TokenKind 
{
    TOKEN_INT = 128,
    TOKEN_NAME,

} TokenKind;

typedef struct Token
{
    TokenKind kind;
    union 
    {
        uint64 value;
        struct {
            const char* start;
            const char* end; 
        };
    };
    
} Token;

Token token;
const char* stream;

// 1234 (x+y) translates into '1234' '(' 'x' '+' 'y' ')'
void next_token()
{
    //token.value = 0;
    switch (*stream)
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            uint64 val = 0;
            while (isdigit(*stream))
            {
                val *= 10;
                val += *stream++ - '0';
            }
            token.kind = TOKEN_INT;
            token.value = val;
            break;
        }
        case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'g': case 'h':
        case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D':
        case 'E': case 'F': case 'G': case 'H':
        case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
        case '_':
        {
            const char* start = stream++;
            while (isalnum(*stream) || *stream == '_')
            {
                stream++;
            }
            const char* end = stream;
            token.kind = TOKEN_NAME;
            token.start = start;
            token.end = stream;
            break;
        }
        default:
        {
            token.kind = *stream++;
            break;
        }
    }
}

void print_token(Token token)
{
    switch (token.kind)
    {
        case TOKEN_INT:
        {
            printf("TOKEN_INT: %lu\n", token.value);
            break;
        }
        case TOKEN_NAME:
        {
            //* - actual length of string to represent
            printf("TOKEN_NAME: %.*s\n", (int32)(token.end - token.start), token.start);
            break;
        }
        default:
        {
            printf("TOKEN: '%c'\n", token.kind);
           break;
        }
    }
}

void lex_test()
{
    char* source = "+()HELLO,WORLD!1234+994";
    stream = source;
    next_token();
    while (token.kind)
    {
        print_token(token);
        next_token();
    }
}

void programming_language_main()
{

    lex_test();

}