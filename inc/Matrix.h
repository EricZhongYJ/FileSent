// This file is a copy of Project 3
#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdlib.h>

// Define data type to store by float
// If you want to use double or long double, replace the float into them.
typedef float __f;
#define fFormat "%f"
// Define the NULL float and NULL matrix
#define NULLF 1.23456e11f // random number
#define NULLMatrix createMatrix(0, 0, NULL)

// Define the size of float and Matrix
#define __SIZEF __SIZEOF_FLOAT__
#if __SIZEOF_POINTER__ == 4 // 32-bits
#define __SIZEM 12
#else // __SIZEOF_POINTER__ == 8 // 64-bits
#define __SIZEM 24
#endif

// Matrix struct
struct Matrix
{
    size_t row, col;
    __f *data;
};
typedef struct Matrix Matrix;

// Functions:
// Return int:  1:sucessful
//             -1:input Matrix mat is NULL pointer
//             -2:input Matrix oth is NULL pointer
//             -3:output Matrix ret is NULL pointer
//             -4:the size of input Matrix is wrong
//             -5:the input float row or col is wrong
//             -6:the Matrix is not invertible
// Create a matrix from string or data
Matrix *createMatrix(size_t row_, size_t col_, __f *data_);
Matrix *createMatrixFromStr(const char *strOrg);

// Delete a matrix
int deleteMatrix(Matrix **mat);

// Transform a matrix to string
char *to_string(const Matrix *mat);

// Copy a matrix (copy the data from a matrix to another)
int copyMatrix(const Matrix *mat, Matrix *ret);
// Get submatrix (include ~Begin, not include ~End)
int subMatrix(const Matrix *mat, size_t rowBegin, size_t rowEnd, size_t colBegin, size_t colEnd, Matrix *ret);
// Get cofactor matrix
int cofactorMatrix(const Matrix *mat, size_t rowAt, size_t colAt, Matrix *ret);

// Compute
// Add two matrices
int addMatrix(const Matrix *mat, const Matrix *oth, Matrix *ret);
// Subtraction of two matrices
int subtractMatrix(const Matrix *mat, const Matrix *oth, Matrix *ret);
// Add a scalar to a matrix
int addScalar(const Matrix *mat, __f num, Matrix *ret);
// Subtract a scalar from a matrix
int subtractScalar(const Matrix *mat, __f num, Matrix *ret);
// Multiply a matrix with a scalar
int multiplyScalar(const Matrix *mat, __f num, Matrix *ret);
// Multiply two matrices
int multiplyMatrix(const Matrix *mat, const Matrix *oth, Matrix *ret);

// Find the minimal values of a matrix
__f minOfMatrix(const Matrix *mat);
// Find the maximal values of a matrix
__f maxOfMatrix(const Matrix *mat);

// Compute the determinant
__f det(const Matrix *mat);

// Compute the inverse
int inv(const Matrix *mat, Matrix *ret);

// Transpose the matrix
int transpose(const Matrix *mat, Matrix *ret);
// Rotate 90 degree on the matrix
int rotate90(const Matrix *mat, Matrix *ret);

// Create datas to store for change return matrix ret
#define __RetMat(_row, _col, _code)                         \
    size_t row = mat->row, col = mat->col;                  \
    if (!ret->data || ret->row != _row || ret->col != _col) \
    {                                                       \
        if (ret->data)                                      \
            free(ret->data);                                \
        ret->row = _row, ret->col = _col;                   \
        ret->data = (__f *)malloc((_row) * (_col)*__SIZEF); \
    }                                                       \
    __f *data = mat->data, *data_ = ret->data;              \
    for (size_t i = 0; i < _row; ++i)                       \
        for (size_t j = 0; j < _col; ++j)                   \
        {                                                   \
            _code                                           \
        }

// Check if Matrix mat and ret are NULL pointer
#define __CheckMatRet       \
    if (!mat || !mat->data) \
        return -1;          \
    if (!ret)               \
        return -3;

#endif