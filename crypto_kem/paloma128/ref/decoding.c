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
#include "utility.h"

/**
 * @brief Function to convert a vector to a polynomial.
 * @brief Extracts bits from the vector and sets corresponding bits in the polynomial
 * @brief s[mt/wordbits] ... s[2] s[1] s[0] = s(t-1)X^(t-1) + ... + s(2)*X^2 + s(1)*X + s(0)
 * 
 * @param [out] sX a syndrome Polynomial s(X).
 * @param [in] s a syndrome vector s representing the polynomial.
 */
void to_poly(OUT gf2m* sX, IN const Word* s)
{
    for (int i = 0; i < (PARAM_M * PARAM_T); i++)
    {
        sX[(i / PARAM_M)] |= ((((s[i / WORD_BITS]) >> (i % WORD_BITS)) & 1) << (i % PARAM_M));
    }
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
            OUT gf2m* vX, 
            OUT gf2m* g1X, 
            OUT gf2m* g2X, 
            OUT gf2m* g12X, 
            IN const gf2m* sX, 
            IN const gf2m* gX, 
            IN const gf2m_tab* gf2m_tables)
{
    gf2m sX_tilde[GF_POLY_LEN] = {0};  // s_tilde(X)
    gf2m s1X[GF_POLY_LEN] = {0};       // s1(X)
    gf2m s2X_tilde[GF_POLY_LEN] = {0}; // s2_tilde(X)
    gf2m g1Xg2X[GF_POLY_LEN] = {0};    // g1(X) * g2(X)
    gf2m uX[GF_POLY_LEN] = {0};        // u(X)
    gf2m tmpX[GF_POLY_LEN] = {0};      // temporary value to use division

    /* s_tilde(X) = 1 + X * s(X) */
    for (int i = 0; i < PARAM_T; i++)
        sX_tilde[i + 1] = sX[i];

    sX_tilde[0] = 1;

    /* g1(X), g2(X) = gcd(g(X), s(X)), gcd(g(X), s_tilde(X)) */
    gf2m_poly_gcd(g1X, gX, sX, gf2m_tables);
    gf2m_poly_gcd(g2X, gX, sX_tilde, gf2m_tables);

    /* g12(X) = g(X) / (g1(X) * g2(X)) */
    gf2m_poly_mul_mod(g1Xg2X, g1X, g2X, gX, gf2m_tables);
    gf2m_poly_div(g12X, tmpX, gX, g1Xg2X, gf2m_tables);
    
    /* s2_tilde(x), s1(X) = s_tilde(x) / g2(X), s(x) / g1(X) */
    gf2m_poly_div(s2X_tilde, tmpX, sX_tilde, g2X, gf2m_tables);
    gf2m_poly_div(s1X, tmpX, sX, g1X, gf2m_tables);

    /* u(X) = g1(X) * s2_tilde(x) * (g2(X) * s1(X))^{-1} mod g12(X) */
    gf2m_poly_mul_mod(uX, g2X, s1X, g12X, gf2m_tables);
    gf2m_poly_inv_mod(uX, uX, g12X, gf2m_tables);
    gf2m_poly_mul_mod(uX, uX, g1X, g12X, gf2m_tables);
    gf2m_poly_mul_mod(uX, uX, s2X_tilde, g12X, gf2m_tables);

    /* v(X) = sqrt(u(X)) mod g12(X) */
    gf2m_poly_sqrt_mod(vX, uX, g12X, gf2m_tables);
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
            OUT gf2m* a2X, 
            OUT gf2m* b1X, 
            IN const gf2m* vX, 
            IN const gf2m* g12X, 
            IN int deg_a, 
            IN int deg_b, 
            IN const gf2m_tab* gf2m_tables)
{
    gf2m a0X[GF_POLY_LEN] = {0};
    gf2m b0X[GF_POLY_LEN] = {0};
    gf2m b2X[GF_POLY_LEN] = {0};

    /* a0(X), a2(X) = v(X), g12(X) */
    gf2m_poly_copy(a0X, vX);
    gf2m_poly_copy(a2X, g12X);

    /* b0(X), b2(X) = 1, 0 */
    b0X[0] = 1;

    while (gf2m_poly_get_deg(a2X) >= 0)
    {
        gf2m qX[GF_POLY_LEN] = {0}; // quotient
        gf2m rX[GF_POLY_LEN] = {0}; // remainder

        /* q(X), r(X) = div(a0(X), a2(X)) */
        gf2m_poly_div(qX, rX, a0X, a2X, gf2m_tables);

        /* a0(X), a2(X) = a2(X), r(X) */
        gf2m_poly_copy(a0X, a2X);
        gf2m_poly_copy(a2X, rX);

        /* b1(X) = b0(X) − q(X) * b2(X) */
        gf2m_poly_mul(b1X, qX, b2X, gf2m_tables);
        gf2m_poly_add(b1X, b1X, b0X);

        /* b0(X), b2(X) = b2(X), b1(X) */
        gf2m_poly_copy(b0X, b2X);
        gf2m_poly_copy(b2X, b1X);

        /* break if deg(a0(X)) <= deg_a and deg(b0(X)) <= deg_b */
        if ((gf2m_poly_get_deg(a0X) <= deg_a) && 
                (gf2m_poly_get_deg(b0X) <= deg_b))
            break;
    }

    /* return a2(X), b1(X) */
    gf2m_poly_copy(a2X, a0X);
    gf2m_poly_copy(b1X, b0X);
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
            OUT gf2m* sigX, 
            IN const gf2m* a2X, 
            IN const gf2m* g2X, 
            IN const gf2m* b1X, 
            IN const gf2m* g1X, 
            IN const gf2m_tab* gf2m_tables)
{
    gf2m aX[GF_POLY_LEN] = {0};
    gf2m bX[GF_POLY_LEN] = {0};

    /* a(X), b(X) = a2(X) * g2(X), b1(X) * g1(X) */
    gf2m_poly_mul(aX, a2X, g2X, gf2m_tables);
    gf2m_poly_mul(bX, b1X, g1X, gf2m_tables);

    /* a(X)^2 and b(X)^2 */
    gf2m_poly_mul(aX, aX, aX, gf2m_tables);
    gf2m_poly_mul(bX, bX, bX, gf2m_tables);

    /* sigma(X) = a(X)^2 + x * b(X)^2 */
    for (int i = PARAM_T - 1; i >= 0; i--)
    {
        sigX[i + 1] ^= bX[i];
        sigX[i] ^= aX[i];
    }

    sigX[PARAM_T] = aX[PARAM_T];

    /* Convert sigma(X) to a monic polynomial */
    gf2m_poly_get_monic(sigX, sigX, gf2m_tables);
}

