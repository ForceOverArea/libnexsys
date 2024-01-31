#ifndef SHUNTING_HPP_
#define SHUNTING_HPP_

#include <functional>
#include <iterator>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "context.hpp"

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