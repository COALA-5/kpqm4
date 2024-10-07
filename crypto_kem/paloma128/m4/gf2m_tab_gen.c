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

#include "gf2m_tab_gen.h"
#include <stdint.h>

/**
 * @brief print all table
 *
 * @param [in] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
void gf2m_print_all_tabs(IN const gf2m_tab* gf2m_tables)
{
    gf2m_print_mul_tab(gf2m_tables);
    gf2m_print_squ_tab(gf2m_tables->squ_tab);
    gf2m_print_sqrt_tab(gf2m_tables->sqrt_tab);
    gf2m_print_inv_tab(gf2m_tables->inv_tab);
}

/**
 * @brief generate precomputation table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
void gf2m_gen_all_tabs(OUT gf2m_tab* gf2m_tables)
{
    gf2m_gen_mul_tab(gf2m_tables);
    gf2m_gen_squ_tab(gf2m_tables->squ_tab);
    gf2m_gen_sqrt_tab(gf2m_tables->sqrt_tab);
    gf2m_gen_inv_tab(gf2m_tables->inv_tab);
}

/**
 * @brief generate gf2m multiplication table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
void gf2m_gen_mul_tab(OUT gf2m_tab* gf2m_tables)
{
    gf2m A1_z7, B1_z7;

    for (gf2m i = 0; i < (1 << GF2M_SPLIT_L); i++)
    {
        for (gf2m j = 0; j < (1 << GF2M_SPLIT_L); j++)
        {
            gf2m_tables->mul00_tab[i][j] = gf2m_mul(i, j); // lower 7bit

            if ((i < (1 << GF2M_SPLIT_H)) && (j < (1 << GF2M_SPLIT_H)))
            {
                A1_z7 = (i << GF2M_SPLIT_L); // times z^7
                B1_z7 = (j << GF2M_SPLIT_L); // times z^7

                // upper 6bit
                gf2m_tables->mul11_tab[i][j] = gf2m_mul(A1_z7, B1_z7); 
            }

            if (i < (1 << GF2M_SPLIT_H))
            {
                A1_z7 = (i << GF2M_SPLIT_L); // times z^7
                
                // upper 6bit x lower 7bit
                gf2m_tables->mul10_tab[i][j] = gf2m_mul(A1_z7, j); 
            }
        }
    }
}

/**
 * @brief generate gf2m square table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
void gf2m_gen_squ_tab(OUT gf2m* squ_tab)
{
    for (gf2m i = 0; i < GF2M_SIZE; i++)
    {
        squ_tab[i] = gf2m_squ(i);
    }
}

/**
 * @brief generate gf2m square root table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
void gf2m_gen_sqrt_tab(OUT gf2m* sqrt_tab)
{
    for (gf2m i = 0; i < GF2M_SIZE; i++)
    {
        sqrt_tab[i] = gf2m_sqrt(i);
    }
}

/**
 * @brief generate gf2m inverse table
 *
 * @param [out] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
void gf2m_gen_inv_tab(OUT gf2m* inv_tab)
{
    for (gf2m i = 0; i < GF2M_SIZE; i++)
    {
        inv_tab[i] = gf2m_inv(i);
    }
}


/**
 * @brief gf2m table performance measurement function
 *
 * @param [in] gf2m_tables tables for efficient arithmetic over GF(2^13).
 */
#define measure_time_gf2m(statement, oper, type, count) \
    do{ \
    start = clock(); \
    for (int i = 0; i < count; i++) statement; \
    end   = clock(); \
    res = (double)(end - start) / CLOCKS_PER_SEC; \
} while(0)

void gf2m_benchmark(IN const gf2m_tab* gf2m_tables)
{
    // srand(time(NULL));

    clock_t start, end;
    double res;
    int count = 10000000;
    gf2m finite_a, finite_b;

    /**************  Addition  **************/
    finite_a = rand() % 0x1FFF;
    finite_b = rand() % 0x1FFF;

    measure_time_gf2m(finite_a = gf2m_add(finite_a, finite_b), "ADD ", "", count);

    /**************  Multiplication  **************/
    finite_a = rand() % 0x1FFF;
    finite_b = rand() % 0x1FFF;

    measure_time_gf2m(finite_a = gf2m_mul(finite_a, finite_b), "MUL ", "", count);
    measure_time_gf2m(finite_a = gf2m_mul_w_tab(finite_a, finite_b, gf2m_tables), "MUL ", "with tab.", count);

    /**************  Square  **************/
    finite_a = rand() % 0x1FFF;

    measure_time_gf2m(finite_a = gf2m_squ(finite_a), "SQU ", "", count);
    measure_time_gf2m(finite_a = gf2m_squ_w_tab(finite_a, gf2m_tables->squ_tab), "SQU ", "with tab.", count);

    /**************  Square root  **************/
    finite_a = rand() % 0x1FFF;

    measure_time_gf2m(finite_a = gf2m_sqrt(finite_a), "SQRT", "", count);
    measure_time_gf2m(finite_a = gf2m_sqrt_w_tab(finite_a, gf2m_tables->sqrt_tab), "SQRT", "with tab.", count);

    /**************  Inverse  **************/
    finite_a = rand() % 0x1FFF;

    measure_time_gf2m(finite_a = gf2m_inv(finite_a), "INV ", "", count);
    measure_time_gf2m(finite_a = gf2m_inv_w_tab(finite_a, gf2m_tables->inv_tab), "INV ", "with tab.", count);

}
