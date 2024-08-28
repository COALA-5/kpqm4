#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mqs_config.h"
#include "mqs_keypair.h"
#include "mqs.h"

#include "api.h"
#include "utils_hash.h"
#include "randombytes.h"

#if defined(_SUPERCOP_)
#include "crypto_sign.h"
#endif

#include "hal.h" 
#include "sendfn.h"
#include "_keys.h" 
#include "flashpk_stm32l4r5.h"

// #define ADDR_FLASH_SK ((uint32_t)(0x8000000 + 4 * 26))
// #define ADDR_FLASH_PK (ADDR_FLASH_SK + CRYPTO_SECRETKEYBYTES)


#define LEN CRYPTO_SECRETKEYBYTES
extern unsigned char SK_DAT[LEN]; 

static void printbytes(const unsigned char *x, unsigned long long xlen)
{
  char outs[2*xlen+1];
  unsigned long long i;
  for(i=0;i<xlen;i++)
    sprintf(outs+2*i, "%02x", x[i]);
  outs[2*xlen] = 0;
  hal_send_str(outs);
}

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk, unsigned char *sk_seed)
{
	int r;
	hal_send_str("entering generate_keypair_mqlr");
	//r = generate_keypair_mqlr((pk_mqs*)pk, (sk_mqlr*)sk, sk_seed);
	hal_send_str("out generate_keypair_mqlr");

	// (void) sk_seed;
	pk = _PK;
	sk = _SK;
	return r;
}


int
crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk, const uint8_t* sk_seed, const uint8_t *ss)
{
	hal_send_str("entering crypto_sign");
	unsigned char digest[_HASH_LEN];

	hash_msg( digest , _HASH_LEN , m , mlen );

	memcpy( sm , m , mlen );
	smlen[0] = mlen + _SIGNATURE_BYTE;
	return mqlr_sign(sm + mlen, (sk_mqlr*)sk, sk_seed, m, (uint32_t) mlen, ss);
}



int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
				const unsigned char *sm, unsigned long long smlen,
				const unsigned char *pk)
{
	if( _SIGNATURE_BYTE > smlen ) return -1;

	memcpy( m , sm , smlen-_SIGNATURE_BYTE );
	mlen[0] = smlen-_SIGNATURE_BYTE;
	
	return mqlr_verify(m, (uint32_t) mlen[0], sm + smlen - _SIGNATURE_BYTE, pk);
}

