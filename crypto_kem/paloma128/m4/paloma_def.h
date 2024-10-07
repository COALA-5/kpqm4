#ifndef DEFINITION_H
#define DEFINITION_H
/* *************************************** */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>

/* *************************************** */

#include "paloma_config.h"
#include "paloma_local.h"
#include "paloma_string.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

#define IN
#define OUT

#define YES 1
#define NO  0

/* *************************************** */

typedef uint8_t     u08;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef uint16_t    gf2m;

/* *************************************** */

#if WORD == 32
typedef u32         Word;
#define WORD_BITS   32
#define WORD_BYTES  4
#define WORD_MASK   0xffffffff
#define WORD_LOG2   5

#elif WORD == 64
typedef u64         Word;
#define WORD_BITS   64
#define WORD_BYTES  8
#define WORD_MASK   0xffffffffffffffff
#define WORD_LOG2   6

#endif

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* DEFINITION_H */