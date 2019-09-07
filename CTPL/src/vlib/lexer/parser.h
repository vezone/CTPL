#pragma once

/*
expr3 = INT | '(' expr ')'
expr2 = [-] expr2 | expr3)
expr1 = expr2 ([/*]) expr2)
expr0 = expr1 ([+-] expr1)
expr  = expr0
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
        fatal("expected integer or (, got %s", token_kind_str(token.kind));
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