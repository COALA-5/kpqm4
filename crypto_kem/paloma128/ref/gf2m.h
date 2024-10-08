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
    This file is for gf2m arithmetic operation
*/

#ifndef GF2M_H
#define GF2M_H

#include "paloma_data.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

/* all elements of F2m is stored in 2-byte as follows.
    
    z^12 + ... + a_0z^0 <-> 0 || 0 || 0 || a_12 || a_11 || .... || a_0

*/

/* *************************************** */
/* Variables for Multiplication Precalculation Tables */

/* Variables for input validation */
#define GF2M_SIZE       (1 << PARAM_M)
#define GF2M_BITMASK    (GF2M_SIZE - 1)

#define GF2M_IRR_POLY           0b10000011100001 // x^13 + x^7 + x^6 + x^5 + 1
#define GF2M_SPLIT_L            7
#define GF2M_SPLIT_H            (PARAM_M - GF2M_SPLIT_L)    // 6
#define GF2M_SPLIT_MASK_L_BIT   ((1 << GF2M_SPLIT_L) - 1)   // 127
#define GF2M_SPLIT_MASK_H_BIT   ((1 << GF2M_SPLIT_H) - 1)   // 63

/* *************************************** */

/* Multiplication Pre-computation Table  
mul11_tab: A1(z)z^7 * B1(z)z^7 mod f(z) (upper 6bit x upper 6bit)
mul10_tab: A1(z)z^7 * B0(z),A0(z) * B1(z)z^7 mod f(z) (upper 6bit x lower 7bit)
mul00_tab: A0(z) * B0(z) mod f(z) (lower 7bit x lower 7bit) */
typedef struct
{
    // Multiplication Pre-computation Table
    gf2m mul11_tab[1 << GF2M_SPLIT_H][1 << GF2M_SPLIT_H]; 
    gf2m mul10_tab[1 << GF2M_SPLIT_H][1 << GF2M_SPLIT_L];  
    gf2m mul00_tab[1 << GF2M_SPLIT_L][1 << GF2M_SPLIT_L];   
    
    gf2m squ_tab[GF2M_SIZE];  // Square Pre-computation Table
    gf2m inv_tab[GF2M_SIZE];  // Inverse Pre-computation Table
    gf2m sqrt_tab[GF2M_SIZE]; // Square root Pre-computation Table
} gf2m_tab;

void gf2m_print(IN gf2m in);

gf2m gf2m_add(IN gf2m in1, gf2m in2);

gf2m gf2m_mul(IN gf2m in1, gf2m in2);
gf2m gf2m_mul_w_tab(IN gf2m in1, IN gf2m in2, IN const gf2m_tab* gf2m_tables);

gf2m gf2m_squ(IN gf2m in);
gf2m gf2m_squ_w_tab(IN gf2m in, IN const gf2m* squ_tab);

gf2m gf2m_sqrt(IN gf2m in);
gf2m gf2m_sqrt_w_tab(IN gf2m in, IN const gf2m* sqrt_tab);

gf2m gf2m_inv(IN gf2m in);
gf2m gf2m_inv_w_tab(IN gf2m in, IN const gf2m* inv_tab);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* GF2M_H */