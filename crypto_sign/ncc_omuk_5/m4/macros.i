#ifndef MACROS_I
#define MACROS_I
// 3
.macro montgomery_mul_32 a, b, Qprime, Q, tmp, tmp2
    smull \tmp, \a, \a, \b
    mul \tmp2, \tmp, \Qprime
    smlal \tmp, \a, \tmp2, \Q
.endm

// 2
.macro addSub1 c0, c1
    add.w \c0, \c1
    sub.w \c1, \c0, \c1, lsl #1
.endm

// 3
.macro addSub2 c0, c1, c2, c3
    add \c0, \c1
    add \c2, \c3
    sub.w \c1, \c0, \c1, lsl #1
    sub.w \c3, \c2, \c3, lsl #1
.endm

// 6
.macro addSub4 c0, c1, c2, c3, c4, c5, c6, c7
    add \c0, \c1
    add \c2, \c3
    add \c4, \c5
    add \c6, \c7
    sub.w \c1, \c0, \c1, lsl #1
    sub.w \c3, \c2, \c3, lsl #1
    sub.w \c5, \c4, \c5, lsl #1
    sub.w \c7, \c6, \c7, lsl #1
.endm

/*
.macro last_GS c0, c1, c2, c3, c4, c5, c6, c7, f, Qprime, Q, tmp, tmp2, f1, f2
    add \c0, \c1
    add \c2, \c3
    add \c4, \c5
    add \c6, \c7
    sub.w \c1, \c0, \c1, lsl #1
    sub.w \c3, \c2, \c3, lsl #1
    sub.w \c5, \c4, \c5, lsl #1
    sub.w \c7, \c6, \c7, lsl #1
.endm
*/

.macro _2_layer_CT_32 c0, c1, c2, c3, zeta0, zeta1, zeta2, Qprime, Q, tmp, tmp2
    montgomery_mul_32 \c2, \zeta0, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \zeta0, \Qprime, \Q, \tmp, \tmp2
    addSub2 \c0, \c2, \c1, \c3

    montgomery_mul_32 \c1, \zeta1, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \zeta2, \Qprime, \Q, \tmp, \tmp2
    addSub2 \c0, \c1, \c2, \c3
.endm

.macro _2_layer_GS_32 c0, c1, c2, c3, zeta0, zeta1, zeta2, Qprime, Q, tmp, tmp2
    addSub2 \c0, \c1, \c2, \c3
    montgomery_mul_32 \c1, \zeta0, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \zeta1, \Qprime, \Q, \tmp, \tmp2    

    addSub2 \c0, \c2, \c1, \c3
    montgomery_mul_32 \c2, \zeta2, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \zeta2, \Qprime, \Q, \tmp, \tmp2    
.endm



.macro _first_3_layer_CT_32 c0, c1, c2, c3, c4, c5, c6, c7, xi0, xi1, xi2, xi3, xi4, xi5, xi6, twiddle, Qprime, Q, tmp, tmp2
    vmov.w \twiddle, \xi0
    vmov.w s10, \c4
	vmov.w s11, \c5
	vmov.w s12, \c6
	vmov.w s13, \c7
    montgomery_mul_32 \c4, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub4 \c0, \c4, \c1, \c5, \c2, \c6, \c3, \c7
    vmov.w \tmp, s10
    add \c4, \tmp
    vmov.w \tmp, s11
    add \c5, \tmp
    vmov.w \tmp, s12
    add \c6, \tmp
    vmov.w \tmp, s13
    add \c7, \tmp

    vmov.w \twiddle, \xi1
    montgomery_mul_32 \c2, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi2
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub4 \c0, \c2, \c1, \c3, \c4, \c6, \c5, \c7

    vmov.w \twiddle, \xi3
    montgomery_mul_32 \c1, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi4
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi5
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi6
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub4 \c0, \c1, \c2, \c3, \c4, \c5, \c6, \c7
.endm

.macro _3_layer_CT_32 c0, c1, c2, c3, c4, c5, c6, c7, xi0, xi1, xi2, xi3, xi4, xi5, xi6, twiddle, Qprime, Q, tmp, tmp2
    vmov.w \twiddle, \xi0
    montgomery_mul_32 \c4, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub4 \c0, \c4, \c1, \c5, \c2, \c6, \c3, \c7

    vmov.w \twiddle, \xi1
    montgomery_mul_32 \c2, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi2
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub4 \c0, \c2, \c1, \c3, \c4, \c6, \c5, \c7

    vmov.w \twiddle, \xi3
    montgomery_mul_32 \c1, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi4
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi5
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi6
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub4 \c0, \c1, \c2, \c3, \c4, \c5, \c6, \c7
.endm



