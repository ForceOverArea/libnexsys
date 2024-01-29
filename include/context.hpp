#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <string>
#include <unordered_map>
#include <utility>

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

/// @brief Represents a possible contained value for a `Token`. In most cases, this
/// will just be 8 bytes of `0x00` for operators and other tokens like `(` and `,`.
/// For constants, variables, and functions, however, this type contains
/// either a `double*`, `double`, or function pointer, respectively.
/// 
/// It should be noted that `var_value` and `func_value`, as pointers, may be
/// 32 OR 64 bits wide depending on architecture. So in order to ensure that
/// all relevant data is copied over from a value-containing `Token` value it is 
/// best to copy the data in `num_value` to always get 64-bits copied over
/// to the new value. 
union ValueType
{
public:
    double* var_value;
    double num_value;
    std::pair<size_t, double(*)(double[])>* func_value;

    ValueType() {}

    ValueType(const ValueType& other)
    {
        this->num_value = other.num_value; // TODO: This looks sketchy af... Will this cause problems between arch's?
    }

    ValueType operator=(const ValueType& rhs)
    {
        return ValueType(rhs);
    }

    ~ValueType() {}
};

/// @brief A tagged union (i.e. Rust-style enum) that represents a single 
/// token in a math expression, possibly also containing a constant, variable, 
/// or function value
struct Token 
{
public:
    TokenType type;
    ValueType value;

    Token() {}

    Token(const Token& other) 
    {
        this->type = other.type;
        this->value = other.value;
    }
    
    Token operator=(const Token& rhs) 
    {
        return Token(rhs);
    }

    ~Token() {}
};

/// A typedef for a `std::unordered_map` of `std::string`s to token
/// values that they hold in the context of a math expression. 
typedef std::unordered_map<std::string, Token> ContextMap;

/// @brief Converts a string to a `Token`, if possible
/// @param token_like the string to try and convert to a `Token`
/// @param token a `Token` reference whose value should be set to the parsed token's value
/// @param ctx a `ContextMap` of the names of the valid tokens that may be found
/// @return a `bool` indicating if parsing was successful
bool try_tokenize(std::string token_like, Token &token);

/// @brief Converts a 1-character string to a `Token`, if possible
/// @param token_like the string to convert
/// @param token a token, passed by reference, whose value should reflect the token contained in `token_like`
/// @param ctx a `ContextMap` containing any constants, variables, or functions that should be parsable
/// @return a `bool` indicating if the token could be converted
Token tokenize_with_context(std::string token_like, ContextMap ctx);

/// @brief Creates a token with type `Plus`
/// @return a new `Token`
inline Token plus();

/// @brief Creates a token with type `Minus`
/// @return a new `Token`
inline Token minus();

/// @brief Creates a token with type `Mul`
/// @return a new `Token`
inline Token mul();

/// @brief Creates a token with type `Div`
/// @return a new `Token`
inline Token div();

/// @brief Creates a token with type `Exp`
/// @return a new `Token`
inline Token exp();

/// @brief Creates a token with type `LeftParenthesis`
/// @return a new `Token`
inline Token left_parenthesis();

/// @brief Creates a token with type `Comma`
/// @return a new `Token`
inline Token comma();

/// @brief Creates a token with type `Num`
/// @return a new `Token`
Token num(double value);

/// @brief Creates a token with type `Var`
/// @return a new `Token`
Token var(double *value);

/// @brief Creates a token with type `Func` from a function pointer and the number of arguments it takes
/// @return a new `Token`
Token func(size_t argc, double (*value)(double[]));

#endif