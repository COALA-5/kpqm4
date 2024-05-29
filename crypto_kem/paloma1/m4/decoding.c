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

#include "decoding.h"

/**
 * @brief Function to convert a vector to a polynomial.
 *
 * @param [out] sX a syndrome Polynomial s(X).
 * @param [in] s a syndrome vector s representing the polynomial.
 */
void to_poly(OUT gf* sX, IN const Word* s)
{
    /* Extracts bits from the vector and sets corresponding bits 
        in the polynomial */
    for (int i = 0; i < (PARAM_M * PARAM_T); i++)
        sX[(i / PARAM_M)] |=
             ((((s[i / WORD_BITS]) >> (i % WORD_BITS)) & 1) << (i % PARAM_M));
}

/**
 * @brief Function to construct the key equation.
 *
 * @param [out] vX Polynomial used to construct the key equation.
 * @param [out] g1X Polynomial used to construct the key equation.
 * @param [out] g2X Polynomial used to construct the key equation.
 * @param [out] g12X Polynomial used to construct the key equation.
 * @param [in] sX Syndrome polynomial.
 * @param [in] gX Goppa polynomial.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void construct_key_eqn(
            OUT gf* vX, 
            OUT gf* g1X, 
            OUT gf* g2X, 
            OUT gf* g12X, 
            IN const gf* sX, 
            IN const gf* gX, 
            IN const gf2m_tab* gf2m_tables)
{
    gf sX_tilde[GF_POLY_LEN] = {0};  // s_tilde(X)
    gf s1X[GF_POLY_LEN] = {0};       // s1(X)
    gf s2X_tilde[GF_POLY_LEN] = {0}; // s2_tilde(X)
    gf g1Xg2X[GF_POLY_LEN] = {0};    // g1(X) * g2(X)
    gf uX[GF_POLY_LEN] = {0};        // u(X)
    gf tmpX[GF_POLY_LEN] = {0};      // temporary value to use division

    /* s_tilde(X) = 1 + X * s(X) */
    for (int i = 0; i < PARAM_T; i++)
        sX_tilde[i + 1] = sX[i];

    sX_tilde[0] = 1;

    /* g1(X), g2(X) = gcd(g(X), s(X)), gcd(g(X), s_tilde(X)) */
    gf_poly_gcd(g1X, gX, sX, gf2m_tables);
    gf_poly_gcd(g2X, gX, sX_tilde, gf2m_tables);

    /* g12(X) = g(X) / (g1(X) * g2(X)) */
    gf_poly_mul(g1Xg2X, g1X, g2X, gX, gf2m_tables);
    gf_poly_div(g12X, tmpX, gX, g1Xg2X, gf2m_tables);
    
    /* s2_tilde(x), s1(X) = s_tilde(x) / g2(X), s(x) / g1(X) */
    gf_poly_div(s2X_tilde, tmpX, sX_tilde, g2X, gf2m_tables);
    gf_poly_div(s1X, tmpX, sX, g1X, gf2m_tables);

    /* u(X) = g1(X) * s2_tilde(x) * (g2(X) * s1(X))^{-1} mod g12(X) */
    gf_poly_mul(uX, g2X, s1X, g12X, gf2m_tables);
    gf_poly_inv(uX, uX, g12X, gf2m_tables);
    gf_poly_mul(uX, uX, g1X, g12X, gf2m_tables);
    gf_poly_mul(uX, uX, s2X_tilde, g12X, gf2m_tables);

    /* v(X) = sqrt(u(X)) mod g12(X) */
    gf_poly_sqrt(vX, uX, g12X, gf2m_tables);
}

/**
 * @brief Function to find solutions to the key equation.
 *
 * @param [out] a2X Solution of the key equation.
 * @param [out] b1X Solution of the key equation.
 * @param [in] vX Polynomial used to construct the key equation.
 * @param [in] g12X Polynomial used to construct the key equation.
 * @param [in] deg_a Degree range used to find solutions to the key equation.
 * @param [in] deg_b Degree range used to find solutions to the key equation.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void solve_key_eqn(
            OUT gf* a2X, 
            OUT gf* b1X, 
            IN const gf* vX, 
            IN const gf* g12X, 
            IN int deg_a, 
            IN int deg_b, 
            IN const gf2m_tab* gf2m_tables)
{
    gf a0X[GF_POLY_LEN] = {0};
    gf b0X[GF_POLY_LEN] = {0};
    gf b2X[GF_POLY_LEN] = {0};

    /* a0(X), a2(X) = v(X), g12(X) */
    gf_poly_copy(a0X, vX);
    gf_poly_copy(a2X, g12X);

    /* b0(X), b2(X) = 1, 0 */
    b0X[0] = 1;

    while (gf_poly_get_degree(a2X) >= 0)
    {
        gf qX[GF_POLY_LEN] = {0}; // quotient
        gf rX[GF_POLY_LEN] = {0}; // remainder

        /* q(X), r(X) = div(a0(X), a2(X)) */
        gf_poly_div(qX, rX, a0X, a2X, gf2m_tables);

        /* a0(X), a2(X) = a2(X), r(X) */
        gf_poly_copy(a0X, a2X);
        gf_poly_copy(a2X, rX);

        /* b1(X) = b0(X) − q(X) * b2(X) */
        poly_mul(b1X, qX, b2X, gf2m_tables);
        gf_poly_add(b1X, b1X, b0X);

        /* b0(X), b2(X) = b2(X), b1(X) */
        gf_poly_copy(b0X, b2X);
        gf_poly_copy(b2X, b1X);

        /* break if deg(a0(X)) <= deg_a and deg(b0(X)) <= deg_b */
        if ((gf_poly_get_degree(a0X) <= deg_a) && 
                (gf_poly_get_degree(b0X) <= deg_b))
            break;
    }

    /* return a2(X), b1(X) */
    gf_poly_copy(a2X, a0X);
    gf_poly_copy(b1X, b0X);
}

