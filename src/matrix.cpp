#include <vector>
#include <functional>
#include "matrix.hpp"

/// @brief Creates a new zero matrix with the given number of rows and columns
/// @tparam T The type of the data contained in the Matrix
/// @param rows The number of rows the matrix should have
/// @param cols The number of columns the matrix should have
template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols): rows(rows), cols(cols)
{
    vals = std::vector<T>();
    vals.reserve(rows * cols);
    this->inplaceMapOver([] (T _elem) { return (T)0; });
}

/// @brief Creates a new `Matrix<T>` from the data in `vals`, but only 
/// if the number of elements in `vals` is evenly divisible by `cols`
/// @tparam T The type of the data contained in the Matrix
/// @param vals The data that the matrix should contain
/// @param cols The number of columns the data should be divided into
template<typename T>
Matrix<T>::Matrix(std::vector<T> vals, size_t cols): cols(cols), vals(vals)
{
    if (vals.size() % cols != 0)
    {
        throw;
    }
    rows = vals.size() / cols;
}

template<typename T>
inline T& Matrix<T>::getIndex(size_t i, size_t j)
{
    return &vals[i * cols + j];
}

template<typename T>
void Matrix<T>::inplaceMapOver(std::function<T (T)> func)
{
    for (auto p = vals.begin(); p != vals.end(); p++)
    {
        *p = func(*p);
    }
}

template<typename T>
void Matrix<T>::inplaceRowSwap(size_t r1, size_t r2)
{
    for (int j = 0; j < cols; j++)
    {
        T stor = this->getIndex(r1, j);
        this->getIndex(r1, j) = this->getIndex(r2, j);
        this->getIndex(r2, j) = stor;
    }
}

template<typename T>
void Matrix<T>::inplaceRowScale(size_t row, T scalar)
{
    for (int j = 0; j < cols; j++)
    {
        this->getIndex(row, j) *= scalar;
    }
}

template<typename T>
void Matrix<T>::inplaceRowAdd(size_t add_row, size_t to_row)
{
    for (int j = 0; j < cols; j++)
    {
        this->getIndex(to_row, j) += this->getIndex(add_row, j);
    }
}

template<typename T>
void Matrix<T>::inplaceScaledRowAdd(size_t add_row, T scaled_by, size_t to_row)
{
    for (int j = 0; j < cols; j++)
    {
        this->getIndex(to_row, j) += this->getIndex(add_row, j) * scaled_by;
    }
}