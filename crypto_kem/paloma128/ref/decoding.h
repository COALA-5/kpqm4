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
    This file is for decoding
*/

#ifndef DECODING_H_
#define DECODING_H_

#include "paloma_data.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

void to_poly(OUT gf2m* sX, IN const Word* s);

void construct_key_eqn(OUT gf2m* vX, OUT gf2m* g1X, OUT gf2m* g2X, OUT gf2m* g12X, 
                       IN const gf2m* sX, IN const gf2m* gX, 
                       IN const gf2m_tab* gf2m_tables);

void solve_key_eqn(OUT gf2m* a1X, OUT gf2m* b2X, IN const gf2m* vX, IN const gf2m* g12X,
                   IN int deg_a, IN int deg_b, IN const gf2m_tab* gf2m_tables);
                   
void get_err_loc_poly(OUT gf2m* sigX, IN const gf2m* a2X, IN const gf2m* g2X, 
                      IN const gf2m* b2X, IN const gf2m* g1X, 
                      IN const gf2m_tab* gf2m_tables);

void comp_err_loc_poly(OUT gf2m* sigX, IN const gf2m* sX, IN const gf2m* gX, 
                       IN const gf2m_tab* gf2m_tables);

void div_X2n_Xn(OUT gf2m* f, IN int len);
void rearrange_f0f1(OUT gf2m* list, IN int len);
void get_f0f1(OUT gf2m* f, IN int len);
void get_twist_poly(OUT gf2m* f, IN int len, IN gf2m twist, IN const gf2m_tab* gf2m_tabs);

void get_le_GG_DD(OUT gf2m* last_elt, OUT gf2m* GG, OUT gf2m* DD, 
                  IN int len, IN const gf2m_tab* gf2m_tabs);

void get_all_elts(OUT gf2m* all_elts, IN gf2m* basis, IN int len);

void convert(OUT gf2m* list, IN gf2m* g_allelts, IN int basis_len,
             IN const gf2m_tab* gf2m_tabs);

gf2m reversal(IN gf2m v);

void find_err_vec_fft(OUT Word* err_vec, IN const gf2m* sigX, IN const gf2m* L, 
                      IN const gf2m_tab* gf2m_tables);

void recover_err_vec(OUT Word* err_vec, IN const SecretKey* sk, 
                     IN const Word* synd_vec, IN const gf2m_tab* gf2m_tables);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif