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

#include "gf_tab_gen.h"



/**
 * @brief generate precomputation table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void gen_precomputation_tab(OUT gf2m_tab* gf2m_tables)
{
    gen_mul_tab(gf2m_tables);
    gen_square_tab(gf2m_tables->square_tab);
    gen_sqrt_tab(gf2m_tables->sqrt_tab);
    gen_inv_tab(gf2m_tables->inv_tab);
}

/**
 * @brief generate gf2m multiplication table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void gen_mul_tab(OUT gf2m_tab* gf2m_tables)
{
    gf A1_z7, B1_z7;

    for (gf i = 0; i < (1 << GF2M_SPLIT_LOW); i++)
    {
        for (gf j = 0; j < (1 << GF2M_SPLIT_LOW); j++)
        {
            gf2m_tables->mul_00_tab[i][j] = gf2m_mul(i, j); // lower 7bit

            if ((i < (1 << GF2M_SPLIT_HIGH)) && (j < (1 << GF2M_SPLIT_HIGH)))
            {
                A1_z7 = (i << GF2M_SPLIT_LOW); // times z^7
                B1_z7 = (j << GF2M_SPLIT_LOW); // times z^7

                // upper 6bit
                gf2m_tables->mul_11_tab[i][j] = gf2m_mul(A1_z7, B1_z7); 
            }

            if (i < (1 << GF2M_SPLIT_HIGH))
            {
                A1_z7 = (i << GF2M_SPLIT_LOW); // times z^7
                
                // upper 6bit x lower 7bit
                gf2m_tables->mul_10_tab[i][j] = gf2m_mul(A1_z7, j); 
            }
        }
    }
}

/**
 * @brief generate gf2m square table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void gen_square_tab(OUT gf* square_tab)
{
    for (gf i = 0; i < BITSIZE; i++)
    {
        square_tab[i] = gf2m_square(i);
    }
}

/**
 * @brief generate gf2m square root table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void gen_sqrt_tab(OUT gf* sqrt_tab)
{
    for (gf i = 0; i < BITSIZE; i++)
    {
        sqrt_tab[i] = gf2m_sqrt(i);
    }
}

/**
 * @brief generate gf2m inverse table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void gen_inv_tab(OUT gf* inv_tab)
{
    for (gf i = 0; i < BITSIZE; i++)
    {
        inv_tab[i] = gf2m_inv(i);
    }
}


/**
 * @brief gf2m table verification
 *
 * @param [in] gf2m_tables tables for efficient arithmetic over GF(2^m).
 */
void tab_verify_check(IN const gf2m_tab* gf2m_tables)
{
    int count = 1000000;
    srand(time(NULL));

    /**************  Multiplication  **************/
    gf finite_a, finite_b;
    finite_a = rand() % 0x1FFF;
    finite_b = rand() % 0x1FFF;
    gf finite_c = finite_a;

    for (int i = 0; i <= count; i++)
    {
        finite_a = gf2m_mul(finite_a, finite_b);
        finite_c = gf2m_mul_w_tab(finite_c, finite_b, gf2m_tables);
        if (finite_a != finite_c)
            return;
    }

    /**************  Square  **************/
    finite_a = rand() % 0x1FFF;
    finite_c = finite_a;

    for (int i = 0; i <= count; i++)
    {
        finite_a = gf2m_square(finite_a);
        finite_c = gf2m_square_w_tab(finite_c, gf2m_tables->square_tab);
        if (finite_a != finite_c)
            return;
    }

    /**************  Square root  **************/
    finite_a = rand() % 0x1FFF;
    finite_c = finite_a;

    for (int i = 0; i <= count; i++)
    {
        finite_a = gf2m_sqrt(finite_a);
        finite_c = gf2m_sqrt_w_tab(finite_c, gf2m_tables->sqrt_tab);
        if (finite_a != finite_c)
            return;    
        }

    /**************  Inverse  **************/
    finite_a = rand() % 0x1FFF;
    finite_c = finite_a;

    for (int i = 0; i <= count; i++)
    {
        finite_a = gf2m_inv(finite_a);
        finite_c = gf2m_inv_w_tab(finite_c, gf2m_tables->inv_tab);
        if (finite_a != finite_c)
            return;
    }
}