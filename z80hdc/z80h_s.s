
	.include "../z80hdc/z80h.inc"
	.include "../z80hdc/z80h_mn.inc"
	.include "../z80hdc/z80h_s.inc"

	.globl	z80h_sb

	.extern	_iocore_out
	.extern	_iocore_inp
	.extern	mem_read16
	.extern	mem_write16
	.extern	_ievent_eoi

	.extern	_z80core

	.text
	.align	2

	
_ld_nop:
	jmp	@r11
	nop
_im_0:
	swap.w	r10,r10
	shlr8	r10
	shll8	r10
	jmp	@r11
	swap.w	r10,r10
_im_1:
	swap.w	r10,r0
	shlr8	r0
	shll8	r0
	or	#((0x01 << 16) >> 16),r0
	jmp	@r11
	swap.w	r0,r10
_im_2:
	swap.w	r10,r0
	shlr8	r0
	shll8	r0
	or	#((0x02 << 16) >> 16),r0
	jmp	@r11
	swap.w	r0,r10
	
_out_c_b:	OUTR8	CPU_B
_out_c_c:	OUTR8	CPU_C
_out_c_d:	OUTR8	CPU_D
_out_c_e:	OUTR8	CPU_E
_out_c_h:	OUTR8	CPU_H
_out_c_l:	OUTR8	CPU_L
_out_c_0:
	mov	#0,r5
	OUT8
_out_c_a:
	mov	r12,r5
	shlr8	r5
	OUT8
_in_b_c:	INPR8	CPU_B
_in_c_c:	INPR8	CPU_C
_in_d_c:	INPR8	CPU_D
_in_e_c:	INPR8	CPU_E
_in_h_c:	INPR8	CPU_H
_in_l_c:	INPR8	CPU_L
_in_0_c:
	INP8
	jmp	@r11
	nop

	_INP8
_in_a_c:
	mov.l	200f,r3
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r4
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov	r0,r4
	mov.l	@r15+,r1
	mov	#C_FLAG,r6
	and	r12,r6
	mov.b	@(r0,r1),r5
	shll8	r4
	or	r6,r4
	mov.l	@(CPU_REMCLOCK,gbr),r0
	extu.b	r5,r12
	mov	r0,r14
	jmp	@r11
	or	r4,r12

	.align	2
200:	.long	_iocore_inp
_ld_i_a:
	swap.b	r12,r4
	extu.b	r4,r4
	shll8	r4
	swap.b	r10,r10
	shlr8	r10
	shll8	r10
	swap.b	r10,r10
	jmp	@r11
	or	r4,r10
_ld_a_i:
	swap.b	r10,r4
	mov	r12,r0
	extu.b	r4,r4
	and	#C_FLAG,r0
	shll8	r4
	or	r4,r0
	shll8	r4
	tst	r4,r4
	bf/s	1f
	cmp/pz	r4
	or	#Z_FLAG,r0
1:
	mov	#(1 << IFF_IFLAG),r3
	bt/s	2f
	tst	r3,r10
	or	#S_FLAG,r0
2:
	bf	3f
	or	#V_FLAG,r0
3:
	jmp	@r11
	mov	r0,r12
_ld_r_a:
	mov	r12,r4
	shlr8	r4
	mov	r4,r0
	shll8	r10
	mov.b	r0,@(CPU_R2,gbr)
	mov	#24,r3
	shlr8	r10
	shld	r3,r4
	jmp	@r11
	or	r4,r10
_ld_a_r:
	mov.b	@(CPU_R2,gbr),r0
	mov	#(32 - 8),r3
	mov	#0x7f,r5
	shld	r3,r5
	extu.b	r0,r4
	and	r10,r5
	mov	#C_FLAG,r6
	mov	r4,r0
	and	r12,r6
	tst	#0x80,r0
	mov	#0x80,r0
	shld	r3,r0
	add	r0,r5
	mov	r6,r0
	bf/s	1f
	tst	r5,r5
	or	#Z_FLAG,r0
1:
	bf/s	2f
	cmp/pz	r5
	or	#Z_FLAG,r0
