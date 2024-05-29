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
    This file is for gf arithmetic operation
*/

#ifndef GF_H
#define GF_H

#include "paloma_constant.h"

/* Multiplication Pre-computation Table  
mul_11_tab: A1(z)z^7 * B1(z)z^7 mod f(z) (upper 6bit x upper 6bit)
mul_10_tab: A1(z)z^7 * B0(z),A0(z) * B1(z)z^7 mod f(z) (upper 6bit x lower 7bit)
mul_00_tab: A0(z) * B0(z) mod f(z) (lower 7bit x lower 7bit) */
typedef struct
{
    // Multiplication Pre-computation Table
    gf mul_11_tab[1 << GF2M_SPLIT_HIGH][1 << GF2M_SPLIT_HIGH]; 
    gf mul_10_tab[1 << GF2M_SPLIT_HIGH][1 << GF2M_SPLIT_LOW];  
    gf mul_00_tab[1 << GF2M_SPLIT_LOW][1 << GF2M_SPLIT_LOW];   

    gf square_tab[BITSIZE]; // Square Pre-computation Table
    gf sqrt_tab[BITSIZE];   // Square root Pre-computation Table
    gf inv_tab[BITSIZE];    // Inverse Pre-computation Table

} gf2m_tab;

void gf2m_print(IN gf in);

gf gf2m_add(IN gf in1, gf in2);

gf gf2m_mul(IN gf in1, gf in2);
gf gf2m_mul_w_tab(IN gf in1, IN gf in2, IN const gf2m_tab* gf2m_tables);

gf gf2m_square(IN gf in);
gf gf2m_square_w_tab(IN gf in, IN const gf* square_tab);

gf gf2m_sqrt(IN gf in);
gf gf2m_sqrt_w_tab(IN gf in, IN const gf* sqrt_tab);

gf gf2m_inv(IN gf in);
gf gf2m_inv_w_tab(IN gf in, IN const gf* inv_tab);

#endif