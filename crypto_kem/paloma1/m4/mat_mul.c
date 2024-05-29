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

#include "mat_mul.h"

inline void xor_vec(
            OUT Word* out_vec, 
            IN const Word* in_mat, 
            IN int index, 
            IN int num)
{
    for (int i = 0; i < num; i++)
    {
        out_vec[i] ^= in_mat[index + i];
    }
}

/**
 * @brief Matrix x Vector
 *
 * @param [out] out_vec output Vector
 * @param [in] in_mat  input Matrix
 * @param [in] in_vec  input Vector
 * @param [in] rownum  input Matrix row num
 * @param [in] colnum  input Matrix column num
 */
void matXvec(
            OUT Word* out_vec, 
            IN const Word* in_mat, 
            IN const Word* in_vec, 
            IN int rownum, 
            IN int colnum)
{
    int num_of_WORD_BITS_in_row = rownum / WORD_BITS;
    int col_index;

    for (int i = 0; i < colnum; i++)
    {
        col_index = (i * num_of_WORD_BITS_in_row);

        if ((in_vec[i / WORD_BITS] >> (i % WORD_BITS)) & 1)
        {
            xor_vec(out_vec, in_mat, col_index, num_of_WORD_BITS_in_row);
        }
    }
}

/**
 * @brief generate Identity Matrix
 *
 * @param [out] I_Mat Identity Matrix
 * @param [in] row  Matrix row num
 */
void gen_identity_mat(OUT Word* I_Mat, IN int row)
{
    int rowgf = row * WORD_BITS;
    Word a = 1;

    for (int i = 0; i < row; i++)
    {
        I_Mat[(i * rowgf) + (row * WORD_BITS)] = a << (i % WORD_BITS);
    }
}

/**
 * @brief Gaussian elimination
 *
 * @param [out] systematic_mat systematic Matrix
 * @param [in] in_mat  input Matrix
 * @return (-1: Unable to genreate systematic_mat),(0: genreate systematic_mat)
 */
int gaussian_row(OUT Word* systematic_mat, IN const Word* in_mat)
{
    Word HP_hat[HP_NROWS][HP_NCOLS_WORDS] = {{0}};
    Word tmp = 0;

    int cnt = 0;
    for (int j = 0; j < PARAM_N; j++)
    {
        for (int i = 0; i < HP_NROWS; i++)
        {
            HP_hat[i][cnt] |= 
                (((in_mat[(i + (j * HP_NROWS)) / WORD_BITS] 
                    >> (i % WORD_BITS)) & 1) << (j % WORD_BITS));
        }
        if (j % WORD_BITS == WORD_BITS - 1)
        {
            cnt++;
        }
    }

    /* Gaussian elimination */

    int r = 0;
    while (r < HP_NROWS)
    {
        int flag = 0;
        int colIdx = r / WORD_BITS;
        int bitIdx = r % WORD_BITS;
        Word* HP_r = HP_hat[r];

        for (int i = r; i < HP_NROWS; i++)
        {
            Word mask = (HP_hat[i][colIdx] >> bitIdx) & 1;

            if (mask)
            {
                if (i != r)
                {
                    Word* HP_i = HP_hat[i];
                    for (int j = colIdx; j < HP_NCOLS_WORDS; j++)
                    {
                        tmp = HP_i[j];
                        HP_i[j] = HP_r[j];
                        HP_r[j] = tmp;
                    }
                }
                flag = 1;
            }

            if (flag)
            {
                for (int j = r+1; j < HP_NROWS; j++)
                {
                    Word mask_inner = (HP_hat[j][colIdx] >> bitIdx) & 1;
                    
                    if (mask_inner)
                    {
                        for (int x = colIdx; x < HP_NCOLS_WORDS; x++)
                        {
                            HP_hat[j][x] ^= HP_r[x];
                        }
                    }
                }
                break;
            }
        }

        if (!flag)
        {
            return -1;
        }

        r++;
    }

    r = 1;
    while (r < HP_NROWS)
    {
        int colIdx = r / WORD_BITS;
        int bitIdx = r % WORD_BITS;
        Word* HP_r = HP_hat[r];

        for (int j = 0; j < r; j++)
        {
            Word mask_inner = (HP_hat[j][colIdx] >> bitIdx) & 1;

            if (mask_inner)
            {
                for (int x = colIdx; x < HP_NCOLS_WORDS; x++)
                {
                    HP_hat[j][x] ^= HP_r[x];
                }
            }
        }
        r++;
    }

    memset(systematic_mat, 0, (sizeof(Word)) * HP_WORDS);

    for (int i = 0; i < HP_NROWS; i++)
    {
        int rowWordIdx = i / WORD_BITS;
        int rowBitIdx = i % WORD_BITS;

        for (int j = HP_NROWS; j < HP_NCOLS; j++)
        {
            systematic_mat[rowWordIdx + (j * HP_NROWS_WORDS)] |= 
                ((Word)((HP_hat[i][j / WORD_BITS] >> 
                        (j % WORD_BITS)) & 1) << rowBitIdx);
        }
    }

    return 0;
}
