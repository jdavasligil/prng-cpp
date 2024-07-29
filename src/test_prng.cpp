#include <cstdlib>
#include <iostream>

#include "include/prng.hpp"

static inline int roll(PRNG& rng, int n) {
    return rng.next_int(1, n+1);
}

int main() {
    PRNG rng = PRNG(0);

    for (int i = 0; i < 10; i++) {
        std::cout << rng.next_f32() << "\n";
    }
 
    return 0;
}
