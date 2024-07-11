.text /* text section */
.syntax unified /* Unified Assembly Syntax - UAL */
.thumb /* Thumb instruction set */

.macro montgomery_multiplication res, pa, pb, q, qinv
    smull \pa, \res, \pa, \pb
    mul \pb, \pa, \qinv
    smlal \pa, \res, \pb, \q
.endm

.macro montgomery_reduction temp, pa_l, pa_h, q, qinv
    mul \temp, \pa_l, \qinv
    smlal \pa_l, \pa_h, \temp, \q
    //return pa_h
.endm



// void asm_pointwise_montgomery_1(int32_t c[N], const int32_t a[N], const int32_t b[N]);
.global asm_pointwise_montgomery_1
.type asm_pointwise_montgomery_1,%function
.align 2
asm_pointwise_montgomery_1:
    push.w {r4-r11, r14}
    c_ptr .req r0
    a_ptr .req r1
    b_ptr .req r2
    qinv  .req r3
    q     .req r4
    pa0   .req r5
    pa1   .req r6
    pa2   .req r7
    pb0   .req r8
    pb1   .req r9
    pb2   .req r10
    tmp0  .req r11
    ctr   .req r12
    res   .req r14

    movw qinv, #:lower16:0x7a29f27f
    movt qinv, #:upper16:0x7a29f27f
    movw q, #0x3281
    movt q, #0x80


    // 384x3 = 1152 coefficients
    movw ctr, #384
    1:
        ldr.w pa1, [a_ptr, #4] //offset + 4를 의미
        ldr.w pa2, [a_ptr, #8]
        ldr pa0, [a_ptr], #12	//pa0 ldr 이후 a_ptr+=12
        ldr.w pb1, [b_ptr, #4]
        ldr.w pb2, [b_ptr, #8]
        ldr pb0, [b_ptr], #12

		montgomery_multiplication res, pa0, pb0, q, qinv
        str res, [c_ptr], #4
        montgomery_multiplication res, pa1, pb1, q, qinv
        str res, [c_ptr], #4
        montgomery_multiplication res, pa2, pb2, q, qinv
        str res, [c_ptr], #4

    subs ctr, #1
    bne.w 1b

    //// final coefficient
    //ldr.w pa0, [a_ptr]
    //ldr.w pb0, [b_ptr]
    //montgomery_multiplication res, pa0, pb0, q, qinv
    //str.w res, [c_ptr]

    pop.w {r4-r11, pc}

	.unreq qinv
    .unreq q
    .unreq pa0
    .unreq pb0
    .unreq tmp0


// void asm_pointwise_montgomery_3(int32_t c[N], const int32_t a[N], const int32_t b[N]);
.global asm_pointwise_montgomery_3
.type asm_pointwise_montgomery_3,%function
.align 2
asm_pointwise_montgomery_3:
    push.w {r4-r11, r14}
    c_ptr .req r0
    a_ptr .req r1
    b_ptr .req r2
    z_ptr .req r3
    qinv  .req r4
    q     .req r5
    pa0	  .req r6
    pb0   .req r7
    zeta  .req r8
    tmp0   .req r9
    tmp1   .req r10
    tem2   .req r11
    ctr   .req r12
    res   .req r14

    movw qinv, #:lower16:0x83fc21ff
    movt qinv, #:upper16:0x83fc21ff
    movw q, #0x2201
    movt q, #0x80


    // 256x6 = 1536 coefficients
    movw ctr, #256
    1:
    	ldr.w zeta, [z_ptr]
    	//x^0

        ldr.w pa0, [a_ptr, #8] //a2b1
        ldr.w pb0, [b_ptr, #4]
		smull tmp0, res, pa0, pb0
		ldr.w pa0, [a_ptr, #4] //a1b2
        ldr.w pb0, [b_ptr, #8]
        smlal tmp0, res, pa0, pb0
        montgomery_reduction tmp1, tmp0, res, q, qinv

		smull tmp0, res, res, zeta
		ldr.w pa0, [a_ptr] //a0b0
        ldr.w pb0, [b_ptr]
        smlal tmp0, res, pa0, pb0
		montgomery_reduction tmp1, tmp0, res, q, qinv

		str res, [c_ptr], #4

		//x^1
        ldr.w pa0, [a_ptr, #8] //a2b2
        ldr.w pb0, [b_ptr, #8]
		montgomery_multiplication res, pa0, pb0, q, qinv

		smull tmp0, res, res, zeta
		ldr.w pa0, [a_ptr] //a0b1
        ldr.w pb0, [b_ptr, #4]
        smlal tmp0, res, pa0, pb0
		ldr.w pa0, [a_ptr, #4] //a1b0
        ldr.w pb0, [b_ptr]
        smlal tmp0, res, pa0, pb0
		montgomery_reduction tmp1, tmp0, res, q, qinv
		str res, [c_ptr], #4


		//x^2
		ldr.w pa0, [a_ptr, #8] //a2b0
        ldr.w pb0, [b_ptr]
        smull tmp0, res, pa0, pb0
        ldr.w pa0, [a_ptr] //a0b2
        ldr.w pb0, [b_ptr, #8]
        smlal tmp0, res, pa0, pb0
        ldr.w pa0, [a_ptr, #4] //a1b1
        ldr.w pb0, [b_ptr, #4]
        smlal tmp0, res, pa0, pb0
        montgomery_reduction tmp1, tmp0, res, q, qinv
        str res, [c_ptr], #4

        //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
        add a_ptr, a_ptr, #12
        add b_ptr, b_ptr, #12

		mvn zeta, zeta
		add zeta, zeta, #1

        //x^0
        ldr.w pa0, [a_ptr, #8] //a2b1
        ldr.w pb0, [b_ptr, #4]
		smull tmp0, res, pa0, pb0
		ldr.w pa0, [a_ptr, #4] //a1b2
        ldr.w pb0, [b_ptr, #8]
        smlal tmp0, res, pa0, pb0
        montgomery_reduction tmp1, tmp0, res, q, qinv

		smull tmp0, res, res, zeta
		ldr.w pa0, [a_ptr] //a0b0
        ldr.w pb0, [b_ptr]
        smlal tmp0, res, pa0, pb0
		montgomery_reduction tmp1, tmp0, res, q, qinv

		str res, [c_ptr], #4

		//x^1
        ldr.w pa0, [a_ptr, #8] //a2b2
        ldr.w pb0, [b_ptr, #8]
		montgomery_multiplication res, pa0, pb0, q, qinv

		smull tmp0, res, res, zeta
		ldr.w pa0, [a_ptr] //a0b1
        ldr.w pb0, [b_ptr, #4]
        smlal tmp0, res, pa0, pb0
		ldr.w pa0, [a_ptr, #4] //a1b0
        ldr.w pb0, [b_ptr]
        smlal tmp0, res, pa0, pb0
		montgomery_reduction tmp1, tmp0, res, q, qinv
		str res, [c_ptr], #4


		//x^2
		ldr.w pa0, [a_ptr, #8] //a2b0
        ldr.w pb0, [b_ptr]
        smull tmp0, res, pa0, pb0
        ldr.w pa0, [a_ptr] //a0b2
        ldr.w pb0, [b_ptr, #8]
        smlal tmp0, res, pa0, pb0
        ldr.w pa0, [a_ptr, #4] //a1b1
        ldr.w pb0, [b_ptr, #4]
        smlal tmp0, res, pa0, pb0
        montgomery_reduction tmp1, tmp0, res, q, qinv
        str res, [c_ptr], #4

        add a_ptr, a_ptr, #12
        add b_ptr, b_ptr, #12
        add z_ptr, z_ptr, #4
    subs ctr, #1
    bne.w 1b

    //// final coefficient
    //ldr.w pa0, [a_ptr]
    //ldr.w pb0, [b_ptr]
    //montgomery_multiplication res, pa0, pb0, q, qinv
    //str.w res, [c_ptr]

    pop.w {r4-r11, pc}

    .unreq qinv
    .unreq q
    .unreq pa0
    .unreq pb0
    .unreq tmp0

// void asm_pointwise_montgomery_5(int32_t c[N], const int32_t a[N], const int32_t b[N]);
.global asm_pointwise_montgomery_5
.type asm_pointwise_montgomery_5,%function
.align 2
asm_pointwise_montgomery_5:
    push.w {r4-r11, r14}
    c_ptr .req r0
    a_ptr .req r1
    b_ptr .req r2
    qinv  .req r3
    q     .req r4
    pa0   .req r5
    pa1   .req r6
    pa2   .req r7
    pb0   .req r8
    pb1   .req r9
    pb2   .req r10
    tmp0  .req r11
    ctr   .req r12
    res   .req r14

    movw qinv, #:lower16:0xf0803fff
    movt qinv, #:upper16:0xf0803fff
    movw q, #0x4001
    movt q, #0x80
	//mov q, sp
	//ldr q, [q, #4]

    // 768x3 = 2304 coefficients
    movw ctr, #768
    1:
        ldr.w pa1, [a_ptr, #4] //offset + 4를 의미
        ldr.w pa2, [a_ptr, #8]
        ldr pa0, [a_ptr], #12	//pa0 ldr 이후 a_ptr+=12
        ldr.w pb1, [b_ptr, #4]
        ldr.w pb2, [b_ptr, #8]
        ldr pb0, [b_ptr], #12

        montgomery_multiplication res, pa0, pb0, q, qinv
        str res, [c_ptr], #4
        montgomery_multiplication res, pa1, pb1, q, qinv
        str res, [c_ptr], #4
        montgomery_multiplication res, pa2, pb2, q, qinv
        str res, [c_ptr], #4
    subs ctr, #1
    bne.w 1b

    //// final coefficient
    //ldr.w pa0, [a_ptr]
    //ldr.w pb0, [b_ptr]
    //montgomery_multiplication res, pa0, pb0, q, qinv
    //str.w res, [c_ptr]

    pop.w {r4-r11, pc}
//.size asm_pointwise_montgomery, .-asm_pointwise_montgomery






.end /* End of file */
