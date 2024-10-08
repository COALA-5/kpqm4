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
    This file is for functions used in various files
*/

#ifndef UTILITY_H
#define UTILITY_H

#include "paloma_def.h"
#include "paloma_param.h"
#include "paloma_constant.h"

#include "gf2m.h"
#include "gf2m_poly.h"
#include "gf2m_tab_gen.h"

#include "lsh512.h"
//#include "aes256_ctr_drbg.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

void shuffle(
            OUT uint16_t*       shuffled_list,
            IN  int             list_len,
            IN  const u08*  seed);

void gen_rand_bytes(OUT u08* bytes, IN size_t size);    /* Generate random bytes */
void show_bytes(IN char* str, IN u08* bytes, IN size_t size);   /* Generate random bytes */

void rand_oracle_G(OUT u08* seed, IN const Word* msg);  /* Random Oracle G */
void rand_oracle_H(OUT u08* seed, IN const Word* msg);  /* Random Oracle H */

void gen_perm_mat(OUT uint16_t* P, OUT uint16_t* P_inv, IN int n, IN const u08* seed);

void perm(OUT Word* dst_v, IN const Word* src_v, IN const u08* seed);
void perm_inv(OUT Word* dst_v, IN const Word* src_v, IN const u08* seed);

void gen_err_vec(OUT Word* err_vec, IN const u08* seed);
int get_Hamming_weight(IN const Word* err_vec);

int is_equal(const Word* src1, const Word* src2, int word_len);

#define get_new_data(data_type, var_name, data_len) { \
    var_name = NULL; \
    var_name = (data_type*)calloc(data_len, sizeof(data_type)); \
    if (var_name == NULL) { \
    } \
}


/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif