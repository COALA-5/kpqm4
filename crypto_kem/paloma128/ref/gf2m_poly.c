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

#include "gf2m_poly.h"

/**
 * @brief Function to return the degree of a polynomial.
 *
 * @param [out] aX Polynomial.
 * @return int Degree of the polynomial.
 */
int gf2m_poly_get_deg(IN const gf2m* aX)
{
    int degree = -1;

    /* Get degree of polynomial */
    for (int i = 0; i < GF_POLY_LEN; i++)
    {
        degree = i * (aX[i] != 0) + degree * (aX[i] == 0);
    }
    
    return degree;
}

/**
 * @brief Function to copy a polynomial.
 *
 * @param [out] cX Destination polynomial.
 * @param [in] aX Source polynomial.
 */
void gf2m_poly_copy(OUT gf2m* cX, IN const gf2m* aX)
{
	/* Copy */
	memcpy(cX, aX, sizeof(gf2m) * GF_POLY_LEN);
}

/**
 * @brief Function to convert a polynomial to a monic polynomial.
 *
 * @param [out] cX Monic polynomial.
 * @param [in] aX Polynomial.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_get_monic(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m lc_inv = 0;
	int deg = 0;

	/* Get inverse of leading coefficient */
	deg = gf2m_poly_get_deg(aX);
	lc_inv = gf2m_inv_w_tab(aX[deg], gf2m_tables->inv_tab);

	/* Change to monic polynomial */
	for (int i = 0; i <= PARAM_T; i++)
		cX[i] = gf2m_mul_w_tab(aX[i], lc_inv, gf2m_tables);
}

/**
 * @brief Polynomial addition function.
 *
 * @param [out] cX Result of addition.
 * @param [in] aX Operand.
 * @param [in] bX Operand.
 */
void gf2m_poly_add(OUT gf2m* cX, IN const gf2m* aX, IN const gf2m* bX)
{
	/* Addition */
	for (int i = 0; i <= PARAM_T; i++)
		cX[i] = gf2m_add(aX[i], bX[i]);
}

/**
 * @brief Karatsuba Multiplication.
 * 
 * @param [out] cX Result of multiplication.
 * @param [in] aX Operand.
 * @param [in] bX Operand.
 * @param [in] deg maximum degree to multiplication.  
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void kara(
	OUT gf2m* cX, 
	IN const gf2m* aX, 
	IN const gf2m* bX, 
	IN u64 deg, 
	IN const gf2m_tab* gf2m_tables)
{
	u64 n_deg = deg >> 1;

	gf2m a1[GF_POLY_LEN] = {0}, a2[GF_POLY_LEN] = {0}, 
		 b1[GF_POLY_LEN] = {0}, b2[GF_POLY_LEN] = {0},
		 ta[GF_POLY_LEN] = {0}, tb[GF_POLY_LEN] = {0};

	gf2m m1[2 * PARAM_T + 1] = {0}, 
	     m2[2 * PARAM_T + 1] = {0}, 
		 m3[2 * PARAM_T + 1] = {0};

	/* stop recursion */
    if (deg == 16)
    {
        for (int i = 0; i <= deg; i++)
        {
            for (int j = 0; j <= deg; j++)
            {
                cX[i + j] ^= gf2m_mul_w_tab(aX[i], bX[j], gf2m_tables);
            }
        }

        return;
    }

    /* split */
    for (int i = 0; i < n_deg; i++)
    {
        a1[i] = aX[i];
		b1[i] = bX[i];
		a2[i] = aX[n_deg + i];
		b2[i] = bX[n_deg + i];
    }
    a2[n_deg] = aX[deg];
	b2[n_deg] = bX[deg];

    /* high and low */
    kara(m1, a1, b1, n_deg, gf2m_tables);
    kara(m2, a2, b2, n_deg, gf2m_tables);

	/* middle */
    for (int i = 0; i <= n_deg; i++)
    {
        ta[i] = a1[i] ^ a2[i];
		tb[i] = b1[i] ^ b2[i];
    }
    kara(m3, ta, tb, n_deg, gf2m_tables);
	
	/* final */
    for (int i = 0; i <= deg; i++)
    {
		cX[i] ^= m1[i];
		cX[deg + i] ^= m2[i];
		cX[n_deg + i] ^= m1[i] ^ m2[i] ^ m3[i];
    }
}