2:
	mov	#(1 << IFF_IFLAG),r3
	bt/s	3f
	tst	r3,r10
	or	#S_FLAG,r0
3:	
	bf/s	4f
	mov	r5,r12
	or	#V_FLAG,r0
4:
	shlr16	r12
	jmp	@r11
	or	r0,r12
_ld_xword_bc:	LDx16	CPU_BC
_ld_xword_de:	LDx16	CPU_DE
_ld_xword_hl:	LDx16	CPU_HL

_ld_bc_xword:	LD16x	CPU_BC
_ld_de_xword:	LD16x	CPU_DE
_ld_hl_xword:	LD16x	CPU_HL

_adc_hl_bc:	MADC16	CPU_BC
_adc_hl_de:	MADC16	CPU_DE
_adc_hl_hl:	MADCHL2
_adc_hl_sp:	MADCSP
	
_sbc_hl_bc:	MSBC16	CPU_BC
_sbc_hl_de:	MSBC16	CPU_DE

_sbc_hl_hl:	MSBCHL2
_sbc_hl_sp:	MSBCSP

_ld_xword_sp:	GETPC16
	mov.l	10001f,r3
	mov	r0,r4
	lds	r11,pr
	jmp	@r3
	mov	r13,r5

	.align	2
10000:	.long	mem_read16
10001:	.long	mem_write16

_ld_sp_xword:	GETPC16
	mov.l	10000f,r3
	mov	r0,r4	
	jsr	@r3
	shlr16	r13
	shll16	r13
	jmp	@r11
	or	r0,r13

	.align	2
10000:	.long	mem_read16

_rrd:
	mov.w	@(CPU_HL,gbr),r0
	mov	#0x0f,r5
	shll8	r5
	extu.w	r0,r4
	and	r12,r5
	MEMLEA8	r4, r6, r7
	mov	r4,r0
	mov.b	@(r0,r6),r0
	extu.b	r0,r2
	mov	#-4,r3
	shld	r3,r5
	mov	#0x0f,r6
	and	r2,r6
	swap.b	r12,r12
	shlr8	r12
	shll8	r12
	or	r6,r12
	swap.b	r12,r12
	shld	r3,r2
	or	r2,r5
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r0,r12
	mov	r12,r2
	mov	r1,r0
	shlr8	r12
	mov.b	@(r0,r12),r12
	mov	r4,r0
	extu.b	r12,r12
	mov.b	r5,@(r0,r7)
	jmp	@r11
	or	r2,r12
_rld:
	mov.w	@(CPU_HL,gbr),r0
	mov	#0x0f,r5
	shll8	r5
	extu.w	r0,r4
	and	r12,r5
	MEMLEA8	r4, r6, r7
	mov	r4,r0
	mov.b	@(r0,r6),r2
	extu.b	r2,r2
	shlr8	r5
	mov	r2,r0
	and	#0xf0,r0
	swap.b	r12,r12
	shlr8	r12
	shll8	r12
	swap.b	r12,r12
	mov	#4,r3
	shld	r3,r0
	or	r0,r12
	shld	r3,r2
	or	r2,r5
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r0,r12
	mov	r12,r2
	mov	r1,r0
	shlr8	r12
	mov.b	@(r0,r12),r12
	mov	r4,r0
	extu.b	r12,r12
	mov.b	r5,@(r0,r7)
	jmp	@r11
	or	r2,r12
_neg:
	swap.b	r12,r4
	extu.b	r4,r4
	shll8	r4
	mov	#0,r5
	mov	r4,r3
	shll16	r3
	mov	r5,r6
	mov	r6,r12
	cmp/hs	r3,r12
	sub	r3,r6
	swap.w	r6,r0
	or	#N_FLAG,r0
	mov	r4,r5
	shll16	r5
	xor	r6,r5
	bt/s	1f
	cmp/ge	r3,r12
	or	#C_FLAG,r0
1:
	bt/s	2f
	tst	r6,r6
	or	#V_FLAG,r0
2:
	bf/s	3f
	cmp/pz	r6
	or	#Z_FLAG,r0
3:
	bt	4f
	or	#S_FLAG,r0
