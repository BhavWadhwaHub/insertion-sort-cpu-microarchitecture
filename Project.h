#ifndef PROJECT_H
#define PROJECT_H
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------
// C Implementations
//-------------------------------------------------------

void insertion_sort_personal(uint64_t *arr,uint64_t size);

     //a normal implementation of insertion sort

void insertion_sort_optimized(uint64_t *arr,uint64_t size);

     //a more optimized and compiler friendly version of insertion sort

//-------------------------------------------------------
//Assembly Implementations
//-------------------------------------------------------

void insertion_sort_asm(uint64_t *arr,uint64_t size);

     // assembly implemenation of insertion sort with branches

//-------------------------------------------------------
//SIMD Implementations
//-------------------------------------------------------

void insertion_sort_simd_c(uint64_t *arr,uint64_t size);

     //assisted insertion sort using SIMD instructions
void insertion_sort_simd_asm(uint64_t *arr,uint64_t size);

    //using 8 bit integers to store the difference between the current and the next element

#ifdef __cplusplus
}
#endif

#endif