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
    This file is for Key Structure
*/

#ifndef PALOMA_DATA_H
#define PALOMA_DATA_H

#include "paloma_constant.h"
#include "gf2m.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

/* ***************************************
    Secret Key = (L, g(X), S_inv, r_C_hat, r)
*/
typedef struct{
    gf2m    L[PARAM_N];
    gf2m    gX[PARAM_T];
    u08     S_inv[S_INV_BYTES];
    u08     r_C_hat[SEED_BYTES];
    u08     r[SEED_BYTES];
} SecretKey;

/* ***************************************
    Public Key = H_hat
*/
typedef struct{
    u08    H_hat[PK_BYTES];
} PublicKey;

/* ***************************************
    Ciphertext = (r_hat, s_hat)
*/
typedef struct{
    u08 r_hat[SEED_BYTES];
    Word    s_hat[SYND_WORDS];
} Ciphertext;

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* PALOMA_DATA_H */