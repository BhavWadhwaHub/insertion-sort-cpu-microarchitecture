# Project README

## Compilation Flags

All programs should be compiled with:
- `-std=c++17`
- `-march=haswell`
- Optimization level: `-O1`, `-O2`, or `-O3`
- Include path: `-I./version2-master`
- Link libraries: `-lm -lstdc++`

## 1. Timing Comparison

To compare performance across different optimization levels, use `runtimes_all_testing.c`:

### Compilation:
```bash
g++ -std=c++17 -march=haswell -O1 -I./version2-master -o runtimes_O1 runtimes_all_testing.c Project.c Project_s.cpp Project.S -lm -lstdc++
g++ -std=c++17 -march=haswell -O2 -I./version2-master -o runtimes_O2 runtimes_all_testing.c Project.c Project_s.cpp Project.S -lm -lstdc++
g++ -std=c++17 -march=haswell -O3 -I./version2-master -o runtimes_O3 runtimes_all_testing.c Project.c Project_s.cpp Project.S -lm -lstdc++
```

### Execution:
```bash
./runtimes_O1 <array_size>
./runtimes_O2 <array_size>
./runtimes_O3 <array_size>
```

Example:
```bash
./runtimes_O1 20000
./runtimes_O2 20000
./runtimes_O3 20000
```

## 2. Branch Mispredictions and Cache Analysis

To analyze branch predictions and cache behavior, use `branches_caches_testing.c`:

### Compilation:
```bash
g++ -std=c++17 -march=haswell -O3 -I./version2-master -o branches_caches branches_caches_testing.c Project.c Project_s.cpp Project.S -lm -lstdc++
```

### Execution with perf:
```bash
perf stat -e branches,branch-misses,L1-dcache-load-misses,cycles,instructions ./branches_caches <array_size>
```

Example with different array sizes:
```bash
perf stat -e branches,branch-misses,L1-dcache-load-misses,cycles,instructions ./branches_caches 20000
```

### Detailed branch analysis with perf record:
```bash
perf record -e branch-misses,branches ./branches_caches 20000
perf report
perf annotate

perf record -e L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./branches_caches 20000
perf report
perf annotate
```

## 3. SIMD Performance Testing

To test SIMD implementations, use `simd_testing.c`:

### Compilation:
```bash
g++ -std=c++17 -march=haswell -O3 -I./version2-master -o simd_O3 simd_testing.c Project.c Project_s.cpp Project.S -lm -lstdc++
```

### Execution:
```bash
./simd_O3 <array_size>
```

Example:
```bash
./simd_O3 20000
```
## 4. Accuracy Testing

To verify all sorting functions work correctly, use `test_accuracy.c`:

### Compilation:
```bash
g++ -std=c++17 -march=haswell -O2 -I./version2-master -o test_accuracy test_accuracy.c Project.c Project_s.cpp Project.S -lm -lstdc++
```
### Execution:
```bash
./test_accuracy
```

