#include "equation.hpp"

using std::regex;
using std::regex_search;
using std::regex_constants::icase;
using std::smatch;
using std::string;
using std::vector;

namespace nexsys
{
    const regex LEGAL_VARIABLE_REGEX { "[a-z][a-zA-Z0-9_]*" };

    vector<string> get_variables_in_equation(string equation)
    {
        auto results = regex_search(equation, smatch, LEGAL_VARIABLE_REGEX, icase);

        
    }
}