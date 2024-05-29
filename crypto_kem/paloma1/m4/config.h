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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef WORD
#define WORD 64
#endif

#if WORD == 32
typedef uint32_t Word;
#define WORD_BITS 32
#define WORD_BYTES 4

#elif WORD == 64
typedef uint64_t Word;
#define WORD_BITS 64
#define WORD_BYTES 8
#endif

/**
 * @brief Set PALOMA_MODE
 * @brief PALOMA_MODE == 128
 * @brief PALOMA_MODE == 192
 * @brief PALOMA_MODE == 256
 */
#ifndef PALOMA_MODE
#define PALOMA_MODE 128
#endif

#if PALOMA_MODE == 128
#define PARAM_N 3904
#define PARAM_T 64
#define PARAM_K (PARAM_N - (PARAM_M * PARAM_T))
#define PUBLICKEYBYTES 319488
#define SECRETKEYBYTES 94528
#define CIPHERTEXTBYTES 136
#define BYTES 32

#elif PALOMA_MODE == 192
#define PARAM_N 5568
#define PARAM_T 128
#define PARAM_K (PARAM_N - (PARAM_M * PARAM_T))
#define PUBLICKEYBYTES 812032
#define SECRETKEYBYTES 357568
#define CIPHERTEXTBYTES 240
#define BYTES 32

#elif PALOMA_MODE == 256
#define PARAM_N 6592
#define PARAM_T 128
#define PARAM_K (PARAM_N - (PARAM_M * PARAM_T))
#define PUBLICKEYBYTES 1025024
#define SECRETKEYBYTES 359616
#define CIPHERTEXTBYTES 240
#define BYTES 32
#endif

#define PARAM_M 13

#define SEED_BITS 256
#define SEED_BYTES 256 / 8
#define SEED_WORDS 256 / WORD_BITS

#define IN
#define OUT

#endif