4:
	mov	#H_FLAG,r7
	mov	#24,r3
	shld	r3,r7
	tst	r7,r5
	bt	5f
	or	#H_FLAG,r0
5:	
	jmp	@r11
	extu.w	r0,r12
_retn:
	mov	#~(1 << IFF_NMI),r3
	and	r3,r12
	MRET
_reti:
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@r15+,r1
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	MRET
	
	.align	2
200:	.long	_ievent_eoi
_ldi:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	mov.w	@(CPU_DE,gbr),r0
	extu.w	r0,r5
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r6
	MEMRD8	r7, r4
	MEMWR8	r5, r7
	add	#1,r4
	dt	r6
	add	#1,r5
	mov	r4,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r5,r0
	mov.w	r0,@(CPU_DE,gbr)
	mov	r6,r0
	mov.w	r0,@(CPU_BC,gbr)
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - 0xe9),r0
	shll8	r12
	bt/s	1f
	or	r12,r0
	or	#V_FLAG,r0
1:
	jmp	@r11
	mov	r0,r12
_ldd:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	mov.w	@(CPU_DE,gbr),r0
	extu.w	r0,r5
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r6
	MEMRD8	r7, r4
	MEMWR8	r5, r7
	add	#-1,r4
	dt	r6
	add	#-1,r5
	mov	r4,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r5,r0
	mov.w	r0,@(CPU_DE,gbr)
	mov	r6,r0
	mov.w	r0,@(CPU_BC,gbr)
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - 0xe9),r0
	shll8	r12
	bt/s	1f
	or	r12,r0
	or	#V_FLAG,r0
1:
	jmp	@r11
	mov	r0,r12
_ldir:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	mov.w	@(CPU_DE,gbr),r0
	extu.w	r0,r5
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r6
	MEMRD8	r7, r4
	MEMWR8	r5, r7
	add	#1,r4
	dt	r6
	add	#1,r5
	mov	r4,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r5,r0
	mov.w	r0,@(CPU_DE,gbr)
	mov	r6,r0
	mov.w	r0,@(CPU_BC,gbr)
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - 0xe9),r0
	shll8	r12
	bt/s	1f
	or	r12,r0
	or	#V_FLAG,r0
	mov	#2,r3
	shll16	r3
	sub	r3,r13
	add	#-5,r14
1:
	jmp	@r11
	mov	r0,r12
_lddr:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	mov.w	@(CPU_DE,gbr),r0
	extu.w	r0,r5
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r6
	MEMRD8	r7, r4
	MEMWR8	r5, r7
	add	#-1,r4
	dt	r6
	add	#-1,r5
	mov	r4,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r5,r0
	mov.w	r0,@(CPU_DE,gbr)
	mov	r6,r0
	mov.w	r0,@(CPU_BC,gbr)
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - 0xe9),r0
	shll8	r12
	bt/s	1f
	or	r12,r0
	or	#V_FLAG,r0
	mov	#2,r3
	shll16	r3
	sub	r3,r13
	add	#-5,r14
1:
	jmp	@r11
	mov	r0,r12
_outi:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r6
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r6
	add	#1,r6
	mov	#1,r0
	shll8	r0
	cmp/hs	r0,r4
	bt/s	1f
	sub	r0,r4
	mov	#1,r3
	shll16	r3
	add	r3,r4
1:	
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r4,r0
	mov.w	r0,@(CPU_BC,gbr)
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	shlr8	r0
	tst	#0xff,r0
	mov	r12,r0
	bf/s	2f
	or	#N_FLAG,r0
	or	#Z_FLAG,r0
2:	
	mov	r0,r12
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@r15+,r1
	mov.l	@(CPU_REMCLOCK,gbr),r0
	jmp	@r11
	mov	r0,r14
	
	.align	2
200:	.long	_iocore_out
_outd:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r6
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r6
	add	#-1,r6
	mov	#1,r0
	shll8	r0
	cmp/hs	r0,r4
	bt/s	1f
	sub	r0,r4
	mov	#1,r3
	shll16	r3
	add	r3,r4
