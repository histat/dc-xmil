
	.include "../z80hdc/z80h.inc"

	.globl	z80h_cb
	.globl	z80h_ixcb
	.globl	z80h_iycb

	.extern	_z80core


	.text
	.align	2

	
.macro BITA	b, s
	swap.b	r12,r0
	tst	#\b,r0
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	bf/s	1f
	or	r12,r0
	or	#(H_FLAG + Z_FLAG),r0
	jmp	@r11
	mov	r0,r12
1:
	or	#(H_FLAG + \s),r0
	jmp	@r11
	mov	r0,r12
.endm

.macro BRA		b
	swap.b	r12,r12
	mov	r12,r0
	shlr8	r12
	and	#~(\b),r0
	shll8	r12
	or	r0,r12
	jmp	@r11
	swap.b	r12,r12
.endm

.macro BSA		b
	swap.b	r12,r0
	or	#\b,r0
	jmp	@r11
	swap.b	r0,r12
.endm

	
rlc_a:
	mov	#0x7f,r4
	shll8	r4
	mov	#0x80,r0
	extu.b	r0,r0
	shll8	r0
	cmp/hs	r0,r12
	bf/s	1f
	and	r12,r4
	mov	r4,r0
	or	#0x80,r0
	mov	r0,r4
1:
	mov	r4,r5
	addc	r4,r5
	mov	r4,r0
	mov	#-7,r3
	shld	r3,r0
	mov.b	@(r0,r1),r6
	mov	r5,r12
	extu.b	r6,r6
	jmp	@r11
	add	r6,r12
rrc_a:
	mov	r12,r4
	shlr8	r4
	shlr	r4
	bf/s	1f
	mov	r4,r0
	or	#0x80,r0
	mov	r0,r4
1:
	mov	r1,r0
	mov.b	@(r0,r4),r5
	mov	r4,r12
	extu.b	r5,r5
	shll8	r12
	jmp	@r11
	addc	r5,r12
rl_a:
	mov	#0x7f,r4
	shll8	r4
	mov	r12,r0
	tst	#C_FLAG,r0
	mov	#0x80,r3
	extu.b	r3,r3
	shll8	r3
	and	r12,r4
	bt/s	1f
	cmp/hs	r3,r12
	mov	r4,r0
	or	#0x80,r0
	mov	r0,r4
1:
	mov	r4,r0
	mov	#-7,r3
	shld	r3,r0
	mov.b	@(r0,r1),r5
	mov	#1,r3
	extu.b	r5,r5
	mov	r4,r12
	shld	r3,r12
	jmp	@r11
	addc	r5,r12
rr_a:
	mov	r12,r4
	mov	#-9,r3
	mov	r12,r0
	tst	#C_FLAG,r0
	bt/s	1f
	shld	r3,r4
	mov	r4,r0
	or	#0x80,r0
	mov	r0,r4
1:
	mov	r1,r0
	mov.b	@(r0,r4),r5
	mov	r12,r6
	extu.b	r5,r5
	shlr8	r6
	mov	r4,r12
	shlr	r6
	shll8	r12
	jmp	@r11
	addc	r5,r12
sla_a:
	mov	#0x7f,r4
	shll8	r4
	mov	r12,r6
	and	r12,r4
	mov	#-15,r3
	shld	r3,r6
	mov	r1,r0
	mov	r4,r12
	shlr	r6
	mov	#-7,r3
	shld	r3,r4
	mov.b	@(r0,r4),r5
	mov	#1,r3
	extu.b	r5,r5
	shld	r3,r12
	jmp	@r11
	addc	r5,r12
sra_a:
	exts.w	r12,r0
	cmp/pz	r0
	bt	srl_a
	swap.w	r12,r0
	or	#1,r0
	swap.w	r0,r12
srl_a:
	mov	r12,r4
	shlr8	r4
	mov	r1,r0
	shlr	r4
	mov.b	@(r0,r4),r5
	mov	r4,r12
	extu.b	r5,r5
	shll8	r12
	jmp	@r11
	addc	r5,r12
