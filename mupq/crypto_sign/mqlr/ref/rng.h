///
///  @file rng.h
///  @brief An AES256 CTR DRBG
///
///  Created by Bassham, Lawrence E (Fed) on 8/29/17.
///  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
///
///  An AES256 CTR DRBG
///

#ifndef rng_h
#define rng_h

#include <stdio.h>
#include "nistseedexpander.h"




typedef struct {
    unsigned char   Key[32];
    unsigned char   V[16];
    int             reseed_counter;
} AES256_CTR_DRBG_struct;


void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *V);

void
handleErrors(void);

void
randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength);

#if defined(_SUPERCOP_)
#include "randombytes.h"
#else
int
randombytes(unsigned char *x, unsigned long long xlen);
#endif




void
randombytes_init_with_state( AES256_CTR_DRBG_struct * states, unsigned char *entropy_input_48bytes );

int
randombytes_with_state( AES256_CTR_DRBG_struct * states, unsigned char *x, unsigned long long xlen);


#endif /* rng_h */
