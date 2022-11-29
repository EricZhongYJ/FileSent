// This file is a copy of Project 3
// Only change: int -> size_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/Matrix.h"

// Functions:
// Create a matrix from Data
Matrix *createMatrix(size_t row_, size_t col_, __f *data_)
{
    // Check if the pointer is NULL, return 0x0 Matrix
    if (row_ < 0 || col_ < 0 || (!data_ && (row_ || col_)))
        return NULLMatrix;
    Matrix *ret = (Matrix *)malloc(__SIZEM);
    ret->row = row_;
    ret->col = col_;
    ret->data = data_;
    return ret;
}

// Create a matrix from string
Matrix *createMatrixFromStr(const char *strOrg)
{
    if (!strOrg)
        return NULLMatrix;
    // Copy the string
    size_t orgLen = strlen(strOrg);
    char *str = (char *)malloc(orgLen * sizeof(char));
    strcpy(str, strOrg);
    // Replace the blanks into ','
    for (size_t i = 0, j = 0; j <= orgLen; ++j)
        if (str[j] != ' ')
            str[i++] = str[j];
        else if (j != 0 && ((str[j - 1] >= '0' && str[j - 1] <= '9') || str[j - 1] == '.'))
            str[i++] = ',';
    size_t len = strlen(str);
    // Check format error
    if (len < 3 || str[0] != '[' || str[1] == ',' || str[1] == ';' || str[1] == ']' || str[len - 1] != ']')
        return NULLMatrix;
    while (str[len - 1] == ',' || str[len - 1] == ';' || str[len - 1] == ']')
        str[--len] = '\0';
    // Count rows and cols
    size_t countD = 0, countF = 0;
    for (size_t i = 1; i < len; ++i)
        if (str[i] == ';')
            ++countF;
        else if (str[i] == ',' && !countF)
            ++countD;
    // Read from string
    Matrix *ret = (Matrix *)malloc(__SIZEM);
    size_t At = 1;
    ret->row = countF + 1, ret->col = countD + 1;
    __f *data_ = (__f *)malloc((countF + 1) * (countD + 1) * __SIZEF);
    for (size_t i = 0; i <= countF; ++i)
        for (size_t j = 0; j <= countD; ++j, ++At)
        {
            sscanf(&str[At], fFormat, &data_[i * (countD + 1) + j]);
            for (; At < len; ++At)
            {
                if (str[At] == ',')
                {
                    if (str[At + 1] == ';')
                        ++At;
                    break;
                }
                else if (str[At] == ';')
                {
                    if (j != countD)
                        goto RETURN_NULL;
                    break;
                }
                else if ((str[At] < '0' || str[At] > '9') && str[At] != '.' && str[At] != '-' && str[At] != 'e' && str[At] != 'E')
                    goto RETURN_NULL;
            }
        }
    if (At < len)
        goto RETURN_NULL;
    free(str);
    ret->data = data_;
    return ret;
RETURN_NULL:
    free(str), free(ret), free(data_);
    return NULLMatrix;
}

// Delete a matrix
int deleteMatrix(Matrix **mat)
{
    if (!mat || !*mat)
        return -1;
    if ((*mat)->data)
        free((*mat)->data);
    free(*mat);
    *mat = NULL;
    return 1;
}

// Transform a matrix to string
char *to_string(const Matrix *mat)
{
    if (!mat || !mat->data || mat->row == 0 || mat->col == 0)
        return (char *)"Matrix 0x0: []\n";
    size_t row = mat->row, col = mat->col;
    __f *data = mat->data;
    char *ret = (char *)malloc((15 * row * col + row + 25) * sizeof(char));
    char *tmp = (char *)malloc(20 * sizeof(char));
    sprintf(ret, "Matrix %ldx%ld:\n[\n", row, col);
    for (size_t i = 0; i < row; ++i)
    {
        for (size_t j = 0; j < col; ++j)
        {
            sprintf(tmp, "%15.6e", data[i * col + j]);
            strcat(ret, tmp);
        }
        strcat(ret, "\n");
    }
    strcat(ret, "]\n");
    free(tmp);
    return ret;
}

