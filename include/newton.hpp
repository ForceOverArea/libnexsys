#ifndef _NEWTON_HPP
#define _NEWTON_HPP

#include "matrix.hpp"
#include "shunting.hpp" // also includes "context.hpp", "variable.hpp"

namespace nexsys 
{
    /// @brief A constant used by `nexsys` newton-raphson solver functions. It represents the quantity represented by 'dx' in calculus.
    constexpr double DX = 0.0001;

    /// @brief Finds the root of a function of a single unknown variable.
    /// @param func The function whose root should be found
    /// @param guess The initial guess value for the root of the function
    /// @param margin The margin of error for the root
    /// @param limit The maximum number of iterations that should be attempted in finding the root
    /// @return The root of the given function
    double newton_raphson(std::function<double (double)> func, double guess, double min, double max, double margin, size_t limit);

    /// @brief Finds the root of a multivariate system of functions
    /// @param system The `std::vector` of functions in the system
    /// @param guess The initial guess for the root of the system
    /// @param margin The margin of error for the root
    /// @param limit The maximum number of iterations that chould be attempted in finding the root
    /// @return The root of the given system
    std::unordered_map<std::string, double> newton_raphson_multivariate(std::vector<std::function<double (std::unordered_map<std::string, double>)>> system, std::unordered_map<std::string, double> guess, double margin, size_t limit);
}

#endif