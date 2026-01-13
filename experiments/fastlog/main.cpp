#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>
#include <random>

// Function 1: Float hack
inline size_t fastlog2_float(const size_t& in) {
    float g = static_cast<float>(in);
    uint32_t& t = reinterpret_cast<uint32_t&>(g);
    return (t >> 23) - 127;
}

// Function 2: Builtin CLZ
inline size_t fastlog2_builtin(const size_t& value) {
    return 63 - __builtin_clzll(value);
}

int main() {
    const size_t ITERATIONS = 1e10;
    
    // Setup inputs (prevent branch prediction optimization with some randomness, 
    // but keep it simple to measure the function itself)
    // We'll just loop a counter to avoid memory access latency dominating the test
    
    std::cout << "Benchmarking " << ITERATIONS << " iterations..." << std::endl;

    // --- Benchmark Float Hack ---
    {
        size_t accumulator = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 1; i <= ITERATIONS; ++i) {
            // if (fastlog2_float(i) != fastlog2_builtin(i) or i <= 10) {
            //     std::cout << "Error: fastlog2_float(" << i << ") = " << fastlog2_float(i) << " != fastlog2_builtin(" << i << ") = " << fastlog2_builtin(i) << std::endl;
            // }
            accumulator += fastlog2_float(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        
        std::cout << "Float hack time: " << diff.count() << " s" << std::endl;
        std::cout << "Result check: " << accumulator << std::endl; // Print to prevent DCE
    }

    // --- Benchmark Builtin ---
    {
        size_t accumulator = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 1; i <= ITERATIONS; ++i) {
            accumulator += fastlog2_builtin(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        
        std::cout << "Builtin time:   " << diff.count() << " s" << std::endl;
        std::cout << "Result check: " << accumulator << std::endl;
    }

    return 0;
}

