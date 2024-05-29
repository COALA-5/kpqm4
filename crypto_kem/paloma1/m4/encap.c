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

#include "encap.h"

/**
 * @brief Encapsulation
 *
 * @param [out] c ciphertext c = (r_hat, s_hat)
 * @param [out] key 256-bit key
 * @param [in] pk a public key pk
 */
void encap(OUT Ciphertext* c, OUT Word* key, IN const PublicKey* pk)
{
    Word r_star[SEED_WORDS] = {0};
    Word e_star[PARAM_N_WORDS] = {0};
    Word e_hat[PARAM_N_WORDS] = {0};
    Word e_r_s[ROH_INPUT_WORDS] = {0};

    /* generate 256-bit seed r_star for error vector */
    gen_rand_sequence(r_star, SEED_BITS);

    /* generate random error vector e_star such that W_H(e_star) = t */
    gen_err_vec(e_star, r_star);

    /* generate 256-bit seed r_hat for permutation matrix */
    /* r_hat = ROG(e_star) */
    rand_oracle_G(c->r_hat, e_star);

    /* generate random permutation matrix to use seed r_hat */
    perm(e_hat, e_star, c->r_hat);

    /* encrypt */
    encrypt(c->s_hat, pk, e_hat);

    /* key k = ROH(e_star, r_hat, s_hat) */
    memcpy(e_r_s, e_star, (sizeof(Word)) * PARAM_N_WORDS);
    memcpy(e_r_s + PARAM_N_WORDS, c->r_hat, (sizeof(Word)) * SEED_WORDS);
    memcpy(e_r_s + PARAM_N_WORDS + SEED_WORDS, c->s_hat, 
                (sizeof(Word)) * SYND_WORDS);

    rand_oracle_H(key, e_r_s);
}