.macro _3_layer_GS_32 c0, c1, c2, c3, c4, c5, c6, c7, xi0, xi1, xi2, xi3, xi4, xi5, xi6, twiddle, Qprime, Q, tmp, tmp2
    addSub4 \c0, \c1, \c2, \c3, \c4, \c5, \c6, \c7
    vmov.w \twiddle, \xi0    
    montgomery_mul_32 \c1, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi1
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi2
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi3
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    
    
    addSub4 \c0, \c2, \c1, \c3, \c4, \c6, \c5, \c7
    vmov.w \twiddle, \xi4
    montgomery_mul_32 \c2, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi5
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    
    
    addSub4 \c0, \c4, \c1, \c5, \c2, \c6, \c3, \c7
    vmov.w \twiddle, \xi6
    montgomery_mul_32 \c4, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2   
        
.endm

.macro _last_3_layer_GS_32 c0, c1, c2, c3, c4, c5, c6, c7, xi0, xi1, xi2, xi3, xi4, xi5, xi6, twiddle, Qprime, Q, tmp, tmp2, f1, f2
    addSub4 \c0, \c1, \c2, \c3, \c4, \c5, \c6, \c7
    vmov.w \twiddle, \xi0    
    montgomery_mul_32 \c1, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi1
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi2
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi3
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    
    
    addSub4 \c0, \c2, \c1, \c3, \c4, \c6, \c5, \c7
    vmov.w \twiddle, \xi4
    montgomery_mul_32 \c2, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    vmov.w \twiddle, \xi5
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2
    
    
    
    addSub4 \c0, \c4, \c1, \c5, \c2, \c6, \c3, \c7
    vmov.w \twiddle, \xi6
    montgomery_mul_32 \c4, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2   

    //last GS
    vmov \twiddle, \f1
    sub \c0, \c0, \c4
    sub \c1, \c1, \c5
    sub \c2, \c2, \c6
    sub \c3, \c3, \c7
    montgomery_mul_32 \c0, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c1, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c2, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2

    vmov \twiddle, \f2
    montgomery_mul_32 \c4, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c5, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    montgomery_mul_32 \c7, \twiddle, \Qprime, \Q, \tmp, \tmp2    
    
.endm

/************************************************************
* Name:         _3_layer_inv_butterfly_light_fast_first
*
* Description:  upper half of 3-layer inverse butterfly
*               defined over X^8 - 1
*
* Input:        (c4, c1, c6, c3) = coefficients on the upper half;
*               (xi0, xi1, xi2, xi3, xi4, xi5, xi6) =
*               (  1,  1,  w_4,   1, w_8, w_4, w_8^3) in
*               Montgomery domain
*
* Symbols:      R = 2^32
*
* Constants:    Qprime = -MOD^{-1} mod^{+-} R, Q = MOD
*
* Output:
*               c4 =  c4 + c1        + (c6 + c3)
*               c5 = (c4 - c1) w_4   + (c6 + c3) w_8^3
*               c6 =  c4 + c1        - (c6 + c3)
*               c7 = (c4 - c1) w_8^3 + (c6 + c3) w_4
************************************************************/
// 15
.macro _3_layer_inv_butterfly_light_fast_first c0, c1, c2, c3, c4, c5, c6, c7, xi0, xi1, xi2, xi3, xi4, xi5, xi6, twiddle, Qprime, Q, tmp, tmp2
    addSub2 \c4, \c1, \c6, \c3
    addSub1 \c4, \c6

    vmov.w \tmp, \xi4
    vmov.w \tmp2, \xi6

    smull.w \c0, \c5, \c1, \tmp
    smlal.w \c0, \c5, \c3, \tmp2
    mul.w \twiddle, \c0, \Qprime
    smlal.w \c0, \c5, \twiddle, \Q

    smull.w \c2, \c7, \c1, \tmp2
    smlal.w \c2, \c7, \c3, \tmp
    mul.w \twiddle, \c2, \Qprime
    smlal.w \c2, \c7, \twiddle, \Q
.endm

