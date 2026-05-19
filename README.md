# Insertion Sort CPU Microarchitecture Study

This project evaluates how different low-level implementation choices affect the performance of insertion sort on modern x86-64 CPUs. It compares plain C, optimized C, hand-written scalar assembly, SIMD-assisted C, and SIMD-assisted assembly implementations using timing tests, branch/cache experiments, and correctness checks.

The main purpose of the project is not to create the fastest general-purpose sorting program. Instead, it uses insertion sort as a controlled algorithm to study CPU microarchitecture concepts such as compiler optimization, branch prediction, cache behavior, scalar assembly, and SIMD/AVX2 vector instructions.

## Project Overview

Insertion sort was chosen because it is simple, easy to verify, and highly sensitive to input ordering. Its inner loop performs comparisons and shifts based on the current array contents, which makes it useful for observing how different implementations behave on sorted, random, and reverse-sorted data.

The project includes five sorting implementations:

| Implementation | Source File | Function | Description |
|---|---|---|---|
| Personal C implementation | `Project.c` | `insertion_sort_personal` | Straightforward insertion sort using repeated swaps. |
| Optimized C implementation | `Project.c` | `insertion_sort_optimized` | More compiler-friendly insertion sort that stores the current value once and shifts larger elements instead of repeatedly swapping. |
| Scalar assembly implementation | `Project.S` | `insertion_sort_asm` | Hand-written x86-64 assembly version using scalar registers, branches, and explicit memory movement. |
| SIMD C implementation | `Project_s.cpp` | `insertion_sort_simd_c` | SIMD-assisted version using the Vector Class Library and `Vec4q` to shift blocks of four 64-bit values when possible. |
| SIMD assembly implementation | `Project.S` | `insertion_sort_simd_asm` | Hand-written AVX2 assembly version that uses 256-bit YMM register moves for block shifting. |

## What the Project Demonstrates

This project is mainly a performance and architecture experiment. It demonstrates that:

- Compiler-optimized C can outperform hand-written assembly when the compiler is able to schedule instructions and use registers efficiently.
- Insertion sort is not naturally SIMD-friendly because finding the insertion position is a serial, data-dependent process.
- SIMD can only help with part of the shifting step, and the overhead can outweigh the benefit.
- Input ordering has a major effect on insertion sort performance.
- Sorted arrays are fast because little or no shifting is needed.
- Reverse-sorted arrays are slow because they require the maximum amount of shifting.
- Cache behavior and memory movement can dominate runtime more than branch mispredictions for this workload.

## Repository Structure

```text
.
├── Project.c
├── Project.h
├── Project.S
├── Project_s.cpp
├── branches_caches_testing.c
├── runtimes_all_testing.c
├── simd_testing.c
├── test_accuracy.c
├── report.pdf
└── README.md
```

### File Descriptions

#### `Project.h`

Header file that declares all sorting functions used by the test programs. It also wraps the declarations in `extern "C"` when compiled with C++ so that the C/C++ and assembly functions can link correctly.

Declared functions:

```cpp
void insertion_sort_personal(uint64_t *arr, uint64_t size);
void insertion_sort_optimized(uint64_t *arr, uint64_t size);
void insertion_sort_asm(uint64_t *arr, uint64_t size);
void insertion_sort_simd_c(uint64_t *arr, uint64_t size);
void insertion_sort_simd_asm(uint64_t *arr, uint64_t size);
```

#### `Project.c`

Contains the two C implementations of insertion sort.

`insertion_sort_personal` is the basic version. It compares adjacent values and swaps them until the current value reaches the correct position.

`insertion_sort_optimized` improves the structure of the algorithm by storing the current value in a local variable, shifting larger elements to the right, and placing the saved value into its final position. This reduces unnecessary swaps and memory writes.

#### `Project.S`

Contains two hand-written x86-64 assembly implementations.

`insertion_sort_asm` is a scalar assembly insertion sort. It uses general-purpose registers and branch instructions to compare and shift elements.

`insertion_sort_simd_asm` is an AVX2-assisted assembly implementation. It uses YMM registers and `vmovdqu` instructions to move blocks of data during the shifting step.

The assembly is written using GNU assembler style AT&T syntax.

#### `Project_s.cpp`

Contains the SIMD C implementation, `insertion_sort_simd_c`.

This file includes:

```cpp
#include "vectorclass.h"
```

That means the project depends on Agner Fog's Vector Class Library or a compatible `vectorclass.h` file. The uploaded repository does not include this dependency, so it must be added before compiling the SIMD C version.

The SIMD C implementation uses `Vec4q`, which represents a vector of four 64-bit integer values. The code uses SIMD only for block shifting. The search for the insertion position is still scalar.

