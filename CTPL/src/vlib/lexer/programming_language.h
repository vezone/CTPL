#pragma once

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

// lexing: translating char stream to toke stream
typedef enum TokenKind 
{
    TOKEN_INT = 128,
    TOKEN_NAME,

} TokenKind;

typedef struct Token
{
    TokenKind kind;
    const char* start;
    const char* end; 
    union 
    {
        int32 value;
        const char* name;
    };
} Token;

const char* token_kind_name(TokenKind kind)
{
    static char buf[256];
    switch (kind)
    {
        case TOKEN_INT:
        {
            sprintf(buf, "integer");
            break;
        }
        case TOKEN_NAME:
        {
            sprintf(buf, "name");
            break;
        }
        default:
        {
            if (kind < 128 && isprint(kind))
            {
                sprintf(buf, "%c", kind);
            }
            else
            {
                sprintf(buf, "<ASCII %d>", kind);
            }
            break;
        }
    }
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

// 1234 (x+y) translates into '1234' '(' 'x' '+' 'y' ')'
void next_token()
{
    token.start = stream;
    switch (*stream)
    {
        case '0' ... '9':
        {
            int32 val = 0;
            while (isdigit(*stream))
            {
                val *= 10;
                val += *stream++ - '0';
            }
            token.kind  = TOKEN_INT;
            token.value = val;
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
            printf("TOKEN_INT: %d\n", token.value);
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
    char* source = "+()HELLO,WORLD!1234+994{WORLD HELLO _WORLD}";
    stream = source;
    next_token();
    while (token.kind)
    {
        print_token(token);
        next_token();
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
        fatal("expected token: %s got %s\n", 
            token_kind_name(kind),
            token_kind_name(token.kind));
        return false;    
    }
}

/*
expr3 = INT | '(' expr ')'
expr2 = [-] expr3
expr1 = expr2 ([/*])*
expr0 = expr1 ([+-]expr1)*
expr = expr0
*/

int32 parse_expr();

int32 parse_expr3()
{
    if (is_token(TOKEN_INT))
    {
        int32 val = token.value;
        next_token();
        return val;
    }
    else if (match_token('('))
    {
        int32 val = parse_expr();
        expect_token(')');
        return val;
    }
    else
    {
        fatal("expected integer or (, got %s", token_kind_name(token.kind));
        return 0;
    }
}

int32 parse_expr2()
{
    if (match_token('-'))
    {
        return -parse_expr3();
    }
    else
    {
        return parse_expr3();
    }
}

int32 parse_expr1()
{
    int32 val = parse_expr2();
    while (is_token('*') || is_token('/'))
    {
        char op = token.kind;
        next_token();
        int rval = parse_expr2();
        if (op == '*')
        {
            val *= rval;
        }
        else
        {
            if (op != '/')
                fatal("op != /");
            if (op == 0)
                fatal("op == 0");
            val /= rval;
        }
    }
    return val;
}

int32 parse_expr0()
{
    int32 val = parse_expr1();
    while (is_token('+') || is_token('-'))
    {
        char op = token.kind;
        next_token();
        int32 rval = parse_expr1();
        if (op == '+')
        {
            val += rval; 
        }
        else
        {
            if (op != '-')
                fatal("op != -");
            val -= rval;
        }
    }
    return val;
}

int32 parse_expr()
{
    return parse_expr0();
}

int32 parse_expr_str(const char* expr)
{
    init_stream(expr);
    return parse_expr();
}

#define TEST(x) assert(parse_expr_str(#x) == (x))
void parse_test()
{
    TEST(1);
    TEST((12));
    TEST(-1);
    TEST(1-2-3);
    TEST(2*3+4*5);
    TEST(2+-3);
}
#undef TEST 

void programming_language_main()
{
//4
#if 0
    lex_test();
#endif
    parse_test();

#if 0
    vstring_intern_test();
#endif

}