/************************************************************
* Name:         _3_layer_inv_butterfly_light_fast_second
*
* Description:  lower half of 3-layer inverse butterfly
*               defined over X^8 - 1, and the 2nd
*               layer of butterflies
*
* Input:
*               (c4, c5, c6, c7) = results of the upper half;
*               (c0, c1, c2, c3) = coefficients on the lower half;
*               (xi0, xi1, xi2, xi3, xi4, xi5, xi6) =
*               (  1,  1,  w_4,   1, w_8, w_4, w_8^3) in
*               Montgomery domain
*
* Symbols:      R = 2^32
*
* Constants:    Qprime = -MOD^{-1} mod^{+-} R, Q = MOD
*
* Output:       (normal order)
*               c0 =   c0 + c1     + (c2 + c3)         + (  c4 + c5     + (c6 + c7)       )
*               c1 =  (c0 - c1) w3 + (c2 - c3)  w4     + ( (c4 - c5) w5 + (c6 - c7) w6    )
*               c2 = ( c0 + c1     - (c2 + c3)) w1     + (( c4 + c5     - (c6 + c7)   ) w2)
*               c3 = ((c0 - c1) w3 - (c2 - c3)  w4) w1 + (((c4 - c5) w5 - (c6 - c7) w6) w2)
*               c4 =   c0 + c1     - (c2 + c3)         - (  c4 + c5     + (c6 + c7)       ) w0
*               c5 =  (c0 - c1) w3 + (c2 - c3)  w4     - ( (c4 - c5) w5 + (c6 - c7) w6    ) w0
*               c6 = ( c0 + c1     - (c2 + c3)) w1     - (( c4 + c5     - (c6 + c7)   ) w2) w0
*               c7 = ((c0 - c1) w3 - (c2 - c3)  w4) w1 - (((c4 - c5) w5 - (c6 - c7) w6) w2) w0
************************************************************/
// 19
.macro _3_layer_inv_butterfly_light_fast_second c0, c1, c2, c3, c4, c5, c6, c7, xi0, xi1, xi2, xi3, xi4, xi5, xi6, twiddle, Qprime, Q, tmp, tmp2
    addSub2 \c0, \c1, \c2, \c3

    vmov.w \twiddle, \xi2
    montgomery_mul_32 \c3, \twiddle, \Qprime, \Q, \tmp, \tmp2
    addSub2 \c0, \c2, \c1, \c3

    montgomery_mul_32 \c6, \twiddle, \Qprime, \Q, \tmp, \tmp2

    addSub4 \c0, \c4, \c1, \c5, \c2, \c6, \c3, \c7
.endm


//radix_3 *******************************

.macro CT_32_butterfly_radix3 c, c0, c1, c2, zeta0, zeta1, Qprime, Q, Wmont, tmp, tmp2
    montgomery_mul_32 \c1, \zeta0, \Qprime, \Q, \tmp, \tmp2 //t1
	montgomery_mul_32 \c2, \zeta1, \Qprime, \Q, \tmp, \tmp2 //t2
	mov.w \c, \c1
	sub \c, \c2
	montgomery_mul_32 \c, \Wmont, \Qprime, \Q, \tmp, \tmp2 //t3

    mov.w \tmp, \c0
	sub \tmp, \c1
	sub \tmp, \c

	mov.w \tmp2, \c0
	sub \tmp2, \c2
	add \tmp2, \c

	add \c0, \c1
	add \c0, \c2
.endm

.macro _2_layer_CT_32_radix3 c_tmp, c_tmp0, c_tmp1, c_tmp2, xi0, xi1, xi2, xi3, xi4, xi5, xi6, xi7, xi8, z0, z1, z2, z3, z4, z5, z6, z7, zeta0, zeta1, Qprime, Q, Wmont, tmp, tmp2
    vmov \zeta0, \z0
    vmov \zeta1, \z1
    vmov \c_tmp0, \xi0
    vmov \c_tmp1, \xi3
    vmov \c_tmp2, \xi6
    
    CT_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi0, \c_tmp0
    vmov \xi3, \tmp2
    vmov.w \xi6, \tmp
    
    vmov.w \c_tmp0, \xi1
    vmov.w \c_tmp1, \xi4
    vmov.w \c_tmp2, \xi7
    
    CT_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov.w \xi1, \c_tmp0
    vmov.w \xi4, \tmp2
    vmov.w \xi7, \tmp
    
    vmov.w \c_tmp0, \xi2
    vmov.w \c_tmp1, \xi5
    vmov.w \c_tmp2, \xi8
    
    CT_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov.w \xi2, \c_tmp0
    vmov.w \xi5, \tmp2
    vmov.w \xi8, \tmp
    
    
    vmov.w \zeta0, \z2
    vmov.w \zeta1, \z3
    
    vmov.w \c_tmp0, \xi0
    vmov.w \c_tmp1, \xi1
    vmov.w \c_tmp2, \xi2
    
    CT_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov.w \xi0, \c_tmp0
    vmov.w \xi1, \tmp2
    vmov.w \xi2, \tmp
    
    vmov.w \zeta0, \z4
    vmov.w \zeta1, \z5
    
    vmov.w \c_tmp0, \xi3
    vmov.w \c_tmp1, \xi4
    vmov.w \c_tmp2, \xi5
    
    CT_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov.w \xi3, \c_tmp0
    vmov.w \xi4, \tmp2
    vmov.w \xi5, \tmp
    
    vmov.w \zeta0, \z6
    vmov.w \zeta1, \z7
    
    vmov.w \c_tmp0, \xi6
    vmov.w \c_tmp1, \xi7
    vmov.w \c_tmp2, \xi8
    
    CT_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov.w \xi6, \c_tmp0
    vmov.w \xi7, \tmp2
    vmov.w \xi8, \tmp

