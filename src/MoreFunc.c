// This file is the addition funcions.
// Author: 12012613 Zhong Yuanji钟元吉
// Encode: UTF-8
// Version: gcc (Ubuntu 11.2.0-19ubuntu1) 11.2.0
// Version: gcc (x86_64-posix-sjlj-rev0, Built by MinGW-W64 project) 12.2.0
// Date: 2022/11/27
#include <stdlib.h>
#include <time.h>
#include "../inc/MoreFunc.h"

#include <omp.h>   // openMP
#ifdef __aarch64__ // Arm core
#include <arm_neon.h>
#define __m256 float32x4_t
#define _mm256_setzero_ps() vdupq_n_f32(0)
#define _mm256_loadu_ps vld1q_f32
#define _mm256_add_ps vaddq_f32
#define _mm256_mul_ps vmulq_f32
#define _mm256_storeu_ps vst1q_f32
#define _N_ 4
#else // Intel core
#include <immintrin.h>
#define _N_ 8
#endif

// Variable about random seed
int __TIME_SEED = -1;

// Multiply without optimization by order i->k->j
int matmul_plain(const Matrix *mat, const Matrix *oth, Matrix *ret)
{
    __Check_and_reset_Matrix_ret;
    // Initial and multiply by order i->k->j
    for (size_t i = 0; i < row; ++i)
        for (size_t k = 0; k < col; ++k)
        {
            __f d1 = data[i * col + k];
            for (size_t j = 0; j < col2; ++j)
                data_[i * col2 + j] += d1 * data2[k * col2 + j];
        }
    return 1;
}
// Multiply with only storage access optimization by order k->i->j
int matmul_improved_sa(const Matrix *mat, const Matrix *oth, Matrix *ret)
{
    __Check_and_reset_Matrix_ret;
    // Initial and multiply by order k->i->j
    for (size_t k = 0; k < col; ++k)
    {
        __f *d2 = data2 + k * col2;
        for (size_t i = 0; i < row; ++i)
        {
            __f d1 = data[i * col + k];
            __f *d3 = data_ + i * col2;
            for (size_t j = 0; j < col2; ++j)
                *(d3 + j) += d1 * *(d2 + j);
        }
    }
    return 1;
}
// Multiply with optimization by using SIMD and OpenMP
int matmul_improved(const Matrix *mat, const Matrix *oth, Matrix *ret)
{
    __Check_and_reset_Matrix_ret;
    size_t FLOOR = (size_t)(col / _N_) * _N_;
    __f copy2[FLOOR];
#pragma omp parallel for
    for (size_t c = 0; c < col2; ++c)
    {
        // Copy the line c from oth
        for (size_t r = 0; r < FLOOR; ++r)
            copy2[r] = data2[r * col2 + c];
        // Calculate the answer at [r,c]
        __f sum[_N_];
        __m256 Prod;
        for (size_t r = 0; r < row; ++r)
        {
            Prod = _mm256_setzero_ps();
            for (size_t j = 0; j < FLOOR; j += _N_)
                Prod = _mm256_add_ps(Prod, _mm256_mul_ps(
                                               _mm256_loadu_ps(data + (r * row + j)),
                                               _mm256_loadu_ps(copy2 + j)));
            _mm256_storeu_ps(sum, Prod);
            __f *target = data_ + (r * col2 + c);
            for (int i = 0; i < _N_; ++i)
                *target += sum[i];
            for (size_t k = FLOOR; k < col; ++k)
                *target += data[r * row + k] * data2[k * col + c];
        }
    }
    return 1;
}
// Generate random matrix with seed from -N to N
int rand_matrix_seed(Matrix *ret, __f N, size_t row, size_t col, size_t seed)
{
    if (!ret)
        return -3;
    if (row <= 0 || col <= 0)
        return -5;
    if (!ret->data || ret->row != row || ret->col != col)
    {
        if (ret->data)
            free(ret->data);
        ret->row = row, ret->col = col;
        ret->data = (__f *)malloc(row * col * __SIZEF);
    }
    __f *data_ = ret->data;
    srand(seed); // Get rand by time: seed=(int)time(NULL)
    rand();      // Ignore the first one
    for (size_t i = 0; i < row; ++i)
        for (size_t j = 0; j < col; ++j)
            // data_[i * row + j] = 2 * rand() - RAND_MAX;
            data_[i * row + j] = ((__f)2 * rand() / RAND_MAX - 1) * N;
    return 1;
}
// Generate random matrix with time seed from -100 to 100
int rand_matrix(Matrix *ret, __f N, size_t row, size_t col)
{
    if (__TIME_SEED == -1) // Relate with time
        __TIME_SEED = (int)time(NULL);
    else // Generate a random seed only depend on time
    {
        srand(__TIME_SEED);
        __TIME_SEED = rand();
    }
    return rand_matrix_seed(ret, N, row, col, __TIME_SEED);
}
