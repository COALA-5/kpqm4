#ifndef SMAUG_KEM_H
#define SMAUG_KEM_H

#include "ciphertext.h"
#include "hash.h"
#include "indcpa.h"
#include "key.h"
#include "parameters.h"
#include "randombytes.h"
#include "verify.h"

int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int crypto_kem_enc(uint8_t *ctxt, uint8_t *ss, const uint8_t *pk);
int crypto_kem_dec(uint8_t *ss, const uint8_t *ctxt, const uint8_t *sk);

#endif // SMAUG_KEM_H
