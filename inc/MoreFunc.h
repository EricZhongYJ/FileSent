// This file is the function head of addition funcions.
// Author: 12012613 Zhong Yuanji钟元吉
// Encode: UTF-8
// Version: gcc (Ubuntu 11.2.0-19ubuntu1) 11.2.0
// Version: gcc (x86_64-posix-sjlj-rev0, Built by MinGW-W64 project) 12.2.0
// Date: 2022/11/13
#ifndef __MOREFUN_H__
#define __MOREFUN_H__

#include "Matrix.h"

// Function for output
#define __show(name, mat)               \
    temp = to_string(mat);              \
    printf("mat%d = %s", (name), temp); \
    if ((mat)->row && (mat)->col)       \
        free(temp);

// Function for check and reset the Matrix ret
#define __Check_and_reset_Matrix_ret                        \
    if (!oth || !oth->data)                                 \
        return -2;                                          \
    __CheckMatRet;                                          \
    if (mat->col != oth->row)                               \
        return -4;                                          \
    size_t row = mat->row, col = mat->col, col2 = oth->col; \
    __f *data = mat->data, *data2 = oth->data;              \
    if (!ret->data || ret->row != row || ret->col != col2)  \
    {                                                       \
        if (ret->data)                                      \
            free(ret->data);                                \
        ret->row = row, ret->col = col2;                    \
        ret->data = (__f *)calloc(__SIZEF, row * col2);     \
    }                                                       \
    __f *data_ = ret->data;

// Multiply without optimization by order i->k->j
int matmul_plain(const Matrix *mat, const Matrix *oth, Matrix *ret);
// Multiply with only storage access optimization by order k->i->j
int matmul_improved_sa(const Matrix *mat, const Matrix *oth, Matrix *ret);
// Multiply with optimization by using SIMD and OpenMP
int matmul_improved(const Matrix *mat, const Matrix *oth, Matrix *ret);
// Generate random matrix with seed from -100 to 100
int rand_matrix_seed(Matrix *ret, __f N, size_t row, size_t col, size_t seed);
// Generate random matrix with time seed from -100 to 100
int rand_matrix(Matrix *ret, __f N, size_t row, size_t col);

#endif