/**
 * @brief comp_err_loc_poly
 * 1. Construct the key equation 2. Find solutions to the key equation 
 * 3. Find the error locator polynomial 
 *
 * @param [out] sigX Error locator polynomial.
 * @param [in] sX Syndrome polynomial.
 * @param [in] gX Goppa polynomial.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void comp_err_loc_poly(OUT gf2m* sigX, IN const gf2m* sX, IN const gf2m* gX, 
                       IN const gf2m_tab* gf2m_tables)
{
    /* Parameters to construct the key equation */
    gf2m vX[GF_POLY_LEN] = {0};
    gf2m g1X[GF_POLY_LEN] = {0};
    gf2m g2X[GF_POLY_LEN] = {0};
    gf2m g12X[GF_POLY_LEN] = {0};
    int deg_a = 0;
    int deg_b = 0;

    /* Solution of the key equation */
    gf2m a2X[GF_POLY_LEN] = {0};
    gf2m b1X[GF_POLY_LEN] = {0};

    /* Convert 'sk->g_X(degree : t-1)' -> monic gX 'gX(degree : t)' <-  */
    gf2m tmp_gx[GF_POLY_LEN]; // monic gX 'gX(degree : t)'
    memcpy(tmp_gx, gX, sizeof(gf2m) * PARAM_T);
    tmp_gx[PARAM_T] = 1;

    construct_key_eqn(vX, g1X, g2X, g12X, sX, tmp_gx, gf2m_tables);

    deg_a = (PARAM_T / 2) - gf2m_poly_get_deg(g2X);
    deg_b = ((PARAM_T - 1) / 2) - gf2m_poly_get_deg(g1X);

    /* Find solutions to the key equation */
    solve_key_eqn(a2X, b1X, vX, g12X, deg_a, deg_b, gf2m_tables);

    /* Find the error locator polynomial */
    get_err_loc_poly(sigX, a2X, g2X, b1X, g1X, gf2m_tables);
}