/**
 * @brief Polynomial multiplication function. (Without reduction.)
 *
 * @param [out] cX Result of multiplication.
 * @param [in] aX Operand.
 * @param [in] bX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_mul(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m* bX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX[PARAM_T * 2 + 1] = {0};

	/* Multiplication */
	kara(retX, aX, bX, PARAM_T, gf2m_tables);

	/* Return */
	gf2m_poly_copy(cX, retX);
}

/**
 * @brief Polynomial multiplication function.
 *
 * @param [out] cX Result of multiplication.
 * @param [in] aX Operand.
 * @param [in] bX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_mul_mod(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m* bX, 
			IN const gf2m* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX[PARAM_T * 2 + 1] = {0};

	/* Multiplication */
	kara(retX, aX, bX, PARAM_T, gf2m_tables);

	/* Get degree of g(X) (or g12(X)) */
	int dg = gf2m_poly_get_deg(gX);

	/* Reduction */
	for (int i = PARAM_T * 2; i >= dg; i--)
		for (int j = 0; j <= dg; j++)
			retX[i - dg + j] ^= gf2m_mul_w_tab(retX[i], gX[j], gf2m_tables);

	/* Return */
	gf2m_poly_copy(cX, retX);
}

/**
 * @brief Polynomial square function.
 *
 * @param [out] cX Result of squaring.
 * @param [in] aX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_squ_mod(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX[PARAM_T * 2 + 1] = {0};

	/* Squaring */
	for (int i = 0; i <= PARAM_T; i++)
		retX[2 * i] ^= gf2m_squ_w_tab(aX[i], gf2m_tables->squ_tab);

	/* Get degree of g(X) (or g12(X)) */
	int dg = gf2m_poly_get_deg(gX);

	/* Reduction */
	for (int i = PARAM_T * 2; i >= dg; i--)
		for (int j = 0; j < dg + 1; j++)
			retX[i - dg + j] ^= gf2m_mul_w_tab(retX[i], gX[j], gf2m_tables);

	/* Return */
	gf2m_poly_copy(cX, retX);
}

/**
 * @brief Polynomial square root function.
 *
 * @param [out] cX Result of multiplication.
 * @param [in] aX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_sqrt_mod(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX[GF_POLY_LEN] = {0};
	gf2m fX_even[GF_POLY_LEN] = {0};
	gf2m fX_odd[GF_POLY_LEN] = {0};

	/* Set f_even(X) and f_odd(X) */
	for (int i = PARAM_T / 2 - 1; i >= 0; i--)
	{
		fX_even[i] = gf2m_sqrt_w_tab(aX[i * 2], gf2m_tables->sqrt_tab);
		fX_odd[i] = gf2m_sqrt_w_tab(aX[i * 2 + 1], gf2m_tables->sqrt_tab);
	}

	fX_even[PARAM_T / 2] = gf2m_sqrt_w_tab(aX[PARAM_T], gf2m_tables->sqrt_tab);

	/* c(X) = x */
	retX[1] = 1;

	/* Compute sqrt(X) */
	for (int i = 0; i < PARAM_M - 1; i++)
		gf2m_poly_squ_mod(retX, retX, gX, gf2m_tables);

	/* c(X) = f_even(X) + f_odd(X) * sqrt(X) mod g(X) */
	gf2m_poly_mul_mod(retX, retX, fX_odd, gX, gf2m_tables);
	gf2m_poly_add(retX, retX, fX_even);

	/* Return */
	gf2m_poly_copy(cX, retX);
}

