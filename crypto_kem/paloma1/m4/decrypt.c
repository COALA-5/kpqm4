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

#include "decrypt.h"
/**
 * @brief Decryption
 *
 * @param [out] e_hat Error vector e_hat with w_H(e_hat) = t
 * @param [in] sk Secret key.
 * @param [in] s_hat Syndrome vector
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void decrypt(
            OUT Word* e_hat, 
            IN const SecretKey* sk, 
            IN const Word* s_hat, 
            IN const gf2m_tab* gf2m_tables)
{
    Word s[SYND_WORDS] = {0};
    Word e[PARAM_N_WORDS] = {0};

    /* s = S^{-1} * s_hat */
    matXvec(s, sk->S_inv, s_hat, SYND_BITS, SYND_BITS);

    /* Recover the error vector (using extended Patterson decoding) */
    recover_err_vec(e, sk, s, gf2m_tables);

    /* Generate the permutation matrix */
    perm_inv(e_hat, e, sk->r_C_hat);
}
