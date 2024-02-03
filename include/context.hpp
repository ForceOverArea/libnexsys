#ifndef _CONTEXT_HPP
#define _CONTEXT_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "variable.hpp"

namespace nexsys 
{    
    /// @brief The different legal tokens that may be found in a math expression
    enum TokenType
    {
        Plus,
        Minus,
        Mul,
        Div,
        Exp,
        LeftParenthesis,
        Comma,
        Num,
        Var,
        Func,
    };

    /// @brief A possible contained value of a `Token`. Not intended for use in external code.
    union _TokenValue
    {
        void* _phantom_ptr;
        double _phantom_double;    
    };

    /// @brief Type alias for a `std::pair` containing a `std::function` and the number of arguments it takes. 
    typedef std::pair<size_t, double (*)(double[])>* FunctionDataPtr;

    /// @brief A tagged union (i.e. Rust-style enum) that represents a single 
    /// token in a math expression, possibly also containing a constant, variable, 
    /// or function value
    struct Token 
    {
    private:
        TokenType type;
        _TokenValue value;

    public:
        /// @brief Creates a new `+` token
        /// @return a new `Token` 
        static Token plus();

        /// @brief Creates a new `-` token
        /// @return a new `Token` 
        static Token minus();

        /// @brief Creates a new `*` token
        /// @return a new `Token` 
        static Token mul();

        /// @brief Creates a new `/` token
        /// @return a new `Token` 
        static Token div();

        /// @brief Creates a new `^` token
        /// @return a new `Token` 
        static Token exp();

        /// @brief Creates a new `(` token
        /// @return a new `Token` 
        static Token left_parenthesis();

        /// @brief Creates a new `,` token
        /// @return a new `Token` 
        static Token comma();

        /// @brief Creates a new token for a constant `double` value
        /// @return a new `Token` 
        static Token num(double value);

        /// @brief Creates a new token for a variable `double` value
        /// @return a new `Token` 
        static Token var(Variable* value);

        /// @brief Creates a new token for a function
        /// @return a new `Token` 
        static Token func(size_t argc, double (*value)(double[]));

        /// @brief Provides read-only visibility to this `Token`'s type.
        /// @return this `Token`'s `TokenType`
        TokenType get_type() const;

        /// @brief Unwraps a constant `Token`, if possible.
        /// @param value A read/write reference to hold the wrapped `double` value
        /// @return A `bool` indicating success or failure.
        bool try_unwrap_num(double& value) const;

        /// @brief Unwraps a variable `Token`, if possible.
        /// @param value A read/write reference to hold the wrapped `double*` value
        /// @return A `bool` indicating success or failure.
        bool try_unwrap_var(Variable*& value) const;

        /// @brief Unwraps a variable `Token`, if possible.
        /// @param value A read/write reference to hold the wrapped function pointer value
        /// @return A `bool` indicating success or failure.
        bool try_unwrap_func(size_t& argc, double (*&value)(double[])) const;
    };

    /// @brief Similar to `std::unordered_map`, but with additional methods for specifying 
    /// symbols for `Token` values in an expression given as a `std::string`  
    class ContextMap final: public std::unordered_map<std::string, Token>
    {
    private:
        std::vector<std::string> malloced_vars;
        using std::unordered_map<std::string, Token>::erase; // Private so that users cannot leak memory by erasing a default variable.

    public:
        void add_num_to_ctx(std::string symbol, double value);

        void add_var_to_ctx(std::string symbol, Variable* value);

        inline void add_var_to_ctx(std::string symbol)
        {
            add_var_to_ctx(symbol, new Variable());
        }

        void add_func_to_ctx(std::string symbol, size_t argc, double (*value)(double[]));

        ~ContextMap();
    };

    /// @brief Converts a string to a `Token`, if possible
    /// @param token_like the string to try and convert to a `Token`
    /// @param token a `Token` reference whose value should be set to the parsed token's value
    /// @param ctx a `ContextMap` of the names of the valid tokens that may be found
    /// @return a `bool` indicating if parsing was successful
    bool try_tokenize(std::string token_like, Token &token) noexcept;

    /// @brief Converts a 1-character string to a `Token`, if possible
    /// @param token_like the string to convert
    /// @param token a token, passed by reference, whose value should reflect the token contained in `token_like`
    /// @param ctx a `ContextMap` containing any constants, variables, or functions that should be parsable
    /// @return a `bool` indicating if the token could be converted
    Token tokenize_with_context(std::string token_like, ContextMap ctx);
}

#endif