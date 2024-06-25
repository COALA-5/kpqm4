#include "hash.h"

void shake256_absorb_twice_squeeze(uint8_t *out, unsigned long long out_bytes,
                                   const uint8_t *in1, unsigned long long in1_bytes,
                                   const uint8_t *in2, unsigned long long in2_bytes) {
    shake256incctx state;
    shake256_init(&state);
    shake256_absorb(&state, in1, in1_bytes);
    shake256_absorb(&state, in2, in2_bytes);
    shake256_finalize(&state);
    shake256_inc_squeeze(out, out_bytes, &state);
}