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
    This file is for gf polynomial arithmetic operation
*/

#ifndef GF_POLY_H
#define GF_POLY_H

#include "gf.h"

int gf_poly_get_degree(IN const gf* aX);

void gf_poly_copy(OUT gf* cX, IN const gf* aX);

void gf_poly_change_monic(OUT gf* cX, IN const gf* aX, 
                          IN const gf2m_tab* gf2m_tables);

void gf_poly_add(OUT gf* cX, IN const gf* aX, IN const gf* bX);

void poly_mul(OUT gf* cX, IN const gf* aX, IN const gf* bX, 
              IN const gf2m_tab* gf2m_tables);

void gf_poly_mul(OUT gf* cX, IN const gf* aX, IN const gf* bX, IN const gf* gX, 
                 IN const gf2m_tab* gf2m_tables);

void gf_poly_sqr(OUT gf* cX, IN const gf* aX, IN const gf* gX, 
                 IN const gf2m_tab* gf2m_tables);

void gf_poly_sqrt(OUT gf* cX, IN const gf* aX, IN const gf* gX, 
                  IN const gf2m_tab* gf2m_tables);

void gf_poly_div(OUT gf* qx, OUT gf* rx, IN const gf* aX, IN const gf* bX, 
                 IN const gf2m_tab* gf2m_tables);

void gf_poly_inv(OUT gf* cX, IN const gf* aX, IN const gf* gX, 
                 IN const gf2m_tab* gf2m_tables);

gf gf_poly_eval(IN const gf* aX, IN gf gf_a, IN const gf2m_tab* gf2m_tables);

void gf_poly_gcd(OUT gf* cX, IN const gf* aX, IN const gf* bX, 
                 IN const gf2m_tab* gf2m_tables);

#endif