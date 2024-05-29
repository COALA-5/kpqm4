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

#include "gf_poly.h"

/**
 * @brief Function to return the degree of a polynomial.
 *
 * @param [out] aX Polynomial.
 * @return int Degree of the polynomial.
 */
int gf_poly_get_degree(IN const gf* aX)
{
	/* Get degree of polynomial */
	for (int i = PARAM_T; i >= 0; i--)
	{
		if (aX[i] != 0)
			return i;
	}

	/* Polynomial is zero */
	return -1;
}

/**
 * @brief Function to copy a polynomial.
 *
 * @param [out] cX Destination polynomial.
 * @param [in] aX Source polynomial.
 */
void gf_poly_copy(OUT gf* cX, IN const gf* aX)
{
	/* Copy */
	memcpy(cX, aX, sizeof(gf) * GF_POLY_LEN);
}

/**
 * @brief Function to convert a polynomial to a monic polynomial.
 *
 * @param [out] cX Monic polynomial.
 * @param [in] aX Polynomial.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf_poly_change_monic(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf lc_inv = 0;
	int deg = 0;

	/* Get inverse of leading coefficient */
	deg = gf_poly_get_degree(aX);
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
void gf_poly_add(OUT gf* cX, IN const gf* aX, IN const gf* bX)
{
	/* Addition */
	for (int i = 0; i <= PARAM_T; i++)
		cX[i] = gf2m_add(aX[i], bX[i]);
}

/**
 * @brief Polynomial multiplication function. (Without reduction.)
 *
 * @param [out] cX Result of multiplication.
 * @param [in] aX Operand.
 * @param [in] bX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void poly_mul(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf* bX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX[PARAM_T * 2 + 1] = {0};

	/* Multiplication */
	for (int i = 0; i <= PARAM_T; i++)
		for (int j = 0; j <= PARAM_T; j++)
			retX[i + j] ^= gf2m_mul_w_tab(aX[i], bX[j], gf2m_tables);

	/* Return */
	gf_poly_copy(cX, retX);
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
void gf_poly_mul(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf* bX, 
			IN const gf* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX[PARAM_T * 2 + 1] = {0};

	/* Multiplication */
	for (int i = 0; i <= PARAM_T; i++)
		for (int j = 0; j <= PARAM_T; j++)
			retX[i + j] ^= gf2m_mul_w_tab(aX[i], bX[j], gf2m_tables);

	/* Get degree of g(X) (or g12(X)) */
	int dg = gf_poly_get_degree(gX);

	/* Reduction */
	for (int i = PARAM_T * 2; i >= dg; i--)
		for (int j = 0; j <= dg; j++)
			retX[i - dg + j] ^= gf2m_mul_w_tab(retX[i], gX[j], gf2m_tables);

	/* Return */
	gf_poly_copy(cX, retX);
}

/**
 * @brief Polynomial square function.
 *
 * @param [out] cX Result of squaring.
 * @param [in] aX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf_poly_sqr(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX[PARAM_T * 2 + 1] = {0};

	/* Squaring */
	for (int i = 0; i <= PARAM_T; i++)
		retX[2 * i] ^= gf2m_square_w_tab(aX[i], gf2m_tables->square_tab);

	/* Get degree of g(X) (or g12(X)) */
	int dg = gf_poly_get_degree(gX);

	/* Reduction */
	for (int i = PARAM_T * 2; i >= dg; i--)
		for (int j = 0; j < dg + 1; j++)
			retX[i - dg + j] ^= gf2m_mul_w_tab(retX[i], gX[j], gf2m_tables);

	/* Return */
	gf_poly_copy(cX, retX);
}

/**
 * @brief Polynomial square root function.
 *
 * @param [out] cX Result of multiplication.
 * @param [in] aX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf_poly_sqrt(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX[GF_POLY_LEN] = {0};
	gf fX_even[GF_POLY_LEN] = {0};
	gf fX_odd[GF_POLY_LEN] = {0};

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
		gf_poly_sqr(retX, retX, gX, gf2m_tables);

	/* c(X) = f_even(X) + f_odd(X) * sqrt(X) mod g(X) */
	gf_poly_mul(retX, retX, fX_odd, gX, gf2m_tables);
	gf_poly_add(retX, retX, fX_even);

	/* Return */
	gf_poly_copy(cX, retX);
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
void gf_poly_div(
			OUT gf* qX, 
			OUT gf* rX, 
			IN const gf* aX, 
			IN const gf* bX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX_q[GF_POLY_LEN] = {0};
	gf retX_r[GF_POLY_LEN] = {0};
	int db, dr;

	/* r(X) = a(X) */
	gf_poly_copy(retX_r, aX);

	/* Get degree of r(X) and b(X) */
	dr = gf_poly_get_degree(retX_r);
	db = gf_poly_get_degree(bX);

	while ((dr >= db) && (dr != -1))
	{
		gf tmpX;

		/* q(X) = q(X) + lead(r(X))/lead(b(X)) */
		tmpX = gf2m_inv_w_tab(bX[db], gf2m_tables->inv_tab);
		tmpX = gf2m_mul_w_tab(retX_r[dr], tmpX, gf2m_tables);
		retX_q[dr - db] ^= tmpX;

		/* r(X) = r(X) - lead(r(X))/lead(b(X)) * b(X) */
		for (int i = 0; i <= db; i++)
			retX_r[i + dr - db] ^= gf2m_mul_w_tab(tmpX, bX[i], gf2m_tables);

		/* Get degree of r(X) */
		dr = gf_poly_get_degree(retX_r);
	}

	/* Return */
	gf_poly_copy(qX, retX_q);
	gf_poly_copy(rX, retX_r);
}

/**
 * @brief Polynomial inverse function. (Using extended Euclidean algorithm)
 *
 * @param [out] cX Inverse.
 * @param [in] aX Operand.
 * @param [in] gX Operand.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 */
void gf_poly_inv(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf* gX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX[GF_POLY_LEN] = {0};
	gf rX_old[GF_POLY_LEN] = {0};
	gf rX_new[GF_POLY_LEN] = {0};
	gf sX_old[GF_POLY_LEN] = {0};
	gf sX_new[GF_POLY_LEN] = {0};
	gf qX[GF_POLY_LEN] = {0};
	gf rX[GF_POLY_LEN] = {0};
	gf tmpX[GF_POLY_LEN] = {0};
	gf ir;

	/* old_r(X) = g(X), new_r(X) = a(X) */
	gf_poly_copy(rX_old, gX);
	gf_poly_copy(rX_new, aX);

	/* old_s(X) = 0, new_s(X) = 1 */
	sX_new[0] = 1;

	while (gf_poly_get_degree(rX_new) != -1)
	{
		gf_poly_div(qX, rX, rX_old, rX_new, gf2m_tables);

		/* old_r(X), new_r(X) = new_r(X), old_r(X) − q(X) × new_r(X) */
		gf_poly_copy(rX_old, rX_new);
		gf_poly_copy(rX_new, rX);

		/* old_s(X), new_s(X) = new_s(X), old_s(X) − q(X) × new_s(X) */
		gf_poly_mul(tmpX, qX, sX_new, gX, gf2m_tables);

		gf_poly_add(tmpX, tmpX, sX_old);
		gf_poly_copy(sX_old, sX_new);
		gf_poly_copy(sX_new, tmpX);
	}

	/* Check irreducible */
	if (gf_poly_get_degree(rX_old) > 0)
	{
		// printf("Inverse error.\n");
		// exit(1);
		return 1;
	}

	/* Compute inverse of r_0 of old_r(X) */
	ir = gf2m_tables->inv_tab[rX_old[0]];

	/* c(X) = 1 / r_0 * old_s(X) */
	for (int i = 0; i <= PARAM_T; i++)
		retX[i] = gf2m_mul_w_tab(sX_old[i], ir, gf2m_tables);

	/* Return */
	gf_poly_copy(cX, retX);
}

/**
 * @brief Polynomial evaluation function (using Horner's algorithm)
 *
 * @param [in] aX Polynomial to evaluate.
 * @param [in] gfa Value to substitute.
 * @param [in] gf2m_tables GF(2^m) operation tables.
 * @return gf Evaluation result.
 */
gf gf_poly_eval(IN const gf* aX, IN gf gfa, IN const gf2m_tab* gf2m_tables)
{
	gf ret = aX[PARAM_T];

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
void gf_poly_gcd(
			OUT gf* cX, 
			IN const gf* aX, 
			IN const gf* bX, 
			IN const gf2m_tab* gf2m_tables)
{
	gf retX[GF_POLY_LEN] = {0};
	gf rX_old[GF_POLY_LEN] = {0};
	gf rX_new[GF_POLY_LEN] = {0};
	gf rX[GF_POLY_LEN] = {0};
	gf qX[GF_POLY_LEN] = {0};

	/* old_r(X), new_r(X) = a(X), b(X) */
	gf_poly_copy(rX_old, aX);
	gf_poly_copy(rX_new, bX);

	while (gf_poly_get_degree(rX_new) >= 0)
	{
		/* q(X) = old_r(X) / new_r(X) */
		gf_poly_div(qX, rX, rX_old, rX_new, gf2m_tables);

		/* old_r(X), new_r(X) = new_r(X), old_r(X) - q(X) * new_r(X) */
		gf_poly_copy(rX_old, rX_new);
		gf_poly_copy(rX_new, rX);
	}

	/* Change to monic polynomial */
	gf_poly_change_monic(retX, rX_old, gf2m_tables);

	/* Return */
	gf_poly_copy(cX, retX);
}