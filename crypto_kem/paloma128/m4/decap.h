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
    This file is for decapsulation
*/

#ifndef DECAP_H
#define DECAP_H

#include "paloma_data.h"
#include "decrypt.h"
#include "utility.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

/**
 * @brief Decapsulation
 *
 * @param [out] ss          Shared Secret to be recovered.
 * @param [in]  sk          Secret key.
 * @param [in]  ct          Ciphertext c = (r_hat, s_hat)
 * @param [in]  gf2m_tables GF(2^m) operation tables.

 * https://kmu-fdl-dc.notion.site/image/https%3A%2F%2Fprod-files-secure.s3.us-west-2.amazonaws.com%2F4ebccb7c-7e99-4d75-a4da-0bc83ac99e44%2Fbae1b3d1-36e4-4421-82d7-2ce1f904a652%2Fpaloma_ver1.2_decap.png?table=block&id=0f72524d-9fbf-4f6d-858d-dfa729746a36&spaceId=4ebccb7c-7e99-4d75-a4da-0bc83ac99e44&width=1260&userId=&cache=v2

 */
void decap(OUT u08* ss, IN const SecretKey* sk, IN const Ciphertext* ct, IN const gf2m_tab* gf2m_tables);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* DECAP_H */