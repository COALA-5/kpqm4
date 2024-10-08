#ifndef PALOMA_PARAM_H
#define PALOMA_PARAM_H

#include "paloma_def.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

#define SEED_BITS   256
#define SEED_BYTES  32
#define SEED_WORDS  (256 / WORD_BITS)

#define PARAM_M     13

/* *************************************** */

#if PALOMA_SECURITY_LEVEL == 128
#define PARAM_N     3904
#define PARAM_T     64
#define PARAM_K     (PARAM_N - (PARAM_M * PARAM_T))
#define PK_BYTES    319488
#define SK_BYTES    94528
#define CT_BYTES    136
#define SS_BYTES    32

#elif PALOMA_SECURITY_LEVEL == 192
#define PARAM_N     5568
#define PARAM_T     128
#define PARAM_K     (PARAM_N - (PARAM_M * PARAM_T))
#define PK_BYTES    812032
#define SK_BYTES    357568
#define CT_BYTES    240
#define SS_BYTES    32

#elif PALOMA_SECURITY_LEVEL == 256
#define PARAM_N     6592
#define PARAM_T     128
#define PARAM_K     (PARAM_N - (PARAM_M * PARAM_T))
#define PK_BYTES    1025024
#define SK_BYTES    359616
#define CT_BYTES    240
#define SS_BYTES    32

#endif

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* PALOMA_PARAM_H */