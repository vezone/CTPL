#pragma once

#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../core/vtypes.h"
#include "../core/varray.h"
#include "../core/vstring_intern.h"


void fatal(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    printf("FATAL: ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
    exit(1);
}

void syntax_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("Sytax Error: ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

// lexing: translating char stream to toke stream
typedef enum TokenKind 
{
    TOKEN_EOF = 0,
    TOKEN_LAST_CHAR = 127,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_NAME,
} TokenKind;

typedef enum TokenModifier
{
    TOKENMOD_NONE = 0,
    TOKENMOD_HEX,
    TOKENMOD_BIN,
    TOKENMOD_OCT,
    TOKENMOD_CHAR,
} TokenModifier;

typedef struct Token
{
    TokenKind kind;
    TokenModifier modifier;
    const char* start;
    const char* end; 
    union 
    {
        uint64 int_value;
        double float_value;
        const char* string_value;
        const char* name;
    };
} Token;

int32 copy_token_kind_str(char* dest, int32 dest_size, TokenKind kind)
{
    int32 n = 0;
    switch (kind)
    {
        case '\0':
        {
            n = snprintf(dest, dest_size, "end of file");
            break;
        }
        case TOKEN_INT:
        {
            n = snprintf(dest, dest_size, "integer");
            break;
        }
        case TOKEN_FLOAT:
        {
            n = snprintf(dest, dest_size, "float");
            break;
        }
        case TOKEN_NAME:
        {
            n = snprintf(dest, dest_size, "name");
            break;
        }
        default:
        {
            if (kind < 128 && isprint(kind))
            {
                n = snprintf(dest, dest_size, "%c", kind);
            }
            else
            {
                n = snprintf(dest, dest_size, "<ASCII %d>", kind);
            }
            break;
        }
    }
    return n;
}

const char* token_kind_str(TokenKind kind)
{
    static char buf[256];
    int32 n = copy_token_kind_str(buf, sizeof(buf), kind);
    assert((n + 1) <= sizeof(buf));
    return buf;
}

Token token;
const char* stream;

const char* keyword_if;
const char* keyword_for;
const char* keyword_while;

void init_keywords()
{
    keyword_if = vstring_intern("if");
    keyword_for = vstring_intern("for");
    keyword_while = vstring_intern("while");
}

uint8 char_to_digit[256] =
{
    ['0'] = 0,
    ['1'] = 1,
    ['2'] = 2,
    ['3'] = 3,
    ['4'] = 4,
    ['5'] = 5,
    ['6'] = 6,
    ['7'] = 7,
    ['8'] = 8,
    ['9'] = 9,
    ['a'] = 10, ['A'] = 10,
    ['b'] = 11, ['B'] = 11,
    ['c'] = 12, ['C'] = 12,
    ['d'] = 13, ['D'] = 13,
    ['e'] = 14, ['E'] = 14,
    ['f'] = 15, ['F'] = 15,
};

void scan_int()
{
    uint64 base = 10;
    if (*stream == '0')
    {
        stream++;
        if (vstring_to_lowerc(*stream) == 'x')
        {
            base = 16;
            stream++;
        }
        else if (isdigit(*stream))
        {
            base = 8;
        }
        else if (vstring_to_lowerc(*stream) == 'b')
        {
            base = 2;
            stream++;
        }
        else
        {
            syntax_error("Invalid integer literal suffix '%c'", *stream);
            stream++;            
        }
    }
    uint64 val = 0;
    for (;;)
    {
        uint64 digit = char_to_digit[*stream];
        if (digit == 0 && *stream != '0')
        {
            //syntax_error("Invalid integer literal suffix '%c'");
            break;
        }
        if (digit >= base)
        {
            syntax_error("Digit '%c' out of range for base %llu", *stream, base);
            digit = 0;
        }
        if (val > (UINT64_MAX - digit) / base)
        {
            printf("Integer literal overflow\n");
            while (isdigit(*stream))
            {
                stream++;
            }
            val = 0;
        }
        
        val = val * base + digit;
        stream++;
    }
    token.int_value = val;
    token.kind = TOKEN_INT;
}

void scan_float()
{
    const char* start = stream;
    while (isdigit(*stream))
    {
        stream++;
    }
    if (*stream != '.')
    {
        stream++;
    }
    stream++;
    while (isdigit(*stream))
    {
        stream++;
    }
    if (vstring_to_lowerc(*stream) == 'e')
    {
        stream++;
        if (*stream == '+' | *stream == '-')
        {
            stream++;
        }
        if (!isdigit(*stream))
        {
            syntax_error("Expected digit after float literal exponent, found '%c'", *stream);
        }
        while (isdigit(*stream))
        {
            stream++;
        }
    }
    const char* end = stream;
    double val = strtod(start, NULL);
    if (val == HUGE_VAL || val == -HUGE_VAL)
    {
        syntax_error("Float literal overflow");
    }
    token.float_value = val;
    token.kind = TOKEN_FLOAT;
}

char escape_to_char[256] =
{
    ['n'] = '\n',
    ['r'] = '\r',
    ['t'] = '\t',
    ['v'] = '\v',
    ['b'] = '\b',
    ['a'] = '\a',
    ['0'] = '\0',
};

void scan_char()
{
    assert(*stream == '\'');
    stream++;
    char val = '\0';
    if(*stream == '\'')
    {
        syntax_error("Char literal cannot be empty");
        stream++;
    }
    else if (*stream == '\n')
    {
        syntax_error("Char literal cannot contain newline");
    }
    else if (*stream == '\\')
    {
        stream++;
        val = escape_to_char[*stream];
        if (val == 0 && *stream != '0')
        {
            syntax_error("Invalid char literal escape '\\%c'", *stream);
        }
        stream++;
    }
    else
    {
        val = *stream;
        stream++;
    }
    if (*stream != '\'')
    {
        syntax_error("Expected closing char quotes, got '%c'", *stream);
    }
    else { stream++; }
    token.kind = TOKEN_INT;
    token.modifier = TOKENMOD_CHAR;
    token.int_value = val;
}

void scan_string()
{
    assert(*stream == '"');
    stream++;
    char* str = NULL;
    while (*stream && *stream != '"')
    {
        char val = *stream;
        if (val == '\n')
        {
            syntax_error("String literal cannot contain newline");
        }
        else if (val == '\\')
        {
            stream++;
            val = escape_to_char[*stream];
            if (val == 0 && *stream != '0')
            {
                syntax_error("Invalid string literal escape '\\%c'", *stream);
            }
        }
        varray_push(str, val);
        stream++;
    }
    if (*stream)
    {
        assert(*stream == '"');
        stream++;
    }
    else
    {
        syntax_error("Unexpected end of file within string literal");
    }
    varray_push(str, 0);
    token.kind = TOKEN_STRING;
    token.string_value = str;
}

// 1234 (x+y) translates into '1234' '(' 'x' '+' 'y' ')'
void next_token()
{
    top:
    token.start = stream;
    token.modifier = TOKENMOD_NONE;
    switch (*stream)
    {
        case ' ': case '\n': case '\t': case '\r': case '\v':
        {
            while (isspace(*stream)) { stream++; }
            goto top;
            break;
        }
        case '\'':
        {
            scan_char();
            break;            
        }
        case '"':
        {
            scan_string();
            break;            
        }
        case '.':
        {
            scan_float();
            break;
        }
        case '0' ... '9':
        {
            while (isdigit(*stream))
            {
                stream++;
            }
            if (*stream == '.' || vstring_to_lowerc(*stream) == 'e')
            {
                stream = token.start;
                scan_float();
            }
            else
            {
                stream = token.start;
                scan_int();
            }
            break;
        }
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
        {
            while (isalnum(*stream) || *stream == '_')
            {
                stream++;
            }
            token.kind = TOKEN_NAME;
            token.name = vstring_intern_range(token.start, stream);
            break;
        }
        default:
        {
            token.kind = *stream++;
            break;
        }
    }
    token.end = stream;
}

void init_stream(const char* str)
{
    stream = str;
    next_token();
}

void print_token(Token token)
{
    switch (token.kind)
    {
        case TOKEN_INT:
        {
            printf("TOKEN_INT: %ld\n", token.int_value);
            break;
        }
        case TOKEN_FLOAT:
        {
            printf("TOKEN_FLOAT: %f\n", token.float_value);
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

//inline
bool is_token(TokenKind kind)
{
    return token.kind == kind;
}

//inline
bool is_token_name(const char* name)
{
    return token.kind == TOKEN_NAME && token.name == name;
}

//inline
bool match_token(TokenKind kind)
{
    if (is_token(kind))
    {
        next_token();
        return true;
    }
    else
    {
        return false;
    }
}

//inline
bool expect_token(TokenKind kind)
{
    if (is_token(kind))
    {
        next_token();
        return true;
    }
    else
    {
        char buf[256];
        copy_token_kind_str(buf, sizeof(buf), kind);
        fatal("expected token: %s got %s\n", 
            token_kind_str(kind));
        return false;    
    }
}

#define assert_token(x) assert(match_token(x))
#define assert_token_name(x) assert(token.name == vstring_intern(x) && match_token(TOKEN_NAME))
#define assert_token_int(x) assert(token.int_value == (x) && match_token(TOKEN_INT))
#define assert_token_float(x) assert(token.float_value == (x) && match_token(TOKEN_FLOAT))
#define assert_token_string(x) assert(vstring_compare(token.string_value, (x)) && match_token(TOKEN_STRING))
#define assert_token_eof(x) assert(is_token(0))

void lex_test()
{
    init_stream("+()HELLO,");
    assert_token('+');
    assert_token('(');
    assert_token(')');
    assert_token_name("HELLO");
    assert_token(',');
    assert_token_eof();

    init_stream("3.14 .0123 3. 3e10");
    assert_token_float(3.14);
    assert_token_float(.0123);
    assert_token_float(3.);
    assert_token_float(3e10);
    assert_token_eof();

    init_stream("18446744073709551615 0xffffffffffffffff 042 0b1111");
    assert_token_int(18446744073709551615ull);
    assert_token_int(0xffffffffffffffffull);
    assert_token_int(042);
    assert_token_int(0xF);
    assert_token_eof();

    init_stream("'a' '\\n'");
    assert_token_int('a');
    assert_token_int('\n');
    assert_token_eof();

    init_stream("\"string\" \"a\\nb\"");
    assert_token_string("string");
    assert_token_string("a\nb");
    assert_token_eof();
}

void programming_language_main()
{
//4: 10:00
    lex_test();

    
#if 0
    vstring_intern_test();
#endif

}