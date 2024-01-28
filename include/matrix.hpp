#ifndef MATRIX_HPP_
#define MATRIX_HPP_
// NOTE: This header has no .cpp file counterpart to allow for ease of use with generics

#include <vector>
#include <functional>

template<typename T>
class Matrix
{
private:
    // Default constructor to avoid extra allocations. for internal use only
    Matrix();

    // Helper methods
    bool inplaceInvert2() noexcept;
    bool inplaceInvert3() noexcept;
    bool inplaceInvert4() noexcept;
    bool inplaceInvertN() noexcept;

protected:
    size_t rows;
    size_t cols;
    std::vector<T> vals;

public:
    // Constructors
    Matrix(size_t rows, size_t cols);
    Matrix(std::vector<T> vals, size_t cols);

    // Pseudo-constructors
    static Matrix<T> identity(size_t n);
    Matrix<T> mapOver(std::function<T (T)> func);

    // Getters and setters
    inline T getIndex(size_t i, size_t j) const;
    inline T& getIndexRef(size_t i, size_t j);

    // Operator overloads
    Matrix<T> operator*(T scalar) const;
    Matrix<T> operator*(Matrix<T> rhs) const;

    // Methods
    void inplaceMapOver(std::function<T (T)> func);
    void inplaceRowSwap(size_t r1, size_t r2);
    void inplaceRowScale(size_t row, T scalar);
    void inplaceRowAdd(size_t add_row, size_t to_row);
    void inplaceScaledRowAdd(size_t add_row, T scaled_by, size_t to_row);

    bool tryInplaceInvert() noexcept;
};

/// Default constructor for a `Matrix<T>`, intended for internal class usage only
template<typename T>
Matrix<T>::Matrix()
{
    rows = 0;
    cols = 0;
    vals = std::vector<T>();
}

