#include <stdio.h>
#include "umesimd/UMESimd.h"

#define TESTSIZE 100
#define SIMDSIZE 32

void evDSP_sign(float* __A, int* __IA, float* __B, float* __C, float* __D, int* __ID, int* __N){
    int iteration, remainder, src_offset=0, dst_offset=0;
    UME::SIMD::SIMDVec_f<float, SIMDSIZE> vec;
    UME::SIMD::SIMDVec_f<float, SIMDSIZE> zeros=0.0f;
    
    UME::SIMD::SIMDVecMask<SIMDSIZE> mask;

    iteration = *__N / SIMDSIZE;
    remainder = *__N % SIMDSIZE;

    // iteration
    for (int i = 0; i < iteration; i++){
        vec.gatheru(__A + src_offset, *__IA);
        // *__A > 0 => *__D = *__B
        mask = vec > zeros;
        vec = vec.blend(mask, *__B);
        // *__A < 0 => *__D = *__C
        mask = vec < zeros;
        vec = vec.blend(mask, *__C);
        vec.scatteru(__D + dst_offset, *__ID);

        // update offset
        src_offset += *__IA*SIMDSIZE;
        dst_offset += *__ID*SIMDSIZE;
    }

    // remainder
    __A += src_offset;
    __D += dst_offset;
    for (int i = 0; i < remainder; i++){
        if (*__A > 0){
            *__D = *__B;
        }
        else if (*__A < 0){
            *__D = *__C;
        }
        else{
            *__D = 0;
        }
        __A += *__IA;
        __D += *__ID;
    }
}

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
    float a[TESTSIZE] = { 0.0f };
    a[0] = 2.5f;
    a[4] = -3.2f;
    a[8] = -1.3f;

    float one = 1.0f, none = -1.0f;
    int stride = 4;
    int size = TESTSIZE;
    evDSP_sign(a, &stride, &one, &none, a, &stride, &size);
    for (int i = 0; i < TESTSIZE; i++){
        printf("%f\n", a[i]);
    }
    return 0;
}