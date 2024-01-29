#ifndef SHUNTING_HPP_
#define SHUNTING_HPP_

#include <math.h>
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

/// @brief Evaluates a compiled reverse polish notation expression
/// @param rpn_expr The reverse polish notation expression as a `std::vector<Token>`
/// @return the value of the expression as a `double`
double eval_rpn_expression(std::vector<Token> rpn_expr);

#endif