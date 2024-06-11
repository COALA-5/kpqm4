#ifndef CONFIG_H
#define CONFIG_H

#ifndef NIMS_TRI_NTT_MODE
#define NIMS_TRI_NTT_MODE 3
#endif

#if NIMS_TRI_NTT_MODE == 1
#define CRYPTO_ALGNAME "NCC-Sign1"
#elif NIMS_TRI_NTT_MODE == 3
#define CRYPTO_ALGNAME "NCC-Sign3"
#elif NIMS_TRI_NTT_MODE == 5
#define CRYPTO_ALGNAME "NCC-Sign5"
#endif

#endif
