#include "io.h"

void save_to_string(uint8_t *output, const ciphertext *ctxt) {
    Rp_vec_to_bytes(output, &(ctxt->c1));
    Rp2_to_bytes(output + CTPOLYVEC_BYTES, &(ctxt->c2));
}

void load_from_string(ciphertext *ctxt, const uint8_t *input) {
    bytes_to_Rp_vec(&(ctxt->c1), input);
    bytes_to_Rp2(&(ctxt->c2), input + CTPOLYVEC_BYTES);
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void save_to_string_sk(uint8_t *output, const secret_key *sk) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        Sx_to_bytes(output + SKPOLY_BYTES * i, &sk->vec[i]);
}

void load_from_string_sk(secret_key *sk, const uint8_t *input) {
    for (size_t i = 0; i < MODULE_RANK; ++i)
        bytes_to_Sx(&sk->vec[i], input + SKPOLY_BYTES * i);
}

/*----------------------------------------------------------------------------*/

void save_to_string_pk(uint8_t *output, const public_key *pk) {
    memcpy(output, pk->seed, sizeof(uint8_t) * PKSEED_BYTES);
    Rq_vec_to_bytes(output + PKSEED_BYTES, &(pk->b));
}

void load_from_string_pk(public_key *pk, const uint8_t *input) {
    memcpy(pk->seed, input, PKSEED_BYTES);
    genAx(pk->A, pk->seed);
    bytes_to_Rq_vec(&(pk->b), input + PKSEED_BYTES);
}