1:	
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r4,r0
	mov.w	r0,@(CPU_BC,gbr)
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	shlr8	r0
	tst	#0xff,r0
	mov	r12,r0
	bf/s	2f
	or	#N_FLAG,r0
	or	#Z_FLAG,r0
2:
	mov	r0,r12
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@r15+,r1
	mov.l	@(CPU_REMCLOCK,gbr),r0
	jmp	@r11
	mov	r0,r14

	.align	2
200:	.long	_iocore_out
_otir:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r6
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r6
	add	#1,r6
	mov	#1,r0
	shll8	r0
	cmp/hs	r0,r4
	bt/s	1f
	sub	r0,r4
	mov	#1,r3
	shll16	r3
	add	r3,r4
1:	
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r4,r0
	mov.w	r0,@(CPU_BC,gbr)
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	shlr8	r0
	tst	#0xff,r0
	mov	r12,r0
	bf/s	2f
	or	#N_FLAG,r0
	add	#-5,r14
	or	#Z_FLAG,r0
	mov	#2,r3
	shll16	r3
	sub	r3,r13
2:
	mov	r0,r12
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@r15+,r1
	mov.l	@(CPU_REMCLOCK,gbr),r0
	jmp	@r11
	mov	r0,r14
	
	.align	2
200:	.long	_iocore_out
_otdr:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r6
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r6
	add	#-1,r6
	mov	#1,r0
	shll8	r0
	cmp/hs	r0,r4
	bt/s	1f
	sub	r0,r4
	mov	#1,r3
	shll16	r3
	add	r3,r4
1:	
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r4,r0
	mov.w	r0,@(CPU_BC,gbr)
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	shlr8	r0
	tst	#0xff,r0
	mov	r12,r0
	bf/s	2f
	or	#N_FLAG,r0
	add	#-5,r14
	or	#Z_FLAG,r0
	mov	#2,r3
	shll16	r3
	sub	r3,r13
2:
	mov	r0,r12
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@r15+,r1
	mov.l	@(CPU_REMCLOCK,gbr),r0
	jmp	@r11
	mov	r0,r14
	
	.align	2
200:	.long	_iocore_out
_ini:
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.w	@(CPU_BC,gbr),r0
	jsr	@r3
	extu.w	r0,r4
	mov.l	@r15+,r1
	mov	r0,r4
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r5
	mov.b	@(CPU_B,gbr),r0
	extu.b	r0,r6
	MEMWR8	r5, r4
	add	#1,r5
	dt	r6
	mov	r5,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r0
	mov.b	r0,@(CPU_B,gbr)
	shlr8	r12
	extu.b	r12,r0
	shll8	r0
	bf/s	1f
	or	#N_FLAG,r0
	or	#Z_FLAG,r0
1:
	jmp	@r11
	mov	r0,r12
	
	.align	2
200:	.long	_iocore_inp
_ind:
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.w	@(CPU_BC,gbr),r0
	jsr	@r3
	extu.w	r0,r4
	mov.l	@r15+,r1
	mov	r0,r4
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r5
	mov.b	@(CPU_B,gbr),r0
	extu.b	r0,r6
	MEMWR8	r5, r4
	add	#-1,r5
	dt	r6
	mov	r5,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r0
	mov.b	r0,@(CPU_B,gbr)
	shlr8	r12
	extu.b	r12,r0
	shll8	r0
	bf/s	1f
	or	#N_FLAG,r0
	or	#Z_FLAG,r0
1:
	jmp	@r11
	mov	r0,r12
	
	.align	2
200:	.long	_iocore_inp
_inir:
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.w	@(CPU_BC,gbr),r0
	jsr	@r3
	extu.w	r0,r4
	mov.l	@r15+,r1
	mov	r0,r4
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r5
	mov.b	@(CPU_B,gbr),r0
	extu.b	r0,r6
	MEMWR8	r5, r4
	add	#1,r5
	dt	r6
	mov	r5,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r0
	mov.b	r0,@(CPU_B,gbr)
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	mov	#N_FLAG,r0
	bt/s	1f
	or	r0,r12
	mov	#2,r3
	shll16	r3
	sub	r3,r13
	jmp	@r11
	add	#-5,r14
