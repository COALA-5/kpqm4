/*
 * Copyright (c) 2024 FDL(Future cryptography Design Lab.) Kookmin University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "paloma_def.h"
#include "encap.h"

/**
 * @brief Encapsulation
 *
 * @param [out] c ciphertext c = (r_hat, s_hat)
 * @param [out] key 256-bit key
 * @param [in] pk a public key pk
 */
void encap(OUT Ciphertext* ct, OUT u08* ss, IN const PublicKey* pk)
{
    u08* r_star;    get_new_data(u08, r_star, SEED_BYTES);
    Word* e_star;   get_new_data(Word, e_star, PARAM_N_WORDS);
    Word* e_hat;    get_new_data(Word, e_hat, PARAM_N_WORDS);
    Word* e_r_s;    get_new_data(Word, e_r_s, ROH_INPUT_WORDS);

    /* generate 256-bit seed r_star for error vector */
    gen_rand_bytes(r_star, SEED_BYTES);

    /* generate random error vector e_star such that W_H(e_star) = t */
    gen_err_vec(e_star, r_star);

    /* generate 256-bit seed r_hat for permutation matrix */
    /* r_hat <- ROG(e_star) */
    rand_oracle_G(ct->r_hat, e_star);

    /* generate random permutation matrix to use seed r_hat */
    perm(e_hat, e_star, ct->r_hat);

    /* encrypt */
    encrypt(ct->s_hat, pk, e_hat);

    /* ss <- ROH(e_star, r_hat, s_hat) */
    memcpy(e_r_s, e_star, sizeof(Word) * PARAM_N_WORDS);
    memcpy(e_r_s + PARAM_N_WORDS, ct->r_hat, sizeof(Word) * SEED_WORDS);
    memcpy(e_r_s + PARAM_N_WORDS + SEED_WORDS, ct->s_hat, sizeof(Word) * SYND_WORDS);

    rand_oracle_H(ss, e_r_s);

    free(r_star);
    free(e_star);
    free(e_hat);
    free(e_r_s);
}