#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000
typedef short pholder;
pholder sum_unroll_1(pholder *a) {
    pholder sum = 0;
    for (int i = 0; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_2(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 1; i += 2)
        sum += a[i] + a[i+1];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_3(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 2; i += 3)
        sum += a[i] + a[i+1] + a[i+2];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_4(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 3; i += 4)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_5(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 4; i += 5)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_6(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 5; i += 6)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_7(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 6; i += 7)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_8(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 7; i += 8)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_9(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 8; i += 9)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_10(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 9; i += 10)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_11(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 10; i += 11)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_12(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 11; i += 12)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_13(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 12; i += 13)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_14(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 13; i += 14)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_15(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 14; i += 15)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_16(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 15; i += 16)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] +
               a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_17(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 16; i += 17)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_18(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 17; i += 18)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_19(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 18; i += 19)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_20(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 19; i += 20)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] +
               a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_21(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 20; i += 21)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_22(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 21; i += 22)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_23(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 22; i += 23)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_24(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 23; i += 24)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_25(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 24; i += 25)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_26(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 25; i += 26)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_27(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 26; i += 27)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_28(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 27; i += 28)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_29(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 28; i += 29)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_30(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 29; i += 30)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28] + a[i+29];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_31(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 30; i += 31)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] + a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] + a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] + a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28] + a[i+29] + a[i+30];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

pholder sum_unroll_32(pholder *a) {
    pholder sum = 0;
    int i;
    for (i = 0; i < N - 31; i += 32)
        sum += a[i] + a[i+1] + a[i+2] + a[i+3] + a[i+4] + a[i+5] + a[i+6] + a[i+7] +
               a[i+8] + a[i+9] + a[i+10] + a[i+11] + a[i+12] + a[i+13] + a[i+14] + a[i+15] +
               a[i+16] + a[i+17] + a[i+18] + a[i+19] + a[i+20] + a[i+21] + a[i+22] + a[i+23] +
               a[i+24] + a[i+25] + a[i+26] + a[i+27] + a[i+28] + a[i+29] + a[i+30] + a[i+31];
    for (; i < N; i++)
        sum += a[i];
    return sum;
}

int main() {
    pholder *a = malloc(N * sizeof(pholder));
    pholder sum;
    double start, end;
    int unroll_factors[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    pholder (*unroll_functions[])(pholder*) = {
        sum_unroll_1, sum_unroll_2, sum_unroll_3, sum_unroll_4, sum_unroll_5,
        sum_unroll_6, sum_unroll_7, sum_unroll_8, sum_unroll_9, sum_unroll_10,
        sum_unroll_11, sum_unroll_12, sum_unroll_13, sum_unroll_14, sum_unroll_15,
        sum_unroll_16, sum_unroll_17, sum_unroll_18, sum_unroll_19, sum_unroll_20,
        sum_unroll_21, sum_unroll_22, sum_unroll_23, sum_unroll_24, sum_unroll_25,
        sum_unroll_26, sum_unroll_27, sum_unroll_28, sum_unroll_29, sum_unroll_30,
        sum_unroll_31, sum_unroll_32
    };

    for (int i = 0; i < N; i++)
        a[i] = 1;

    printf("Unroll Factor\tSum\t\tTime (ms)\n");
    printf("===========================================\n");

    for (int i = 0; i < 32; i++) {
        start = (double)clock() / CLOCKS_PER_SEC;
        sum = unroll_functions[i](a);
        end = (double)clock() / CLOCKS_PER_SEC;
        
        printf("U = %2d\t\t%d\t\t%.15f\n", unroll_factors[i], sum, (end - start) * 1000.0);
    }

    free(a);
    return 0;
}