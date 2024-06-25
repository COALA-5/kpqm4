#ifndef SMAUG_IO_H
#define SMAUG_IO_H

#include <stdlib.h>

#include "ciphertext.h"
#include "key.h"
#include "parameters.h"
#include "poly.h"
#include "verify.h"

// For ciphertext
#define save_to_string SMAUG_NAMESPACE(save_to_string)
void save_to_string(uint8_t *output, const ciphertext *ctxt);
#define load_from_string SMAUG_NAMESPACE(load_from_string)
void load_from_string(ciphertext *ctxt, const uint8_t *input);

// For secret key
#define save_to_string_sk SMAUG_NAMESPACE(save_to_string_sk)
void save_to_string_sk(uint8_t *output, const secret_key *sk);
#define load_from_string_sk SMAUG_NAMESPACE(load_from_string_sk)
void load_from_string_sk(secret_key *sk, const uint8_t *input);

// For public key
#define save_to_string_pk SMAUG_NAMESPACE(save_to_string_pk)
void save_to_string_pk(uint8_t *output, const public_key *pk);
#define load_from_string_pk SMAUG_NAMESPACE(load_from_string_pk)
void load_from_string_pk(public_key *pk, const uint8_t *input);

#endif // SMAUG_IO_H