/**
 * @brief Division by (X^2 + X)^n
 * @brief f0 || f1+f2+f3 || f2+f3 || f3 <- f0 || f1 || f2 || f3
 * 
 * @param [in, out] f polynomial f.
 * @param [in] len Support set.
 */
void div_X2n_Xn(OUT gf2m* f, IN int len)
{
    if (len >= 4)
    {
        int chuch_size = len >> 2; /* len / 4 */
        gf2m* f1 = f + chuch_size;
        gf2m* f2 = f + chuch_size*2;
        gf2m* f3 = f + chuch_size*3;
        for (int i = 0; i < chuch_size; i++)
        {
            gf2m tmp = f2[i] ^ f3[i];
            *(f1 + i) ^= tmp;   /* r = f0    || f1+f2+f3 */
            *(f2 + i) = tmp;    /* q = f2+f3 || f3    */
        }
    }
}

/**
 * @brief Rearrange the list into two parts
 * 
 * rearrange the list such that the first half contains all even-indexed elements
 * (f0 = f[0::2]) and the second half contains all odd-indexed 
 * elements (f1 = f[1::2]). 
 * 
 * @param [in, out] list list for rearrange.
 * @param [in] len list length.
 */
void rearrange_f0f1(OUT gf2m* list, IN int len)
{
    gf2m* tmp;
    get_new_data(gf2m, tmp, len);
    
    int even_idx = 0;           /* even_idx: for f0. f0 = f[0::2] */
    int odd_idx = len >> 1;     /* odd_idx:  for f1. f1 = f[1::2] */

    for (int i = 0; i < len; i += 2) { /* rearrange the list */
        tmp[even_idx++] = list[i];   
        tmp[odd_idx++] = list[i+1];
    }
    memcpy(list, tmp, sizeof(gf2m)*len);

    free(tmp);
}

/**
 * @brief get f0,f1. f -> f0 || f1
 * 
 * @param [in, out] f polynomial f.
 * @param [in] len length of f.
 */
void get_f0f1(OUT gf2m* f, IN int len)
{
    int l = 0;
#if PARAM_T == 128
    l = 8;
#elif PARAM_T == 64
    l = 7;
#endif
    for (int i = 0; i < l-1; i++)
    {
        int chunk = len >> i;
        for (int j = 0; j < (1 << i); j++)
        {
            div_X2n_Xn(f + chunk*j, chunk);
        }
    }
    rearrange_f0f1(f, len);
}

/**
 * @brief compute f(a*X) with the twist a
 * 
 * @param [in, out] f polynomial f.
 * @param [in] len length of f.
 * @param [in] twist twist a.
 * @param [in] gf2m_tabs tables for efficient arithmetic over GF(2^m).
 */
void get_twist_poly(OUT gf2m* f, IN int len, IN gf2m twist, IN const gf2m_tab* gf2m_tabs)
{
    gf2m tmp = 1;
    for (int i = 1; i < len; i++)
    {
        tmp = gf2m_mul_w_tab(tmp, twist, gf2m_tabs);
        f[i] = gf2m_mul_w_tab(f[i], tmp, gf2m_tabs);
    }
}

