#include <iostream>
#include <cstdint>
#include <cmath>
#include <iomanip>

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
    std::cout << "Checking powers of 2..." << std::endl;
    std::cout << std::left << std::setw(10) << "Power" 
              << std::setw(20) << "Value" 
              << std::setw(10) << "Float" 
              << std::setw(10) << "Builtin" 
              << std::setw(10) << "DeBruijn"
              << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    int errors = 0;
    for (size_t p = 0; p < 64; ++p) {
        size_t val = 1ULL << p;
        
        size_t res_float = fastlog2_float(val);
        size_t res_builtin = fastlog2_builtin(val);
        size_t res_debruijn = specLog2(val);
        
        bool ok = (res_float == p) && (res_builtin == p) && (res_debruijn == p);
        if (!ok) errors++;

        std::cout << std::left << std::setw(10) << p 
                  << std::setw(20) << val 
                  << std::setw(10) << res_float 
                  << std::setw(10) << res_builtin 
                  << std::setw(10) << res_debruijn
                  << std::setw(10) << (ok ? "OK" : "FAIL") << std::endl;
    }

    if (errors == 0) {
        std::cout << "\nAll powers of 2 checks passed!" << std::endl;
    } else {
        std::cout << "\nFound " << errors << " errors." << std::endl;
    }

    return 0;
}