1:
	mov	#Z_FLAG,r0
	jmp	@r11
	or	r0,r12
	
	.align	2
200:	.long	_iocore_inp
_indr:
	mov.l	200f,r3
	mov	r14,r0
	mov.l	r1,@-r15
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.w	@(CPU_BC,gbr),r0
	jsr	@r3
	extu.w	r0,r4
	mov.l	@r15+,r1
	mov	r0,r4
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r5
	mov.b	@(CPU_B,gbr),r0
	extu.b	r0,r6
	MEMWR8	r5, r4
	add	#-1,r5
	dt	r6
	mov	r5,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r0
	mov.b	r0,@(CPU_B,gbr)
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	and	r0,r12
	mov	#N_FLAG,r0
	bt/s	1f
	or	r0,r12
	mov	#2,r3
	shll16	r3
	sub	r3,r13
	jmp	@r11
	add	#-5,r14
1:
	mov	#Z_FLAG,r0
	jmp	@r11
	or	r0,r12
	
	.align	2
200:	.long	_iocore_inp
_cpi:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r4				! T
	add	#1,r4
	mov	r12,r6
	shlr8	r6				! A
	mov	r4,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r7
	sub	r5,r7
	tst	r7,r7
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r12,r0
	mov	r6,r4
	bf/s	1f
	xor	r7,r4
	or	#Z_FLAG,r0
1:
	mov	r0,r12
	xor	r5,r4
	mov	r7,r0
	mov	#H_FLAG,r3
	and	#0x80,r0
	and	r3,r4
	or	r0,r12
	mov.w	@(CPU_BC,gbr),r0
	mov	#N_FLAG,r3
	extu.w	r0,r5
	or	r3,r4
	dt	r5
	or	r4,r12
	mov	r5,r0
	bt/s	2f
	mov.w	r0,@(CPU_BC,gbr)
	mov	#V_FLAG,r0
	or	r0,r12
2:
	jmp	@r11
	nop
_cpd:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r4				! T
	add	#-1,r4
	mov	r12,r6
	mov	r4,r0
	shlr8	r6				! A
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r7
	sub	r5,r7
	tst	r7,r7
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r12,r0
	mov	r6,r4
	bf/s	1f
	xor	r7,r4
	or	#Z_FLAG,r0
1:
	mov	r0,r12
	xor	r5,r4
	mov	r7,r0
	mov	#H_FLAG,r3
	and	#0x80,r0
	and	r3,r4
	or	r0,r12
	mov.w	@(CPU_BC,gbr),r0
	mov	#N_FLAG,r3
	extu.w	r0,r5
	or	r3,r4
	dt	r5
	or	r4,r12
	mov	r5,r0
	bt/s	2f
	mov.w	r0,@(CPU_BC,gbr)
	mov	#V_FLAG,r0
	or	r0,r12
2:
	jmp	@r11
	nop
_cpir:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r4				! T
	add	#1,r4
	mov	r12,r6
	mov	r4,r0
	shlr8	r6				! A
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r7
	sub	r5,r7
	tst	r7,r7
	mov	r6,r4
	xor	r7,r4
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r12,r0
	xor	r5,r4
	or	#N_FLAG,r0
	mov	#H_FLAG,r3
	mov	r0,r12
	and	r3,r4
	mov.w	@(CPU_BC,gbr),r0
	or	r4,r12
	bt/s	__cpir0
	extu.w	r0,r5
	mov	r7,r0
	and	#0x80,r0
	mov	r0,r6
	dt	r5
	or	r6,r12
	mov	r5,r0
	bt/s	1f
	mov.w	r0,@(CPU_BC,gbr)
	mov	#V_FLAG,r0
	mov	#2,r3
	or	r0,r12
	shll16	r3
	sub	r3,r13
	add	#-5,r14
1:	
	jmp	@r11
	nop
__cpir0:
	mov	r12,r0
	dt	r5
	bt/s	1f
	or	#Z_FLAG,r0
	or	#V_FLAG,r0
1:
	mov	r0,r12
	mov	r5,r0
	jmp	@r11
	mov.w	r0,@(CPU_BC,gbr)