// Copy a matrix (copy the data from a matrix to another)
int copyMatrix(const Matrix *mat, Matrix *ret)
{
    __CheckMatRet;
    // __RetMat(row, col, data_[i * col + j] = data[i * col + j];);
    size_t row = mat->row, col = mat->col;
    if (!ret->data || ret->row != row || ret->col != col)
    {
        if (ret->data)
            free(ret->data);
        ret->row = row, ret->col = col;
        ret->data = (__f *)malloc((row) * (col)*__SIZEF);
    }
    // memory copy
    memcpy(ret->data, mat->data, row * col * __SIZEF);
    return 1;
}

// Get submatrix (include ~Begin, not include ~End)
int subMatrix(const Matrix *mat, size_t rowBegin, size_t rowEnd, size_t colBegin, size_t colEnd, Matrix *ret)
{
    __CheckMatRet;
    // Check if 0 <= ~Begin < ~End <= mat.~
    if (0 > rowBegin || rowBegin >= rowEnd || rowEnd > mat->row ||
        0 > colBegin || colBegin >= colEnd || colEnd > mat->col)
        return -5;
    __RetMat(rowEnd - rowBegin, colEnd - colBegin,
             data_[i * (colEnd - colBegin) + j] = data[(rowBegin + i) * col + colBegin + j];);
    return 1;
}

// Get cofactor matrix
int cofactorMatrix(const Matrix *mat, size_t rowAt, size_t colAt, Matrix *ret)
{
    __CheckMatRet;
    // Check if 0 <= ~At < mat.~
    if (0 > rowAt || rowAt >= mat->row || 0 > colAt || colAt >= mat->col)
        return -5;
    __RetMat(row - 1, col - 1, data_[i * (col - 1) + j] = data[((i >= rowAt) ? (i + 1) : i) * col + ((j >= colAt) ? (j + 1) : j)];);
    return 1;
}

// Add two matrices
int addMatrix(const Matrix *mat, const Matrix *oth, Matrix *ret)
{
    if (!oth || !oth->data)
        return -2;
    __CheckMatRet;
    if (mat->row != oth->row || mat->col != oth->col)
        return -4;
    __f *data2 = oth->data;
    __RetMat(row, col, data_[i * col + j] = data[i * col + j] + data2[i * col + j];);
    return 1;
}

// Subtraction of two matrices
int subtractMatrix(const Matrix *mat, const Matrix *oth, Matrix *ret)
{
    if (!oth || !oth->data)
        return -2;
    __CheckMatRet;
    if (mat->row != oth->row || mat->col != oth->col)
        return -4;
    __f *data2 = oth->data;
    __RetMat(row, col, data_[i * col + j] = data[i * col + j] - data2[i * col + j];);
    return 1;
}

// Add a scalar to a matrix
int addScalar(const Matrix *mat, __f num, Matrix *ret)
{
    __CheckMatRet;
    __RetMat(row, col, data_[i * col + j] = data[i * col + j] + num;);
    return 1;
}

// Subtract a scalar from a matrix
int subtractScalar(const Matrix *mat, __f num, Matrix *ret)
{
    return addScalar(mat, -num, ret);
}

// Multiply a matrix with a scalar
int multiplyScalar(const Matrix *mat, __f num, Matrix *ret)
{
    __CheckMatRet;
    __RetMat(row, col, data_[i * col + j] = data[i * col + j] * num;);
    return 1;
}

// Multiply two matrices(change:int->size_t)
int multiplyMatrix(const Matrix *mat, const Matrix *oth, Matrix *ret)
{
    // Check and reset Matrix ret
    if (!oth || !oth->data)
        return -2;
    __CheckMatRet;
    if (mat->col != oth->row)
        return -4;
    size_t row = mat->row, col = mat->col, col2 = oth->col;
    __f *data = mat->data, *data2 = oth->data;
    if (!ret->data || ret->row != row || ret->col != col2)
    {
        if (ret->data)
            free(ret->data);
        ret->row = row, ret->col = col2;
        ret->data = (__f *)malloc(row * col2 * __SIZEF);
    }
    __f *data_ = ret->data;
    // Initial and multiply by order j->k->i
    for (size_t i = 0; i < row * col2; ++i)
        data_[i] = 0;
    for (size_t i = 0; i < row; ++i)
        for (size_t k = 0; k < col; ++k)
        {
            __f num1 = data[i * col + k];
            for (size_t j = 0; j < col2; ++j)
                data_[i * col2 + j] += num1 * data2[k * (col2) + j];
        }
    return 1;
}

