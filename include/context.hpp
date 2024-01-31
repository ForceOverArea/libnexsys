#ifndef _CONTEXT_HPP
#define _CONTEXT_HPP

#include <string>
#include <unordered_map>
#include <utility>

namespace nexsys 
{    
    /// @brief A possible contained value of a `Token`. Not intended for use in external code.
    union _TokenValue
    {
        void* _phantom_ptr;
        double _phantom_double;    
    };

    /// @brief Type alias for a `std::pair` containing a `std::function` and the number of arguments it takes. 
    typedef std::pair<size_t, double (*)(double[])>* FunctionDataPtr;

    /// @brief Type alias for a `std::unordered_map` of `std::string`s to token
    /// values that they hold in the context of a math expression. 
    typedef std::unordered_map<std::string, Token> ContextMap;

    /// @brief A tagged union (i.e. Rust-style enum) that represents a single 
    /// token in a math expression, possibly also containing a constant, variable, 
    /// or function value
    struct Token 
    {
    private:
        size_t type;
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
        static Token var(double* value);

        /// @brief Creates a new token for a function
        /// @return a new `Token` 
        static Token func(size_t argc, double (*value)(double[]));

        /// @brief Unwraps a constant `Token`, if possible.
        /// @param value A read/write reference to hold the wrapped `double` value
        /// @return A `bool` indicating success or failure.
        bool try_unwrap_num(double& value);

        /// @brief Unwraps a variable `Token`, if possible.
        /// @param value A read/write reference to hold the wrapped `double*` value
        /// @return A `bool` indicating success or failure.
        bool try_unwrap_var(double*& value);

        /// @brief Unwraps a variable `Token`, if possible.
        /// @param value A read/write reference to hold the wrapped function pointer value
        /// @return A `bool` indicating success or failure.
        bool try_unwrap_func(size_t& argc, double (*&value)(double[]));

        Token() {}
        ~Token() {}

        Token(const Token& other) 
        {
            this->type = other.type;
            this->value = other.value;
        }
        Token operator=(const Token& rhs) 
        {
            return Token(rhs);
        }
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