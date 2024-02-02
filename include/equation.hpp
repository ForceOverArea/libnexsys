#ifndef _EQUATION_HPP
#define _EQUATION_HPP

#include <regex>

#include "newton.hpp" // also includes "shunting.hpp", "context.hpp", "variable.hpp"

namespace nexsys
{
    std::vector<std::string> get_variables_in_equation(std::string equation);
}

#endif