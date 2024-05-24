#ifndef SMAUG_HWT_H
#define SMAUG_HWT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fips202.h"
#include "parameters.h"

#ifndef SMAUGT_HASHDEF
#define SMAUGT_HASHDEF
typedef shake256incctx keccak_state;
#define shake256_init shake256_inc_init
#define shake256_absorb shake256_inc_absorb
#define shake256_finalize shake256_inc_finalize
#define shake128_absorb_once shake128_absorb_once_kpqc 
#define shake256_absorb_once shake256_absorb_once_kpqc 
#endif



#if SMAUG_MODE == 1
#define HS_XOF 5
#define HR_XOF 5

#elif SMAUG_MODE == 3
#define HS_XOF 7
#define HR_XOF 7

#elif SMAUG_MODE == 5
#define HS_XOF 7
#define HR_XOF 7
#endif

#define hwt SMAUG_NAMESPACE(hwt)
void hwt(uint8_t *res, uint8_t *cnt_arr, const uint8_t *input,
         const size_t input_size, const uint16_t hmwt);

#endif // SMAUG_HWT_H