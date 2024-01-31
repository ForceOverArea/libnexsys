#include "matrix.hpp"
#include "newton.hpp"

using std::function;
using std::string;
using std::unordered_map;
using std::vector;

namespace nexsys 
{
    constexpr double DX = 0.0001;

    double newton_raphson(
        function<double (double)> func, 
        double guess, 
        double min, 
        double max, 
        double margin, 
        size_t limit)
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

    inline double newton_raphson(function<double (double)> func, double margin, size_t limit)
    {
        return newton_raphson(func, 1.0, -INFINITY, INFINITY, margin, limit);
    }

    inline double newton_raphson(function<double (double)> func)
    {
        return newton_raphson(func, 0.0001, 100);
    }

    unordered_map<string, double> newton_raphson_multivariate(
        vector<function<double (unordered_map<string, double>)>> system, 
        unordered_map<string, double> guess,
        double margin,
        size_t limit)
    {
        if (margin <= 0.0)
        {
            throw; // TODO
        }

        if (limit == 0)
        {
            throw; // TODO
        }

        size_t n = system.size();
        if (guess.size() != n)
        {
            throw; // TODO
        }

        vector<double> elements;
        vector<double> error;
        double mag_error = 0;
        for (auto func: system)
        {
            // Build jacobian with f(x) values. We will mutate to partial derivatives later.
            double f_of_x = func(guess);

            // Write down error values
            error.push_back(f_of_x);
            mag_error += powl(f_of_x, 2);

            for (size_t i = 0; i < n; i++)
            {
                elements.push_back(f_of_x);
            }
        }

        Matrix<double> jacobian(elements, n);
        vector<string> vars;
        for (auto var_val: guess)
        {
            vars.push_back(var_val.first);
        }

        for (size_t j = 0; j < n; j++)
        {
            auto var_val = guess.find(vars[j]);
            if (var_val == guess.end())
            {
                throw; // TODO - failed to find variable that should be in guess map. see line 136 of newton.rs for context
            }

            var_val->second += DX;
            for (size_t i = 0; i < n; i++)
            {
                // mutate jacobian values to partial derivatives
                jacobian.get_index_ref(i, j) = (system[i](guess) - jacobian.get_index(i, j)) / DX;
            }
            var_val->second -= DX;
        }

        if (!jacobian.try_inplace_invert())
        {
            throw; // TODO - matrix inversion error
        }

        double mag_delta = 0;
        double* mag_delta_ptr = &mag_delta;

        Matrix<double> deltas = jacobian * Matrix<double>::from_col_vec(error);
        deltas.inplace_map_over(
            [mag_delta_ptr](double dx)
            {
                *mag_delta_ptr += powl(dx, 2);
                return dx;
            }
        );

        // If we are within the required radius of the correct value and solution
        if (sqrtl(mag_delta) <= margin && sqrtl(mag_error) <= margin)
        {
            return guess;
        }

        //...otherwise, modify guess and retry
        for (auto var_val: guess)
        {
            size_t i = &var_val - &*guess.begin();  // TODO: this is crusty as fuck...
            var_val.second -= deltas.get_index(i, 0);
        }

        return newton_raphson_multivariate(system, guess, margin, limit);
    }
}
