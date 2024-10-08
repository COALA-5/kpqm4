//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//

#include <string.h>
#include "rng.h"
#include "aes.h"

AES256_CTR_DRBG_struct  DRBG_ctx;

void    AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer);


void handleErrors(void)
{
    fprintf(stderr, "Error detected.");
    abort();
}

// Use whatever AES implementation you have. This uses AES from openSSL library
//    key - 256-bit AES key
//    ctr - a 128-bit plaintext value
//    buffer - a 128-bit ciphertext value
void
AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer)
{
    size_t nblocks;
    nblocks = 1;

    aes256ctx ctx;

    aes256_ecb_keyexp(&ctx, key);
    aes256_ecb(buffer, ctr, nblocks, &ctx);
    aes256_ctx_release(&ctx);
}

void
randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength)
{
    unsigned char   seed_material[48];
    security_strength = security_strength;	// dummy for suppress compile warning

    memcpy(seed_material, entropy_input, 48);
    if(personalization_string)
        for(int i = 0 ; i < 48 ; i++)
            seed_material[i] ^= personalization_string[i];
    memset(DRBG_ctx.Key, 0x00, 32);
    memset(DRBG_ctx.V, 0x00, 16);
    AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter = 1;
}

int
randombytes(unsigned char *x, unsigned long long xlen)
{
    unsigned char   block[16];
    int             i = 0;

    while(xlen > 0)
    {
        //increment V
        for(int j = 15 ; j >= 0 ; j--)
        {
            if(DRBG_ctx.V[j] == 0xff)
                DRBG_ctx.V[j] = 0x00;
            else
            {
                DRBG_ctx.V[j]++;
                break;
            }
        }
        AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
        if(xlen > 15)
        {
            memcpy(x + i, block, 16);
            i += 16;
            xlen -= 16;
        }
        else
        {
            memcpy(x + i, block, xlen);
            xlen = 0;
        }
    }
    AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter++;

    return RNG_SUCCESS;
}

void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *V)
{
    unsigned char   temp[48];

    for(int i = 0 ; i < 3 ; i++)
    {
        //increment V
        for(int j = 15 ; j >= 0 ; j--)
        {
            if(V[j] == 0xff)
                V[j] = 0x00;
            else
            {
                V[j]++;
                break;
            }
        }

        AES256_ECB(Key, V, temp + 16 * i);
    }

    if(provided_data != NULL)
        for(int i = 0 ; i < 48 ; i++)
            temp[i] ^= provided_data[i];

    memcpy(Key, temp, 32);
    memcpy(V, temp + 32, 16);
}







/////////////////////////////////////////////////////////

void
randombytes_init_with_state( AES256_CTR_DRBG_struct * states, unsigned char *entropy_input_48bytes )
{

    unsigned char   seed_material[48];
    memcpy(seed_material, entropy_input_48bytes, 48);

    memset(states->Key, 0x00, 32);
    memset(states->V, 0x00, 16);
    AES256_CTR_DRBG_Update(seed_material, states->Key, states->V);
    states->reseed_counter = 1;
}

int
randombytes_with_state( AES256_CTR_DRBG_struct * states, unsigned char *x, unsigned long long xlen)
{

    unsigned char   block[16];
    int             i = 0;

    while ( xlen > 0 ) {
        //increment V
        for (int j=15; j>=0; j--) {
            if ( states->V[j] == 0xff )
                states->V[j] = 0x00;
            else {
                states->V[j]++;
                break;
            }
        }
        AES256_ECB(states->Key, states->V, block);
        if ( xlen > 15 ) {
            memcpy(x+i, block, 16);
            i += 16;
            xlen -= 16;
        }
        else {
            memcpy(x+i, block, xlen);
            xlen = 0;
        }
    }
    AES256_CTR_DRBG_Update(NULL, states->Key, states->V);
    states->reseed_counter++;

    return RNG_SUCCESS;
}


