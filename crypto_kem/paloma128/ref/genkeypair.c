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

#include "genkeypair.h"

/**
 * @brief generate goppa polynomial (x-a_1)(x-a_2)...(x-a_t).
 *
 * @param [out] gx goppa polynomial.
 * @param [in] gf_set number t GF(2^m) elements.
 * @param [in] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void gen_goppa_poly(
            OUT gf2m* gx, 
            IN const gf2m* gf_set, 
            IN const gf2m_tab* gf2m_tables)
{
    gf2m t_gx[GF_POLY_LEN] = {0}; // gx_temp
    gf2m temp_result[GF_POLY_LEN] = {0};

    /* init: set g(X) = (X - a_0) */
    t_gx[0] = gf_set[0];
    t_gx[1] = 1;

    /* compute (X - a_{t-1}) * ... * (X - a_1) * (X - a_0) */
    for (int i = 1; i < PARAM_T; i++)
    {
        /* multiplicate g(X) with (X - a_i) */
        for (int j = 0; j <= i; j++)
        {   
            temp_result[j + 1] = t_gx[j];   // t(x) = g(x) * x                    
            // t(x) = g(x) * x + g(x) * a_i = g(x) * (x - a_i)   
            temp_result[j] ^= gf2m_mul_w_tab(t_gx[j], gf_set[i], gf2m_tables); 
            t_gx[j] = temp_result[j];       // new g(x) = t(x)
        }

        /* final compute */
        t_gx[i + 1] = temp_result[i + 1];

        /* set zero */
        temp_result[0] = 0;
    }

    /* return */
    for (int i = 0; i < PARAM_T; i++)
        gx[i] = t_gx[i];
}

/**
 * @brief Generate Parity-Check Matrix H
 *
 * @param [out] H Parity-Check Matrix H
 * @param [in] gf_poly goppa polynomial
 * @param [in] support_set support set
 * @param [in] gf2m_tables GF(2^m) Arith Precomputation Table
 */
void gen_parity_check_mat( /* memory dynamic allocation */
            OUT Word* H, 
            IN const gf2m* gf_poly, 
            IN const gf2m* support_set, 
            IN const gf2m_tab* gf2m_tables)
{
    gf2m* mat_BC;
    get_new_data(gf2m, mat_BC, PARAM_T * PARAM_N);

    gf2m* mat_ABC;
    get_new_data(gf2m, mat_ABC, PARAM_T * PARAM_N);

    gf2m tmp_gf_poly[GF_POLY_LEN];

    /* Convert 'gf_poly(degree : t-1)' -> monic gx 'tmp_gf_poly(degree : t)' */
    memcpy(tmp_gf_poly, gf_poly, sizeof(gf2m) * PARAM_T);
    tmp_gf_poly[PARAM_T] = 1;

    /* Generate ABC (\in F_{2^{13}[t][n] ) */
    for (int i = 0; i < PARAM_N; i++)
    {
        gf2m tmp = 1;
        gf2m HHH[PARAM_T];
        HHH[PARAM_T - 1] = 1;

        for (int j = 0; j < PARAM_T - 1; j++)
        {
            tmp = gf2m_mul_w_tab(tmp, support_set[i], gf2m_tables) ^ tmp_gf_poly[PARAM_T - 1 - j];
            HHH[PARAM_T - 2 - j] = tmp;
        }

        tmp = gf2m_mul_w_tab(tmp, support_set[i], gf2m_tables) ^ tmp_gf_poly[0]; /* = g(a) */
        tmp = gf2m_inv_w_tab(tmp, gf2m_tables->inv_tab); /* g(a)^-1 */

        for (int j = 0; j < PARAM_T; j++)
        {
            *(mat_ABC + PARAM_N*j + i) = gf2m_mul_w_tab(HHH[j], tmp, gf2m_tables);
        }
    }

    /* Matrix ABC -> WORD_BITS-bit(Word)  */
    int row;
    Word tmp_Word;

    u08 cnt_remainder_Word = 0;
    u08 idx_remainder_Word = WORD_BITS - PARAM_M;

    for (int j = 0; j < PARAM_N; j++)
    {
        row = 0;
        for (int i = 0; i < PARAM_T; i++)
        {   
            // Check if the remaining space in the current Word is enough to fit the GF(2^m) element.
            if (cnt_remainder_Word < idx_remainder_Word)
            {
                H[row + j * PK_NROWS_WORDS] |= (((Word)(*(mat_ABC + PARAM_N*i + j))) << cnt_remainder_Word);

                cnt_remainder_Word += PARAM_M;
            }
            else
            {   
                // Split the GF(2^m) element across two Word elements in H.
                tmp_Word = (*(mat_ABC + PARAM_N*i + j)) & ((1 << (WORD_BITS - cnt_remainder_Word)) - 1);
                H[row + j * PK_NROWS_WORDS] |= (tmp_Word << cnt_remainder_Word);
                H[row + j * PK_NROWS_WORDS + 1] |= ((*(mat_ABC + PARAM_N*i + j)) >> (WORD_BITS - cnt_remainder_Word));

                row++;
                cnt_remainder_Word -= idx_remainder_Word;
            }
        }
    }

    free(mat_BC);
    free(mat_ABC);
}


