#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>
#include <random>

// Function 1: Float hack
inline size_t fastlog2_float(float g) {
    //float g = static_cast<float>(in);
    uint32_t& t = reinterpret_cast<uint32_t&>(g);
    return (t >> 23) - 127;
}

// Function 2: Builtin CLZ
inline size_t fastlog2_builtin(const size_t& value) {
    return 63 - __builtin_clzll(value);
}

// Function 3: De Bruijn sequence multiplication
constexpr uint64_t tab64[64] = {
      63,  0, 58,  1, 59, 47, 53,  2,
      60, 39, 48, 27, 54, 33, 42,  3,
      61, 51, 37, 40, 49, 18, 28, 20,
      55, 30, 34, 11, 43, 14, 22,  4,
      62, 57, 46, 52, 38, 26, 32, 41,
      50, 36, 17, 19, 29, 10, 13, 21,
      56, 45, 25, 31, 35, 16,  9, 12,
      44, 24, 15,  8, 23,  7,  6,  5
};

constexpr uint64_t specLog2(uint64_t value) {
    return tab64[(value * 0x07EDD5E59A4E28C2ULL) >> 58];
}

int main() {
    const size_t ITERATIONS = 1e9;
    
    // Create vector of powers of 2
    std::vector<size_t> powers;
    for (size_t i = 0; i < 64; ++i) {
        powers.push_back(1ULL << i);
    }

    // Pre-generate random indices to access powers
    const size_t MASK_SIZE = 1024 * 1024;
    std::vector<size_t> indices(MASK_SIZE);
    std::mt19937_64 rng(42);
    for (auto& idx : indices) {
        idx = rng() % 64; // Indices 0..63
    }
    
    std::cout << "Benchmarking " << ITERATIONS << " iterations on powers of 2..." << std::endl;

    // --- Benchmark Float Hack ---
    {
        size_t accumulator = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < ITERATIONS; ++i) {
            size_t val = powers[indices[i & (MASK_SIZE - 1)]]; 
            accumulator += fastlog2_float(val);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        
        std::cout << "Float hack time: " << diff.count() << " s" << std::endl;
        std::cout << "Result check: " << accumulator << std::endl;
    }

    // --- Benchmark Builtin ---
    {
        size_t accumulator = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < ITERATIONS; ++i) {
            size_t val = powers[indices[i & (MASK_SIZE - 1)]];
            accumulator += fastlog2_builtin(val);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        
        std::cout << "Builtin time:   " << diff.count() << " s" << std::endl;
        std::cout << "Result check: " << accumulator << std::endl;
    }

    // --- Benchmark De Bruijn ---
    {
        size_t accumulator = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < ITERATIONS; ++i) {
            size_t val = powers[indices[i & (MASK_SIZE - 1)]];
            accumulator += specLog2(val);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        
        std::cout << "De Bruijn time: " << diff.count() << " s" << std::endl;
        std::cout << "Result check: " << accumulator << std::endl;
    }

    return 0;
}
