#ifndef _VARIABLE_HPP
#define _VARIABLE_HPP
// NOTE: This header has no .cpp file counterpart due to its simplicity.

#include <cmath>

namespace nexsys
{
    struct Variable 
    {
    private:
        double min_bound;
        double max_bound;
        double value;

    public:
        /// @brief Constructor for a `Variable`
        /// @param value the value that the variable should have
        /// @param min_bound the lower bound of the variable's domain
        /// @param max_bound the upper bound of the variable's domain
        Variable(double value, double min_bound, double max_bound): value(value), min_bound(min_bound), max_bound(max_bound) {}

        /// @brief Constructor for a `Variable` whose domain is all real numbers
        /// @param value the value that the variable should have
        Variable(double value)
        {
            Variable(value, -INFINITY, INFINITY);
        }

        /// @brief Constructor for a `Variable` whose domain is all real numbers and whose value should default to `1.0`
        Variable()
        {
            Variable(1.0);
        }

        /// @brief Assignment operator for a `Variable`. Allows one to assign directly to `this->value` AND bounds the value to this `Variable`'s domain.
        /// @param rhs The value to be assigned
        /// @return A `Variable` set as close to `rhs` as allowed. 
        Variable operator=(double rhs)
        {
            value = rhs;
            if (max_bound != INFINITY && value > max_bound)
            {
                value = max_bound;
            }
            else if (min_bound != -INFINITY && value < min_bound)
            {
                value = min_bound;
            }
            
            return *this;
        }

        /// @brief Add-assignment operator for a `Variable`. Allows one to assign directly to `this->value` AND bounds the value to this `Variable`'s domain.
        /// @param rhs The value to be add-assigned
        /// @return A `Variable` set as close to `this->value + rhs` as allowed. 
        Variable operator+=(double rhs)
        {
            value += rhs;
            if (max_bound != INFINITY && value > max_bound)
            {
                value = max_bound;
            }
            else if (min_bound != -INFINITY && value < min_bound)
            {
                value = min_bound;
            }

            return *this;
        }

        /// @brief Subtract-assignment operator for a `Variable`. Allows one to assign directly to `this->value` AND bounds the value to this `Variable`'s domain.
        /// @param rhs The value to be subtract-assigned
        /// @return A `Variable` set as close to `this->value - rhs` as allowed. 
        Variable operator-=(double rhs)
        {
            value -= rhs;
            if (min_bound != -INFINITY && value < min_bound)
            {
                value = min_bound;
            }
            else if (max_bound != INFINITY && value > max_bound)
            {
                value = max_bound;
            }

            return *this;
        }
    };
}

#endif