.endm

.macro GS_32_butterfly_radix3 c, c0, c1, c2, c3, zeta0, zeta1, Qprime, Q, Wmont, tmp, tmp2

    sub \c, \c1, \c0    //1 : t1
    sub \c3, \c2, \c0   //2 : t2

    montgomery_mul_32 \c, \Wmont, \Qprime, \Q, \tmp, \tmp2 //3 : t1
    add \c3, \c //4 : t2
	
    add \c0, \c1
    add \c0, \c2 //6

    sub \c2, \c1
    sub \c2, \c //5

	montgomery_mul_32 \c3, \zeta0, \Qprime, \Q, \tmp, \tmp2 //3 : c1 : c3 
    montgomery_mul_32 \c2, \zeta1, \Qprime, \Q, \tmp, \tmp2 //3 : c2 : c2
.endm

.macro _2_layer_GS_32_radix3 c_tmp, c_tmp0, c_tmp1, c_tmp2, c_tmp3, xi0, xi1, xi2, xi3, xi4, xi5, xi6, xi7, xi8, z0, z1, z2, z3, z4, z5, z6, z7, zeta0, zeta1, Qprime, Q, Wmont, tmp, tmp2
    vmov \zeta0, \z0
    vmov \zeta1, \z1
    vmov \c_tmp0, \xi0
    vmov \c_tmp1, \xi1
    vmov \c_tmp2, \xi2
    
    GS_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \c_tmp3, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi0, \c_tmp0
    vmov \xi1, \c_tmp3
    vmov.w \xi2, \c_tmp2

    vmov \zeta0, \z2
    vmov \zeta1, \z3
    vmov \c_tmp0, \xi3
    vmov \c_tmp1, \xi4
    vmov \c_tmp2, \xi5
    
    GS_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \c_tmp3, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi3, \c_tmp0
    vmov \xi4, \c_tmp3
    vmov.w \xi5, \c_tmp2

    vmov \zeta0, \z4
    vmov \zeta1, \z5
    vmov \c_tmp0, \xi6
    vmov \c_tmp1, \xi7
    vmov \c_tmp2, \xi8
    
    GS_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \c_tmp3, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi6, \c_tmp0
    vmov \xi7, \c_tmp3
    vmov.w \xi8, \c_tmp2

    /***stage2***/
    vmov \zeta0, \z6
    vmov \zeta1, \z7
    vmov \c_tmp0, \xi0
    vmov \c_tmp1, \xi3
    vmov \c_tmp2, \xi6
    
    GS_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \c_tmp3, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi0, \c_tmp0
    vmov \xi3, \c_tmp3
    vmov.w \xi6, \c_tmp2

    vmov \c_tmp0, \xi1
    vmov \c_tmp1, \xi4
    vmov \c_tmp2, \xi7
    
    GS_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \c_tmp3, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi1, \c_tmp0
    vmov \xi4, \c_tmp3
    vmov.w \xi7, \c_tmp2

    vmov \c_tmp0, \xi2
    vmov \c_tmp1, \xi5
    vmov \c_tmp2, \xi8
    
    GS_32_butterfly_radix3 \c_tmp, \c_tmp0, \c_tmp1, \c_tmp2, \c_tmp3, \zeta0, \zeta1, \Qprime, \Q, \Wmont, \tmp, \tmp2    
    
    vmov \xi2, \c_tmp0
    vmov \xi5, \c_tmp3
    vmov.w \xi8, \c_tmp2
    

.endm

#endif /* MACROS_I */
