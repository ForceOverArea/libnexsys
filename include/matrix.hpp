#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <functional>

template<typename T>
class Matrix
{
private:
    size_t rows;
    size_t cols;
    std::vector<T> vals;

public:
    Matrix(size_t rows, size_t cols);
    Matrix(std::vector<T> vals, size_t cols);

    T& getIndex(size_t i, size_t j);

    void inplaceMapOver(std::function<T (T)> func);
    void inplaceRowSwap(size_t r1, size_t r2);
    void inplaceRowScale(size_t row, T scalar);
    void inplaceRowAdd(size_t add_row, size_t to_row);
    void inplaceScaledRowAdd(size_t add_row, T scaled_by, size_t to_row);
};

#endif