/**
 * @brief Polynomial division function. (No reduction.)
 *
 * @param [out] qX Quotient.
 * @param [out] rX Remainder.
 * @param [in] aX Dividend.
 * @param [in] bX Divisor.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_div(
			OUT gf2m* qX, 
			OUT gf2m* rX, 
			IN const gf2m* aX, 
			IN const gf2m* bX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX_q[GF_POLY_LEN] = {0};
	gf2m retX_r[GF_POLY_LEN] = {0};
	int db, dr;

	if (gf2m_poly_get_deg(bX) == -1)
    {
		return;
    }

	/* r(X) = a(X) */
	gf2m_poly_copy(retX_r, aX);

	/* Get degree of r(X) and b(X) */
	dr = gf2m_poly_get_deg(retX_r);
	db = gf2m_poly_get_deg(bX);

	while ((dr >= db) && (dr != -1))
	{
		gf2m tmpX;

		/* q(X) = q(X) + lead(r(X))/lead(b(X)) */
		tmpX = gf2m_inv_w_tab(bX[db], gf2m_tables->inv_tab);
		tmpX = gf2m_mul_w_tab(retX_r[dr], tmpX, gf2m_tables);
		retX_q[dr - db] ^= tmpX;

		/* r(X) = r(X) - lead(r(X))/lead(b(X)) * b(X) */
		for (int i = 0; i <= db; i++)
			retX_r[i + dr - db] ^= gf2m_mul_w_tab(tmpX, bX[i], gf2m_tables);

		/* Get degree of r(X) */
		dr = gf2m_poly_get_deg(retX_r);
	}

	/* Return */
	gf2m_poly_copy(qX, retX_q);
	gf2m_poly_copy(rX, retX_r);
}

/**
 * @brief Polynomial inverse function. (Using extended Euclidean algorithm)
 *
 * @param [out] cX Inverse.
 * @param [in] aX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_inv_mod(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX[GF_POLY_LEN] = {1};

	for (int i = 0; i < PARAM_M - 1; i++)
	{
		gf2m_poly_squ_mod(retX, retX, gX, gf2m_tables);
		gf2m_poly_mul_mod(retX, retX, aX, gX, gf2m_tables);
	}

	gf2m_poly_squ_mod(retX, retX, gX, gf2m_tables);

	/* Return */
	gf2m_poly_copy(cX, retX);
}

/**
 * @brief Polynomial evaluation function (using Horner's algorithm)
 *
 * @param [in] aX Polynomial to evaluate.
 * @param [in] gfa Value to substitute.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 * @return gf2m Evaluation result.
 */
gf2m gf2m_poly_eval(IN const gf2m* aX, IN gf2m gfa, IN const gf2m_tab* gf2m_tables)
{
	gf2m ret = aX[PARAM_T];

	/* Compute a(gf_a) */
	for (int i = PARAM_T - 1; i >= 0; i--)
	{
		ret = gf2m_mul_w_tab(ret, gfa, gf2m_tables);
		ret = gf2m_add(ret, aX[i]);
	}

	return ret;
}

/**
 * @brief Polynomial greatest common divisor function.
 *
 * @param [out] cX Greatest common divisor.
 * @param [in] aX Operand.
 * @param [in] bX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf2m_poly_gcd(
			OUT gf2m* cX, 
			IN const gf2m* aX, 
			IN const gf2m* bX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf2m retX[GF_POLY_LEN] = {0};
	gf2m rX_old[GF_POLY_LEN] = {0};
	gf2m rX_new[GF_POLY_LEN] = {0};
	gf2m rX[GF_POLY_LEN] = {0};
	gf2m qX[GF_POLY_LEN] = {0};

	/* old_r(X), new_r(X) = a(X), b(X) */
	gf2m_poly_copy(rX_old, aX);
	gf2m_poly_copy(rX_new, bX);

	while (gf2m_poly_get_deg(rX_new) >= 0)
	{
		/* q(X) = old_r(X) / new_r(X) */
		gf2m_poly_div(qX, rX, rX_old, rX_new, gf2m_tables);

		/* old_r(X), new_r(X) = new_r(X), old_r(X) - q(X) * new_r(X) */
		gf2m_poly_copy(rX_old, rX_new);
		gf2m_poly_copy(rX_new, rX);
	}

	/* Change to monic polynomial */
	gf2m_poly_get_monic(retX, rX_old, gf2m_tables);

	/* Return */
	gf2m_poly_copy(cX, retX);
}