#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "Project.h"

static void fill_random(uint64_t *arr, uint64_t n) {
    for (uint64_t i = 0; i < n; i++)
        arr[i] = rand();
}

static uint64_t* make_copy(uint64_t* src, uint64_t n) {
    uint64_t* dst = (uint64_t*)malloc(n * sizeof(uint64_t));
    memcpy(dst, src, n * sizeof(uint64_t));
    return dst;
}

static void time_run(const char* label, void (*sortfn)(uint64_t*, uint64_t),
                     uint64_t *original, uint64_t n)
{
    uint64_t* arr = make_copy(original, n);
    struct timespec start, end;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    sortfn(arr, n);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%20s took %8.3f ms\n", label, elapsed * 1000);
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

    // Allocate ONE random array for all 6 implementations
    uint64_t* base = (uint64_t*)malloc(n * sizeof(uint64_t));
    fill_random(base, n);

    // 3 warmup runs (optimized C only)
    for (int i = 0; i < 3; i++) {
        uint64_t* warm = make_copy(base, n);
        insertion_sort_optimized(warm, n);
        free(warm);
    }

    printf("\n=== SAME RANDOM ARRAY TIMINGS ===\n");

    time_run("personal",       insertion_sort_personal, base, n);
    time_run("optimized",      insertion_sort_optimized, base, n);
    time_run("asm",            insertion_sort_asm, base, n);
    time_run("simd_c",         insertion_sort_simd_c, base, n);
    time_run("simd_asm",       insertion_sort_simd_asm, base, n);

    free(base);
    return 0;
}
