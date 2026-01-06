#define _POSIX_C_SOURCE 199309L
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Project.h"


#define TIMING_RESULT(DESCR, CODE) do { \
    struct timespec start, end; \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); \
    CODE; \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); \
    double elapsed = (end.tv_sec - start.tv_sec) + \
                     (end.tv_nsec - start.tv_nsec) / 1e9; \
    printf("%25s took %7.2f ms\n", DESCR, elapsed * 1000); \
} while(0)

uint64_t* create_random_array(uint64_t n) {
    uint64_t* arr = (uint64_t*)malloc(n * sizeof(uint64_t));
    for (uint64_t i = 0; i < n; i++)
        arr[i] = rand();
    return arr;
}
// clone array for each test so timings are fair
uint64_t* clone_array(uint64_t* src, uint64_t n) {
    uint64_t* out = (uint64_t*)malloc(n * sizeof(uint64_t));
    memcpy(out, src, n * sizeof(uint64_t));
    return out;
}

void sort_speed(const char* descr,
                void sort_fn(uint64_t*, uint64_t),
                uint64_t* base, uint64_t n)
{
    uint64_t* arr = clone_array(base, n);

    TIMING_RESULT(descr,
        sort_fn(arr, n)
    );

    free(arr);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: ./a.out <size>\n");
        return 1;
    }

    uint64_t n = atol(argv[1]);
    srand(0);
    uint64_t* base = create_random_array(n);
    assert(base != NULL);
    // Warmup (just like Lab 11)
    sort_speed("warmup", insertion_sort_optimized, base, n);
    sort_speed("warmup", insertion_sort_optimized, base, n);
    printf("\n======= SIMD vs SCALAR (Lab 11 Style) =======\n");
    sort_speed("optimized_C",    insertion_sort_optimized, base, n);
    sort_speed("asm_scalar",     insertion_sort_asm,       base, n);
    sort_speed("simd_C",         insertion_sort_simd_c,    base, n);
    sort_speed("simd_asm",       insertion_sort_simd_asm,  base, n);
    free(base);
    return 0;
}
