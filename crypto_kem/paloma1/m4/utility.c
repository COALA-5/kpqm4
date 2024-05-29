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

#include "utility.h"

/**
 * @brief PALOMA SHUFFLE : Shuffle with 256-bit seed r
 *
 * @param [out] shuffled_set
 * @param [in] set_len Set length.
 * @param [in] _256bits_seed_r 256-bit seed r
 */
void shuffle(
            OUT gf* shuffled_set, 
            IN int set_len, 
            IN const Word* _256bits_seed_r)
{
    int i, j;
    int w = 0;
    gf seed[16];
    gf tmp;

    /* Generate [0,1,...,n-1] set */
    for (i = 0; i < set_len; i++)
    {
        shuffled_set[i] = i;
    }

    /* Separate seeds by 16-bit */
    memcpy(seed, _256bits_seed_r, SEED_BYTES);

    /* Shuffling */
    for (i = set_len - 1; i > 0; i--)
    {
        j = ((seed[w % 16]) % (i + 1));
        /* Swap */
        tmp = shuffled_set[j];
        shuffled_set[j] = shuffled_set[i];
        shuffled_set[i] = tmp;

        w = (w + 1) & 0xf;
    }
}

/**
 * @brief Function to generate random sequence
 *
 * @param [out] out_vec Random sequence
 * @param [in] sequence_len Bit length of random sequence
 */
void gen_rand_sequence(OUT Word* rand_sequence, IN int sequence_len)
{
    memset(rand_sequence, 0, 32);

    for (int i = 0; i < 32; i++)
    {   
        Word rand_k = (rand() & 0xff);
        rand_sequence[i / WORD_BYTES] |= (rand_k << ((i * 8) % WORD_BITS)); 
    }
}

/**
 * @brief Random Oracle G
 *
 * @param [out] seed: Oracle result
 * @param [in] msg: Oracle input data
 */
void rand_oracle_G(
            OUT Word* seed, 
            IN const Word* msg)
{
    /* Use LSH-512, output length : 512-bit */
    lsh_type algtype = LSH_MAKE_TYPE(1, 512);
    lsh_u8 src[8 + (PARAM_N / 8)];

    /* Generate oracle input data */
    /* PALOMAGG  ASCII value*/
    src[0] = 0x50;
    src[1] = 0x41;
    src[2] = 0x4c;
    src[3] = 0x4f;
    src[4] = 0x4d;
    src[5] = 0x41;
    src[6] = 0x47;
    src[7] = 0x47;
    

    /* input data */
    for (int i = 0; i < (PARAM_N / 8); i++) 
    {
        src[8 + i] = (msg[i / WORD_BYTES] >> ((8 * i) % WORD_BITS)) & 0xff;
    }

    /* Store output hash values */
    lsh_u8 result[512 / 8] = {0};

    /* Generate hash values */
    lsh_digest(algtype, src, 64 + PARAM_N, result); 

    for (int i = 0; i < 32; i++)
    {
        seed[i / WORD_BYTES] |= ((Word)result[i] << ((i * 8) % WORD_BITS));
    }
}

/**
 * @brief Random Oracle H
 *
 * @param [out] seed: Oracle result
 * @param [in] msg: Oracle input data
 */
void rand_oracle_H(
            OUT Word* seed, 
            IN const Word* msg)
{
    /* Use LSH-512, output length : 512-bit */
    lsh_type algtype = LSH_MAKE_TYPE(1, 512);
    lsh_u8 src[8 + (ROH_INPUT_BITS / 8)];

    /* Generate oracle input data */
    /* PALOMAHH ASCII value*/
    src[0] = 0x50;
    src[1] = 0x41;
    src[2] = 0x4c;
    src[3] = 0x4f;
    src[4] = 0x4d;
    src[5] = 0x41;
    src[6] = 0x48;
    src[7] = 0x48;

    /* input data */
    for (int i = 0; i < (ROH_INPUT_BITS / 8); i++) 
    {
        src[8 + i] = (msg[i / WORD_BYTES] >> ((8 * i) % WORD_BITS)) & 0xff;
    }

    /* Store output hash values */
    lsh_u8 result[512 / 8] = {0};

    /* Generate hash values */
    lsh_digest(algtype, src, 64 + ROH_INPUT_BITS, result); 

    for (int i = 0; i < 32; i++)
    {
        seed[i / WORD_BYTES] |= ((Word)result[i] << ((i * 8) % WORD_BITS));
    }
}

/**
 * @brief Generation of a Random Permutation Matrix
 * @param [out] P An n × n permutation matrix P
 * @param [out] P_inv An n × n permutation matrix P^{-1}
 * @param [in] n n s.t. output n x n matrix
 * @param [in] r A random 256-bit string r
 */
void gen_perm_mat(OUT gf* P, OUT gf* P_inv, IN int n, IN const Word* r)
{
    /* Shuffle([n], r) */ 
    shuffle(P, n, r);

    for (size_t i = 0; i < n; i++)
        P_inv[P[i]] = i;
}

/**
 * @brief Substitute a vector src_v with a 256-bit string r 
 *        and permutation matrix P.
 * 
 * @param [out] dst_v Output vector dst_v = P * src_v
 * @param [in] src_v Input vector src_v (\in F_2^n)
 * @param [in] r a 256-bit string r
 */
void perm(OUT Word* dst_v, IN const Word* src_v, IN const Word* r)
{   
    gf P[PARAM_N] = {0};
    gf P_inv[PARAM_N] = {0};
    gen_perm_mat(P, P_inv, PARAM_N, r);

    /* dst_v = P * src_V */
    memset(dst_v, 0, (sizeof(Word)) * PARAM_N_WORDS);
    for (size_t i = 0; i < PARAM_N; i++)
    {
        Word bit = ((src_v[P[i] / WORD_BITS] >> (P[i] % WORD_BITS)) & 1);
        dst_v[i / WORD_BITS] ^= bit << (i % WORD_BITS);
    }
}

/**
 * @brief Substitute a vector src_v with a 256-bit string r 
 *        and permutation matrix P^{-1}.
 * 
 * @param [out] dst_v Output vector dst_v = P^{-1} * src_v
 * @param [in] src_v Input vector src_v (\in F_2^n)
 * @param [in] r a 256-bit string r
 */
void perm_inv(OUT Word* dst_v, IN const Word* src_v, IN const Word* r)
{   
    gf P[PARAM_N] = {0};
    gf P_inv[PARAM_N] = {0};
    gen_perm_mat(P, P_inv, PARAM_N, r);

    /* dst_v = P * src_V */
    memset(dst_v, 0, (sizeof(Word)) * PARAM_N_WORDS);
    for (size_t i = 0; i < PARAM_N; i++)
    {
        Word bit = 
            ((src_v[P_inv[i] / WORD_BITS] >> (P_inv[i] % WORD_BITS)) & 1);
            
        dst_v[i / WORD_BITS] ^= bit << (i % WORD_BITS);
    }
}

/**
 * @brief Function to generate error vector
 *
 * @param [out] err_vec error vector
 * @param [in] seed 256-bit seed r
 */
void gen_err_vec(OUT Word* err_vec, IN const Word* seed)
{
    gf err[PARAM_N] = {0};
    const Word One = 1;

    shuffle(err, PARAM_N, seed);

    for (int i = 0; i < PARAM_T; i++)
    {
        err_vec[err[i] / WORD_BITS] |= (One << (err[i] % WORD_BITS));
    }
}