sll_a:
	mov	#0x7f,r4
	shll8	r4
	and	r12,r4
	mov	r4,r0
	or	#0x80,r0
	mov	r0,r4
	mov	r12,r6
	mov	#-15,r3
	shld	r3,r6
	shlr	r6
	mov	r4,r0
	mov	#-7,r3
	shld	r3,r0
	mov.b	@(r0,r1),r5
	mov	r4,r12
	mov	#1,r3
	extu.b	r5,r5
	shld	r3,r12
	jmp	@r11
	addc	r5,r12

bit0_a:	BITA	0x01, 0
bit1_a:	BITA	0x02, 0
bit2_a:	BITA	0x04, 0
bit3_a:	BITA	0x08, 0
bit4_a:	BITA	0x10, 0
bit5_a:	BITA	0x20, 0
bit6_a:	BITA	0x40, 0
bit7_a:	BITA	0x80, S_FLAG

br0_a:	BRA		0x01
br1_a:	BRA		0x02
br2_a:	BRA		0x04
br3_a:	BRA		0x08
br4_a:	BRA		0x10
br5_a:	BRA		0x20
br6_a:	BRA		0x40
br7_a:	BRA		0x80

bs0_a:	BSA		0x01
bs1_a:	BSA		0x02
bs2_a:	BSA		0x04
bs3_a:	BSA		0x08
bs4_a:	BSA		0x10
bs5_a:	BSA		0x20
bs6_a:	BSA		0x40
bs7_a:	BSA		0x80



.macro BITR4	b, s
	extu.b	r4,r0
	tst	#\b,r0
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	bf/s	1f
	or	r0,r12
	mov	#(H_FLAG + Z_FLAG),r0
	jmp	@r11
	or	r0,r12
1:
	mov	r12,r0
	or	#(H_FLAG + \s),r0
	jmp	@r11
	mov	r0,r12
.endm

.macro BRR4	b
	mov	r4,r0
	and	#~(\b),r0
	jmp	@r11
	mov.b	r0,@r8
.endm

.macro BSR4	b
	mov	r4,r0
	or	#\b,r0
	jmp	@r11
	mov.b	r0,@r8
.endm

rlc_m:	Z80WORK	23
rlc_r:
	mov	#0x7f,r5
	and	r4,r5
	mov	#0x80,r0
	extu.b	r0,r0
	cmp/hs	r0,r4
	addc	r5,r5
	!carry
	cmp/hs	r0,r4
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	mov	r1,r0
	mov.b	@(r0,r5),r7
	mov.b	r5,@r8
	extu.b	r7,r7
	jmp	@r11
	addc	r7,r12
	
rrc_m:	Z80WORK	23
rrc_r:
	swap.b	r12,r0
	and	#0xff,r0
	shlr	r4
	bf/s	1f
	swap.b	r0,r12
	mov	r4,r0
	or	#0x80,r0
	mov	r0,r4
1:	
	mov	r1,r0
	mov.b	@(r0,r4),r5
	mov.b	r4,@r8
	extu.b	r5,r5
	jmp	@r11
	addc	r5,r12

rl_m:	Z80WORK	23
rl_r:
	mov	#C_FLAG,r5
	mov	#24,r3
	and	r12,r5
	shld	r3,r4
	shll	r4
	mov	r5,r7
	shld	r3,r7
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	or	r4,r7
	mov	#-24,r3
	mov	r1,r0
	shld	r3,r7
	mov.b	@(r0,r7),r6
	mov.b	r7,@r8
	extu.b	r6,r6
	jmp	@r11
	addc	r6,r12
	
rr_m:	Z80WORK	23
rr_r:
	mov	#C_FLAG,r5
	and	r12,r5
	shlr	r4
	mov	r5,r7
	mov	#7,r3
	shld	r3,r7
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	or	r4,r7
	mov	r1,r0
	mov.b	@(r0,r7),r5
	mov.b	r7,@r8
	extu.b	r5,r5
	jmp	@r11
	addc	r5,r12
	
sla_m:	Z80WORK	23
sla_r:
	mov	r4,r5
	mov	#24,r3
	shld	r3,r5
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	shll	r5
	mov	#-24,r3
	mov	r1,r0
	shld	r3,r5
	mov.b	@(r0,r5),r7
	mov.b	r5,@r8
	extu.b	r7,r7
	jmp	@r11
	addc	r7,r12
	
