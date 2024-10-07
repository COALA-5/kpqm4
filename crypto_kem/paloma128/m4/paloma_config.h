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
    This file is for parameter configuring 
*/

#ifndef PALOMA_CONFIG_H
#define PALOMA_CONFIG_H

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

#if 0
#define _DEBUG_
#endif

#if 1
#define BENCHMARK
#endif

/* ***************************************
 * @brief Set PALOMA_SECURITY_LEVEL
 * @brief PALOMA_SECURITY_LEVEL == 128
 * @brief PALOMA_SECURITY_LEVEL == 192
 * @brief PALOMA_SECURITY_LEVEL == 256
 */
#ifndef PALOMA_SECURITY_LEVEL
#define PALOMA_SECURITY_LEVEL 128
#endif

/* ***************************************
    WORD = 32 or 64
*/
#ifndef WORD
#define WORD 64
#endif

/* ***************************************
    WITH_OPENSSL = YES(1) or NO(0)
*/
#ifndef WITH_OPENSSL
#define WITH_OPENSSL    0
#endif

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* PALOMA_CONFIG_H */