/*
NIST-developed software is provided by NIST as a public service. You may use, copy, and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify, and create derivative works of the software or any portion of the software, and you may copy and distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.
 
NIST-developed software is expressly provided "AS IS." NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT, OR ARISING BY OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 
You are solely responsible for determining the appropriateness of using and distributing the software and you assume all risks associated with its use, including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and the unavailability or interruption of operation. This software is not intended to be used in any situation where a failure could cause risk of injury or damage to property. The software developed by NIST employees is not subject to copyright protection within the United States.
*/

#ifndef PALOMA_API_H
#define PALOMA_API_H

#include "paloma_data.h"
#include "gf2m_tab_gen.h"
#include "gf2m_tab.h"
#include "genkeypair.h"
#include "encap.h"
#include "decap.h"

/* *************************************** */
#ifdef __cplusplus
extern "C" {
#endif
/* *************************************** */

#define CRYPTO_PUBLICKEYBYTES     PK_BYTES
#define CRYPTO_SECRETKEYBYTES     SK_BYTES
#define CRYPTO_CIPHERTEXTBYTES     CT_BYTES
#define CRYPTO_BYTES     SS_BYTES

/* *************************************** */
/* Change the algorithm name */
#define CRYPTO_ALGNAME "PALOMA"
#define KPQM4_PALOMA

/* *************************************** */
/**
 * @brief Generate public and secret(private) key
 *
 * @param pk pointer to output public key 
 *           (a structure composed of (matrix H[n-k]))
 * @param sk pointer to output secret key
 *           (a structure composed of (support set L, goppa polynomial g(X),
 *            matrix S^{-1}, seed r for permutation matrix)
 * @return int
 */
int crypto_kem_keypair(
            OUT u08*            pk, 
            OUT u08*            sk);

/**
 * @brief Encapsulation (Generate ciphertext and shared key)
 *
 * @param ct pointer to output ciphertext (a structure composed of
 *           (seed r, vector s_hat))
 * @param ss pointer to output shared key
 * @param pk pointer to input public key (a structure composed of 
 *           (matrix H[n-k]))
 * @return int
 */
int crypto_kem_enc(
            OUT u08*        ct, 
            OUT u08*        ss, 
            IN  const u08*  pk);

/**
 * @brief Decapsulation (Generate shared key using secret key and ciphertext)
 *
 * @param ss pointer to output shared key
 * @param ct pointer to input ciphertext (a structure composed of 
 *           (seed r, vector s_hat))
 * @param sk pointer to input secret key
 *           (a structure composed of (support set L, goppa polynomial g(X), 
 *           matrix S^{-1}, seed r for permutation matrix)
 * @return int
 */
int crypto_kem_dec(
            OUT u08*        ss, 
            IN const u08*   ct, 
            IN const u08*   sk);

/* *************************************** */
#ifdef __cplusplus
}
#endif
/* *************************************** */

#endif /* PALOMA_API_H */