// Find the minimal values of a matrix
__f minOfMatrix(const Matrix *mat)
{
    if (!mat || !mat->data || mat->row == 0 || mat->col == 0)
        return NULLF;
    size_t row = mat->row, col = mat->col;
    __f *data = mat->data, ret = data[0];
    for (size_t i = 0; i < row * col; ++i)
        if (data[i] < ret)
            ret = data[i];
    return ret;
}

// Find the maximal values of a matrix
__f maxOfMatrix(const Matrix *mat)
{
    if (!mat || !mat->data || mat->row == 0 || mat->col == 0)
        return NULLF;
    size_t row = mat->row, col = mat->col;
    __f *data = mat->data, ret = data[0];
    for (size_t i = 0; i < row * col; ++i)
        if (data[i] > ret)
            ret = data[i];
    return ret;
}

// Compute the determinant of the matrix
__f det(const Matrix *mat)
{
    // Check if exist and rows equals columns
    if (!mat || !mat->data || mat->row == 0 || mat->col != mat->row)
        return NULLF;
    // Copy the data
    size_t row = mat->row, col = mat->col;
    __f *data1 = mat->data, ret = 1;
    __f *data = (__f *)malloc(row * col * __SIZEF);
    for (size_t i = 0; i < row * col; ++i)
        data[i] = data1[i];
    // Eliminate for each row using Gauss Method
    for (size_t i = 0; i < row; ++i)
    {
        // Check M_{i,i} != 0, if not change the rows
        for (size_t j = i; j < row; ++j)
            if (data[j * col + i] != 0)
            {
                if (i != j)
                { // Exchange two rows, determinant
                    for (size_t k = i; k < col; ++k)
                    {
                        __f tmpF = data[i * col + k];
                        data[i * col + k] = data[j * col + k];
                        data[j * col + k] = tmpF;
                    }
                    if ((j - i) % 2)
                        ret *= -1;
                }
                break;
            }
        // If no rows can make M_{i,i} != 0, the determinant is 0
        if (data[i * col + i] == 0)
            return 0.f;
        // Eliminate by using Gauss Method
        ret *= data[i * col + i];
        for (size_t j = i + 1; j < row; ++j)
        {
            __f num = data[j * col + i] / data[i * col + i];
            for (size_t k = i; k < col; ++k)
                data[j * col + k] -= num * data[i * col + k];
        }
    }
    // Delete the copy and return
    free(data);
    return ret;
}

// Compute the inverse of the matrix
int inv(const Matrix *mat, Matrix *ret)
{
    // Check if exist and rows equals columns
    __CheckMatRet;
    if (mat->row == 0 || mat->row != mat->col)
        return -4;
    __f matDet = det(mat);
    if (matDet == 0)
        return -6;
    // If the size of matrix is 1x1
    if (mat->row == 1)
    {
        if (!ret->data || ret->row != 1 || ret->col != 1)
        {
            if (ret->data)
                free(ret->data);
            ret->row = 1, ret->col = 1;
            ret->data = (__f *)malloc(__SIZEF);
        }
        ret->data[0] = 1 / mat->data[0];
        return 1;
    }
    // Else compute by using algebraic complement
    __RetMat(row, col,
             Matrix *tmp = NULLMatrix;
             cofactorMatrix(mat, j, i, tmp);
             data_[i * col + j] = ((i + j) % 2 ? -det(tmp) : det(tmp)) / matDet;
             deleteMatrix(&tmp);
             ;);
    return 1;
}

// Transpose the matrix
int transpose(const Matrix *mat, Matrix *ret)
{
    __CheckMatRet;
    __RetMat(col, row, data_[i * row + j] = data[j * col + i];);
    return 1;
}

// Rotate 90 degree on the matrix
int rotate90(const Matrix *mat, Matrix *ret)
{
    __CheckMatRet;
    __RetMat(col, row, data_[i * row + j] = data[j * col + col - i - 1];);
    return 1;
}