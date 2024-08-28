// // SPDX-License-Identifier: Apache-2.0 or CC0-1.0
// #include "api.h"
// #include "hal.h"
// #include "sendfn.h"
// #include "randombytes.h"

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>

// #define MLEN 59

// // https://stackoverflow.com/a/1489985/1711232
// #define PASTER(x, y) x##y
// #define EVALUATOR(x, y) PASTER(x, y)
// #define NAMESPACE(fun) EVALUATOR(MUPQ_NAMESPACE, fun)

// // use different names so we can have empty namespaces
// #define MUPQ_CRYPTO_PUBLICKEYBYTES NAMESPACE(CRYPTO_PUBLICKEYBYTES)
// #define MUPQ_CRYPTO_SECRETKEYBYTES NAMESPACE(CRYPTO_SECRETKEYBYTES)
// #define MUPQ_CRYPTO_BYTES          NAMESPACE(CRYPTO_BYTES)
// #define MUPQ_CRYPTO_ALGNAME        NAMESPACE(CRYPTO_ALGNAME)

// #define MUPQ_crypto_sign_keypair NAMESPACE(crypto_sign_keypair)
// #define MUPQ_crypto_sign NAMESPACE(crypto_sign)
// #define MUPQ_crypto_sign_open NAMESPACE(crypto_sign_open)
// #define MUPQ_crypto_sign_signature NAMESPACE(crypto_sign_signature)
// #define MUPQ_crypto_sign_verify NAMESPACE(crypto_sign_verify)

// #define printcycles(S, U) send_unsignedll((S), (U))

int main(void)
{
//   unsigned char sk[MUPQ_CRYPTO_SECRETKEYBYTES];
//   unsigned char pk[MUPQ_CRYPTO_PUBLICKEYBYTES];
//   unsigned char sm[MLEN+MUPQ_CRYPTO_BYTES];
//   size_t smlen;
//   unsigned long long t0, t1;
//   int i;

//   hal_setup(CLOCK_BENCHMARK);

//   hal_send_str("==========================");

//     unsigned long long keygen = 0;
//     unsigned long long sign = 0;
//     unsigned long long verify = 0;

//   for(i=0;i<MUPQ_ITERATIONS; i++)
//   {

//     // Key-pair generation
//     t0 = hal_get_time();
// #ifdef KPQM4_MQSIGN
//     uint8_t seed[48] = {0,};
//     uint8_t ss[32] = {0,};
//     //randombytes_init(seed, NULL, 256);
//     randombytes(seed, 48);
//     MUPQ_crypto_sign_keypair(pk, sk, seed);
// #else 
//     MUPQ_crypto_sign_keypair(pk, sk);
// #endif
//     t1 = hal_get_time();
//     printcycles("keypair cycles:", t1-t0);
//     keygen += (t1 - t0);

//     // Signing
//     t0 = hal_get_time();
// #ifdef KPQM4_MQSIGN
//     MUPQ_crypto_sign(sm, &smlen, sm, MLEN, sk, seed, ss);
// #else 
//     MUPQ_crypto_sign(sm, &smlen, sm, MLEN, sk);
// #endif
//     t1 = hal_get_time();
//     printcycles("sign cycles:", t1-t0);
//     sign += (t1 - t0);

//     // Verification
//     t0 = hal_get_time();
//     MUPQ_crypto_sign_open(sm, &smlen, sm, smlen, pk);
//     t1 = hal_get_time();
//     printcycles("verify cycles:", t1-t0);
//     verify += (t1 - t0);

//     hal_send_str("+");
//   }

//     keygen /= MUPQ_ITERATIONS;
//     sign /= MUPQ_ITERATIONS;
//     verify /= MUPQ_ITERATIONS;

//   printcycles("keygen avg:", (int)(keygen));
//   printcycles("sign avg:", (int)(sign));
//   printcycles("verify avg:", (int)(verify));
//   hal_send_str("#");
  return 0;
}
