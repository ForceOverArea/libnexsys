#ifndef _SHUNTING_HPP
#define _SHUNTING_HPP

#include <functional>
#include <iterator>
#include <sstream>
#include <stdlib.h>

#include "context.hpp" // also includes "variable.hpp"

namespace nexsys
{
    /// Some of the different legal tokens that may be found in a math expression in `char` format.
    const std::string OPTOKENS = "+-*/^(,)";

    /// @brief Compiles an expression in infix notation to a multivariate function
    /// @param expr The expression that the given closure should evaluate upon being called
    /// @param ctx The `ContextMap` describing what any variables, functions, or  in the expression are
    /// @return 
    std::function<double (std::unordered_map<std::string, double>)> compile_to_function_of_umap(std::string expr, ContextMap ctx);
}
#endif