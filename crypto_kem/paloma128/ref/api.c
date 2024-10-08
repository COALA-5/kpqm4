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
#include "paloma_def.h"

int crypto_kem_keypair(OUT u08* pk, OUT u08* sk)
{   
    gf2m_tab* p_gf2m_tabs = &g_gf2m_tabs;

    /* Generate Key Pair */
    gen_key_pair((PublicKey*)pk, (SecretKey*)sk, p_gf2m_tabs);

    return 0;
}

int crypto_kem_enc(OUT u08* ct, OUT u08* ss, IN const u08* pk)
{
    /* Encapsulation */
    encap((Ciphertext*)ct, ss, (PublicKey*)pk);

    return 0;
}

int crypto_kem_dec(OUT u08* ss, IN const u08* ct, IN const u08* sk)
{   
    gf2m_tab* p_gf2m_tabs = &g_gf2m_tabs;

    /* Decapsulation */
    decap(ss, (SecretKey*)sk, (Ciphertext*)ct, p_gf2m_tabs);

    return 0;
}
