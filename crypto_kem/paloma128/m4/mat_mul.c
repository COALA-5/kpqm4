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
#include "utility.h"

static INLINE void xor_vec(
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

/* ***************************************
    Parts for gaussian row
*************************************** */

/* ***************************************
    Function to allocate memory and initialize HP_hat
*/
Word* allocate_hp_hat() {

    Word* HP_hat = (Word*)calloc(HP_NROWS * HP_NCOLS_WORDS, sizeof(Word));
    
    // Memory allocation failure
    if (HP_hat == NULL) {
        return NULL;
    }

    return HP_hat;
}

/* ***************************************
    Function to retrieve a specific bit from an array of Word elements
*/
Word get_bit(const Word* array, int word_index, int bit_index) {
    return (array[word_index] >> bit_index) & 1;
}

/* ***************************************
    Function to set a specific bit in an array of Word elements
*/
void set_bit(Word* array, int word_index, int bit_index, Word bit) {
    array[word_index] |= (bit << bit_index);
}

/* ***************************************
    Constant masks for matrix transposition based on word size.
    Depending on the size of the Word (8, 32, or 64 bits).
*/
#if WORD == 8
const Word mask4mat_transpose[WORD_LOG2][2] = {
  {0x55, 0xaa},
  {0x33, 0xcc},
  {0x0f, 0xf0},
};

#elif WORD == 32
const Word mask4mat_transpose[WORD_LOG2][2] = {
  {0x55555555, 0xaaaaaaaa},
  {0x33333333, 0xcccccccc},
  {0x0f0f0f0f, 0xf0f0f0f0},
  {0x00ff00ff, 0xff00ff00},
  {0x0000ffff, 0xffff0000},
};

#elif WORD == 64
const Word mask4mat_transpose[WORD_LOG2][2] = {
  {0x5555555555555555, 0xaaaaaaaaaaaaaaaa},
  {0x3333333333333333, 0xcccccccccccccccc},
  {0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0},
  {0x00ff00ff00ff00ff, 0xff00ff00ff00ff00},
  {0x0000ffff0000ffff, 0xffff0000ffff0000},
  {0x00000000ffffffff, 0xffffffff00000000}
};
#endif

/* ***************************************
    Define function to perform row swapping
*/
#define swap_rows(row1, row2, start_col, n_cols) \
    do { \
        for (int j = (start_col); j < (n_cols); j++) { \
            Word tmp = (row1)[j]; \
            (row1)[j] = (row2)[j]; \
            (row2)[j] = tmp; \
        } \
    } while (0)

/* ***************************************
    Function to perform forward elimination in Gaussian elimination
*/
int forward_elimination(Word* HP_hat)
{
    // Iterate through each row to find a pivot element
    for (int row = 0; row < HP_NROWS; row++)
    {
        int pivot_found = 0;
        int col_word_idx = row / WORD_BITS;
        int col_bit_idx = row % WORD_BITS;

        Word* pivot_row = HP_hat + (row * HP_NCOLS_WORDS);

        // Search for a pivot row starting from the current row downwards
        for (int i = row; i < HP_NROWS; i++)
        {
            Word bit = (HP_hat[i * HP_NCOLS_WORDS + col_word_idx] >> col_bit_idx) & 1;

            // If the current bit is 1, then a pivot is found
            if (bit)
            {
                if (i != row)
                {
                    swap_rows(pivot_row, HP_hat + i * HP_NCOLS_WORDS, col_word_idx, HP_NCOLS_WORDS);
                }
                pivot_found = 1;
                break;
            }
        }

        // If no pivot found for the current column, 
        // return -1 indicating failure
        if (!pivot_found)
        {
            return -1;
        }

        // Eliminate rows below the pivot to form upper triangular matrix
        for (int i = row + 1; i < HP_NROWS; i++)
        {
            Word bit = get_bit(HP_hat, i * HP_NCOLS_WORDS + col_word_idx, col_bit_idx);

            // If the bit is 1, perform row reduction by XORing with the pivot row
            if (bit)
            {
                for (int j = col_word_idx; j < HP_NCOLS_WORDS; j++)
                {
                    HP_hat[i * HP_NCOLS_WORDS + j] ^= pivot_row[j];
                }
            }
        }
    }
    return 0;
}

/* ***************************************
    Function to perform backward elimination in Gaussian elimination
*/
void backward_elimination(Word* HP_hat)
{   
    // Start from the second row and perform elimination upwards for each pivot found
    for (int row = 1; row < HP_NROWS; row++)
    {
        int col_word_idx = row / WORD_BITS;
        int col_bit_idx = row % WORD_BITS;
        Word* pivot_row = HP_hat + (row * HP_NCOLS_WORDS);

        for (int i = 0; i < row; i++)
        {   
            // Check if the bit in the current row at the pivot position is 1
            Word bit = (HP_hat[i * HP_NCOLS_WORDS + col_word_idx] >> col_bit_idx) & 1;
            
            // If the bit is 1, perform row reduction using XOR with the pivot row
            if (bit)
            {
                for (int j = col_word_idx; j < HP_NCOLS_WORDS; j++)
                {
                    HP_hat[i * HP_NCOLS_WORDS + j] ^= pivot_row[j];
                }
            }
        }
    }
}

/* ***************************************
    Function to transpose a square matrix using bit-level operations.
*/
void mat_transpose(Word* mat)
{
    for (int j = (WORD_LOG2-1); j >= 0; j--)
    {
        int s = (1 << j);

        for (int p = 0; p < (WORD_BITS/2)/s; p++)
        {   
            // Loop through each bit position within the current group
            for (int i = 0; i < s; i++)
            {
                int idx0 = p*2*s + i;
                int idx1 = p*2*s + i + s;

                // Mask and extract bits for idx0 and idx1 using the current mask set
                Word x = (mat[idx0] & mask4mat_transpose[j][0]);
                x |= ((mat[idx1] & mask4mat_transpose[j][0]) << s);

                Word y = ((mat[idx0] & mask4mat_transpose[j][1]) >> s);
                y |= (mat[idx1] & mask4mat_transpose[j][1]);

                mat[idx0] = x;
                mat[idx1] = y;
            }
        }
    }
}

/* ***************************************
    Function to convert a column-major matrix into a row-major matrix. 
*/
void convert_col_to_row_vec(Word* HP_hat, const Word* in_mat) 
{
    for (int r = 0; r < HP_NROWS_WORDS; r++)
    {
        for (int c = 0; c < HP_NCOLS_WORDS; c++)
        {   
            // Collect the bits from the input column-major matrix into tmp array
            Word tmp[WORD_BITS] = {0,};
            for (int i = 0; i < WORD_BITS; i++)
            {
                int ind = HP_NROWS_WORDS*i + c*HP_NROWS + r;
                tmp[i] = in_mat[ind];
            }

            // Transpose the temporary array
            mat_transpose(tmp);
            
            // Store the transposed data into the output row-major matrix
            for (int i = 0; i < WORD_BITS; i++)
            {
                int ind = HP_NCOLS_WORDS*i + r*HP_NCOLS + c;
                HP_hat[ind] = tmp[i];
            }
        }
    }
}

/* ***************************************
    Function to convert a row-major matrix into a column-major matrix.
*/
void convert_row_to_col_vec(Word* systematic_mat, Word* HP_hat)
{
    for (int r = 0; r < HP_NROWS_WORDS; r++)
    {
        for (int c = HP_NROWS_WORDS; c < HP_NCOLS_WORDS; c++)
        {   
            // Collect the bits from the input row-major matrix into tmp array
            Word tmp[WORD_BITS] = {0,};
            for (int i = 0; i < WORD_BITS; i++)
            {
                int ind = HP_NCOLS_WORDS*i + r*HP_NCOLS + c;
                tmp[i] = HP_hat[ind];
            }

            // Transpose the temporary array
            mat_transpose(tmp);

            // Store the transposed data into the output column-major matrix
            for (int i = 0; i < WORD_BITS; i++)
            {
                int ind = HP_NROWS_WORDS*i + c*HP_NROWS + r;
                systematic_mat[ind] = tmp[i];
            }
        }
    }
}

/**
 * @brief Gaussian elimination
 *        : Main function to perform Gaussian row reduction
 *
 * @param [out] systematic_mat systematic Matrix
 * @param [in] in_mat  input Matrix
 * @return (-1: Unable to generate systematic_mat),(0: generate systematic_mat)
 */
int gaussian_row(OUT Word* systematic_mat, IN const Word* in_mat) {
    
    // ------------------------------------------------
    // Step 1: Allocate memory and initialize HP_hat
    Word* HP_hat = allocate_hp_hat();

    if (HP_hat == NULL) {
        return -1;
    }

    // ------------------------------------------------
    // Step 2: Convert column vector to row vector
    convert_col_to_row_vec(HP_hat, in_mat);

    // ------------------------------------------------
    // Step 3: Perform Gaussian elimination (forward + backward elimination)
    if (forward_elimination(HP_hat) == -1) 
    {
        free(HP_hat);
        return -1;
    }
    backward_elimination(HP_hat);
    
    // ------------------------------------------------
    // Step 4: Convert row vector back to column vector
    convert_row_to_col_vec(systematic_mat, HP_hat);

    // Free dynamically allocated memory
    free(HP_hat);

    return 0;
}