sra_m:	Z80WORK	23
sra_r:
	extu.b	r4,r0
	tst	#0x80,r0
	bt	1f
	swap.b	r4,r0
	or	#1,r0
	swap.b	r0,r4
1:
	mov	r4,r5
	shlr	r5
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	mov	r5,r0
	mov.b	@(r0,r1),r6
	mov.b	r5,@r8
	extu.b	r6,r6
	jmp	@r11
	addc	r6,r12
	
sll_m:	Z80WORK	23
sll_r:
	mov	r4,r5
	mov	#24,r3
	shld	r3,r5
	shll	r5
	mov	#1,r0
	shld	r3,r0
	add	r0,r5
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	mov	r5,r6
	mov	#-24,r3
	shld	r3,r6
	mov	r5,r0
	shld	r3,r5
	mov.b	@(r0,r1),r7
	mov.b	r6,@r8
	extu.b	r7,r7
	jmp	@r11
	addc	r7,r12
	
srl_m:	Z80WORK	23
srl_r:
	swap.b	r12,r0
	and	#0xff,r0
	swap.b	r0,r12
	shlr	r4
	mov	r1,r0
	mov.b	@(r0,r4),r6
	mov.b	r4,@r8
	extu.b	r6,r6
	jmp	@r11
	addc	r6,r12
	
bit0_m:	Z80WORK	20
bit0_r:	BITR4	0x01, 0
bit1_m:	Z80WORK	20
bit1_r:	BITR4	0x02, 0
bit2_m:	Z80WORK	20
bit2_r:	BITR4	0x04, 0
bit3_m:	Z80WORK	20
bit3_r:	BITR4	0x08, 0
bit4_m:	Z80WORK	20
bit4_r:	BITR4	0x10, 0
bit5_m:	Z80WORK	20
bit5_r:	BITR4	0x20, 0
bit6_m:	Z80WORK	20
bit6_r:	BITR4	0x40, 0
bit7_m:	Z80WORK	20
bit7_r:	BITR4	0x80, S_FLAG

br0_m:	Z80WORK	23
br0_r:	BRR4	0x01
br1_m:	Z80WORK	23
br1_r:	BRR4	0x02
br2_m:	Z80WORK	23
br2_r:	BRR4	0x04
br3_m:	Z80WORK	23
br3_r:	BRR4	0x08
br4_m:	Z80WORK	23
br4_r:	BRR4	0x10
br5_m:	Z80WORK	23
br5_r:	BRR4	0x20
br6_m:	Z80WORK	23
br6_r:	BRR4	0x40
br7_m:	Z80WORK	23
br7_r:	BRR4	0x80

bs0_m:	Z80WORK	23
bs0_r:	BSR4	0x01
bs1_m:	Z80WORK	23
bs1_r:	BSR4	0x02
bs2_m:	Z80WORK	23
bs2_r:	BSR4	0x04
bs3_m:	Z80WORK	23
bs3_r:	BSR4	0x08
bs4_m:	Z80WORK	23
bs4_r:	BSR4	0x10
bs5_m:	Z80WORK	23
bs5_r:	BSR4	0x20
bs6_m:	Z80WORK	23
bs6_r:	BSR4	0x40
bs7_m:	Z80WORK	23
bs7_r:	BSR4	0x80

z80h_cb:	GETPC8
	mov	r0,r4
	mov	#((1 << 24) >> 24),r0
	mov	#24,r3
	shld	r3,r0
	add	r0,r10
	mov	#7,r5
	and	r4,r5	! reg
	mov	#0xf8,r6
	and	r4,r6	! ctrl
	mov	#6,r0
	cmp/hs	r0,r5
	bf/s	cb_r
	cmp/eq	r0,r5
	bf	cb_a
	mov.l	__cb_xhl,r3
	jmp	@r3
	nop

	.align	2
__cb_xhl:	.long	cb_xhl
	
cb_a:
	mova	cba_r,r0
	shlr	r6
	mov.l	@(r0,r6),r6
	Z80WORK	8
	jmp	@r6
	nop
	
	.align	2
