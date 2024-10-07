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

/*
    This file is for PALOMA Constants
*/

#ifndef PALOMA_CONSTANT_H
#define PALOMA_CONSTANT_H

#include "paloma_param.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif

/* *************************************** */
/* Variables for PARAM_N, K */
#define PARAM_N_WORDS   (PARAM_N / WORD_BITS)
#define PARAM_N_BYTES   (PARAM_N / 8)
#define PARAM_K_WORDS   (PARAM_K / WORD_BITS)

/* *************************************** */
/* HP Matrix : mt(=n-k) * n */
#define HP_NROWS        (PARAM_M * PARAM_T)                 // HP Matrix's row bit size : mt(= n-k)
#define HP_NROWS_WORDS  (PARAM_M * PARAM_T / WORD_BITS)     // HP Matrix's row Word size : mt(=n-k) / Wordbits
#define HP_NCOLS        PARAM_N                             // HP Matrix's column bit size : n
#define HP_NCOLS_WORDS  (PARAM_N / WORD_BITS)               // HP Matrix's column Word size : n
#define HP_WORDS        (HP_NROWS * HP_NCOLS / WORD_BITS)   // HP Matrix's total Word size
#define HP_NK_WORDS     (HP_NROWS * HP_NROWS / WORD_BITS)   // HP Matrix's Word size - Public key(H_hat[n-k:n])'s Word size : mtmt / Wordbits

/* *************************************** */
/* Public Key : mt(=n-k) * k */
#define PK_NROWS        (PARAM_M * PARAM_T)                 // Public key(H_hat[n-k:n])'s row bit size : mt(=n-k)
#define PK_NROWS_WORDS  (PARAM_M * PARAM_T / WORD_BITS)     // Public key(H_hat[n-k:n])'s row Word size : mt(=n-k) / Wordbits
#define PK_NCOLS        PARAM_K                             // Public key(H_hat[n-k:n])'s column bit size : k
#define PK_WORDS        (PK_NROWS * PK_NCOLS / WORD_BITS)   // Public key(H_hat[n-k:n])'s Word size

/* *************************************** */
/* S^{-1} Matrix : mt(=n-k) * mt(=n-k) */
#define S_INV_WORDS     (HP_NROWS * HP_NROWS / WORD_BITS)   // S_inv Matrix's Word size : mtmt / Wordbits
#define S_INV_BYTES     (HP_NROWS * HP_NROWS / 8)           // S_inv Matrix's byte size : mtmt / 8

/* *************************************** */
/* Syndrome : mt(=n-k) */
#define SYND_BITS       (PARAM_N - PARAM_K)                 // Syndrome Vector's bit size : mt(= n-k)
#define SYND_BYTES      ((PARAM_N - PARAM_K) / 8)           // Syndrome Vector's byte size : mt(= n-k) / 8
#define SYND_WORDS      ((PARAM_N - PARAM_K) / WORD_BITS)   // Syndrome Vector's Word size : mt(= n-k) / Wordbits

/* *************************************** */
/* ROH Input: e_tilde || r_hat || s_hat */
#define ROH_INPUT_BITS  (PARAM_N + SEED_BITS + (PARAM_N - PARAM_K))
#define ROH_INPUT_WORDS (ROH_INPUT_BITS / WORD_BITS)

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* PALOMA_CONSTANT_H */