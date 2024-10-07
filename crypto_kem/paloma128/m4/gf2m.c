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

#include "gf2m.h"

/**
 * @brief Addition function of finite field elements F2m
 *
 * @param [in] in1 finite field elements F2m
 * @param [in] in2 finite field elements F2m
 * @return result
 */
gf2m gf2m_add(IN gf2m in1, IN gf2m in2)
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
gf2m gf2m_mul(IN gf2m in1, gf2m in2)
{
    in1 &= GF2M_BITMASK;
    in2 &= GF2M_BITMASK;

    gf2m result = 0;
    gf2m t1 = in1;
    gf2m t2 = in2;

    for (; t2; t2 >>= 1)
    {
        result ^= (t1 * (t2 & 1));
        if (t1 & 0x1000)
            t1 = ((t1 << 1)) ^ GF2M_IRR_POLY;
        else
            t1 <<= 1;
    }

    return result & GF2M_BITMASK;
}

/**
 * @brief Multiplication function with table
 *
 * @param [in] in1 finite field elements F2m
 * @param [in] in2 finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf2m gf2m_mul_w_tab(IN gf2m in1, IN gf2m in2, IN const gf2m_tab* gf2m_tables)
{
    in1 &= GF2M_BITMASK;
    in2 &= GF2M_BITMASK;

    gf2m result = 0;

    gf2m int1high = (in1 >> GF2M_SPLIT_L) & GF2M_SPLIT_MASK_H_BIT;
    gf2m int1low = (in1)&GF2M_SPLIT_MASK_L_BIT;
    gf2m int2high = (in2 >> GF2M_SPLIT_L) & GF2M_SPLIT_MASK_H_BIT;
    gf2m int2low = (in2)&GF2M_SPLIT_MASK_L_BIT;

    result ^= gf2m_tables->mul11_tab[int1high][int2high];
    result ^= gf2m_tables->mul10_tab[int1high][int2low];
    result ^= gf2m_tables->mul10_tab[int2high][int1low];
    result ^= gf2m_tables->mul00_tab[int1low][int2low];

    return result;
}

/**
 * @brief Square function of finite field elements F2m
 *
 * @param [in] in finite field elements F2m
 * @return result
 */
gf2m gf2m_squ(IN gf2m in)
{
    in &= GF2M_BITMASK;

    return gf2m_mul(in, in);
}

/**
 * @brief Square function with table
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf2m gf2m_squ_w_tab(IN gf2m in, IN const gf2m* squ_tab)
{
    return squ_tab[in];
}

/**
 * @brief Square root function of finite field elements F2m
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf2m gf2m_sqrt(IN gf2m in)
{
    in &= GF2M_BITMASK;
    gf2m result = in;

    for (int i = 0; i < 12; i++) // a^(2^12)
        result = gf2m_squ(result);

    return result & GF2M_BITMASK;
}

/**
 * @brief Square root function with table
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf2m gf2m_sqrt_w_tab(IN gf2m in, IN const gf2m* sqrt_tab)
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
gf2m gf2m_inv(IN gf2m in)
{
    // a^(p-1) = 1 (mod p) -> a^(p-2) = a^-1 (mod p)
    gf2m a = in & GF2M_BITMASK;
    gf2m a_2 = gf2m_squ(a);     // a^2
    gf2m a_4 = gf2m_squ(a_2);   // a^4
    gf2m a_6 = gf2m_mul(a_4, a_2); // a^6
    gf2m a_7 = gf2m_mul(a_6, a);   // a^7
    gf2m a_63 = a_7;

    for (int i = 0; i < 3; i++)
        a_63 = gf2m_squ(a_63); // a^7 -> a^14-> a^28 -> a^56

    a_63 = gf2m_mul(a_63, a_7); // a^63  = a^56 * a^7

    gf2m result = a_63;

    for (int i = 0; i < 6; i++)
        result = gf2m_squ(result); // a^4032

    result = gf2m_mul(result, a_63); // a^4095
    result = gf2m_squ(result);    // a^8190 ... a^13 - 2

    return result & GF2M_BITMASK;
}

/**
 * @brief Inverse function with table
 *
 * @param [in] in finite field elements F2m
 * @param [in] gf2m_tables table
 * @return result
 */
gf2m gf2m_inv_w_tab(IN gf2m in, IN const gf2m* inv_tab)
{
    return inv_tab[in];
}