cba_r:	.long		rlc_a
			.long		rrc_a
			.long		rl_a
			.long		rr_a
			.long		sla_a
			.long		sra_a
			.long		sll_a
			.long		srl_a
			.long		bit0_a
			.long		bit1_a
			.long		bit2_a
			.long		bit3_a
			.long		bit4_a
			.long		bit5_a
			.long		bit6_a
			.long		bit7_a
			.long		br0_a
			.long		br1_a
			.long		br2_a
			.long		br3_a
			.long		br4_a
			.long		br5_a
			.long		br6_a
			.long		br7_a
			.long		bs0_a
			.long		bs1_a
			.long		bs2_a
			.long		bs3_a
			.long		bs4_a
			.long		bs5_a
			.long		bs6_a
			.long		bs7_a

cb_r:
	mova	cbr4_r,r0
	mov	r1,r8
	mov	#1,r3
	add	#-(CPU_SIZE),r8
	xor	r3,r5
	shlr	r6
	mov.l	@(r0,r6),r6
	Z80WORK	8
	mov	r8,r0
	mov.b	@(r0,r5),r4
	add	r5,r8
	jmp	@r6
	extu.b	r4,r4
	
	.align	2
cbr4_r:	.long		rlc_r
			.long		rrc_r
			.long		rl_r
			.long		rr_r
			.long		sla_r
			.long		sra_r
			.long		sll_r
			.long		srl_r
			.long		bit0_r
			.long		bit1_r
			.long		bit2_r
			.long		bit3_r
			.long		bit4_r
			.long		bit5_r
			.long		bit6_r
			.long		bit7_r
			.long		br0_r
			.long		br1_r
			.long		br2_r
			.long		br3_r
			.long		br4_r
			.long		br5_r
			.long		br6_r
			.long		br7_r
			.long		bs0_r
			.long		bs1_r
			.long		bs2_r
			.long		bs3_r
			.long		bs4_r
			.long		bs5_r
			.long		bs6_r
			.long		bs7_r

cb_xhl:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r8
	MEMLEA8	r8, r4, r5
	add	r8,r4
	mov.b	@r4,r4
	mova	cbr4_m,r0
	extu.b	r4,r4
	shlr	r6
	mov.l	@(r0,r6),r6
	jmp	@r6
	add	r5,r8
	
	.align	2
cbr4_m:	.long		rlc_m
			.long		rrc_m
			.long		rl_m
			.long		rr_m
			.long		sla_m
			.long		sra_m
			.long		sll_m
			.long		srl_m
			.long		bit0_m
			.long		bit1_m
			.long		bit2_m
			.long		bit3_m
			.long		bit4_m
			.long		bit5_m
			.long		bit6_m
			.long		bit7_m
			.long		br0_m
			.long		br1_m
			.long		br2_m
			.long		br3_m
			.long		br4_m
			.long		br5_m
			.long		br6_m
			.long		br7_m
			.long		bs0_m
			.long		bs1_m
			.long		bs2_m
			.long		bs3_m
			.long		bs4_m
			.long		bs5_m
			.long		bs6_m
			.long		bs7_m

z80h_ixcb:	GETIPC	r5, CPU_IX
	mov	#((1 << 24) >> 24),r0
	mov	#24,r3
	shld	r3,r0
	add	r0,r10
	GETPC8
	mov	r0,r4
	MEMLEA8	r5, r6, r7
	mov	#0xf8,r3
	mov.l	200f,r0
	and	r3,r4
	shlr	r4
	mov.l	@(r0,r4),r2
	mov	r6,r0
	mov.b	@(r0,r5),r4
	mov	r7,r8
	extu.b	r4,r4
	jmp	@r2
	add	r5,r8
	
	.align	2
200:	.long	cbr4_m

z80h_iycb:	GETIPC	r5, CPU_IY
	mov	#((1 << 24) >> 24),r0
	mov	#24,r3
	shld	r3,r0
	add	r0,r10
	GETPC8
	mov	r0,r4
	MEMLEA8	r5, r6, r7
	mov	#0xf8,r3
	mov.l	200f,r0
	and	r3,r4
	shlr	r4
	mov.l	@(r0,r4),r2
	mov	r6,r0
	mov.b	@(r0,r5),r4
	mov	r7,r8
	extu.b	r4,r4
	jmp	@r2
	add	r5,r8
	
	.align	2
200:	.long	cbr4_m

	.end
