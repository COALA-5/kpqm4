// SPDX-License-Identifier: Apache-2.0 or CC0-1.0
#include "api.h"
#include "hal.h"
#include "sendfn.h"
#include "randombytes.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MLEN 59
// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(MUPQ_NAMESPACE, fun)

// use different names so we can have empty namespaces
#define MUPQ_CRYPTO_PUBLICKEYBYTES NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define MUPQ_CRYPTO_SECRETKEYBYTES NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define MUPQ_CRYPTO_BYTES          NAMESPACE(CRYPTO_BYTES)
#define MUPQ_CRYPTO_ALGNAME        NAMESPACE(CRYPTO_ALGNAME)

#define MUPQ_crypto_sign_keypair NAMESPACE(crypto_sign_keypair)
#define MUPQ_crypto_sign NAMESPACE(crypto_sign)
#define MUPQ_crypto_sign_open NAMESPACE(crypto_sign_open)
#define MUPQ_crypto_sign_signature NAMESPACE(crypto_sign_signature)
#define MUPQ_crypto_sign_verify NAMESPACE(crypto_sign_verify)

#define printcycles(S, U) send_unsignedll((S), (U))

unsigned long long hash_cycles;

int main(void)
{

  unsigned char sm[MLEN+MUPQ_CRYPTO_BYTES];
    unsigned char seed[32] = {0,};
  size_t smlen;
  unsigned long long t0, t1;
  int i;

  unsigned long long total_keypair = 0;
  unsigned long long total_sign = 0;
  unsigned long long total_verify = 0;

  unsigned long long total_hash_keypair = 0;
  unsigned long long total_hash_sign = 0;
  unsigned long long total_hash_verify = 0;

  hal_setup(CLOCK_BENCHMARK);

  hal_send_str("==========================");

  for(i=0;i<MUPQ_ITERATIONS; i++)
  {
    // Key-pair generation
    hash_cycles = 0;
    t0 = hal_get_time();
#ifdef KPQM4_MQSIGN
#include "key.h"
  unsigned char ss[32] = {0,};

#else 
  unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];
  unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES]; 
    MUPQ_crypto_sign_keypair(pk, sk);
#endif
    t1 = hal_get_time();
    printcycles("keypair cycles:", t1-t0);
    printcycles("keypair hash cycles:", hash_cycles);
    total_keypair += t1-t0;
    total_hash_keypair += hash_cycles;

    // Signing
    hash_cycles = 0;
    t0 = hal_get_time();
#ifdef KPQM4_MQSIGN
    MUPQ_crypto_sign(sm, &smlen, sm, MLEN, sk, seed, ss);
#else 
    MUPQ_crypto_sign(sm, &smlen, sm, MLEN, sk);
#endif
    t1 = hal_get_time();
    printcycles("sign cycles:", t1-t0);
    printcycles("sign hash cycles:", hash_cycles);
    total_sign += t1-t0;
    total_hash_sign += hash_cycles;

    // Verification
    hash_cycles = 0;
    t0 = hal_get_time();
    MUPQ_crypto_sign_open(sm, &smlen, sm, smlen, pk);
    t1 = hal_get_time();
    printcycles("verify cycles:", t1-t0);
    printcycles("verify hash cycles:", hash_cycles);
    total_verify += t1-t0;
    total_hash_verify += hash_cycles;

    hal_send_str("+");
  }
  printcycles("keypair hash ratio:\n", total_hash_keypair * 10000 / total_keypair);
  printcycles("sign hash ratio:\n", total_hash_sign * 10000 / total_sign);
  printcycles("verify hash ratio:\n", total_hash_verify * 10000 / total_verify);
  
  hal_send_str("#");
  return 0;
}
