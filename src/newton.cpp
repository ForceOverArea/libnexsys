#include "newton.hpp"

constexpr double DX = 0.0001;

double newton_raphson(std::function<double (double)> func, double guess, double min, double max, double margin, size_t limit)
{
    if (limit == 0)
    {
        throw; // TODO
    }

    double y        = func(guess);
    double y_prime  = (func(guess + DX) - y) / DX;
    double delta    = y_prime / y;

    if (abs(func(guess)) < margin && abs(delta) < margin)
    {
        if (guess > max)
        {
            return max;
        }
        else if (guess < min)
        {
            return min;
        }
        return guess;
    }
    
    return newton_raphson(func, guess - delta, min, max, margin, limit - 1);
}

inline double newton_raphson(std::function<double (double)> func, double margin, size_t limit)
{
    return newton_raphson(func, 1.0, -INFINITY, INFINITY, margin, limit);
}

inline double newton_raphson(std::function<double (double)> func)
{
    return newton_raphson(func, 0.0001, 100);
}