#### `test_accuracy.c`

A correctness test program. It creates a small fixed array and checks whether each sorting implementation produces sorted output.

The tested array is:

```c
{5, 10, 42, 453, 22, 3, 52, 32, 3}
```

It prints `PASS` or `FAIL` for each implementation.

#### `runtimes_all_testing.c`

A timing program that compares all five sorting implementations on the same randomly generated array.

It creates one base random array, copies it for each implementation, and times each sort using `clock_gettime` with `CLOCK_PROCESS_CPUTIME_ID`.

Tested implementations:

- `personal`
- `optimized`
- `asm`
- `simd_c`
- `simd_asm`

#### `branches_caches_testing.c`

A branch and cache behavior test program. It compares the optimized C version and scalar assembly version on three input patterns:

- Random array
- Already sorted array
- Reverse-sorted array

This file uses C++ standard library functions such as `std::sort` and `std::reverse`, so it should be compiled with `g++`, even though the file extension is `.c`.

This program is intended to be used with Linux `perf` to collect statistics such as branch counts, branch misses, cache misses, cycles, and instructions.

#### `simd_testing.c`

A focused timing program for comparing scalar and SIMD implementations. It runs:

- Optimized C
- Scalar assembly
- SIMD C
- SIMD assembly

It uses the same random base array for all implementations so that each sort receives equivalent input.

#### `report.pdf`

A written project report explaining the motivation, methodology, hardware context, experiments, and conclusions.

## Requirements

### Operating System

This project is designed for Linux. The testing programs use POSIX timing functions and the performance analysis commands use Linux `perf`.

### Compiler

Use `g++` to compile the test programs because some `.c` files include or use C++ features.

Recommended compiler setup:

- `g++`
- GNU assembler support for `.S` files
- C++17 support
- AVX2-capable build target

### CPU Support

The SIMD assembly implementation uses AVX2/YMM instructions. The provided compilation commands use:

```bash
-march=haswell
```

This enables AVX2 instructions and is compatible with Haswell-or-newer x86-64 CPUs. Running the AVX2 versions on a CPU that does not support AVX2 may fail.

### External Dependency

The file `Project_s.cpp` requires:

```cpp
vectorclass.h
```

The current uploaded repository does not include this file. The existing compile commands expect it to be inside a directory named:

```text
version2-master
```

For example:

```text
.
├── Project_s.cpp
└── version2-master/
    └── vectorclass.h
```

Without this dependency, commands that compile `Project_s.cpp` will fail with an error similar to:

```text
Project_s.cpp:2:9: fatal error: vectorclass.h: No such file or directory
```

To compile the full project, add the Vector Class Library files and keep the include path `-I./version2-master`, or update the include path to wherever `vectorclass.h` is located.

## Build Instructions

All commands below assume you are in the project root directory.

### 1. Accuracy Test

Compile:

```bash
g++ -std=c++17 -march=haswell -O2 -I./version2-master \
    -o test_accuracy \
    test_accuracy.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
```

Run:

```bash
./test_accuracy
```

Expected style of output:

```text
Testing with array size 9
personal: PASS
optimized: PASS
asm: PASS
simd_c: PASS
simd_asm: PASS
```

The exact output depends on whether all implementations compile and run successfully.

### 2. Runtime Comparison Across Implementations

Compile with different optimization levels:

```bash
g++ -std=c++17 -march=haswell -O1 -I./version2-master \
    -o runtimes_O1 \
    runtimes_all_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++

g++ -std=c++17 -march=haswell -O2 -I./version2-master \
    -o runtimes_O2 \
    runtimes_all_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++

g++ -std=c++17 -march=haswell -O3 -I./version2-master \
    -o runtimes_O3 \
    runtimes_all_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
```

Run:

```bash
./runtimes_O1 <array_size>
./runtimes_O2 <array_size>
./runtimes_O3 <array_size>
```

Example:

```bash
./runtimes_O2 20000
```

This program prints the time taken by each implementation on the same random input array.

### 3. Branch and Cache Testing

Compile:

```bash
g++ -std=c++17 -march=haswell -O3 -I./version2-master \
    -o branches_caches \
    branches_caches_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
```

Run normally:

```bash
./branches_caches <array_size>
```

Example:

```bash
./branches_caches 20000
```

Run with `perf stat`:

```bash
perf stat -e branches,branch-misses,L1-dcache-load-misses,cycles,instructions \
    ./branches_caches 20000
```

For more detailed profiling:

```bash
perf record -e branch-misses,branches ./branches_caches 20000
perf report
perf annotate
```

For cache-focused profiling:

```bash
perf record -e L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads \
    ./branches_caches 20000
perf report
perf annotate
```

