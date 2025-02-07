#include "miller_rabin.h"

// Safe modular multiplication: (a * b) % mod without overflow
int64_t mod_mul(int64_t a, int64_t b, int64_t mod) {
    int64_t result = 0;
    while (b > 0) {
        if (b % 2 == 1) // If b is odd, add a to result
            result = (result + a) % mod;
        a = (a * 2) % mod; // Double a and halve b
        b /= 2;
    }
    return result;
}

// Modular exponentiation: (base^exp) % mod using modular multiplication
int64_t mod_pow(int64_t base, int64_t exp, int64_t mod) {
    int64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp /= 2;
    }
    return result;
}

// Miller-Rabin Primality Test
bool is_prime(int64_t number) {
    if (number < 2) return false;
    if (number == 2 || number == 3) return true;
    if (number % 2 == 0) return false;  // Even numbers >2 are not prime

    // Step 1: Express (number - 1) as d * 2^s
    int64_t d = number - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    // Step 2: Perform Miller-Rabin Test with stronger randomness
    std::random_device rd; // Cryptographic-quality randomness
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<int64_t> distribute(2, number - 2);

    int trials = 20;  // More trials for larger numbers
    for (int i = 0; i < trials; i++) {
        int64_t a = distribute(generator);
        int64_t x = mod_pow(a, d, number);

        if (x == 1 || x == number - 1)
            continue;

        bool composite = true;
        for (int j = 0; j < s - 1; j++) {
            x = mod_mul(x, x, number);
            if (x == number - 1) {
                composite = false;
                break;
            }
        }

        if (composite)
            return false;
    }

    return true;  // Probably prime
}