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
    This file is for generate precomputation table
*/

#ifndef GF2M_TABLE_GEN_H
#define GF2M_TABLE_GEN_H

#include "gf2m.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

// --------------------------------------------------------
// Generate Precomputation Tables - GF(2^13)
// --------------------------------------------------------
// - Multiplication Table
// - Square Table
// - SquareRoot Table
// - Inverse Table
// --------------------------------------------------------
void gf2m_gen_all_tabs(OUT gf2m_tab* gf2m_tables);
// --------------------------------------------------------

void gf2m_gen_mul_tab(OUT gf2m_tab* gf2m_tables);
void gf2m_gen_squ_tab(OUT gf2m* squ_tab);
void gf2m_gen_inv_tab(OUT gf2m* inv_tab);
void gf2m_gen_sqrt_tab(OUT gf2m* sqrt_tab);

void gf2m_print_all_tabs(IN const gf2m_tab* gf2m_tables);

void gf2m_print_mul_tab(IN const gf2m_tab* gf2m_tables);
void gf2m_print_squ_tab(IN const gf2m* squ_tab);
void gf2m_print_inv_tab(IN const gf2m* inv_tab);
void gf2m_print_sqrt_tab(IN const gf2m* sqrt_tab);

void gf2m_benchmark(IN const gf2m_tab* gf2m_tables);
void gf2m_tab_verify_check(IN const gf2m_tab* gf2m_tables);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* GF2M_TABLE_GEN_H */