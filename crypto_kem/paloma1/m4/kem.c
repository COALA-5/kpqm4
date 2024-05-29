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

#include "api.h"

/**
 * @brief Generate public and secret(private) key
 *
 * @param pk pointer to output public key 
 *           (a structure composed of (matrix H[n-k]))
 * @param sk pointer to output secret key
 *           (a structure composed of (support set L, goppa polynomial g(X),
 *            matrix S^{-1}, seed r for permutation matrix)
 * @param gf2m_tables precomputation table of GF(2^m) 
 *                    (multiplication, square, square root, inverse)
 * @return int
 */
int crypto_kem_keypair(
            unsigned char* pk, 
            unsigned char* sk, 
            const gf2m_tab* gf2m_tables)
{
    gen_key_pair((PublicKey*)pk, (SecretKey*)sk, gf2m_tables);
 
    return 0;
}

/**
 * @brief Encapsulation (Generate ciphertext and shared key)
 *
 * @param ct pointer to output ciphertext (a structure composed of
 *           (seed r, vector s_hat))
 * @param key pointer to output shared key
 * @param pk pointer to input public key (a structure composed of 
 *           (matrix H[n-k]))
 * @return int
 */
int crypto_kem_enc(
            unsigned char* ct, 
            unsigned char* ss, 
            const unsigned char* pk)
{
    /* Encapsulation */
    encap((Ciphertext*)ct, (Word*)ss, (PublicKey*)pk);

    return 0;
}

/**
 * @brief Decapsulation (Generate shared key using secret key and ciphertext)
 *
 * @param key pointer to output shared key
 * @param ct pointer to input ciphertext (a structure composed of 
 *           (seed r, vector s_hat))
 * @param sk pointer to input secret key
 *           (a structure composed of (support set L, goppa polynomial g(X), 
 *           matrix S^{-1}, seed r for permutation matrix)
 * @param gf2m_tables precomputation table of GF(2^m) (multiplication, 
 *                    square, square root, inverse)
 * @return int
 */
int crypto_kem_dec(
            unsigned char* ss, 
            const unsigned char* ct, 
            const unsigned char* sk, 
            const gf2m_tab* gf2m_tables)
{
    /* Decapsulation */
    decap((Word*)ss, (SecretKey*)sk, (Ciphertext*)ct, gf2m_tables);

    return 0;
}