Note: Some systems require elevated permissions or adjusted kernel settings to use certain `perf` events.

### 4. SIMD-Focused Testing

Compile:

```bash
g++ -std=c++17 -march=haswell -O3 -I./version2-master \
    -o simd_O3 \
    simd_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
```

Run:

```bash
./simd_O3 <array_size>
```

Example:

```bash
./simd_O3 20000
```

This test compares optimized scalar C, scalar assembly, SIMD C, and SIMD assembly on equivalent random input.

## How the Sorting Functions Work

### Basic Insertion Sort

The basic version repeatedly swaps the current element leftward until it reaches the correct position.

Simplified idea:

```text
for each index i from 1 to size - 1:
    move arr[i] left while it is smaller than the previous element
```

This is simple, but repeated swapping causes extra loads and stores.

### Optimized Insertion Sort

The optimized C version saves the current element in a variable called `value`, shifts larger elements one position to the right, and then writes `value` once into the correct position.

Simplified idea:

```text
for each index i from 1 to size - 1:
    value = arr[i]
    shift larger previous elements right
    place value into the empty position
```

This reduces memory writes compared with repeated swapping.

### Scalar Assembly Version

The scalar assembly version follows the same general insertion-sort logic as the optimized C version, but the loop and memory operations are written manually in x86-64 assembly.

This allows direct control over registers and branches, but it also means the compiler cannot reorganize the assembly as freely as it can optimize C code.

### SIMD Versions

The SIMD versions try to speed up the shifting stage by moving multiple 64-bit values at once.

However, insertion sort still has a serial search step because each element must be compared against previous elements to find its insertion position. Because of that, the algorithm cannot fully benefit from SIMD parallelism.

## Important Notes About Correctness

The test programs check whether the final array is sorted in nondecreasing order.

The correctness check verifies this condition:

```text
arr[i] <= arr[i + 1]
```

The tests do not check algorithm stability. Since the input values are plain integers and the program only checks sorted order, the project focuses on numerical correctness and performance rather than stable sorting behavior.

## Known Limitations

- The repository does not include `vectorclass.h`, which is required for `Project_s.cpp`.
- The project is Linux-focused because it uses POSIX timing and Linux `perf` commands.
- The SIMD implementations require AVX2 support.
- Insertion sort is O(n²), so large arrays may take a long time.
- The timing programs use `rand()` with `srand(0)` for repeatable random input, not cryptographic or high-quality randomness.
- `clock_gettime(CLOCK_PROCESS_CPUTIME_ID)` measures CPU time used by the process, not wall-clock time.
- The assembly code is architecture-specific and is not portable to non-x86-64 systems.

## Suggested Workflow

A typical workflow for using this project is:

1. Add the missing Vector Class Library dependency so `vectorclass.h` is available.
2. Compile and run `test_accuracy` to verify that all implementations sort correctly.
3. Run `runtimes_all_testing.c` at `-O1`, `-O2`, and `-O3` to compare optimization levels.
4. Run `simd_testing.c` to compare scalar and SIMD versions.
5. Run `branches_caches_testing.c` with `perf` to study branch and cache behavior.
6. Compare results with the explanations in `report.pdf`.

## Example Full Command Sequence

```bash
# 1. Check correctness
g++ -std=c++17 -march=haswell -O2 -I./version2-master \
    -o test_accuracy \
    test_accuracy.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
./test_accuracy

# 2. Compare runtime at O2
g++ -std=c++17 -march=haswell -O2 -I./version2-master \
    -o runtimes_O2 \
    runtimes_all_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
./runtimes_O2 20000

# 3. Compare SIMD and scalar versions
g++ -std=c++17 -march=haswell -O3 -I./version2-master \
    -o simd_O3 \
    simd_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
./simd_O3 20000

# 4. Collect branch/cache stats
g++ -std=c++17 -march=haswell -O3 -I./version2-master \
    -o branches_caches \
    branches_caches_testing.c Project.c Project_s.cpp Project.S \
    -lm -lstdc++
perf stat -e branches,branch-misses,L1-dcache-load-misses,cycles,instructions \
    ./branches_caches 20000
```

## Conclusion

This project shows that low-level performance depends heavily on the relationship between the algorithm and the CPU architecture. For insertion sort, the optimized scalar C version can be highly competitive because the compiler can reduce redundant memory operations and schedule instructions effectively. The SIMD versions are useful for experimentation, but insertion sort does not map naturally to SIMD because its insertion-position search is sequential and data-dependent.

The main lesson from this project is that manual assembly and SIMD are not automatically faster. A well-structured algorithm and compiler-friendly C code can often perform better than manually optimized low-level code, especially when the algorithm itself limits parallelism.
