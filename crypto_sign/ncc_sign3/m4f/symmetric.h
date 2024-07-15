#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <stdint.h>
#include "params.h"
#include "fips202.h"

typedef shake128incctx stream128_state;
typedef shake256incctx stream256_state;

#define shake256_inc_squeezeblocks(OUT, OUTBLOCKS, STATE) \
        shake256_inc_squeeze(OUT, OUTBLOCKS*SHAKE256_RATE, STATE)

void NIMS_shake128_stream_init(stream128_state *state, const uint8_t seed[SEEDBYTES], uint16_t nonce);
void NIMS_shake256_stream_init(stream256_state *state, const uint8_t seed[CRHBYTES], uint16_t nonce);

#define STREAM128_BLOCKBYTES SHAKE128_RATE
#define STREAM256_BLOCKBYTES SHAKE256_RATE

#define stream128_init(STATE, SEED, NONCE) \
        NIMS_shake128_stream_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE) \
        shake128_inc_squeeze(OUT, OUTBLOCKS*SHAKE128_RATE, STATE)
#define stream256_init(STATE, SEED, NONCE) \
        NIMS_shake256_stream_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE) \
        shake256_inc_squeeze(OUT, OUTBLOCKS*SHAKE256_RATE, STATE)

#endif