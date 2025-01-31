// SPDX-License-Identifier: Apache-2.0 or CC0-1.0
#include "api.h"
#include "hal.h"
#include "sendfn.h"

#include <stdint.h>
#include <string.h>

// https://stackoverflow.com/a/1489985/1711232
#define PASTER(x, y) x##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(MUPQ_NAMESPACE, fun)

// use different names so we can have empty namespaces
#define MUPQ_CRYPTO_BYTES           NAMESPACE(CRYPTO_BYTES)
#define MUPQ_CRYPTO_PUBLICKEYBYTES  NAMESPACE(CRYPTO_PUBLICKEYBYTES)
#define MUPQ_CRYPTO_SECRETKEYBYTES  NAMESPACE(CRYPTO_SECRETKEYBYTES)
#define MUPQ_CRYPTO_CIPHERTEXTBYTES NAMESPACE(CRYPTO_CIPHERTEXTBYTES)
#define MUPQ_CRYPTO_ALGNAME NAMESPACE(CRYPTO_ALGNAME)

#define MUPQ_crypto_kem_keypair NAMESPACE(crypto_kem_keypair)
#define MUPQ_crypto_kem_enc NAMESPACE(crypto_kem_enc)
#define MUPQ_crypto_kem_dec NAMESPACE(crypto_kem_dec)

#define printcycles(S, U) send_unsignedll((S), (U))

unsigned long long hash_cycles;

int main(void)
{
  unsigned char key_a[MUPQ_CRYPTO_BYTES], key_b[MUPQ_CRYPTO_BYTES];
  unsigned char ct[MUPQ_CRYPTO_CIPHERTEXTBYTES];
  unsigned long long t0, t1;
  int i;

  hal_setup(CLOCK_BENCHMARK);

  hal_send_str("==========================");

  unsigned long long total_keypair = 0;
  unsigned long long total_encaps = 0;
  unsigned long long total_decaps = 0;

  unsigned long long total_hash_keypair = 0;
  unsigned long long total_hash_encaps = 0;
  unsigned long long total_hash_decaps = 0;

  for(i=0;i<MUPQ_ITERATIONS; i++)
  {
    // Key-pair generation
    hash_cycles = 0;
    t0 = hal_get_time();
#ifdef KPQM4_PALOMA
#include "key.h"
#else
    unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];
    unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES];
    MUPQ_crypto_kem_keypair(pk, sk);
#endif
    t1 = hal_get_time();
    printcycles("keypair cycles:", t1-t0);
    printcycles("keypair hash cycles:", hash_cycles);
    total_keypair += t1-t0;
    total_hash_keypair += hash_cycles;

    // Encapsulation
    hash_cycles = 0;
    t0 = hal_get_time();
    MUPQ_crypto_kem_enc(ct, key_a, pk);
    t1 = hal_get_time();
    printcycles("encaps cycles:", t1-t0);
    printcycles("encaps hash cycles:", hash_cycles);
    total_encaps += t1-t0;
    total_hash_encaps += hash_cycles;

    // Decapsulation
    hash_cycles = 0;
    t0 = hal_get_time();
#if defined (KPQM4_PALOMA)
    MUPQ_crypto_kem_dec(key_b, ct, sk);
#elif defined (KPQM4)
    MUPQ_crypto_kem_dec(key_b, sk, pk, ct);
#else
    MUPQ_crypto_kem_dec(key_b, ct, sk);
#endif
    t1 = hal_get_time();
    printcycles("decaps cycles:", t1-t0);
    printcycles("decaps hash cycles:", hash_cycles);
    total_decaps += t1-t0;
    total_hash_decaps += hash_cycles;

    if (memcmp(key_a, key_b, MUPQ_CRYPTO_BYTES)) {
      hal_send_str("ERROR KEYS\n");
    }
    else {
      hal_send_str("OK KEYS\n");
    }
    hal_send_str("+");
  }
  printcycles("keypair hash ratio:\n", total_hash_keypair * 10000 / total_keypair);
  printcycles("encaps hash ratio:\n", total_hash_encaps * 10000 / total_encaps);
  printcycles("decaps hash ratio:\n", total_hash_decaps * 10000 / total_decaps);
  hal_send_str("#");
  return 0;
}
