#ifndef SHUNTING_HPP_
#define SHUNTING_HPP_

#include <functional>
#include <regex>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>

#include "context.hpp"

/// Some of the different legal tokens that may be found in a math expression in `char` format.
const std::string OPTOKENS = "+-*/^(,)";

/// @brief Parses an expression to the equivalent reverse polish notation expression.
/// @param expr The mathematical expression to be parsed in infix notation
/// @param ctx The `ContextMap` containing any custom constant, variable, or function values in the expression.
/// @return a `std::vector<Token>` of the expression in reverse polish notation
std::vector<Token> rpnify(std::string expr, ContextMap ctx);


#endif