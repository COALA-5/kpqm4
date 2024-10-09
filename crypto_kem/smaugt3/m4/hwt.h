#ifndef SMAUG_HWT_H
#define SMAUG_HWT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fips202.h"
#include "parameters.h"

// https://github.com/dgazzoni/NTRU-sampling/blob/main/jupyter/NTRU-sampling.ipynb
// Use enough seed size 308
#define HWTSEEDBYTES ((16 * 308) / 8)

#define shake256_init shake256_inc_init
#define shake256_absorb shake256_inc_absorb
#define shake256_finalize shake256_inc_finalize
#define shake128_absorb_once shake128_absorb_once_kpqc 
#define shake256_absorb_once shake256_absorb_once_kpqc 
#define hwt SMAUG_NAMESPACE(hwt)
int hwt(int16_t *res, const uint8_t *seed);

#endif // SMAUG_HWT_H
