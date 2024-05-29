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

/**
 * @brief Check error vector's Hamming Weight is t.
 *
 * @param [in] err_vec Error vector.
 * @return True or False.
 */
int check_err_vec(IN const Word* err_vec)
{
#if WORD == 64
    const u64 m1 = 0x5555555555555555;  
    const u64 m2 = 0x3333333333333333;  
    const u64 m4 = 0x0f0f0f0f0f0f0f0f;  
    const u64 h01 = 0x0101010101010101; 
    const u64 shift = 56;
#endif

#if WORD == 32
    const u32 m1 = 0x55555555;  
    const u32 m2 = 0x33333333;  
    const u32 m4 = 0x0f0f0f0f;  
    const u32 h01 = 0x01010101; 
    const u32 shift = 24;
#endif

    Word count = 0;

    for (int i = 0; i < PARAM_N / WORD_BITS; i++)
    {
        Word x = err_vec[i];

        x -= (x >> 1) & m1;             
        x = (x & m2) + ((x >> 2) & m2); 
        x = (x + (x >> 4)) & m4;        
        count += (x * h01) >> shift;    
    }

    return count;
}

/**
 * @brief Decapsulation function.
 *
 * @param [out] key Output key to be recovered.
 * @param [in] sk Secret key.
 * @param [in] c ciphertext c = (r_hat, s_hat)
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void decap(
            OUT Word* ss, 
            IN const SecretKey* sk, 
            IN const Ciphertext* c, 
            IN const gf2m_tab* gf2m_tables)
{
    Word e_hat[PARAM_N_WORDS] = {0};
    Word e_star[PARAM_N_WORDS] = {0};
    Word e_tilde[PARAM_N_WORDS] = {0};
    Word r_hat_p[SEED_WORDS] = {0};    // r_hat_prime
    Word e_r_s[ROH_INPUT_WORDS] = {0};
    Word* e_tmp = NULL;
    int flag = 0;

    /* Decrypt */
    decrypt(e_hat, sk, c->s_hat, gf2m_tables);

    /* Check the recovered error vector is valid */
    if (!check_err_vec(e_hat))
    {
        // puts("invalid error vector: error vector does not have t error.");
        // exit(1);
        return 1;
    }

    /* Generate permutation matrix */
    perm_inv(e_star, e_hat, c->r_hat);
    
    /* r_hat_prime = RO_G(e_star) */
    rand_oracle_G(r_hat_p, e_star);

    /* e_tilde = GenErrVec(r) */
    gen_err_vec(e_tilde, sk->r);

    for (int i = 0; i < 4; i++)
        flag |= (c->r_hat[i] != r_hat_p[i]);

    if (flag)
        e_tmp = e_tilde; // Use e_tilde if r_hat != r_hat_prime
    else
        e_tmp = e_star; // Use e_star if r_hat = r_hat_prime

    /* key = RO_H(e_star(or e_tilde) || r_hat || s_hat) */
    memcpy(e_r_s, e_tmp, sizeof(Word) * PARAM_N_WORDS);
    memcpy(e_r_s + PARAM_N_WORDS, c->r_hat, sizeof(Word) * SEED_WORDS);
    memcpy(e_r_s + PARAM_N_WORDS + SEED_WORDS, c->s_hat, 
            sizeof(Word) * SYND_WORDS);

    rand_oracle_H(ss, e_r_s);
}