/**
 * @brief get last element, basis list GG, basis list DD
 * 
 * @param [out] last_elt last element.
 * @param [in, out] GG basis list GG.
 * @param [in, out] DD basis list DD.
 * @param [in] len number of elements in the basis (i.e., length of basis GG, DD. len = |GG| =|DD|).  
 * @param [in] gf2m_tabs tables for efficient arithmetic over GF(2^m).
 */
void get_le_GG_DD(OUT gf2m* last_elt, OUT gf2m* GG, OUT gf2m* DD, 
                  IN int len, IN const gf2m_tab* gf2m_tabs)
{
    *last_elt = DD[len-1];
    gf2m last_elt_inv = gf2m_inv_w_tab(*last_elt, gf2m_tabs->inv_tab);
    for (int i = 0; i < len-1; i++)
    {
        GG[i] = gf2m_mul_w_tab(DD[i], last_elt_inv, gf2m_tabs);
        gf2m g2 = gf2m_squ_w_tab(GG[i], gf2m_tabs->squ_tab);
        DD[i] = g2 ^ GG[i];
    }
}

/**
 * @brief generate all possible elements from a given basis
 * 
 * this function computes all possible linear combinations of the input basis elements
 * 
 * @param [out] all_elts output array that will hold all generated elements
 * @param [in] basis input array representing the basis
 * @param [in] len number of elements in the basis (i.e., the length of the basis array)
 *               
 */
void get_all_elts(OUT gf2m* all_elts, IN gf2m* basis, IN int len)
{
    memset(all_elts, 0, 1 << len);
    int flag = 1;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < flag; j++)
        {
            all_elts[flag + j] = all_elts[j] ^ basis[i];
        }
        flag = 2*flag;
    }
}

/**
 * @brief perform transformation on a list of elements
 * 
 * This function splits the input list into two halves and the 
 * results are stored back in the original list
 * 
 * @param [in, out] list the input and output elements list
 *                       ransformation results will overwrite the original contents.
 * @param [in] g_allelts an array containing all elements generated from a given basis 
 * @param [in] basis_len the length of the basis used to generate `g_allelts`.                 
 * @param [in] gf2m_tabs tables for efficient multiplication in GF(2^m). 
 */
void convert(OUT gf2m* list, IN gf2m* g_allelts, IN int basis_len, IN const gf2m_tab* gf2m_tabs)
{
    int half_len = 1 << basis_len;
    gf2m* A = list;
    gf2m* B = list + half_len;

    for (int i = 0; i < half_len; i++)
    {
        list[i] = A[i] ^ gf2m_mul_w_tab(g_allelts[i], B[i], gf2m_tabs);
        list[half_len + i] = list[i] ^ B[i];
    }
}

/**
 * @brief bit reverse. 
 *   
 * (a0||a1||a2||...||a15) -> (000||a_12||...||a2||a1||a0) 
 * 
 * @param [in] v 16-bit input value to be reversed.
 * @return 13-bit reversed version of the input (after bitwise operations and shifting).
 */
gf2m reversal(IN gf2m v)
{
    v = ((v >> 1) & 0x5555) | ((v & 0x5555) << 1);
    v = ((v >> 2) & 0x3333) | ((v & 0x3333) << 2);
    v = ((v >> 4) & 0x0F0F) | ((v & 0x0F0F) << 4);
    v = ((v >> 8) & 0x00FF) | ((v & 0x00FF) << 8);
    
    return (v >> 3) & 0x1fff;
}

