
.macro LD8	d, s
	mov.b	@(\s,gbr),r0
	jmp	@r11
	mov.b	r0,@(\d,gbr)
.endm

.macro LD8A	d
	mov	r12,r0
	shlr8	r0
	jmp	@r11
	mov.b	r0,@(\d,gbr)
.endm

.macro LDA8	s
	mov.b	@(\s,gbr),r0
	extu.b	r12,r12
	extu.b	r0,r4
	shll8	r4
	jmp	@r11
	or	r4,r12
.endm

.macro LDAx	src
	mov.w	@(\src,gbr),r0
	extu.b	r12,r12
	extu.w	r0,r4
	MEMRD8	r4, r4
	shll8	r4
	jmp	@r11
	add	r4,r12
.endm

.macro LD8x	d, s
	mov.w	@(\s,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r4
	mov	r5,r0
	jmp	@r11
	mov.b	r0,@(\d,gbr)
.endm

.macro LDxA	dst
	mov.w	@(\dst,gbr),r0
	mov	r12,r5
	extu.w	r0,r4
	shlr8	r5
	MEMWR8	r4, r5
	jmp	@r11
	nop
.endm

.macro LDx8	dst, src
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r4
	mov.b	@(\src,gbr),r0
	extu.b	r0,r5
	MEMWR8	r4, r5
	jmp	@r11
	nop
.endm

.macro LD8b	dst
	GETPC8
	jmp	@r11
	mov.b	r0,@(\dst,gbr)
.endm

.macro LD16w 	dst
	GETPC16
	jmp	@r11
	mov.w	r0,@(\dst,gbr)

	.align	2
10000:	.long	mem_read16	
.endm

.macro LDx16 src
	GETPC16
	mov	r0,r4
	mov.w	@(\src,gbr),r0
	mov.l	10001f,r3
	lds	r11,pr
	jmp	@r3
	extu.w	r0,r5

	.align	2
10000:	.long	mem_read16
10001:	.long	mem_write16
.endm

.macro LD16x dst
	GETPC16
	mov.l	10000f,r3
	jsr	@r3
	mov	r0,r4
	jmp	@r11
	mov.w	r0,@(\dst,gbr)

	.align	2
10000:	.long	mem_read16
.endm

.macro MINC16	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r0
	add	#1,r0
	jmp	@r11
	mov.w	r0,@(\dst,gbr)
.endm

.macro MINC8	dst
	mov.b	@(\dst,gbr),r0
	mov	#1,r5
	extu.b	r0,r4
	shll8	r5	! CPU_INCFLAG
	mov	r12,r0
	add	r1,r5
	shlr8	r12
	add	r4,r5
	and	#~(0xff - C_FLAG),r0
	mov.b	@r5,r6
	shll8	r12
	or	r0,r12
	extu.b	r6,r6
	mov	r4,r0
	add	#1,r0
	or	r6,r12
	jmp	@r11
	mov.b	r0,@(\dst,gbr)
.endm

.macro MINCM8	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r4
	MEMLEA8	r4, r5, r6
	mov	r4,r0
	mov	#1,r2
	mov.b	@(r0,r5),r7
	shll8	r2	! CPU_INCFLAG
	mov	r12,r0
	extu.b	r7,r7
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	add	r1,r2
	shll8	r12
	add	r7,r2
	or	r0,r12
	mov.b	@r2,r5
	add	#1,r7
	extu.b	r5,r5
	or	r5,r12
	mov	r4,r0
	jmp	@r11
	mov.b	r7,@(r0,r6)
.endm

.macro MDEC8	dst
	mov.b	@(\dst,gbr),r0
	mov	#1,r5
	mov	#9,r3
	extu.b	r0,r4
	shld	r3,r5	! CPU_DECFLAG
	mov	r12,r0
	shlr8	r12
	add	r1,r5
	and	#~(0xff - C_FLAG),r0
	add	r4,r5
	shll8	r12
	mov.b	@r5,r6
	or	r0,r12
	mov	r4,r0
	extu.b	r6,r6
	add	#-1,r0
	or	r6,r12
	jmp	@r11
	mov.b	r0,@(\dst,gbr)
.endm

.macro MDECM8	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r4
	MEMLEA8	r4, r5, r6
	mov	r4,r0
	mov.b	@(r0,r5),r7
	mov	#1,r2
	mov	r12,r0
	mov	#9,r3
	extu.b	r7,r7
	shlr8	r12
	shld	r3,r2	! CPU_DECFLAG
	and	#~(0xff - C_FLAG),r0
	add	r1,r2
	shll8	r12
	add	r7,r2
	or	r0,r12
	mov.b	@r2,r5
	add	#-1,r7
	extu.b	r5,r5
	or	r5,r12
	mov	r4,r0
	jmp	@r11
	mov.b	r7,@(r0,r6)
.endm


.macro MADD16	dst, src
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r4
	mov.w	@(\src,gbr),r0
	mov	r4,r6
	extu.w	r0,r5
	add	r5,r6
	mov	r12,r0
	mov	r4,r7
	shlr8	r12
	xor	r5,r7
	and	#~(0xff - (S_FLAG + Z_FLAG + V_FLAG)),r0
	mov	r6,r3
	shll8	r12
	shlr16	r3
	or	r0,r12
	add	r3,r12
	mov	r6,r0
	xor	r6,r7
	mov.w	r0,@(\dst,gbr)
	swap.b	r7,r0
	and	#H_FLAG,r0
	jmp	@r11
	or	r0,r12
.endm

.macro MADD16D	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r4
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - (S_FLAG + Z_FLAG + V_FLAG)),r0
	shll8	r12
	or	r0,r12
	swap.b	r4,r0
	and	#H_FLAG,r0
	extu.b	r0,r5
	mov	r4,r0
	mov	#-15,r3
	shld	r3,r0
	add	r0,r12
	mov	r4,r0
	add	r4,r0
	add	r5,r12
	jmp	@r11
	mov.w	r0,@(\dst,gbr)
