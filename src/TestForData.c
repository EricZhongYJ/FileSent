// This file is test for getting time for plot, using random with time seed.
// Author: 12012613 Zhong Yuanji钟元吉
// Encode: UTF-8
// Version: gcc (Ubuntu 11.2.0-19ubuntu1) 11.2.0
// Version: gcc (x86_64-posix-sjlj-rev0, Built by MinGW-W64 project) 12.2.0
// Date: 2022/11/27

// /* Take this line as a comment
#include "../inc/MoreFunc.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define __TimeStart __TBeg = clock();
#define __TimeEnd(var, str)                             \
    (var) = (clock() - __TBeg) / (float)CLOCKS_PER_SEC; \
    printf("%s %f s.\n", (str), (var));

clock_t __TBeg;
int main(int argc, char const *argv[])
{
    // Optimize None(-O0):
    int NS[] = {10, 20, 40, 80, 120, 200, 300, 400, 500, 600, 700, 800, 900,
                1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900,
                2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900};
    // // Optimize -O3:
    // int NS[] = {25, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
    //             1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000,
    //             3400, 3800, 4200, 4600, 5000, 5500, 6000, 6500, 7000, 8000};
    int LEN = sizeof(NS) / sizeof(NS[0]);
    float Times1[LEN], Times2[LEN], Times3[LEN];
    Matrix *mats[LEN * 5], *at1, *at2;
    for (int i = 0; i < LEN * 5; ++i)
        mats[i] = NULLMatrix;
    float diff, *d1; // For compare the correctness of two method
    // Run for time with time seed
    for (int k = 0; k < LEN; ++k)
    {
        printf("========================check for %dx%d========================\n", NS[k], NS[k]);
        rand_matrix(mats[5 * k], 100.f, NS[k], NS[k]);
        rand_matrix(mats[5 * k + 1], 100.f, NS[k], NS[k]);
        printf("Get random data from time seed\n");
        at1 = mats[5 * k];
        at2 = mats[5 * k + 1];
        __TimeStart;
        matmul_plain(at1, at2, mats[5 * k + 2]);
        __TimeEnd(Times1[k], "matmul_plain       cost:");
        __TimeStart;
        matmul_improved_sa(at1, at2, mats[5 * k + 3]);
        __TimeEnd(Times2[k], "matmul_improved_sa cost:");
        __TimeStart;
        matmul_improved(at1, at2, mats[5 * k + 4]);
        __TimeEnd(Times3[k], "matmul_improved    cost:");
        d1 = mats[5 * k + 2]->data;
        diff = 0;
        for (size_t i = 0; i < NS[k] * NS[k]; ++i)
            diff += abs(d1[i] - mats[5 * k + 3]->data[i]) / NS[k] / NS[k];
        printf("The average difference of answer by 12 methods is:%f\n", diff);
        diff = 0;
        for (size_t i = 0; i < NS[k] * NS[k]; ++i)
            diff += abs(d1[i] - mats[5 * k + 4]->data[i]) / NS[k] / NS[k];
        printf("The average difference of answer by 13 methods is:%f\n", diff);
        for (int i = 5 * k; i < 5 * k + 4; ++i)
            deleteMatrix(mats + i);
    }
    printf("\n\tNS:\n");
    for (int i = 0; i < LEN; ++i)
        printf("%d,", NS[i]);
    printf("\n\tmatmul_plain       cost:\n");
    for (int i = 0; i < LEN; ++i)
        printf("%f,", Times1[i]);
    printf("\n\tmatmul_improved_sa cost:\n");
    for (int i = 0; i < LEN; ++i)
        printf("%f,", Times2[i]);
    printf("\n\tmatmul_improved    cost:\n");
    for (int i = 0; i < LEN; ++i)
        printf("%f,", Times3[i]);
    printf("\n");
    return 0;
}

/* -O0:
        NS:
10,20,40,80,120,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,
        matmul_plain    cost:
0.000015,0.000100,0.000556,0.004983,0.021453,0.058669,0.189414,0.363975,0.706692,1.239663,1.917768,2.833014,4.048620,6.204958,7.386035,9.387250,12.090151,14.521228,18.047867,22.609703,27.089046,32.851536,38.658436,45.998169,52.020687,60.241199,67.256035,75.664818,88.387169,100.632980,111.793961,126.537132,147.178726,
        matmul_improved cost:
0.000017,0.000140,0.003508,0.004883,0.006384,0.011124,0.035586,0.086592,0.157963,0.269087,0.455864,0.640834,0.996916,1.222866,1.638964,2.412303,2.648156,3.410529,4.244712,5.838749,6.330342,7.441198,9.001981,10.419107,11.621061,14.381650,16.140200,17.842556,20.412657,23.695906,26.432596,30.022816,35.152584,

-Ofast:
        NS:
10,20,40,80,120,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,
        matmul_plain    cost:
0.000006,0.000006,0.000019,0.000102,0.000461,0.006986,0.010646,0.027473,0.046472,0.107142,0.099759,0.209806,0.184184,0.272510,0.399440,0.551067,0.769131,1.540052,1.241769,1.607887,2.052627,2.594546,3.160509,4.216679,5.377310,5.367282,5.799335,6.733529,7.582844,9.430331,9.961745,11.620366,12.678252,
        matmul_improved cost:
0.000001,0.000005,0.000021,0.000122,0.000463,0.002540,0.017498,0.019727,0.039312,0.088072,0.101521,0.149893,0.246636,0.329399,0.489576,0.631457,0.978527,1.889567,1.488090,1.862860,2.308334,2.813259,3.560848,4.672939,4.759886,5.488166,6.281740,7.157347,8.688493,9.857400,10.818696,12.079294,14.194523,
*/