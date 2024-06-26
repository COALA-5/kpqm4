#ifndef SMAUG_HASH_H
#define SMAUG_HASH_H

#include "fips202.h"
#include "parameters.h"

#define SMAUGT_HASHDEF
typedef shake256incctx keccak_state;
#define shake256_init shake256_inc_init
#define shake256_absorb shake256_inc_absorb
#define shake256_finalize shake256_inc_finalize

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, OUTBYTES, IN1, IN1BYTES, IN2, IN2BYTES)                    \
    shake256_absorb_twice_squeeze(OUT, OUTBYTES, IN1, IN1BYTES, IN2, IN2BYTES)

void shake256_absorb_twice_squeeze(uint8_t *out, unsigned long long out_bytes,
                                   const uint8_t *in1, unsigned long long in1_bytes,
                                   const uint8_t *in2, unsigned long long in2_bytes);




#endif // SMAUG_HASH_H
