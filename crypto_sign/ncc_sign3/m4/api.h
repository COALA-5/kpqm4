#ifndef API_H
#define API_H
#include "params.h"

#include "config.h"

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int crypto_sign(unsigned char *sm, unsigned int *smlen,
                const unsigned char *msg, unsigned int len,
                const unsigned char *sk);

int crypto_sign_open(unsigned char *m, unsigned int *mlen,
                     const unsigned char *sm, unsigned int smlen,
                     const unsigned char *pk);

#endif