/**
 * @brief Function to find the error vector from the error locator polynomial.
 *
 * @param [out] err_vec Error vector.
 * @param [in] sigX Error locator polynomial.
 * @param [in] L Support set.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void find_err_vec_fft(
            OUT Word* err_vec, 
            IN const gf2m* sigX, 
            IN const gf2m* L, 
            IN const gf2m_tab* gf2m_tabs)
{
    int size_gf2m = (1 << PARAM_M);
    int basis_size = PARAM_M;
    int poly_list_len = 2*PARAM_T;

    /* FFT Phase 1~3 Start */
    /* Phase 1 */
    gf2m basis_D[PARAM_M] = {0,}; /* basis_D = [z^m-1, z^m-2, ..., z^1, z^0] */
    gf2m one = 1;
    for (int i = 0; i < PARAM_M; i++)
    {
        basis_D[i] = one << (PARAM_M-1-i);
    }

    gf2m basis_G[PARAM_M][PARAM_M] = {{0,},};

    int depth = 0;
    if (PARAM_T == 128) 
        depth = 8;
    if (PARAM_T == 64)  
        depth = 7;

    gf2m* f0f1;
    get_new_data(gf2m, f0f1, poly_list_len);
    memcpy(f0f1, sigX, sizeof(gf2m)*(PARAM_T+1)); /* f0f1 <- sigX */

    for (int i = 0; i < depth; i++)
    {
        gf2m last_elt = 0x0;
        /* |G| = |D| = basis_size */
        get_le_GG_DD(&last_elt, basis_G[i], basis_D, basis_size, gf2m_tabs);
        basis_size = basis_size - 1;

        int num_chunk = 1 << i;

        for (int j = 0; j < num_chunk; j++)
        {
            int chunk = poly_list_len >> i;
            get_twist_poly(f0f1 + chunk*j, chunk, last_elt, gf2m_tabs);
            get_f0f1(f0f1 + chunk*j, chunk);
        }
    }

    /* Phase 2: Compute FFT(g,..,GG) when deg(g)=0 */
    gf2m* all_elts;
    get_new_data(gf2m, all_elts, size_gf2m);
    int elt_chunk = 1 << (PARAM_M-depth);

    for (int i = 0; i < poly_list_len; i++)
    {
        for (int j = 0; j < elt_chunk; j++)
        {
            all_elts[elt_chunk*i + j] = f0f1[i];
        }
    }

    /* Phase 3 */
    for (int i = depth; i > 0; i--)
    {
        gf2m* g_allelts;
        get_new_data(gf2m, g_allelts, 1 << (PARAM_M-i));
        get_all_elts(g_allelts, basis_G[i-1], PARAM_M-i);
       
        int chunklen = 1 << (PARAM_M-i + 1);
        int num_chunk = (1 << PARAM_M) / chunklen;
        for (int j = 0; j < num_chunk; j++)
        {
            int idx_s = chunklen*j;
            convert(all_elts + idx_s, g_allelts, PARAM_M-i, gf2m_tabs);
        }
        free(g_allelts);  
    }
    /* FFT Phase 1~3 End */

    /* Finding the error vector phase 1~2 */
    /* phase 1: reverse the list LL */
    /* LL[2^13] = {0,0,...,1,.0.0,,,,1} */
    u08* LL;
    get_new_data(u08, LL, 1 << PARAM_M);

    for (gf2m i = 0; i < 1 << PARAM_M; i++)
    {
        if (all_elts[i] == 0)
        {
            LL[reversal(i)]= 1;
        }
    }

    /* phase 2: Finding the error vector */
    /* if deg(sigX) != t, then return 0^n */
    u08 delta = (gf2m_poly_get_deg(sigX) == PARAM_T);

    for (int i = 0; i < PARAM_N; i++)
    {
        Word err = (LL[L[i]] == 1);
        err &= delta;
        err_vec[i / WORD_BITS] |= (err << (i % WORD_BITS));
    }
    free(LL);

    free(f0f1);
    free(all_elts);
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
    gf2m sX[GF_POLY_LEN] = {0};   // Syndrome polynomial
    gf2m sigX[GF_POLY_LEN] = {0}; // Error locator polynomial

    /* Obtain the syndrome polynomial from the syndrome vector */
    to_poly(sX, synd_vec);

    /* 
       1. Construct the key equation 
       2. Find solutions to the key equation 
       3. Find the error locator polynomial 
    */
    comp_err_loc_poly(sigX, sX, sk->gX, gf2m_tables);

    /* Find the error vector */
    find_err_vec_fft(err_vec, sigX, sk->L, gf2m_tables);
}