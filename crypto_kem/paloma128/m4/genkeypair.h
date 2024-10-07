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
    This file is for key generation
*/

#ifndef GENKEYPAIR_H
#define GENKEYPAIR_H

#include "paloma_data.h"
#include "gf2m_poly.h"
#include "mat_mul.h"
#include "utility.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

void gen_key_pair(
        OUT PublicKey* pk, 
        OUT SecretKey* sk, 
        IN const gf2m_tab* gf2m_tables);

void gen_goppa_poly(
        OUT gf2m* gx, 
        IN const gf2m* gf_set, 
        IN const gf2m_tab* gf2m_tables);

void gen_parity_check_mat(
        OUT Word* H, 
        IN const gf2m* gf_poly, 
        IN const gf2m* support_set, 
        IN const gf2m_tab* gf2m_tables);

void gen_rand_goppa_code(
        OUT u08* r_C, 
        OUT gf2m* support_set, 
        OUT gf2m* goppa_poly,
        OUT Word* H, 
        IN const gf2m_tab* gf2m_tables);

void gen_scrambled_code(
        OUT u08* r_C_hat, 
        OUT Word* S_inv, 
        OUT Word* H_hat, 
        IN const Word* H);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* GENKEYPAIR_H */