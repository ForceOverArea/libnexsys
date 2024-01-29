#ifndef NEWTON_HPP_
#define NEWTON_HPP_

#include <functional>
#include <cmath>

double newton_raphson(std::function<double (double)> func, double guess, double min, double max, double margin, size_t limit);

inline double newton_raphson(std::function<double (double)> func, double margin, size_t limit);

inline double newton_raphson(std::function<double (double)> func);

#endif