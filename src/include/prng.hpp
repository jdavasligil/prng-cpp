#ifndef PRNG_HPP
#define PRNG_HPP

#include <cstdint>


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
    PRNG(uint64_t seed);

    uint64_t next();
    void jump();
    void long_jump();

    float next_f32();
    double next_f64();
    int next_int(int a, int b);
};

#endif// PRNG_HPP
