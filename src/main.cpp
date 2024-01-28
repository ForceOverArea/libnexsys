#include <iostream>
#include "matrix.hpp"

int main(int argc, char **argv)
{
    auto a = Matrix<double>::identity(3);

    std::cout << a.getIndex(1, 1) << '\n';

    std::cout << a.getIndexRef(1, 1) << '\n';

    return -1;
}