.endm

.macro MADDSP	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r13,r6
	extu.w	r0,r4
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - (S_FLAG + Z_FLAG + V_FLAG)),r0
	shll8	r12
	or	r0,r12
	add	r4,r6
	extu.w	r13,r7
	xor	r4,r7
	mov	r6,r0
	shlr16	r0
	add	r0,r12
	xor	r6,r7
	swap.b	r7,r0
	and	#H_FLAG,r0
	or	r0,r12
	mov	r6,r0
	jmp	@r11
	mov.w	r0,@(\dst,gbr)
.endm

.macro MDEC16	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r0
	add	#-1,r0
	jmp	@r11
	mov.w	r0,@(\dst,gbr)
.endm


.macro MADD8	r
	swap.b	r12,r5
	extu.b	r5,r5
	shll8	r5
	mov	\r,r6
	mov	#24,r3
	shld	r3,r6
	clrt
	mov	r5,r7
	shll16	r7
	mov	r7,r3
	addc	r6,r7 !s
	swap.w	r7,r0
	bf/s	1f
	addv	r6,r3
	or	#C_FLAG,r0
1:	
	bf/s	2f
	tst	r7,r7
	or	#V_FLAG,r0
2:
	mov	r5,r4
	shll16	r4
	xor	r6,r4
	bf/s	3f
	cmp/pz	r7
	or	#Z_FLAG,r0
3:	
	mov	r7,r5
	bt/s	4f
	xor	r4,r5
	or	#S_FLAG,r0
4:
	mov	#24,r3
	mov	#H_FLAG,r6
	shld	r3,r6
	and	r5,r6
	extu.w	r0,r12
	mov	#-24,r3
	shld	r3,r6
	jmp	@r11
	or	r6,r12
.endm

.macro MADDR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MADD8	r4
.endm

.macro MADDM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MADD8	r2
.endm

