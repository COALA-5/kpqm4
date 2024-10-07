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
    This file is for gf2m polynomial arithmetic operation
*/

#ifndef GF_POLY_H
#define GF_POLY_H

#include "paloma_data.h"
#include "gf2m.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

/* GF Polynomial : def(gf_poly) = t -> #coef(gf_poly) = t+1 */
#define GF_POLY_LEN     (PARAM_T + 1)

/* *************************************** */

int     gf2m_poly_get_deg(IN const gf2m* aX);
void    gf2m_poly_copy(OUT gf2m* cX, IN const gf2m* aX);
void    gf2m_poly_get_monic(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m_tab* gf2m_tables);

void    gf2m_poly_add(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* bX);
void    gf2m_poly_mul(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* bX, IN const gf2m_tab* gf2m_tables);
void    gf2m_poly_div(OUT gf2m* qx, OUT gf2m* rx, IN const gf2m* aX, IN const gf2m* bX, IN const gf2m_tab* gf2m_tables);
void    gf2m_poly_gcd(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* bX, IN const gf2m_tab* gf2m_tables);

void    gf2m_poly_mul_mod(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* bX, IN const gf2m* gX, IN const gf2m_tab* gf2m_tables);
void    gf2m_poly_squ_mod(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* gX, IN const gf2m_tab* gf2m_tables);
void    gf2m_poly_sqrt_mod(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* gX, IN const gf2m_tab* gf2m_tables);
void    gf2m_poly_inv_mod(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* gX, IN const gf2m_tab* gf2m_tables);

gf2m    gf2m_poly_eval(IN const gf2m* aX, IN gf2m gf_a, IN const gf2m_tab* gf2m_tables);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif