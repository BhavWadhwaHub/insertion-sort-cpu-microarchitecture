#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cassert>

#include "Project.h"

#define DATA_T uint64_t
#define RAND_EXPR (rand())

enum array_ordering { RANDOM, SORTED, REVERSE_SORTED };

DATA_T* create_array(uint64_t n, array_ordering ord)
{
    DATA_T* a = (DATA_T*)malloc(n * sizeof(DATA_T));
    for (uint64_t i = 0; i < n; i++) a[i] = RAND_EXPR;

    if (ord == SORTED)
        std::sort(a, a+n);
    else if (ord == REVERSE_SORTED) {
        std::sort(a, a+n);
        std::reverse(a, a+n);
    }
    return a;
}

bool is_sorted(DATA_T* a, uint64_t n) {
    for (uint64_t i = 0; i < n-1; i++)
        if (a[i] > a[i+1]) return false;
    return true;
}

void time_sort(const char* descr,
               void(*sort)(DATA_T*, uint64_t),
               uint64_t length,
               array_ordering order)
{
    struct timespec start, end;
    DATA_T* array = create_array(length, order);

    if (array == NULL) {
        printf("Couldn't allocate.\n");
        return;
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    sort(array, length);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    assert(is_sorted(array, length));
    free(array);

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%25s sorted %8lu values in %7.2f ms\n", descr, length, elapsed * 1000);
}

void just_sort(void(*sort)(DATA_T*, uint64_t), uint64_t length, array_ordering order)
{
    DATA_T* array = create_array(length, order);

    if (array == NULL) {
        printf("Couldn't allocate.\n");
        return;
    }

    sort(array, length);
    assert(is_sorted(array, length));
    free(array);
}

void time_them(uint64_t length)
{
    // Test optimized C version
    time_sort("optimized/random", insertion_sort_optimized, length, RANDOM);
    time_sort("optimized/sorted", insertion_sort_optimized, length, SORTED);
    time_sort("optimized/reverse", insertion_sort_optimized, length, REVERSE_SORTED);

    // Test assembly with branches
    time_sort("asm/random", insertion_sort_asm, length, RANDOM);
    time_sort("asm/sorted", insertion_sort_asm, length, SORTED);
    time_sort("asm/reverse", insertion_sort_asm, length, REVERSE_SORTED);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Must give array size on command line.\n");
        return 1;
    }

    uint64_t length = atol(argv[1]);
    printf("Array size: %lu kB\n", length * sizeof(DATA_T) / 1024);

    srand(0);

    printf("Warming up CPU and branch predictor...\n");
    
    // Warmup runs for all patterns (3 iterations each)
    for (int i = 0; i < 3; i++) {
        just_sort(insertion_sort_optimized, length, RANDOM);
        just_sort(insertion_sort_optimized, length, SORTED);
        just_sort(insertion_sort_optimized, length, REVERSE_SORTED);
        just_sort(insertion_sort_asm, length, RANDOM);
        just_sort(insertion_sort_asm, length, SORTED);
        just_sort(insertion_sort_asm, length, REVERSE_SORTED);
    }

    printf("\n===== INSERTION SORT TIMINGS =====\n");

    // The real test...
    time_them(length);

    return 0;
}