_cpdr:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	MEMRD8	r5, r4				! T
	add	#-1,r4
	mov	r12,r6
	mov	r4,r0
	shlr8	r6				! A
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r7
	sub	r5,r7
	tst	r7,r7
	mov	r6,r4
	xor	r7,r4
	mov	r12,r0
	xor	r5,r4
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r12,r0
	mov	#H_FLAG,r3
	or	#N_FLAG,r0
	mov	r0,r12
	and	r3,r4
	mov.w	@(CPU_BC,gbr),r0
	or	r4,r12
	bt/s	__cpdr0
	extu.w	r0,r5
	mov	r7,r0
	and	#0x80,r0
	dt	r5
	or	r0,r12
	mov	r5,r0
	bt/s	1f
	mov.w	r0,@(CPU_BC,gbr)
	mov	#V_FLAG,r0
	or	r0,r12
	mov	#2,r3
	shll16	r3
	sub	r3,r13
	add	#-5,r14
1:	
	jmp	@r11
	nop
__cpdr0:
	mov	r12,r0
	dt	r5
	bt/s	1f
	or	#Z_FLAG,r0
	or	#V_FLAG,r0
1:
	mov	r0,r12
	mov	r5,r0
	jmp	@r11
	mov.w	r0,@(CPU_BC,gbr)


! ----

z80h_sb:	GETPC8
	mov	r0,r4
	mova	clktbl,r0
	mov	#24,r3
	mov.b	@(r0,r4),r7
	mova	optbl,r0
	shll2	r4
	mov.l	@(r0,r4),r5
	mov	#((1 << 24) >> 24),r0
	sub	r7,r14
	shld	r3,r0
	jmp	@r5
	add	r0,r10

	.align	2
clktbl:	.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	12,12,15,20, 8, 8, 8, 9,12,12,15,20, 8, 8, 8, 9
				.byte	12,12,15,20, 8, 8, 8, 9,12,12,15,20, 8, 8, 8, 9
				.byte	12,12,15,20, 8, 8, 8,18,12,12,15,20, 8, 8, 8,18
				.byte	12,12,15,20, 8, 8, 8, 0,12,12,15,20, 8, 8, 8, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	16,16,16,16, 0, 0, 0, 0,16,16,16,16, 0, 0, 0, 0
				.byte	16,16,16,16, 0, 0, 0, 0,16,16,16,16, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				.byte	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

optbl:	.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_in_b_c,		_out_c_b,		_sbc_hl_bc,		_ld_xword_bc
		.long		_neg,			_retn,			_im_0,			_ld_i_a
		.long		_in_c_c,		_out_c_c,		_adc_hl_bc,		_ld_bc_xword
		.long		_neg,			_reti,			_im_0,			_ld_r_a

		.long		_in_d_c,		_out_c_d,		_sbc_hl_de,		_ld_xword_de
		.long		_neg,			_retn,			_im_1,			_ld_a_i
		.long		_in_e_c,		_out_c_e,		_adc_hl_de,		_ld_de_xword
		.long		_neg,			_reti,			_im_2,			_ld_a_r

		.long		_in_h_c,		_out_c_h,		_sbc_hl_hl,		_ld_xword_hl
		.long		_neg,			_retn,			_im_0,			_rrd
		.long		_in_l_c,		_out_c_l,		_adc_hl_hl,		_ld_hl_xword
		.long		_neg,			_reti,			_im_0,			_rld

		.long		_in_0_c,		_out_c_0,		_sbc_hl_sp,		_ld_xword_sp
		.long		_neg,			_retn,			_im_1,			_ld_nop
		.long		_in_a_c,		_out_c_a,		_adc_hl_sp,		_ld_sp_xword
		.long		_neg,			_reti,			_im_2,			_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ldi,			_cpi,			_ini,			_outi
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ldd,			_cpd,			_ind,			_outd
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ldir,			_cpir,			_inir,			_otir
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_lddr,			_cpdr,			_indr,			_otdr
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop

		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop
		.long		_ld_nop,		_ld_nop,		_ld_nop,		_ld_nop


	.end