/// @brief Creates a new zero matrix with the given number of rows and columns
/// @tparam T The type of the contained `Matrix<T>` data
/// @param rows The number of rows the matrix should have
/// @param cols The number of columns the matrix should have
template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols): rows(rows), cols(cols)
{
    vals = std::vector<T>();
    vals.reserve(rows * cols);
    this->inplaceMapOver([](T _elem){ return (T)0; });
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

/// @brief Creates a new identity `Matrix<T>` with `n` rows and `n` columns
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

/// @brief Creates a new `Matrix<T>` by applying `func` to the elements of this `Matrix<T>`
/// @tparam T The type of the contained `Matrix<T>` data 
/// @param func The function to use to create the new matrix's elements from the old elements
/// @return a new `Matrix<T>` containing the elements created by `func`
template<typename T>
Matrix<T> Matrix<T>::mapOver(std::function<T (T)> func)
{
    Matrix<T> res = Matrix();
    res.rows = rows;
    res.cols = cols;

    for (auto p = vals.begin(); p != vals.end(); p++)
    { 
        res.vals.push_back(func(*p));
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

/// @brief Returns a new `Matrix<T>` scaled by the given scalar value
/// @tparam T The type of the contained `Matrix<T>` data
/// @param scalar The scalar to multiply this Matrix's elements by
/// @return A new `Matrix<T>` scaled by the given value
template<typename T>
Matrix<T> Matrix<T>::operator*(T scalar) const
{
    return this->mapOver([scalar](T x){ return x * scalar; });
}

/// @brief Returns a new `Matrix<T>` containing the matrix product of two matrices
/// @tparam T The type of the contained `Matrix<T>` data
/// @param rhs The matrix to multiply this matrix by
/// @return The matrix product of the two matrices given as a `Matrix<T>`
template<typename T>
Matrix<T> Matrix<T>::operator*(Matrix<T> rhs) const
{
    if (cols != rhs.rows)
    {
        throw;
    }

    size_t n = cols;
    Matrix<T> res = Matrix(rows, rhs.cols);

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < rhs.cols; j++)
        {
            for (size_t x = 0; x < n; x++)
            {
                res->getIndexRef(i, j) += this->getIndex(i, x) * rhs.getIndex(x, j);
            }
        }
    }

    return res;
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

/// @brief Helper function for `tryInplaceInvert`
template<typename T>
bool Matrix<T>::inplaceInvert2() noexcept
{
    T a11 = this->getIndex(0, 0);
    T a12 = this->getIndex(0, 1);
    T a21 = this->getIndex(1, 0);
    T a22 = this->getIndex(1, 1);

    T det = a11 * a22 - a12 * a21;

    if (det == (T)0)
    {
        return false;
    }

    this->getIndexRef(0, 0) =   a22 / det;
    this->getIndexRef(1, 0) = - a21 / det;
    this->getIndexRef(0, 1) = - a12 / det;
    this->getIndexRef(1, 1) =   a11 / det;

    return true;
}

/// @brief Helper function for `tryInplaceInvert`
template<typename T>
bool Matrix<T>::inplaceInvert3() noexcept
{
    T a11 = this->getIndex(0, 0);
    T a12 = this->getIndex(1, 0);
    T a13 = this->getIndex(2, 0);
    T a21 = this->getIndex(0, 1);
    T a22 = this->getIndex(1, 1);
    T a23 = this->getIndex(2, 1);
    T a31 = this->getIndex(0, 2);
    T a32 = this->getIndex(1, 2);
    T a33 = this->getIndex(2, 2);

    T det = a11*a22*a33 + a21*a32*a13 + a31*a12*a23 
          - a11*a32*a23 - a31*a22*a13 - a21*a12*a33;

    if (det == (T)0)
    {
        return false;
    }

    this->getIndexRef(0, 0) = (a22 * a33 - a23 * a32) / det;
    this->getIndexRef(1, 0) = (a23 * a31 - a21 * a33) / det;
    this->getIndexRef(2, 0) = (a21 * a32 - a22 * a31) / det;
    this->getIndexRef(0, 1) = (a13 * a32 - a12 * a33) / det;
    this->getIndexRef(1, 1) = (a11 * a33 - a13 * a31) / det;
    this->getIndexRef(2, 1) = (a12 * a31 - a11 * a32) / det;
    this->getIndexRef(0, 2) = (a12 * a23 - a13 * a22) / det;
    this->getIndexRef(1, 2) = (a13 * a21 - a11 * a23) / det;
    this->getIndexRef(2, 2) = (a11 * a22 - a12 * a21) / det;

    return true;
}

/// @brief Helper function for `tryInplaceInvert`
template<typename T>
bool Matrix<T>::inplaceInvert4() noexcept
{
    T a11 = this->getIndex(0, 0);
    T a12 = this->getIndex(1, 0);
    T a13 = this->getIndex(2, 0);
    T a14 = this->getIndex(3, 0);
    T a21 = this->getIndex(0, 1);
    T a22 = this->getIndex(1, 1);
    T a23 = this->getIndex(2, 1);
    T a24 = this->getIndex(3, 1);
    T a31 = this->getIndex(0, 2);
    T a32 = this->getIndex(1, 2);
    T a33 = this->getIndex(2, 2);
    T a34 = this->getIndex(3, 2);
    T a41 = this->getIndex(0, 3);
    T a42 = this->getIndex(1, 3);
    T a43 = this->getIndex(2, 3);
    T a44 = this->getIndex(3, 3);

    T det = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43 +
            a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41 + 
            a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42 + 
            a14*a21*a33*a42 + a14*a22*a34*a43 + a14*a23*a32*a41 -
            a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42 -
            a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43 -
            a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41 -
            a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;

    if (det == (T)0)
    {
        return false;
    }

    this->getIndex(0, 0) = (a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42) / det;
    this->getIndex(1, 0) = (a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43) / det;
    this->getIndex(2, 0) = (a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42) / det;
    this->getIndex(3, 0) = (a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33) / det;
    this->getIndex(0, 1) = (a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a34*a41 - a24*a31*a43) / det;
    this->getIndex(1, 1) = (a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41) / det;
    this->getIndex(2, 1) = (a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43) / det;
    this->getIndex(3, 1) = (a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31) / det;
    this->getIndex(0, 2) = (a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41) / det;
    this->getIndex(1, 2) = (a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42) / det;
    this->getIndex(2, 2) = (a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41) / det;
    this->getIndex(3, 2) = (a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32) / det;
    this->getIndex(0, 3) = (a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42) / det;
    this->getIndex(1, 3) = (a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41) / det;
    this->getIndex(2, 3) = (a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42) / det;
    this->getIndex(3, 3) = (a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31) / det;

    return true;
}

/// @brief Helper function for `tryInplaceInvert`
template<typename T>
bool Matrix<T>::inplaceInvertN() noexcept
{
    size_t n = rows;
    Matrix<T> inv = Matrix<T>::identity(n);
    std::vector<T> backup = vals;

    for (size_t j = 0; j < n; j++)
    {
        for (size_t i = 0; i < n; i++)
        {
            if (i == j)
            {
                continue;
            }

            if (this->getIndex(i, j) == (T)0)
            {
                vals = backup;
                return false;
            }

            T scalar = this->getIndex(i, j) / this->getIndex(j, j);
            this->inplaceRowAdd(i, j, -scalar);
            inv.inplaceRowAdd(i, j, -scalar);
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        T scalar = ((T)1) / this->getIndex(i, i);
        this->inplaceRowScale(i, scalar);
        inv.inplaceRowScale(i, scalar);
    }

    this->vals = inv.vals;
    return true;
}

/// @brief Tries to invert the matrix, returning a boolean indicating if the 
/// operation was successful. If the operation fails, the internal data will 
/// be in the same state as it was prior to the inversion attempt.
/// @returns A bool indicating if inversion was successful.
template<typename T>
bool Matrix<T>::tryInplaceInvert() noexcept
{
    if (rows != cols)
    {
        return false;
    }

    switch(rows)
    {
        case 1:
            if (vals[0] == 0) return false;
            vals[0] = 1 / vals[0];
            return true;
            
        case 2:
            return inplaceInvert2();

        case 3:
            return inplaceInvert3();

        case 4:
            return inplaceInvert4();

        default:
            return inplaceInvertN();
    }
}

#endif