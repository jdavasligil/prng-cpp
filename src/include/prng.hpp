#ifndef PRNG_HPP
#define PRNG_HPP

#include <cstdint>

/**
 * @brief Class for generating seeded pseudo random numbers.
 * @author Jaedin Davasligil
 */
class PRNG {
    static constexpr uint64_t JUMP[] = {
        0X180EC6D33CFD0ABA,
        0XD5A61266F0C9392C,
        0XA9582618E03FC9AA,
        0X39ABDC4529B1661C
    };
    static constexpr uint64_t LONG_JUMP[] = {
        0x76e15d3efefdcbbf,
        0xc5004e441c522fb3,
        0x77710069854ee241,
        0x39109bb02acbe635
    };
private:
    uint64_t state[4];

public:
    // Construct a pseudo random number generator with the given seed.
    PRNG(uint64_t seed);

    // Return the next 64 bit unsigned int.
    uint64_t next();

    // Jump to the next subsequence. Equivalent to 2^128 calls to next().
    void jump();

    // Jump to the next subsequence. Equivalent to 2^192 calls to next().
    void long_jump();

    // Return the next 32 bit float in the range [0, 1).
    float next_f32();

    // Return the next 64 bit double in the range [0, 1).
    double next_f64();

    // Return the next int in the range [a, b).
    int next_int(int a, int b);
};

#endif// PRNG_HPP
