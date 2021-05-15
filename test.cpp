#include <stdio.h>
#include "umesimd/UMESimd.h"

#define TESTSIZE 100
#define SIMDSIZE 32


void testsimd(){
    int iteration, remainder, offset;
    float a[TESTSIZE] = { 0.0f };
    uint32_t stride = 2;
    UME::SIMD::SIMDVec_f<float, SIMDSIZE> vec;

    iteration = TESTSIZE / SIMDSIZE;
    remainder = TESTSIZE % SIMDSIZE;

    for (int i = 0; i < iteration; i++){
        offset = i*stride*SIMDSIZE;
        vec.gatheru(&a[offset], stride);
        vec += 3.14;
        vec.scatteru(&a[offset], stride);
    }
    for (int i = 0; i < remainder; i++){
        a[offset + i] += 3.14;
    }
    
    for (int i = 0; i < TESTSIZE; i++){
        printf("%f\n", a[i]);
    }
}

int main(){
    testsimd();
    return 0;
}