.macro MADC8	r
	mov	#C_FLAG,r5
	and	r12,r5
	mov	r12,r0
	shlr8	r0
	add	r0,r5
	add	\r,r5
	mov	\r,r6
	xor	r5,r6		! r ^ b
	mov	r12,r7
	shlr8	r7
	xor	r5,r7		! r ^ A
	mov	#23,r3
	shld	r3,r5
	shll	r5
	mov	r12,r4
	shlr8	r4
	xor	r6,r4		! r ^ b ^ A
	swap.w	r5,r0
	bf/s	1f
	tst	r5,r5
	or	#C_FLAG,r0
1:
	and	r7,r6		! (r ^ b) & (r ^ A)
	bf/s	2f
	cmp/pz	r5
	or	#Z_FLAG,r0
2:	
	mov	#H_FLAG,r3
	bt/s	3f
	and	r3,r4
	or	#S_FLAG,r0
3:
	mov	#5,r3
	mov	#V_FLAG,r12
	shld	r3,r12
	and	r12,r6
	extu.w	r0,r12
	or	r4,r12
	mov	#-5,r3
	shld	r3,r6
	jmp	@r11
	or	r6,r12
.endm

.macro MADCR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MADC8	r4
.endm

.macro MADCM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MADC8	r2
.endm

.macro MSUB8	r
	swap.b	r12,r5
	extu.b	r5,r5
	shll8	r5
	mov	\r,r6
	mov	#24,r3
	shld	r3,r6
	mov	r5,r7
	shll16	r7
	cmp/hs	r6,r7
	mov	r7,r3
	sub	r6,r7	!s
	swap.w	r7,r0
	or	#N_FLAG,r0
	bt/s	1f
	subv	r6,r3
	or	#C_FLAG,r0
1:
	bf/s	2f
	tst	r3,r3
	or	#V_FLAG,r0
2:	
	mov	r5,r4
	shll16	r4
	xor	r6,r4
	bf/s	3f
	cmp/pz	r3
	or	#Z_FLAG,r0
3:
	mov	r7,r5
	bt/s	4f
	xor	r4,r5
	or	#S_FLAG,r0
4:
	mov	#24,r3
	mov	#H_FLAG,r6
	shld	r3,r6
	and	r5,r6
	extu.w	r0,r12
	mov	#-24,r3
	shld	r3,r6
	jmp	@r11
	or	r6,r12
.endm

.macro MSUBR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MSUB8	r4
.endm

.macro MSUBM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MSUB8	r2
.endm


.macro MSBC8	r
	mov	#C_FLAG,r5
	and	r12,r5
	mov	r12,r6
	shlr8	r6
	sub	\r,r6
	mov	r12,r7
	shlr8	r7
	xor	\r,r7
	sub	r5,r6
	mov	r12,r4
	shlr8	r4
	xor	r6,r4
	mov	r7,r5
	xor	r6,r5	
	mov	#23,r3
	shld	r3,r6
	shll	r6
	mov	r6,r3
	and	r7,r4
	swap.w	r6,r0
	or	#N_FLAG,r0
	bf/s	1f
	tst	r3,r3
	or	#C_FLAG,r0
1:
	bf/s	2f
	cmp/pz	r3
	or	#Z_FLAG,r0
2:	
	mov	#H_FLAG,r3
	bt/s	3f
	and	r3,r5
	or	#S_FLAG,r0
3:
	mov	#V_FLAG,r12
	mov	#5,r3
	shld	r3,r12
	and	r12,r4
	extu.w	r0,r12
	or	r5,r12
	mov	#-5,r3
	shld	r3,r4
	jmp	@r11
	or	r4,r12
.endm

.macro MSBCR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MSBC8	r4
.endm

.macro MSBCM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MSBC8	r2
.endm


.macro MAND8	r
	mov	r12,r6
	shlr8	r6
	mov	r1,r0
	and	\r,r6
	mov.b	@(r0,r6),r7
	mov	r6,r12
	extu.b	r7,r7
	shll8	r12
	jmp	@r11
	or	r7,r12
.endm

.macro MANDR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MAND8	r4
.endm

.macro MANDM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MAND8	r2
.endm


