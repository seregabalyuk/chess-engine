# Logarithm Benchmark

This directory contains experiments to compare the performance of different integer base-2 logarithm implementations for 64-bit integers.

## Implementations Compared

1.  **Float Hack**: Uses `float` reinterpretation to extract the exponent.
2.  **Builtin CLZ**: Uses `__builtin_clzll` (hardware instruction) - `63 - clz`.
3.  **De Bruijn**: Uses a magic number multiplication and lookup table (De Bruijn sequence).

## Scripts

### 1. `check_powers`
Verifies that all implementations return the correct exponent for all powers of 2 ($2^0$ to $2^{63}$).

```bash
make check_powers
./check_powers
```

### 2. `benchmark_powers`
Benchmarks the implementations specifically on input values that are powers of 2 (simulating bitboard lookups). It uses a pre-computed array of powers and accesses them using a randomized index mask to simulate realistic usage patterns while minimizing overhead.

```bash
make benchmark_powers
./benchmark_powers
```

### 3. `benchmark`
A general benchmark that runs on sequential numbers $1 \dots 10^{10}$.

```bash
make benchmark
./benchmark
```

## How to Build All

```bash
make
```

## Cleaning

```bash
make clean
```