/**
 * @brief Function to generate the error locator polynomial.
 *
 * @param [out] sigX Error locator polynomial.
 * @param [in] a2X Solution of the key equation.
 * @param [in] g2X Polynomial used to construct the key equation.
 * @param [in] b1X Solution of the key equation.
 * @param [in] g1X Polynomial used to construct the key equation.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void get_err_loc_poly(
            OUT gf* sigX, 
            IN const gf* a2X, 
            IN const gf* g2X, 
            IN const gf* b1X, 
            IN const gf* g1X, 
            IN const gf2m_tab* gf2m_tables)
{
    gf aX[GF_POLY_LEN] = {0};
    gf bX[GF_POLY_LEN] = {0};

    /* a(X), b(X) = a2(X) * g2(X), b1(X) * g1(X) */
    poly_mul(aX, a2X, g2X, gf2m_tables);
    poly_mul(bX, b1X, g1X, gf2m_tables);

    /* a(X)^2 and b(X)^2 */
    poly_mul(aX, aX, aX, gf2m_tables);
    poly_mul(bX, bX, bX, gf2m_tables);

    /* sigma(X) = a(X)^2 + x * b(X)^2 */
    for (int i = PARAM_T - 1; i >= 0; i--)
    {
        sigX[i + 1] ^= bX[i];
        sigX[i] ^= aX[i];
    }

    sigX[PARAM_T] = aX[PARAM_T];

    /* Convert sigma(X) to a monic polynomial */
    gf_poly_change_monic(sigX, sigX, gf2m_tables);
}

/**
 * @brief Function to find the error vector from the error locator polynomial.
 *
 * @param [out] err_vec Error vector.
 * @param [in] sigX Error locator polynomial.
 * @param [in] L Support set.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void find_err_vec(
            OUT Word* err_vec, 
            IN const gf* sigX, 
            IN const gf* L, 
            IN const gf2m_tab* gf2m_tables)
{
    /* Finding the error vector */
    for (int i = 0; i < PARAM_N; i++)
    {
        Word err = gf_poly_eval(sigX, L[i], gf2m_tables) == 0;
        err_vec[i / WORD_BITS] |= (err << (i % WORD_BITS));
    }
}

/**
 * @brief Function to recover the error vector. 
 *        (Using the extended Patterson decoding algorithm.)
 *
 * @param [out] err_vec Error vector.
 * @param [in] sk Secret key.
 * @param [in] synd_vec Syndrome vector.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void recover_err_vec(
            OUT Word* err_vec, 
            IN const SecretKey* sk, 
            IN const Word* synd_vec, 
            IN const gf2m_tab* gf2m_tables)
{
    gf sX[GF_POLY_LEN] = {0};   // Syndrome polynomial
    gf sigX[GF_POLY_LEN] = {0}; // Error locator polynomial

    /* Parameters to construct the key equation */
    gf vX[GF_POLY_LEN] = {0};
    gf g1X[GF_POLY_LEN] = {0};
    gf g2X[GF_POLY_LEN] = {0};
    gf g12X[GF_POLY_LEN] = {0};
    int deg_a = 0;
    int deg_b = 0;

    /* Solution of the key equation */
    gf a2X[GF_POLY_LEN] = {0};
    gf b1X[GF_POLY_LEN] = {0};

    /* Convert 'sk->g_X(degree : t-1)' -> monic gX 'gX(degree : t)' <-  */
    gf tmp_gx[GF_POLY_LEN]; // monic gX 'gX(degree : t)'
    memcpy(tmp_gx, sk->gX, sizeof(gf) * PARAM_T);
    tmp_gx[PARAM_T] = 1;

    /* Obtain the syndrome polynomial from the syndrome vector */
    to_poly(sX, synd_vec);

    /* Construct the key equation */
    construct_key_eqn(vX, g1X, g2X, g12X, sX, tmp_gx, gf2m_tables);

    deg_a = (PARAM_T / 2) - gf_poly_get_degree(g2X);
    deg_b = ((PARAM_T - 1) / 2) - gf_poly_get_degree(g1X);

    /* Find solutions to the key equation */
    solve_key_eqn(a2X, b1X, vX, g12X, deg_a, deg_b, gf2m_tables);

    /* Find the error locator polynomial */
    get_err_loc_poly(sigX, a2X, g2X, b1X, g1X, gf2m_tables);

    /* Find the error vector */
    find_err_vec(err_vec, sigX, sk->L, gf2m_tables);
}