.macro MXOR8	r
	mov	r12,r6
	shlr8	r6
	mov	r1,r0
	xor	\r,r6
	mov.b	@(r0,r6),r7
	mov	r6,r12
	extu.b	r7,r7
	shll8	r12
	jmp	@r11
	or	r7,r12
.endm

.macro MXORR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MXOR8	r4
.endm

.macro MXORM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MXOR8	r2
.endm


.macro MOR8	r
	mov	r12,r6
	shlr8	r6
	mov	r1,r0
	or	\r,r6
	mov.b	@(r0,r6),r7
	mov	r6,r12
	extu.b	r7,r7
	shll8	r12
	jmp	@r11
	or	r7,r12
.endm

.macro MORR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MOR8	r4
.endm

.macro MORM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MOR8	r2
.endm


.macro MCP8	r
	swap.b	r12,r5
	extu.b	r5,r5
	shll8	r5
	mov	\r,r6
	mov	#24,r3
	shld	r3,r6
	mov	r5,r7
	shll16	r7
	cmp/hs	r6,r7
	mov	r5,r0
	or	#N_FLAG,r0
	mov	r12,r4
	shll16	r4
	xor	r6,r4
	bt/s	1f
	subv	r6,r7
	or	#C_FLAG,r0
1:	
	mov	r7,r6
	xor	r4,r6
	bf/s	2f
	tst	r7,r7
	or	#V_FLAG,r0
2:
	mov	#H_FLAG,r4
	mov	#24,r3
	shld	r3,r4
	and	r6,r4
	bf/s	3f
	cmp/pz	r7
	or	#Z_FLAG,r0
3:
	bt	4f
	or	#S_FLAG,r0
4:
	mov	r4,r12
	mov	#-24,r3
	shld	r3,r12
	jmp	@r11
	or	r0,r12
.endm

.macro MCPR8	src
	mov.b	@(\src,gbr),r0
	extu.b	r0,r4
	MCP8	r4
.endm

.macro MCPM8	src
	mov.w	@(\src,gbr),r0
	extu.w	r0,r4
	MEMRD8	r2, r4
	MCP8	r2
.endm





.macro MJR
	GETPC8S
	shll16	r0
	add	r0,r13
	jmp	@r11
	Z80WORK	5
.endm

.macro MJREQ
	bt/s	1f
	mov	#1,r0
	shll16	r0
	jmp	@r11
	add	r0,r13
1:	
	MJR
.endm

.macro MJRNFLG	flag
	extu.b	r12,r0
	tst	#\flag,r0
	MJREQ
.endm

.macro MJRNE
	bf/s	1f
	mov	#1,r0
	shll16	r0
	jmp	@r11
	add	r0,r13
1:	
	MJR
.endm

.macro MJRFLG	flag
	extu.b	r12,r0
	tst	#\flag,r0
	MJRNE
.endm


.macro MJP
	mov	r13,r8
	shll16	r8
	GETPC16
	shlr16	r8
	mov	r0,r13
	shll16	r13
	jmp	@r11
	or	r8,r13
.endm

.macro MJPR16	r
	mov.w	@(\r,gbr),r0
	mov	r13,r8
	extu.w	r0,r4
	shll16	r8
	mov	r4,r13
	shll16	r13
	mov	r8,r0
	shlr16	r0
	jmp	@r11
	or	r0,r13
.endm

.macro MJPNFLG	flag
	extu.b	r12,r0
	tst	#\flag,r0
	bt/s	1f
	mov	#2,r0
	shll16	r0
	jmp	@r11
	add	r0,r13
1:
	MJP
.endm

.macro MJPFLG	flag
	extu.b	r12,r0
	tst	#\flag,r0
	bf/s	1f
	mov	#2,r0
	shll16	r0
	jmp	@r11
	add	r0,r13
1:	
	MJP
.endm


.macro MCALL
	GETPC16
	mov	r13,r6	
	shll16	r6	! sp
	shll16	r0	! new pc
	mov	#2,r3
	shll16	r3
	sub	r3,r6
	mov	r13,r5	
	shlr16	r5
	mov	r6,r4	
	shlr16	r4
	mov	r6,r13	
	shlr16	r13
	or	r0,r13
	mov.l	10001f,r3
	Z80WORK	7
	jmp	@r3
	lds	r11,pr

	.align	2
