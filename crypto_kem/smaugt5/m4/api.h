#ifndef API_H
#define API_H

#include <stdint.h>
#include <stdio.h>
#include "parameters.h"

#define CRYPTO_SECRETKEYBYTES 176
#define CRYPTO_PUBLICKEYBYTES 672
#define CRYPTO_CIPHERTEXTBYTES 672

#define KPQM4


int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ct, uint8_t *ss,
                                      const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *sk,
                                      const uint8_t *pk, const uint8_t *ctxt);

#endif // KEM_SMAUG_H
