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

#include "decap.h"

void decap(
        OUT         u08*        ss,
        IN const    SecretKey*  sk,
        IN const    Ciphertext* ct,
        IN const    gf2m_tab*   gf2m_tables)
{
    Word    *e_hat, *e_star, *e_tilde, *e_r_s;
    u08     *r_hat_p;

    /* Decrypt */
    get_new_data(Word, e_hat, PARAM_N_WORDS);
    decrypt(e_hat, sk, ct->s_hat, gf2m_tables);

    /* Generate permutation matrix */
    get_new_data(Word, e_star, PARAM_N_WORDS);
    perm_inv(e_star, e_hat, ct->r_hat);

    /* r_hat_prime <- RO_G(e_star) */
    get_new_data(u08, r_hat_p, SEED_BYTES);
    rand_oracle_G(r_hat_p, e_star);
    
    /* e_tilde <- GenErrVec(r) for implicit rejection */
    get_new_data(Word, e_tilde, PARAM_N_WORDS);
    gen_err_vec(e_tilde, sk->r);
    
    /* 
        Check the recovered error vector is valid
        1. wH(e) = t?
        2. r_hat = r_hat_p
    */
    Word* e_tmp = NULL;
    
    int ans = is_equal((Word*)ct->r_hat, (Word*)r_hat_p, SEED_WORDS);
    if ((ans == NO) || (get_Hamming_weight(e_hat) != PARAM_T))
    {
        e_tmp = e_tilde; // Use e_tilde if r_hat != r_hat_prime
    }
    else
    {
        e_tmp = e_star; // Use e_star if r_hat = r_hat_prime
    }

    /*
        e_r_s <- e_tmp,r,s  /  key <- RO_H(e_r_s)
        Case 1. key <- RO_H(e_star  || r_hat || s_hat)
        Case 2. key <- RO_H(e_tilde || r_hat || s_hat) : implicit rejction
    */
    get_new_data(Word, e_r_s, PARAM_N_WORDS + SEED_WORDS + SYND_WORDS);
    memcpy(e_r_s,                               e_tmp,      PARAM_N_BYTES);
    memcpy(e_r_s + PARAM_N_WORDS,               ct->r_hat,  SEED_BYTES);
    memcpy(e_r_s + PARAM_N_WORDS + SEED_WORDS,  ct->s_hat,  SYND_BYTES);

    rand_oracle_H(ss, e_r_s);

    free(e_hat);free(e_star);free(e_tilde);free(r_hat_p);free(e_r_s);
}
