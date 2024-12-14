#pragma once

#include <immintrin.h>
#include <cstdint>
#include <limits>

namespace llama {

// Random number engine using RDRAND instruction
class rdrand_engine {
public:
    using result_type = uint64_t;
    
    // Constructor - doesn't need seed but keeps interface compatible 
    explicit rdrand_engine(result_type seed = 0) {
        (void)seed; // Unused parameter
    }

    // Generate random number using RDRAND
    result_type operator()() {
        result_type result;
        while (_rdrand64_step(&result) == 0) {
            // Retry if RDRAND fails
            // In practice this is extremely rare
        }
        return result;
    }

    // Required methods for random engine concept
    static constexpr result_type min() { 
        return std::numeric_limits<result_type>::min(); 
    }
    
    static constexpr result_type max() { 
        return std::numeric_limits<result_type>::max(); 
    }

    // Discard n random numbers - required for random engine concept
    void discard(unsigned long long n) {
        result_type dummy;
        for (unsigned long long i = 0; i < n; ++i) {
            while (_rdrand64_step(&dummy) == 0) {
                // Retry on failure
            }
        }
    }

    // Equality operators - RDRAND has no state so all instances are equal
    bool operator==(const rdrand_engine&) const { return true; }
    bool operator!=(const rdrand_engine&) const { return false; }

    // Seeding is no-op for RDRAND but provided for compatibility
    void seed(result_type = 0) {}
};

} // namespace llama