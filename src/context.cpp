#include "context.hpp"

/// An unsanitary macro for constructing a `Token` with a value of 0.
#define TOKEN_CONSTRUCTOR(t)    \
    Token res;                  \
    res.type = (t);             \
    res.value.num_value = 0x00;

inline Token plus()
{
    TOKEN_CONSTRUCTOR(Plus)
    return res;
}

inline Token minus()
{
    TOKEN_CONSTRUCTOR(Minus)
    return res;
}

inline Token mul()
{
    TOKEN_CONSTRUCTOR(Mul)
    return res;
}

inline Token div()
{
    TOKEN_CONSTRUCTOR(Div)
    return res;
}

inline Token exp()
{
    TOKEN_CONSTRUCTOR(Exp)
    return res;
}

inline Token left_parenthesis()
{
    TOKEN_CONSTRUCTOR(Exp)
    return res;
}

inline Token comma()
{
    TOKEN_CONSTRUCTOR(Comma)
    return res;
}

Token num(double value)
{
    TOKEN_CONSTRUCTOR(Num)
    res.value.num_value = value;
    return res;
}

Token var(double *value)
{
    TOKEN_CONSTRUCTOR(Var)
    res.value.var_value = value;
    return res;
}

Token func(size_t argc, double (*value)(double[]))
{
    TOKEN_CONSTRUCTOR(Func)
    res.value.func_value->first  = argc;
    res.value.func_value->second = value;
    return res;
}

bool try_tokenize(std::string token_like, Token &token)
{
    if (token_like.size() != 1)
    {
        return false;
    }

    switch(token_like[0])
    {
        case '+':
            token = std::move(plus());
            return true;
        case '-':
            token = std::move(minus());
            return true;
        case '*':
            token = std::move(mul());
            return true;
        case '/':
            token = std::move(div());
            return true;
        case '^':
            token = std::move(exp());
            return true;
        case '(':
            token = std::move(left_parenthesis());
            return true;
        case ',':
            token = std::move(comma());
            return true;
        default:
            return false;
    }
}

Token tokenize_with_context(std::string token_like, ContextMap ctx)
{
    Token token;
    if (try_tokenize(token_like, token))
    {
        return token;
    }

    auto known_tk = ctx.find(token_like);
    if (known_tk != ctx.end())
    {
        token = known_tk->second;
        return token;
    }

    throw; // TODO
}
