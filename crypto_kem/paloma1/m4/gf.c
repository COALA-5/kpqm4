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

#include "gf.h"

/**
 * @brief Addition function of finite field elements F2m
 *
 * @param [in] in1 finite field elements F2m
 * @param [in] in2 finite field elements F2m
 * @return result
 */
gf gf2m_add(IN gf in1, IN gf in2)
{
    return in1 ^ in2;
}

/**
 * @brief Multiplication function of finite field elements F2m
 *
 * @param [in] in1 finite field elements F2m
 * @param [in] in2 finite field elements F2m
 * @return result
 */
gf gf2m_mul(IN gf in1, gf in2)
{
    in1 &= MASKBITS;
    in2 &= MASKBITS;

    gf result = 0;
    gf t1 = in1;
    gf t2 = in2;

    for (; t2; t2 >>= 1)
    {
        result ^= (t1 * (t2 & 1));
        if (t1 & 0x1000)
            t1 = ((t1 << 1)) ^ IRR_POLY;
        else
            t1 <<= 1;
    }

    return result & MASKBITS;
}

/**
 * @brief Multiplication function with table
 *
 * @param [in] in1 finite field elements F2m
 * @param [in] in2 finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf gf2m_mul_w_tab(IN gf in1, IN gf in2, IN const gf2m_tab* gf2m_tables)
{
    in1 &= MASKBITS;
    in2 &= MASKBITS;

    gf result = 0;

    gf int1high = (in1 >> GF2M_SPLIT_LOW) & GF2M_SPLIT_MASK_HIGH_BIT;
    gf int1low = (in1)&GF2M_SPLIT_MASK_LOW_BIT;
    gf int2high = (in2 >> GF2M_SPLIT_LOW) & GF2M_SPLIT_MASK_HIGH_BIT;
    gf int2low = (in2)&GF2M_SPLIT_MASK_LOW_BIT;

    result ^= gf2m_tables->mul_11_tab[int1high][int2high];
    result ^= gf2m_tables->mul_10_tab[int1high][int2low];
    result ^= gf2m_tables->mul_10_tab[int2high][int1low];
    result ^= gf2m_tables->mul_00_tab[int1low][int2low];

    return result;
}

/**
 * @brief Square function of finite field elements F2m
 *
 * @param [in] in finite field elements F2m
 * @return result
 */
gf gf2m_square(IN gf in)
{
    in &= MASKBITS;

    return gf2m_mul(in, in);
}

/**
 * @brief Square function with table
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf gf2m_square_w_tab(IN gf in, IN const gf* square_tab)
{
    return square_tab[in];
}

/**
 * @brief Square root function of finite field elements F2m
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf gf2m_sqrt(IN gf in)
{
    in &= MASKBITS;
    gf result = in;

    for (int i = 0; i < 12; i++) // a^(2^12)
        result = gf2m_square(result);

    return result & MASKBITS;
}

/**
 * @brief Square root function with table
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf gf2m_sqrt_w_tab(IN gf in, IN const gf* sqrt_tab)
{
    return sqrt_tab[in];
}

/**
 * @brief Inverse function of finite field elements F2m
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf gf2m_inv(IN gf in)
{
    // a^(p-1) = 1 (mod p) -> a^(p-2) = a^-1 (mod p)
    gf a = in & MASKBITS;
    gf a_2 = gf2m_square(a);     // a^2
    gf a_4 = gf2m_square(a_2);   // a^4
    gf a_6 = gf2m_mul(a_4, a_2); // a^6
    gf a_7 = gf2m_mul(a_6, a);   // a^7
    gf a_63 = a_7;

    for (int i = 0; i < 3; i++)
        a_63 = gf2m_square(a_63); // a^7 -> a^14-> a^28 -> a^56

    a_63 = gf2m_mul(a_63, a_7); // a^63  = a^56 * a^7

    gf result = a_63;

    for (int i = 0; i < 6; i++)
        result = gf2m_square(result); // a^4032

    result = gf2m_mul(result, a_63); // a^4095
    result = gf2m_square(result);    // a^8190 ... a^13 - 2

    return result & MASKBITS;
}

/**
 * @brief Inverse function with table
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf gf2m_inv_w_tab(IN gf in, IN const gf* inv_tab)
{
    return inv_tab[in];
}
