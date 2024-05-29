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

#include "gf_poly.h"
#include "mat_mul.h"

#include "lsh512.h"
#include "lsh_local.h"

void shuffle(
            OUT gf* shuffled_set, 
            IN int set_len, 
            IN const Word* _256bits_seed_r);     /* PALOMA SHUFFLE */

void gen_rand_sequence(
            OUT Word* rand_sequence, 
            IN int sequence_len);                /* Generate random sequence */
            
void rand_oracle_G(
            OUT Word* seed, 
            IN const Word* msg);                   /* Random Oracle G */

void rand_oracle_H(   
            OUT Word* seed, 
            IN const Word* msg);                   /* Random Oracle H */

void gen_perm_mat(OUT gf* P, OUT gf* P_inv, IN int n, IN const Word* r);

void perm(OUT Word* dst_v, IN const Word* src_v, IN const Word* r);
void perm_inv(OUT Word* dst_v, IN const Word* src_v, IN const Word* r);

void gen_err_vec(OUT Word* err_vec, IN const Word* seed);

#endif