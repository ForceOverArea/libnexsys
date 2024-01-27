#ifndef MATRIX_HPP_
#define MATRIX_HPP_
// NOTE: This header has no .cpp file counterpart to allow for ease of use with generics

#include <vector>
#include <functional>

template<typename T>
class Matrix
{
private:
    size_t rows;
    size_t cols;
    std::vector<T> vals;

    bool inplaceInvert2();
    bool inplaceInvert3();
    bool inplaceInvert4();
    bool inplaceInvertN();

public:
    Matrix(size_t rows, size_t cols);
    Matrix(std::vector<T> vals, size_t cols);

    static Matrix<T> identity(size_t n);

    inline T getIndex(size_t i, size_t j) const;
    inline T& getIndexRef(size_t i, size_t j);

    void inplaceMapOver(std::function<T (T)> func);
    void inplaceRowSwap(size_t r1, size_t r2);
    void inplaceRowScale(size_t row, T scalar);
    void inplaceRowAdd(size_t add_row, size_t to_row);
    void inplaceScaledRowAdd(size_t add_row, T scaled_by, size_t to_row);

    bool inplaceInvert();
};

/// @brief Creates a new zero matrix with the given number of rows and columns
/// @tparam T The type of the contained `Matrix<T>` data
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
/// @tparam T The type of the contained `Matrix<T>` data
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

/// @brief Creates a new identity `Matrix<T>` with `n` rows and `n` columns.
/// @tparam T The type of the contained `Matrix<T>` data
/// @param n The number of rows and columns that the matrix should have
/// @return A new identity matrix
template<typename T>
Matrix<T> Matrix<T>::identity(size_t n)
{
    Matrix<T> res = Matrix(n, n);
    for (size_t i = 0; i < n; i++)
    {
        res.getIndexRef(i, i) = (T)1;
    }
    return res;
}

/// @brief Returns the value stored at the `i`th row and `j`th column of the `Matrix<T>`
/// @tparam T The type of the contained `Matrix<T>` data
/// @param i The row to be accessed
/// @param j The column to be accessed
/// @returns The value stored at the given index
template<typename T>
inline T Matrix<T>::getIndex(size_t i, size_t j) const
{
    return vals[i * cols + j];
}

/// @brief Returns a reference to the value stored at the `i`th row and `j`th column of the `Matrix<T>`
/// @tparam T The type of the contained `Matrix<T>` data
/// @param i The row to be accessed
/// @param j The column to be accessed
/// @returns The a reference to the value stored at the given index
template<typename T>
inline T& Matrix<T>::getIndexRef(size_t i, size_t j)
{
    return vals[i * cols + j];
}

/// @brief Applies a function to each element in the `Matrix<T>`, mutating the element's value to the returned result
/// @tparam T The type of the contained `Matrix<T>` data
/// @param func The function that should be applied to each element of the `Matrix<T>`
template<typename T>
void Matrix<T>::inplaceMapOver(std::function<T (T)> func)
{
    for (auto p = vals.begin(); p != vals.end(); p++)
    {
        *p = func(*p);
    }
}

/// @brief Swaps the rows `r1` and `r2` in the `Matrix<T>`
/// @tparam T The type of the contained `Matrix<T>` data
/// @param r1 The first row to swap
/// @param r2 The second row to swap
template<typename T>
void Matrix<T>::inplaceRowSwap(size_t r1, size_t r2)
{
    for (size_t j = 0; j < cols; j++)
    {
        T stor = this->getIndex(r1, j);
        this->getIndexRef(r1, j) = this->getIndex(r2, j);
        this->getIndexRef(r2, j) = stor;
    }
}

/// @brief Scales the given row in the `Matrix<T>` by the given scalar value
/// @tparam T The type of the contained `Matrix<T>` data
/// @param row The row to scale
/// @param scalar The quantity to scale the row by
template<typename T>
void Matrix<T>::inplaceRowScale(size_t row, T scalar)
{
    for (size_t j = 0; j < cols; j++)
    {
        this->getIndexRef(row, j) *= scalar;
    }
}

/// @brief Adds one row to another row in an element-wise fashion, mutating the values in the second row
/// @tparam T The type of the contained `Matrix<T>` data
/// @param add_row The row whose values should be added and not mutated
/// @param to_row The row whose values should be mutably added to
template<typename T>
void Matrix<T>::inplaceRowAdd(size_t add_row, size_t to_row)
{
    for (size_t j = 0; j < cols; j++)
    {
        this->getIndexRef(to_row, j) += this->getIndex(add_row, j);
    }
}

/// @brief Adds one row, scaled by a given value, to another row in an element-wise fashion, mutating the values in the second row
/// @tparam T The type of the contained `Matrix<T>` data
/// @param add_row The row whose values should be added and not mutated
/// @param scaled_by The value to scale `add_row` by
/// @param to_row The row whose values should be mutably added to
template<typename T>
void Matrix<T>::inplaceScaledRowAdd(size_t add_row, T scaled_by, size_t to_row)
{
    for (size_t j = 0; j < cols; j++)
    {
        this->getIndexRef(to_row, j) += this->getIndex(add_row, j) * scaled_by;
    }
}

template<typename T>
bool Matrix<T>::inplaceInvert()
{

}
#endif