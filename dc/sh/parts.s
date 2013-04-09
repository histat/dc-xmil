
	.globl	__randseed
	.globl	_rand_setseed
	.globl	_rand_get
	.globl	_AdjustAfterMultiply
	.globl	_AdjustBeforeDivision
	.globl	_sjis2jis
	.globl	_jis2sjis
	.globl	_satuation_s16
	.globl	_satuation_s16x

	.text

	.align	2
__randseed:	.long		1		
	
_rand_setseed:
	mov.l	dcd_randseed,r5
	rts
	mov.l	r4,@r5
_rand_get:
	mov.l	dcd_randseed,r5
	mov.l	randdcd1,r6
	mov.l	@r5,r4
	mul.l	r4,r6
	mov	#-16,r3
	mov.l	randdcd2,r7
	sts	macl,r6
	add	r7,r6
	mov	r6,r0
	shad	r3,r0
	rts
	mov.l	r6,@r5

	.align	2
dcd_randseed:	.long		__randseed
randdcd1:	.long		0x343fd
randdcd2:	.long		0x269ec3

_AdjustAfterMultiply:
	mov	#205,r5
	extu.b	r4,r0
	extu.b	r5,r5			! 範囲が0-255なので精度低し
	mul.l	r0,r5
	mov	#-11,r3
	sts	macl,r5
	shld	r3,r5
	mov	r5,r2
	shll	r2
	sub	r2,r0
.if 1
	mov	#3,r3
	shld	r3,r5
	rts
	add	r5,r0
.else
	mov	#3,r3
	mov	r5,r2
	shld	r3,r2
	mov	#4,r3
	sub	r2,r0
	shld	r3,r5
	rts
	add	r5,r0
.endif

_AdjustBeforeDivision:
	mov	r4,r0
	mov	#-3,r3
	and	#0xf0,r0
	mov	r0,r5
	mov	r4,r0
	mov	r5,r2
	and	#15,r0
	shld	r3,r5
	shlr	r2
	add	r5,r0
	rts
	add	r2,r0
_sjis2jis:
	mov	r4,r0
	mov	#-7,r3
	and	#0xff,r0
	mov	r0,r5
	shld	r3,r0
	sub	r0,r5
	mov	#23,r3
	shld	r3,r5
	mov	#((0x62 << 23) >> 23),r0
	shld	r3,r0
	add	r0,r5
	cmp/pl	r5
	bf/s	1f
	mov	#((0xa2 << 23) >> 23),r0
	shld	r3,r0
	sub	r0,r5
1:
	mov	#(0x1f00 >> 8),r0
	shll8	r0
	or	#0x21,r0
	mov	#-23,r3
	shld	r3,r5
	add	r0,r5
	swap.b	r4,r0
	and	#(0x3f00 >> 8),r0
	swap.b	r0,r0
	shll	r0
	rts
	add	r5,r4
_jis2sjis:
	swap.b	r4,r0
	mov	#0x7f,r2
	and	#(0x7f00 >> 8),r0
	and	r2,r4
	swap.b	r0,r5
	tst	#1,r0
	bf/s	1f
	mov	#0x60,r0
	add	#0x5e,r4
1:
	cmp/hs	r0,r4
	bf	2f
	add	#1,r4
2:	
	add	#0x1f,r4
	mov	#(0x2100 >> 8),r2
	shll8	r2
	add	r2,r5
	shlr	r5
	swap.b	r5,r0
	and	#0xff,r0
	xor	#(0xa000 >> 8),r0
	shll8	r0
	rts
	or	r4,r0
	
_satuation_s16:
	shlr2	r6
	tst	r6,r6
	mov	#(0x7f00 >> 8),r0
	bf/s	1f
	shll8	r0
	rts
	nop
1:
	or	#0xff,r0
	mov	#0x80,r3
	mov	r0,r2
	shll8	r3
ss16_lp:
	mov.l	@r5+,r7
	mov.l	@r5+,r1
00:
	cmp/gt	r2,r7
	bf	11f
	cmp/ge	r2,r7
	bra	22f
	mov	r2,r7
11:
	cmp/ge	r3,r7
22:
	bt	33f
	mov	r3,r7
33:
00:
	cmp/gt	r2,r1
	bf	11f
	cmp/ge	r2,r1
	bra	22f
	mov	r2,r1
11:
	cmp/ge	r3,r1
22:
	bt	33f
	mov	r3,r1
33:
	shll16	r1
	extu.w	r7,r7
	dt	r6
	add	r1,r7
	mov.l	r7,@r4
	bf/s	ss16_lp
	add	#4,r4
	rts
	nop
	
_satuation_s16x:
	shlr2	r6
	tst	r6,r6
	mov	#(0x7f00 >> 8),r0
	bf/s	1f
	shll8	r0
	rts
	nop
1:
	or	#0xff,r0
	mov	#0x80,r3
	mov	r0,r2
	shll8	r3
ss16x_lp:
	mov.l	@r5+,r7
	mov.l	@r5+,r1
00:
	cmp/gt	r2,r7
	bf	11f
	cmp/ge	r2,r7
	bra	22f
	mov	r2,r7
11:
	cmp/ge	r3,r7
22:
	bt	33f
	mov	r3,r7
33:
00:
	cmp/gt	r2,r1
	bf	11f
	cmp/ge	r2,r1
	bra	22f
	mov	r2,r1
11:
	cmp/ge	r3,r1
22:
	bt	33f
	mov	r3,r1
33:
	shll16	r7
	extu.w	r1,r1
	dt	r6
	add	r1,r7
	mov.l	r7,@r4
	bf/s	ss16x_lp
	add	#4,r4
	rts
	nop
	
	.end

