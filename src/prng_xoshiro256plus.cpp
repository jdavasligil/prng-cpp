/******************************************************************************
 * @file prng_xoshiro256plus.cpp
 *
 * @brief A blazingly fast 64 bit seeded random number generator.
 *
 * @details
 * xoshiro256+ (David Blackman and Sebastiano Vigna, 2018, vigna@acm.org) is a
 * public domain algorithm for quickly generating random floating point numbers.
 *
 * This generator implements the PRNG interface.
 *
 * @see https://prng.di.unimi.it/ 
 ******************************************************************************/

#include "include/prng.hpp"
#include <cstdint>


static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static inline double to_double(uint64_t x) {
    const union { uint64_t i; double d; } u = {.i = UINT64_C(0x3FF) << 52 | x >> 12 };
    return u.d - 1.0;
}

static inline float to_float(uint64_t x) {
    const union { uint64_t i; float f; } u = {.i = UINT32_C(0x7F) << 23 | ((uint32_t)x) >> 9 };
    return u.f - 1.0;
}

// This conversion technique is slightly slower but yields a longer period.

//static inline double to_double(uint64_t x) {
//    return (x >> 11) * 0x1.0p-53;
//}
//
//static inline float to_float(uint64_t x) {
//    return (x >> 40) * 0x1.0p-24;
//}


/**
 * Constructor for Pseudo Random Number Generator (PRNG).
 *
 * Uses splitmix64 with a given seed to perform initialization.
 *
 * @param seed determines the initial state of the generator. Useful for
 *        deterministic procedural generation or testing.
 */ 
PRNG::PRNG(uint64_t seed) : state{} {
    // splitmix64 initialization
    seed += 0x9E3779B97f4A7C15;
    for (int i = 0; i < 4; ++i) {
        seed = (seed ^ (seed >> 30)) * 0xBF58476D1CE4E5B9;
        seed = (seed ^ (seed >> 27)) * 0x94D049BB133111EB;
        state[i] = seed ^ (seed >> 31);
    }
}

/**
 * Perform state transformation and return the next 64 bit unsigned int.
 *
 * @return random 64 bit unsigned int
 */ 
uint64_t PRNG::next() {
    const uint64_t result = state[0] + state[3];
    const uint64_t t = state[1] << 17;

    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];

    state[2] ^= t;

    state[3] = rotl(state[3], 45);

    return result;
}

/**
 * Jump to the next subsequence. Equivalent to 2^128 calls to next().
 *
 * It can be used to generate 2^128 non-overlapping subsequences for parallel
 * computations.
 */ 
void PRNG::jump() {
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    for(int i = 0; i < 4; i++)
        for(int b = 0; b < 64; b++) {
            if (JUMP[i] & UINT64_C(1) << b) {
                s0 ^= state[0];
                s1 ^= state[1];
                s2 ^= state[2];
                s3 ^= state[3];
            }
            next();	
        }

    state[0] = s0;
    state[1] = s1;
    state[2] = s2;
    state[3] = s3;
}

/**
 * Jump to the next subsequence. Equivalent to 2^192 calls to next().
 *
 * It can be used to generate 2^64 starting points, from each of which
 * long_jump() will generate 2^64 non-overlapping subsequences for parallel
 * distributed computations.
 */ 
void PRNG::long_jump() {
    uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(int i = 0; i < 4; i++)
		for(int b = 0; b < 64; b++) {
			if (LONG_JUMP[i] & UINT64_C(1) << b) {
				s0 ^= state[0];
				s1 ^= state[1];
				s2 ^= state[2];
				s3 ^= state[3];
			}
            next();	
		}
		
	state[0] = s0;
	state[1] = s1;
	state[2] = s2;
	state[3] = s3;
}

/**
 * Get the next random number as a 32 bit floating point number.
 *
 * @return unform random float in [0, 1)
 */ 
float PRNG::next_f32() {
    return to_float(next());
}

/**
 * Get the next random number as a 64 bit floating point number.
 *
 * @return unform random float in [0, 1)
 */ 
double PRNG::next_f64() {
    return to_double(next());
}

/**
 * Get the next random int in the interval [a, b).
 *
 * @return unform random int in [a, b)
 */ 
int PRNG::next_int(int a, int b) {
    return a + (b - a) * next_f64();
}
