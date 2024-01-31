#ifndef _NEWTON_HPP
#define _NEWTON_HPP

#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "shunting.hpp" // also includes context.hpp

namespace nexsys 
{
    double newton_raphson(std::function<double (double)> func, double guess, double min, double max, double margin, size_t limit);

    inline double newton_raphson(std::function<double (double)> func, double margin, size_t limit);

    inline double newton_raphson(std::function<double (double)> func);
}

#endif