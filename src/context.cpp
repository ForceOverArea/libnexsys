#include "context.hpp"
#include <iostream>

using std::move;
using std::pair;
using std::string;

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
    static Variable* to_variable(_TokenValue value) noexcept
    {
        return static_cast<Variable*>(value._phantom_ptr);
    }
    
    /// @brief Helper function to convert a `double*` to a token's value. 
    static _TokenValue from_variable(Variable* value)
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

    Token Token::var(Variable* value)
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
        return type;
    }

    bool Token::try_unwrap_num(double& value) const
    {
        if (this->type != Num)
        {
            return false;
        }

        value = to_constant(this->value);
        return true;
    }

    bool Token::try_unwrap_var(Variable*& value) const
    {
        if (this->type != Var)
        {
            return false;
        }

        value = to_variable(this->value);
        return true;
    }

    bool Token::try_unwrap_func(size_t& argc, double (*& value)(double[])) const
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

    void ContextMap::add_num_to_ctx(string symbol, double value)
    {
        this->insert(move(
            pair<string, Token>(
                symbol, 
                move(Token::num(value))
            )
        ));
    }

    void ContextMap::add_var_to_ctx(string symbol, Variable* value)
    {
        this->insert(move(
            pair<string, Token>(
                symbol,
                move(Token::var(value))
            )
        ));
    }

    void ContextMap::add_func_to_ctx(string symbol, size_t argc, double (*value)(double[]))
    {
        this->insert(move(
            pair<string, Token>(
                symbol,
                move(Token::func(argc, value))
            )
        ));
    }

    ContextMap::~ContextMap()
    {
        for (std::string var: malloced_vars)
        {
            Variable* kill_it;
            (void)this->find(var)->second.try_unwrap_var(kill_it);
            delete kill_it;
        }
    }

    /// @brief 
    /// @param expr 
    /// @param result 
    /// @return 
    bool try_parse_double(string expr, double& result)
    {
        result = strtod(expr.c_str(), NULL);
        if (result == 0)
        {
            for (char c: expr)
            {
                if (c != '0' && c != '.')
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool try_tokenize(std::string token_like, Token &token) noexcept
    {
        double maybe_num;
        if (token_like.size() != 1)
        {
            if (try_parse_double(token_like, maybe_num))
            {
                token = move(Token::num(maybe_num));
                return true;
            }
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