10000:	.long	mem_read16
10001:	.long	mem_write16
.endm

.macro MCALLNF	flag
	extu.b	r12,r0
	tst	#\flag,r0
	bt/s	1f
	mov	#2,r0
	shll16	r0
	jmp	@r11
	add	r0,r13
1:	
	MCALL
.endm

.macro MCALLF	flag
	extu.b	r12,r0
	tst	#\flag,r0
	bf/s	1f
	mov	#2,r0
	shll16	r0
	jmp	@r11
	add	r0,r13
1:	
	MCALL
.endm


.macro MRET
	mov	r13,r8
	shll16	r8	! sp
	mov.l	10000f,r3
	mov	r8,r4
	jsr	@r3
	shlr16	r4
	mov	r0,r4
	mov	#2,r3
	shll16	r3
	add	r3,r8
	shll16	r4
	Z80WORK	6
	mov	r8,r13
	shlr16	r13
	jmp	@r11
	add	r4,r13

	.align	2
10000:	.long	mem_read16
.endm

.macro MRETNF	flag
	extu.b	r12,r0
	tst	#\flag,r0
	bt	1f
	jmp	@r11
	nop
1:	
	MRET
.endm

.macro MRETF	flag
	extu.b	r12,r0
	tst	#\flag,r0
	bf	1f
	jmp	@r11
	nop
1:	
	MRET
.endm


.macro MRST	vct
	mov	r13,r4
	shll16	r4
	mov	r13,r5
	mov	#2,r0
	shll16	r0
	shlr16	r5
	sub	r0,r4
	mov	#\vct,r13
	shll16	r13
	shlr16	r4
	mov.l	10001f,r3
	lds	r11,pr
	jmp	@r3
	or	r4,r13

	.align	2
10001:	.long	mem_write16	
.endm


.macro MPUSHr1
	! r1 -> r5
	mov	#2,r3
	shll16	r3
	mov	r13,r4
	lds	r11,pr
	shll16	r4
	cmp/hs	r3,r4
	bt/s	1f
	sub	r3,r4
	mov	#1,r3
	shll16	r3
	add	r3,r13
1:
	mov.l	10001f,r3
	shlr16	r4
	jmp	@r3
	add	#-2,r13

	.align	2
10001:	.long	mem_write16	
.endm

.macro MPUSH	dst
	mov.w	@(\dst,gbr),r0
	extu.w	r0,r5
	MPUSHr1
.endm

.macro MPOPr0
	! r0 -> r4
	mov	r13,r5
	mov	#2,r0
	shll16	r0
	shll16	r5
	mov.l	10000f,r3
	clrt
	addc	r5,r0
	bf/s	1f
	mov	r5,r4
	mov	#1,r0
	shll16	r0
	sub	r0,r13	! round down...
1:	
	jsr	@r3
	shlr16	r4
	add	#2,r13
.endm

.macro MPOP	dst
	MPOPr0
	jmp	@r11
	mov.w	r0,@(\dst,gbr)

	.align	2
10000:	.long	mem_read16	
.endm


.macro MEXSP	src
	mov	r13,r8
	shll16	r8
	mov.l	10000f,r3
	mov	r8,r4
	jsr	@r3
	shlr16	r4
	mov	r0,r4
	mov.w	@(\src,gbr),r0
	lds	r11,pr
	extu.w	r0,r5
	mov	r4,r0
	mov.w	r0,@(\src,gbr)
	mov.l	10001f,r3
	mov	r8,r4
	jmp	@r3
	shlr16	r4
	
	.align	2
10000:	.long	mem_read16
10001:	.long	mem_write16
.endm

.macro MLDSP	src
	mov	r13,r4
	mov.w	@(\src,gbr),r0
	shlr16	r4
	extu.w	r0,r5
	mov	r4,r13
	shll16	r13
	jmp	@r11
	or	r5,r13
.endm
