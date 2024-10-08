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

void show_bytes(IN char* str, IN u08* bytes, IN size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        // printf("%02x ", bytes[i]);
    }
}

/**
 * @brief PALOMA SHUFFLE : Shuffle with 256-bit seed r
 *
 * @param [out]     shuffled_list
 * @param [in]      list_len Set length.
 * @param [in]      _256bits_seed_r 256-bit seed r
 */
void shuffle(
            OUT uint16_t*       shuffled_list, 
            IN  int             list_len, 
            IN  const u08*  seed)
{
    int i, j, w;
    uint16_t tmp;

    // uint16_t block_16bit[16];
    uint16_t* block_16bit = (uint16_t*)seed;

    /* Generate [0,1,...,n-1] set */
    for (i = 0; i < list_len; i++)
        shuffled_list[i] = i;

    /* Shuffling */
    w = 0;
    for (i = list_len - 1; i > 0; i--)
    {
        j = ((block_16bit[w % 16]) % (i + 1));

        /* Swap */
        tmp = shuffled_list[j];
        shuffled_list[j] = shuffled_list[i];
        shuffled_list[i] = tmp;

        w = (w + 1) & 0xf;
    }
}

/**
 * @brief Function to generate random sequence
 *
 * @param [out] bytes Random sequence
 * @param [in] size Bit length of random sequence
 */
void gen_rand_bytes(OUT u08* bytes, IN size_t size)
{
#if WITH_OPENSSL == YES
    randombytes(bytes, size);
#else
    for (size_t i = 0; i < size; i++)
        bytes[i] = rand() % 0xff;
#endif
}

/**
 * @brief Random Oracle G
 *
 * @param [out] seed: Oracle result
 * @param [in] msg: Oracle input data
 */
void rand_oracle_G(
            OUT u08* seed, 
            IN const Word* msg)
{
    memset(seed, 0, SEED_BYTES);

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

    memcpy(seed, result, SEED_BYTES);
}

/**
 * @brief Random Oracle H
 *
 * @param [out] seed: Oracle result
 * @param [in] msg: Oracle input data
 */
void rand_oracle_H(
            OUT u08* seed, 
            IN const Word* msg)
{   
    memset(seed, 0, SEED_BYTES);

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

    memcpy(seed, result, SEED_BYTES);
}

/**
 * @brief Generation of a Random Permutation Matrix
 * @param [out] P An n × n permutation matrix P
 * @param [out] P_inv An n × n permutation matrix P^{-1}
 * @param [in] n n s.t. output n x n matrix
 * @param [in] r A random 256-bit string r
 */
// void gen_perm_mat(OUT gf2m* P, OUT gf2m* P_inv, IN int n, IN const Word* r)
void gen_perm_mat(OUT uint16_t* P, OUT uint16_t* P_inv, IN int n, IN const u08* seed)
{
    /* Shuffle([n], seed) */ 
    shuffle(P, n, seed);

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
void perm(OUT Word* dst_v, IN const Word* src_v, IN const u08* seed)
{   
    gf2m P[PARAM_N] = {0};
    gf2m P_inv[PARAM_N] = {0};
    gen_perm_mat(P, P_inv, PARAM_N, seed);

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
void perm_inv(OUT Word* dst_v, IN const Word* src_v, IN const u08* seed)
{   

    gf2m P[PARAM_N] = {0};
    gf2m P_inv[PARAM_N] = {0};
    gen_perm_mat(P, P_inv, PARAM_N, seed);

    /* dst_v = P * src_V */
    memset(dst_v, 0, (sizeof(Word)) * PARAM_N_WORDS);
    for (size_t i = 0; i < PARAM_N; i++)
    {
        Word bit = ((src_v[P_inv[i] / WORD_BITS] >> (P_inv[i] % WORD_BITS)) & 1);
        dst_v[i / WORD_BITS] ^= bit << (i % WORD_BITS);
    }
}

/**
 * @brief Function to generate error vector
 *
 * @param [out] err_vec error vector
 * @param [in] seed 256-bit seed r
 */
void gen_err_vec(OUT Word* err_vec, IN const u08* seed)
{
    gf2m err[PARAM_N] = {0};
    const Word One = 1;

    shuffle(err, PARAM_N, seed);

    for (int i = 0; i < PARAM_N_WORDS; i++)
    {
     err_vec[i] = 0x0;
    }

    for (int i = 0; i < PARAM_T; i++)
    {
        err_vec[err[i] / WORD_BITS] |= (One << (err[i] % WORD_BITS));
    }
}

/**
 * @brief Get error vector's Hamming Weight
 *
 * @param [in] err_vec Error vector.
 * @return Hamming weight
 */
int get_Hamming_weight(IN const Word* err_vec)
{
    Word weight = 0;

#if WORD == 32
    const u32 m1 = 0x55555555;  
    const u32 m2 = 0x33333333;  
    const u32 m4 = 0x0f0f0f0f;  
    const u32 h01 = 0x01010101; 
    const u32 shift = 24;
#elif WORD == 64
    const u64 m1 = 0x5555555555555555;  
    const u64 m2 = 0x3333333333333333;  
    const u64 m4 = 0x0f0f0f0f0f0f0f0f;  
    const u64 h01 = 0x0101010101010101; 
    const u64 shift = 56;
#endif

    for (int i = 0; i < (PARAM_N / WORD_BITS); i++)
    {
        Word x = err_vec[i];

        x -= (x >> 1) & m1;             
        x = (x & m2) + ((x >> 2) & m2); 
        x = (x + (x >> 4)) & m4;
        weight += (x * h01) >> shift;    
    }
    return (int)weight;
}

/**
 * @brief Compares two arrays of type Word and checks if they are equal.
 *
 * @param [in] src1 The first array to compare.
 * @param [in] src2 The second array to compare.
 * @param [in] word_len The length of arrays.
 * 
 * @return int Returns 1 if all elements in the arrays are equal, 0 otherwise.
 */
int is_equal(const Word* src1, const Word* src2, int word_len)
{
    int flag = 1; /* YES */
    for (int i = 0; i < word_len; i++)
        flag &= (src1[i] == src2[i]);
    return flag;
}