/**
 * @brief Generate random Goppa code
 *
 * @param [out] r_C 256-bit seed r for shuffle
 * @param [out] L support set
 * @param [out] gX goppa polynomial
 * @param [out] H Parity-check Matrix H
 * @param [in] gf2m_tables GF(2^m) Arith Precomputation Table
 */
void gen_rand_goppa_code(
            OUT u08* r_C,
            OUT gf2m* L, 
            OUT gf2m* gX, 
            OUT Word* H, 
            IN const gf2m_tab* gf2m_tables)
{
    gf2m gf_set[1 << PARAM_M] = {0};
    gf2m gf4goppapoly[PARAM_T] = {0};

    /* Generate 256-bit seed r_C */
    gen_rand_bytes(r_C, SEED_BYTES);

    /* Shuffle gf_set using seed r_C */
    shuffle(gf_set, (1 << PARAM_M), r_C);

    /* Generate support set : Top n elements used as supportset elements */
    memcpy(L, gf_set, ((sizeof(gf2m)) * PARAM_N)); 

    /* Generate goppa polynomial : next t elements are used as elements for gopa poly */
    memcpy(gf4goppapoly, gf_set + PARAM_N, ((sizeof(gf2m)) * PARAM_T)); 
    gen_goppa_poly(gX, gf4goppapoly, gf2m_tables);

    /* Generate Parity Check Matrix */
    gen_parity_check_mat(H, gX, L, gf2m_tables);
}

/**
 * @brief Generate scrambled code
 *
 * @param [out] r_C_hat random bits r_C_hat for generate random 
 *                      permutation Matrix P
 * @param [out] S_inv Secret Key(Private Key): invertible Matrix S^{-1}
 * @param [out] H_hat a systematic scrambled parity-check Matrix H_hat
 * @param [in] H a Parity-check Matrix H
 */
void gen_scrambled_code(
            OUT u08* r_C_hat, 
            OUT Word* S_inv, 
            OUT Word* H_hat, 
            IN const Word* H)
{
    while (1)
    {   
        gen_rand_bytes(r_C_hat, SEED_BYTES);

        /* Generate Random Permutation Matrix P */
        gf2m P[PARAM_N] = {0};
        gf2m P_inv[PARAM_N] = {0};
        gen_perm_mat(P, P_inv, PARAM_N, r_C_hat);

        /* Generate a systematic scrambled parity-check Matrix H_hat */
        /* [ H_hat | S ] = [ HP | I_{n-k} ] */
        Word* HP;
        get_new_data(Word, HP, HP_WORDS);
        if (HP == NULL) {
            return;
        }
        memset(HP, 0, sizeof(Word) * HP_WORDS);

        /* Generate Matrix H*P */
        for (int i = 0; i < PARAM_N; i++)
        {
            memcpy(HP + (HP_NROWS_WORDS * P[i]), H + (HP_NROWS_WORDS * i), sizeof(Word) * HP_NROWS_WORDS);
        }

        /* S^{-1} : (n-k) x (n-k) */
        memcpy(S_inv, HP, (sizeof(Word) * S_INV_WORDS));

        /* H_hat <- RREF(HP) */
        int check = 0;
        check = gaussian_row(HP, HP);

        /* if H_hat_{[0:n-k]} == I_{n-k} */
        if (check == 0)
        {
            memcpy(H_hat, HP, (sizeof(Word) * HP_WORDS));
            free(HP);
            break;
        }

        free(HP);
    }
}

/**
 * @brief Generate key pair
 *
 * @param [out] H_hat a systematic scrambled parity-check Matrix H_hat
 * @param [out] S_inv Secret Key(Private Key): invertible Matrix S^{-1}
 * @param [out] r_perm_mat random 256-bit r for generate 
 *                         random permutation Matrix P
 * @param [in] H a Parity-check Matrix H
 */
void gen_key_pair(
            OUT PublicKey* pk, 
            OUT SecretKey* sk, 
            IN const gf2m_tab* gf2m_tables)
{
    Word* H;
    get_new_data(Word, H, HP_WORDS);

    u08 r_C[SEED_BYTES] = {0};

    gen_rand_goppa_code(r_C, sk->L, sk->gX, H, gf2m_tables);
    gen_scrambled_code(sk->r_C_hat, (Word*)(sk->S_inv), H, H);
    
    /* pk <- H_hat[n-k:n] (is the sub matrix of H_hat consisting of the last k columns) */
    memcpy(pk->H_hat, H + HP_NK_WORDS, PK_WORDS*sizeof(Word));

    /* r is used in the implicit rejection case */
    gen_rand_bytes(sk->r, SEED_BYTES);

    free(H);
}

