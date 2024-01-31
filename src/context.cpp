#include "context.hpp"

using std::pair;
using std::move;

namespace nexsys
{
    /// @brief Helper function to convert a token's value to a function.
    static FunctionDataPtr to_function(_TokenValue value) noexcept
    {
        return static_cast<FunctionDataPtr>(value._phantom_ptr);
    }

    /// @brief Helper function to convert a function pointer to a token's value.
    static _TokenValue from_function(size_t argc, double (*value)(double[]))
    {
        _TokenValue tkv;
        tkv._phantom_ptr = reinterpret_cast<void*>(value); // Is this safe?
        return tkv;
    }

    /// @brief Helper function to convert a token's value to a variable (i.e. a `double*`)
    static double* to_variable(_TokenValue value) noexcept
    {
        return static_cast<double*>(value._phantom_ptr);
    }
    
    /// @brief Helper function to convert a `double*` to a token's value. 
    static _TokenValue from_variable(double* value)
    {
        _TokenValue tkv;
        tkv._phantom_ptr = static_cast<void*>(value);
        return tkv;
    }

    /// @brief Helper function to convert a token's value to a constant value (i.e. a `double`)
    static double to_constant(_TokenValue value)
    {
        return static_cast<double>(value._phantom_double);
    }

    /// @brief Helper function to convert a constant value to a token's value.
    static _TokenValue from_constant(double value)
    {
        _TokenValue tkv;
        tkv._phantom_double = value;
        return tkv;
    }

    Token Token::plus()
    {
        Token tk;
        tk.type = Plus;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::minus()
    {
        Token tk;
        tk.type = Minus;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::mul()
    {
        Token tk;
        tk.type = Mul;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::div()
    {
        Token tk;
        tk.type = Div;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::exp()
    {
        Token tk;
        tk.type = Exp;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::left_parenthesis()
    {
        Token tk;
        tk.type = LeftParenthesis;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::comma()
    {
        Token tk;
        tk.type = Comma;
        tk.value = from_constant(0);

        return tk;
    }

    Token Token::num(double value)
    {
        Token tk;
        tk.type = Num;
        tk.value = from_constant(value);

        return tk;
    }

    Token Token::var(double* value)
    {
        Token tk;
        tk.type = Var;
        tk.value = from_variable(value);

        return tk;
    }

    Token Token::func(size_t argc, double (*value)(double[]))
    {
        Token tk;
        tk.type = Func;
        tk.value = from_function(argc, value);

        return tk;
    }

    TokenType Token::get_type() const
    {
        return this->type;
    }

    bool Token::try_unwrap_num(double& value)
    {
        if (this->type != Num)
        {
            return false;
        }

        value = to_constant(this->value);
        return true;
    }

    bool Token::try_unwrap_var(double*& value)
    {
        if (this->type != Var)
        {
            return false;
        }

        value = to_variable(this->value);
        return true;
    }

    bool Token::try_unwrap_func(size_t& argc, double (*& value)(double[]))
    {
        if (this->type != Var)
        {
            return false;
        }

        auto values = to_function(this->value);

        argc = values->first;
        value = values->second;

        return true;
    }

    bool try_tokenize(std::string token_like, Token &token) noexcept
    {
        if (token_like.size() != 1)
        {
            return false;
        }

        switch(token_like[0])
        {
            case '+':
                token = move(Token::plus());
                return true;
            case '-':
                token = move(Token::minus());
                return true;
            case '*':
                token = move(Token::mul());
                return true;
            case '/':
                token = move(Token::div());
                return true;
            case '^':
                token = move(Token::exp());
                return true;
            case '(':
                token = move(Token::left_parenthesis());
                return true;
            case ',':
                token = move(Token::comma());
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
}
