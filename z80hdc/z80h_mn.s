
	.include "../z80hdc/z80h.inc"
	.include "../z80hdc/z80h_mn.inc"

	.globl	_z80h_nonmaskedinterrupt
	.globl	_z80h_interrupt
	.globl	_z80h_execute
	.globl	_z80h_step

	.extern	_z80core
	.extern	z80h_cb
	.extern	z80h_sb
	.extern	z80h_ix
	.extern	z80h_iy
	.extern	_z80dmap
	.extern	mem_read16
	.extern	mem_write16
	.extern	_iocore_out
	.extern	_iocore_inp
	.extern	_dma

	.text
	.align	2

	
_ld_nop:
	jmp	@r11
	nop

_ld_bc_word:	LD16w	CPU_BC
_ld_de_word:	LD16w	CPU_DE
_ld_hl_word:	LD16w	CPU_HL

_ld_xbc_a:	LDxA	CPU_BC
_ld_xde_a:	LDxA	CPU_DE
_ld_xhl_a:	LDxA	CPU_HL

_inc_bc:	MINC16	CPU_BC
_inc_de:	MINC16	CPU_DE
_inc_hl:	MINC16	CPU_HL

_inc_b:	MINC8	CPU_B
_inc_c:	MINC8	CPU_C
_inc_d:	MINC8	CPU_D
_inc_e:	MINC8	CPU_E
_inc_h:	MINC8	CPU_H
_inc_l:	MINC8	CPU_L
_inc_xhl:	MINCM8	CPU_HL

_dec_b:	MDEC8	CPU_B
_dec_c:	MDEC8	CPU_C
_dec_d:	MDEC8	CPU_D
_dec_e:	MDEC8	CPU_E
_dec_h:	MDEC8	CPU_H
_dec_l:	MDEC8	CPU_L
_dec_xhl:	MDECM8	CPU_HL

_ld_b_byte:	LD8b	CPU_B
_ld_c_byte:	LD8b	CPU_C
_ld_d_byte:	LD8b	CPU_D
_ld_e_byte:	LD8b	CPU_E
_ld_h_byte:	LD8b	CPU_H
_ld_l_byte:	LD8b	CPU_L

_add_hl_bc:	MADD16	CPU_HL, CPU_BC
_add_hl_de:	MADD16	CPU_HL, CPU_DE
_add_hl_hl:	MADD16D	CPU_HL
_add_hl_sp:	MADDSP	CPU_HL

_dec_bc:	MDEC16	CPU_BC
_dec_de:	MDEC16	CPU_DE
_dec_hl:	MDEC16	CPU_HL

_ld_xword_hl:	LDx16	CPU_HL
_ld_hl_xword:	LD16x	CPU_HL


_ld_b_c:	LD8		CPU_B, CPU_C
_ld_b_d:	LD8		CPU_B, CPU_D
_ld_b_e:	LD8		CPU_B, CPU_E
_ld_b_h:	LD8		CPU_B, CPU_H
_ld_b_l:	LD8		CPU_B, CPU_L

_ld_c_b:	LD8		CPU_C, CPU_B
_ld_c_d:	LD8		CPU_C, CPU_D
_ld_c_e:	LD8		CPU_C, CPU_E
_ld_c_h:	LD8		CPU_C, CPU_H
_ld_c_l:	LD8		CPU_C, CPU_L

_ld_d_b:	LD8		CPU_D, CPU_B
_ld_d_c:	LD8		CPU_D, CPU_C
_ld_d_e:	LD8		CPU_D, CPU_E
_ld_d_h:	LD8		CPU_D, CPU_H
_ld_d_l:	LD8		CPU_D, CPU_L

_ld_e_b:	LD8		CPU_E, CPU_B
_ld_e_c:	LD8		CPU_E, CPU_C
_ld_e_d:	LD8		CPU_E, CPU_D
_ld_e_h:	LD8		CPU_E, CPU_H
_ld_e_l:	LD8		CPU_E, CPU_L

_ld_h_b:	LD8		CPU_H, CPU_B
_ld_h_c:	LD8		CPU_H, CPU_C
_ld_h_d:	LD8		CPU_H, CPU_D
_ld_h_e:	LD8		CPU_H, CPU_E
_ld_h_l:	LD8		CPU_H, CPU_L

_ld_l_b:	LD8		CPU_L, CPU_B
_ld_l_c:	LD8		CPU_L, CPU_C
_ld_l_d:	LD8		CPU_L, CPU_D
_ld_l_e:	LD8		CPU_L, CPU_E
_ld_l_h:	LD8		CPU_L, CPU_H

_ld_b_xhl:	LD8x	CPU_B, CPU_HL
_ld_c_xhl:	LD8x	CPU_C, CPU_HL
_ld_d_xhl:	LD8x	CPU_D, CPU_HL
_ld_e_xhl:	LD8x	CPU_E, CPU_HL
_ld_h_xhl:	LD8x	CPU_H, CPU_HL
_ld_l_xhl:	LD8x	CPU_L, CPU_HL

_ld_xhl_b:	LDx8	CPU_HL, CPU_B
_ld_xhl_c:	LDx8	CPU_HL, CPU_C
_ld_xhl_d:	LDx8	CPU_HL, CPU_D
_ld_xhl_e:	LDx8	CPU_HL, CPU_E
_ld_xhl_h:	LDx8	CPU_HL, CPU_H
_ld_xhl_l:	LDx8	CPU_HL, CPU_L

_ld_b_a:	LD8A	CPU_B
_ld_c_a:	LD8A	CPU_C
_ld_d_a:	LD8A	CPU_D
_ld_e_a:	LD8A	CPU_E
_ld_h_a:	LD8A	CPU_H
_ld_l_a:	LD8A	CPU_L

_ld_a_b:	LDA8	CPU_B
_ld_a_c:	LDA8	CPU_C
_ld_a_d:	LDA8	CPU_D
_ld_a_e:	LDA8	CPU_E
_ld_a_h:	LDA8	CPU_H
_ld_a_l:	LDA8	CPU_L

_ld_a_xbc:	LDAx	CPU_BC
_ld_a_xde:	LDAx	CPU_DE
_ld_a_xhl:	LDAx	CPU_HL


_add_a_b:	MADDR8	CPU_B
_add_a_c:	MADDR8	CPU_C
_add_a_d:	MADDR8	CPU_D
_add_a_e:	MADDR8	CPU_E
_add_a_h:	MADDR8	CPU_H
_add_a_l:	MADDR8	CPU_L
_add_a_xhl:	MADDM8	CPU_HL

_adc_a_b:	MADCR8	CPU_B
_adc_a_c:	MADCR8	CPU_C
_adc_a_d:	MADCR8	CPU_D
_adc_a_e:	MADCR8	CPU_E
_adc_a_h:	MADCR8	CPU_H
_adc_a_l:	MADCR8	CPU_L
_adc_a_xhl:	MADCM8	CPU_HL

_sub_b:	MSUBR8	CPU_B
_sub_c:	MSUBR8	CPU_C
_sub_d:	MSUBR8	CPU_D
_sub_e:	MSUBR8	CPU_E
_sub_h:	MSUBR8	CPU_H
_sub_l:	MSUBR8	CPU_L
_sub_xhl:	MSUBM8	CPU_HL
_sbc_a_b:	MSBCR8	CPU_B
_sbc_a_c:	MSBCR8	CPU_C
_sbc_a_d:	MSBCR8	CPU_D
_sbc_a_e:	MSBCR8	CPU_E
_sbc_a_h:	MSBCR8	CPU_H
_sbc_a_l:	MSBCR8	CPU_L
_sbc_a_xhl:	MSBCM8	CPU_HL

_and_b:	MANDR8	CPU_B
_and_c:	MANDR8	CPU_C
_and_d:	MANDR8	CPU_D
_and_e:	MANDR8	CPU_E
_and_h:	MANDR8	CPU_H
_and_l:	MANDR8	CPU_L
_and_xhl:	MANDM8	CPU_HL
_xor_b:	MXORR8	CPU_B
_xor_c:	MXORR8	CPU_C
_xor_d:	MXORR8	CPU_D
_xor_e:	MXORR8	CPU_E
_xor_h:	MXORR8	CPU_H
_xor_l:	MXORR8	CPU_L
_xor_xhl:	MXORM8	CPU_HL

_or_b:	MORR8	CPU_B
_or_c:	MORR8	CPU_C
_or_d:	MORR8	CPU_D
_or_e:	MORR8	CPU_E
_or_h:	MORR8	CPU_H
_or_l:	MORR8	CPU_L
_or_xhl:	MORM8	CPU_HL
_cp_b:	MCPR8	CPU_B
_cp_c:	MCPR8	CPU_C
_cp_d:	MCPR8	CPU_D
_cp_e:	MCPR8	CPU_E
_cp_h:	MCPR8	CPU_H
_cp_l:	MCPR8	CPU_L
_cp_xhl:	MCPM8	CPU_HL

_push_af:
	mov	r12,r5
				MPUSHr1
_push_bc:	MPUSH	CPU_BC
_push_de:	MPUSH	CPU_DE
_push_hl:	MPUSH	CPU_HL

_pop_af:	MPOPr0
	jmp	@r11
	mov	r0,r12

	.align	2
10000:	.long	mem_read16

_pop_bc:	MPOP	CPU_BC
_pop_de:	MPOP	CPU_DE
_pop_hl:	MPOP	CPU_HL


! --- PCレジスタ

_jr:	MJR
_jr_nz:	MJRNFLG	Z_FLAG
_jr_nc:	MJRNFLG	C_FLAG
_jr_z:	MJRFLG	Z_FLAG
_jr_c:	MJRFLG	C_FLAG

				! ループする事が真と考える事！
_djnz:
	mov.b	@(CPU_B,gbr),r0
	extu.b	r0,r4
	!
	dt	r4
	!
	mov	r4,r0
	mov.b	r0,@(CPU_B,gbr)

	MJRNE

_jp:	MJP
_jp_hl:	MJPR16	CPU_HL
_jp_nz:	MJPNFLG	Z_FLAG
_jp_nc:	MJPNFLG	C_FLAG
_jp_po:	MJPNFLG	V_FLAG
_jp_p:	MJPNFLG	S_FLAG
_jp_z:	MJPFLG	Z_FLAG
_jp_c:	MJPFLG	C_FLAG
_jp_pe:	MJPFLG	V_FLAG
_jp_m:	MJPFLG	S_FLAG

_call:	MCALL
_call_nz:	MCALLNF	Z_FLAG
_call_nc:	MCALLNF	C_FLAG
_call_po:	MCALLNF	V_FLAG
_call_p:	MCALLNF	S_FLAG
_call_z:	MCALLF	Z_FLAG
_call_c:	MCALLF	C_FLAG
_call_pe:	MCALLF	V_FLAG
_call_m:	MCALLF	S_FLAG

_ret:	MRET
_ret_nz:	MRETNF	Z_FLAG
_ret_nc:	MRETNF	C_FLAG
_ret_po:	MRETNF	V_FLAG
_ret_p:	MRETNF	S_FLAG
_ret_z:	MRETF	Z_FLAG
_ret_c:	MRETF	C_FLAG
_ret_pe:	MRETF	V_FLAG
_ret_m:	MRETF	S_FLAG

_rst_00:	MRST	0x00
_rst_08:	MRST	0x08
_rst_10:	MRST	0x10
_rst_18:	MRST	0x18
_rst_20:	MRST	0x20
_rst_28:	MRST	0x28
_rst_30:	MRST	0x30
_rst_38:	MRST	0x38




! --- HLレジスタ

_ld_xhl_byte:
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r8
	GETPC8
	mov	r0,r4
	MEMWR8	r8, r4
	jmp	@r11
	nop

! --- Aレジスタ


_ld_xbyte_a:	GETPC16
	mov	r0,r4
	mov	r12,r5
	shlr8	r5
	MEMWR8	r4, r5
	jmp	@r11
	nop

	.align	2
10000:	.long	mem_read16

_ld_a_xbyte:	GETPC16
	mov	r0,r4
	MEMRD8	r5, r4
	extu.b	r12,r6
	mov	r5,r12
	shll8	r12
	jmp	@r11
	or	r6,r12

	.align	2
10000:	.long	mem_read16
_inc_a:
	mov	r12,r5
	mov	r5,r0
	shlr8	r5
	and	#~(0xff - C_FLAG),r0
	shll8	r5
	or	r0,r5
	mov	#1,r0
	shll8	r0		! CPU_INCFLAG
	add	r1,r0
	mov	#1,r3
	shll8	r3
	add	r3,r5
	shlr8	r12
	mov.b	@(r0,r12),r6
	mov	#~(1),r3
	swap.w	r5,r0
	and	r3,r0
	extu.b	r6,r12
	swap.w	r0,r5
	jmp	@r11
	or	r5,r12
_dec_a:
	mov	r12,r5
	mov	r5,r0
	shlr8	r5
	and	#~(0xff - C_FLAG),r0
	shll8	r5
	or	r0,r5
	mov	#1,r3
	shll8	r3
	sub	r3,r5
	mov	#1,r0
	mov	#9,r3
	shld	r3,r0	! CPU_DECFLAG
	add	r1,r0
	shlr8	r12
	mov.b	@(r0,r12),r6
	cmp/pz	r5
	bt/s	1f
	extu.b	r6,r12
	mov	#1,r3
	shll16	r3
	add	r3,r5
1:	
	jmp	@r11
	or	r5,r12
_ld_a_byte:	GETPC8
	extu.b	r12,r12
	shll8	r0
	jmp	@r11
	or	r0,r12
_add_a_a:
	swap.b	r12,r4
	extu.b	r4,r4
	shll8	r4
	mov	#H_FLAG,r5
	mov	#7,r3
	shld	r3,r5
	and	r12,r5
	mov	r4,r6
	shll16	r6
	mov	r4,r7
	shll16	r7
	clrt
	mov	r7,r3
	addc	r6,r7
	swap.w	r7,r0
	bf/s	1f
	addv	r6,r3
	or	#C_FLAG,r0
1:
	bf/s	2f
	tst	r7,r7
	or	#V_FLAG,r0
2:
	bf/s	3f
	cmp/pz	r7
	or	#Z_FLAG,r0
3:
	bt	4f
	or	#S_FLAG,r0
4:	
	extu.w	r0,r12
	mov	#-7,r3
	shld	r3,r5
	jmp	@r11
	or	r5,r12

_adc_a_a:
	mov	r12,r0
	tst	#C_FLAG,r0
	shlr8	r0
	extu.b	r0,r0
	bt/s	1f
	shll8	r0
	or	#0x80,r0
1:
	mov	r0,r4
	mov	#H_FLAG,r5
	mov	#7,r3
	shld	r3,r5
	and	r12,r5
	mov	r4,r6
	shll16	r6
	clrt
	mov	r4,r7
	shll16	r7
	mov	r7,r3
	addc	r6,r7
	swap.w	r7,r0
	bf/s	1f
	addv	r6,r3
	or	#C_FLAG,r0
1:
	bf/s	2f
	tst	r7,r7
	or	#V_FLAG,r0
2:
	bf/s	3f
	cmp/pz	r7
	or	#Z_FLAG,r0
3:
	bt	4f
	or	#S_FLAG,r0
4:	
	extu.w	r0,r12
	mov	#-7,r3
	shld	r3,r5
	jmp	@r11
	or	r5,r12
_sub_a:
	jmp	@r11
	mov	#(Z_FLAG + N_FLAG),r12
_sbc_a_a:
	mov	r12,r0
	tst	#C_FLAG,r0
	bf	1f
	jmp	@r11
	mov	#(Z_FLAG + N_FLAG),r12
1:
	mov	#0xff,r0
	shll8	r0
	or	#(S_FLAG + H_FLAG + N_FLAG + C_FLAG),r0
	jmp	@r11
	extu.w	r0,r12

_and_a:	!
_or_a:
	mov	r12,r0
	shlr8	r0
	mov.b	@(r0,r1),r4
	swap.b	r12,r5
	extu.b	r5,r5
	shll8	r5
	extu.b	r4,r12
	jmp	@r11
	or	r5,r12
_xor_a:
	jmp	@r11
	mov	#(Z_FLAG + V_FLAG),r12
_cp_a:
	swap.b	r12,r4
	extu.b	r4,r4
	shll8	r4
	mov	#(Z_FLAG + N_FLAG),r12
	jmp	@r11
	or	r4,r12

_add_a_byte:	GETPC8
	mov	r0,r4
	MADD8	r4
_adc_a_byte:	GETPC8
	mov	r0,r4
	MADC8	r4
_sub_byte:	GETPC8
	mov	r0,r4
	MSUB8	r4
_sbc_a_byte:	GETPC8
	mov	r0,r4
	MSBC8	r4
_and_byte:	GETPC8
	mov	r0,r4
	MAND8	r4
_xor_byte:	GETPC8
	mov	r0,r4
	MXOR8	r4
_or_byte:	GETPC8
	mov	r0,r4
	MOR8	r4
_cp_byte:	GETPC8
	mov	r0,r4
	MCP8	r4

! --- SPレジスタ

_ex_xsp_hl:	MEXSP	CPU_HL

_ld_sp_hl:	MLDSP	CPU_HL

_ld_sp_word:	GETPC16
	mov	r0,r4
	mov	r13,r5
	shlr16	r5
	mov	r5,r13
	shll16	r13
	jmp	@r11
	or	r4,r13

	.align	2
10000:	.long	mem_read16

_inc_sp:
	add	#1,r13
	mov	r13,r4
	shll16	r4
	tst	r4,r4
	bf/s	1f
	mov	#1,r0
	shll16	r0
	sub	r0,r13
1:
	jmp	@r11
	nop
_dec_sp:
	mov	r13,r4
	shll16	r4
	tst	r4,r4
	bf/s	1f
	add	#-1,r13
	mov	#1,r0
	shll16	r0
	add	r0,r13
1:	
	jmp	@r11
	nop

! ---- AFレジスタ

_ex_af_af:
	mov.w	@(CPU_AF2,gbr),r0
	mov	r0,r4
	mov	r12,r0
	mov.w	r0,@(CPU_AF2,gbr)
	jmp	@r11
	extu.w	r4,r12
_rlca:
	mov	#0x7f,r4
	shll8	r4
	and	r12,r4
	mov	#0xec,r5
	extu.b	r5,r5
	and	r12,r5
	swap.b	r12,r0
	tst	#0x80,r0
	mov	r4,r12
	mov	#1,r3
	shld	r3,r12
	bt/s	1f
	add	r5,r12
	add	#C_FLAG,r12
	mov	#1,r3
	shll8	r3
	add	r3,r12
1:	
	jmp	@r11
	nop
_rrca:
	mov	r12,r4
	shlr8	r4	! a
	mov	#0xec,r5
	extu.b	r5,r5
	shlr	r4
	and	r12,r5	! f
	mov	r4,r12
	shll8	r12
	bf/s	1f
	add	r5,r12
	mov	#15,r3
	mov	#1,r0
	add	#C_FLAG,r12
	shld	r3,r0
	add	r0,r12
1:
	jmp	@r11
	nop
_rla:
	mov	#0x7f,r4
	shll8	r4
	mov	#0xec,r5
	extu.b	r5,r5
	mov	r12,r0
	and	r12,r4
	tst	#C_FLAG,r0
	bt/s	1f
	and	r12,r5
	mov	#1,r3
	shll8	r3
	add	r3,r5
1:
	swap.b	r12,r0
	tst	#0x80,r0
	bt/s	2f
	mov	r4,r12
	add	#C_FLAG,r5
2:
	shll	r12
	jmp	@r11
	add	r5,r12
_rra:
	mov	#0xec,r4
	extu.b	r4,r4
	and	r12,r4
	mov	r12,r5
	shlr8	r5
	shlr	r5
	bf/s	1f
	mov	r12,r0
	add	#C_FLAG,r4
1:
	tst	#C_FLAG,r0
	bt/s	2f
	mov	#0x80,r0
	extu.b	r0,r0
	shll8	r0
	add	r0,r4
2:
	mov	r5,r12
	shll8	r12
	jmp	@r11
	add	r4,r12
_daa:
	swap.b	r12,r4
	extu.b	r4,r4
	shll8	r4		! dst
	mov	#0x0f,r5
	shll8	r5
	mov	r12,r0
	tst	#N_FLAG,r0
	bf/s	_daa_n
	and	r12,r5	! alow
	mov	#(N_FLAG + C_FLAG),r6
	mov	#0x09,r0
	shll8	r0
	cmp/hi	r0,r5
	and	r12,r6
	bf/s	1f
	mov	r6,r0
	bra	2f
	or	#H_FLAG,r0		! -> ne..
1:
	mov	#H_FLAG,r3
	tst	r3,r12
	bt	3f
2:
	mov	#0x06,r3
	shll8	r3
	add	r3,r4
3:
	mov	#0xa0,r3
	extu.b	r3,r3
	shll8	r3
	cmp/hs	r3,r4
	bf	4f
	or	#C_FLAG,r0
4:	
	tst	#C_FLAG,r0
	bt/s	5f
	mov	r0,r6
	mov	#0x60,r3
	shll8	r3
	add	r3,r4
5:
	bra	daa_setr8
	nop
_daa_n:
	mov	#(N_FLAG + C_FLAG),r6
	mov	#0x9a,r0
	shll8	r0
	and	r12,r6
	cmp/hs	r0,r4
	bf/s	1f
	mov	r6,r0
	or	#C_FLAG,r0
1:
	tst	#C_FLAG,r0
	bt/s	2f
	mov	r0,r6
	mov	#0x60,r3
	shll8	r3
	sub	r3,r4
2:
	mov	r12,r0
	tst	#H_FLAG,r0
	bf	_daa_nh
	mov	#0x0a,r0
	shll8	r0
	cmp/hs	r0,r5
	bf/s	daa_setr8
	mov	#0x06,r3
	shll8	r3
	sub	r3,r4
	cmp/pz	r4
	bt/s	3f
	mov	r6,r0
	or	#C_FLAG,r0
3:	
	bra	daa_setr8
	mov	r0,r6
_daa_nh:
	mov	#0x06,r0
	shll8	r0
	cmp/hs	r0,r5
	bt/s	1f
	mov	#H_FLAG,r3
	or	r3,r6
1:
	sub	r0,r4
daa_setr8:
	swap.b	r4,r5
	extu.b	r5,r5
	shll8	r5
	mov	r6,r12
	add	r5,r12
	mov	r5,r0
	shlr8	r0
	mov.b	@(r0,r1),r4	! CPU_SZPFLAG
	extu.b	r4,r4
	jmp	@r11
	add	r4,r12
_cpl:
	mov	#0xff,r0
	extu.b	r0,r0
	shll8	r0
	xor	r12,r0
	or	#(H_FLAG + N_FLAG),r0
	jmp	@r11
	mov	r0,r12
_scf:
	mov	#~(H_FLAG + N_FLAG),r3
	mov	#C_FLAG,r0
	and	r3,r12
	jmp	@r11
	or	r0,r12
_ccf:
	mov	#~(H_FLAG + N_FLAG),r5
	mov	#C_FLAG,r0
	tst	r0,r12
	bt/s	1f
	and	r12,r5
	mov	#H_FLAG,r0
	or	r0,r5
1:
	mov	#C_FLAG,r12
	jmp	@r11
	xor	r5,r12


! ---- システム

_di:
	mov	#(1 << IFF_IFLAG),r0
	jmp	@r11
	or	r0,r10
_ei:
	mov	#(1 << IFF_IFLAG),r0
	tst	r0,r10
	bf	1f
	jmp	@r11
	nop
1:
	mov	#~(1 << IFF_IFLAG),r0
	mov	r14,r5
	add	#-1,r5
	cmp/pl	r5
	bt/s	2f
	and	r0,r10
	jmp	@r11
	nop
2:
	mov	#(1 << IFF_NMI),r0
	tst	r0,r10
	bt/s	3f
	mov.l	@(CPU_REQIRQ,gbr),r0
	jmp	@r11
	nop
3:	
	cmp/eq	#0,r0
	bf	4f
	jmp	@r11
	nop
4:	
	mov.l	@(CPU_BASECLOCK,gbr),r0
	mov	#1,r14
	sub	r5,r0
	jmp	@r11
	mov.l	r0,@(CPU_BASECLOCK,gbr)
_halt:
	mov	#1,r3
	shll16	r3
	mov	#(1 << IFF_HALT),r0
	sub	r3,r13
	or	r0,r10
	jmp	@r11
	mov	#0,r14

_ex_de_hl:
	mov.w	@(CPU_DE,gbr),r0
	mov	r0,r4
	mov.w	@(CPU_HL,gbr),r0
	mov.w	r0,@(CPU_DE,gbr)
	mov	r4,r0
	jmp	@r11
	mov.w	r0,@(CPU_HL,gbr)
	
_exx:
	mov.w	@(CPU_BC,gbr),r0
	mov	r0,r4
	mov.w	@(CPU_DE,gbr),r0
	mov	r0,r5
	mov.w	@(CPU_HL,gbr),r0
	mov	r0,r6
	mov.w	@(CPU_BC2,gbr),r0
	mov	r0,r7
	mov.w	@(CPU_DE2,gbr),r0
	mov	r0,r8
	mov.w	@(CPU_HL2,gbr),r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r7,r0
	mov.w	r0,@(CPU_BC,gbr)
	mov	r8,r0
	mov.w	r0,@(CPU_DE,gbr)
	mov	r4,r0
	mov.w	r0,@(CPU_BC2,gbr)
	mov	r5,r0
	mov.w	r0,@(CPU_DE2,gbr)
	mov	r6,r0
	jmp	@r11
	mov.w	r0,@(CPU_HL2,gbr)
	
_out_byte_a:	GETPC8
	mov	r0,r4
	swap.b	r12,r6
	extu.b	r6,r6
	shll8	r6
	mov	r4,r5
	mov.l	200f,r3
	or	r6,r4
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov.l	@r15+,r1
	jmp	@r11
	mov	r0,r14
	
	.align	2
200:	.long	_iocore_out
_in_a_byte:	GETPC8
	mov	r0,r4
	swap.b	r12,r5
	extu.b	r5,r5
	shll8	r5
	mov.l	200f,r3
	mov	r14,r0
	or	r5,r4
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov	r0,r4
	mov.l	@r15+,r1
	extu.b	r12,r5
	mov	r4,r12
	shll8	r12
	or	r5,r12
	mov.l	@(CPU_REMCLOCK,gbr),r0
	jmp	@r11
	mov	r0,r14

	.align	2
200:	.long	_iocore_inp


! ----

_z80h_nonmaskedinterrupt:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	200f,r0
	mov.l	201f,r1
	ldc	r0,gbr
	mov.l	@(_CPU_IFF,gbr),r0
	mov	r0,r10
	!
	mov	#(1 << IFF_NMI),r3
	tst	r3,r10
	mov.l	@(_CPU_SP,gbr),r0
	bf/s	__nmiend
	mov	r0,r13
	!
	mov	#(1 << IFF_HALT),r0
	mov	r13,r4
	tst	r0,r10
	bt/s	1f
	shll16	r4
	mov	#1,r3
	shll16	r3
	add	r3,r13
	mov	#~(1 << IFF_HALT),r0
	and	r0,r10
1:
	mov	#2,r3
	shll16	r3
	mov	r4,r6
	sub	r3,r6	! stack
	mov	#(1 << IFF_NMI),r0
	mov	r6,r4
	or	r0,r10
	mov	r13,r5
	shlr16	r4
	mov	#0x66,r13
	shlr16	r5
	mov.l	10001f,r3
	shll16	r13
	jsr	@r3
	or	r4,r13
	!
	mov	r13,r0
	mov.l	r0,@(_CPU_SP,gbr)
	!
	mov	r10,r0
	mov.l	r0,@(_CPU_IFF,gbr)
__nmiend:
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8

	.align	2
200:	.long	_z80core
201:	.long	_z80core + CPU_SIZE
10001:	.long	mem_write16

_z80h_interrupt:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	200f,r0
	mov.l	201f,r1
	ldc	r0,gbr
	mov.l	@(_CPU_IFF,gbr),r0
	mov	r0,r10
	mov.w	@(_CPU_AF,gbr),r0
	extu.w	r0,r12
	mov.l	@(_CPU_SP,gbr),r0
	mov	r0,r13
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov	#(1 << IFF_HALT),r3
	tst	r3,r10
	mov	#(1 << IFF_IFLAG),r0
	bt/s	1f
	or	r0,r10
	mov	#~(1 << IFF_HALT),r0
	mov	#1,r3
	and	r0,r10
	shll16	r3
	add	r3,r13
1:
	mov	r10,r0
	shlr16	r0
	tst	#3,r0
	bt	__intrim0
	mov	r13,r6
	shll16	r6	! stack
	mov	r13,r5
	shlr16	r5	! pc
	mov	#2,r3
	shll16	r3
	tst	r3,r10
	bf/s	__intrim2
	sub	r3,r6
__intrim1:
	mov	r6,r4
	shlr16	r4
	Z80WORK	11
	mov	#0x38,r13
	mova	__intrend,r0
	shll16	r13
	mov.l	10001f,r3
	lds	r0,pr
	jmp	@r3
	or	r4,r13

	.align	2
200:	.long	_z80core
201:	.long	_z80core + CPU_SIZE
10001:	.long	mem_write16
__intrim2:
	swap.b	r10,r7
	extu.b	r7,r7
	shll8	r7
	mov	r6,r13
	shlr16	r13
	mov	r4,r8
	mov.l	10001f,r3
	or	r7,r8
	mov	r6,r4
	jsr	@r3
	shlr16	r4
	mov.l	10000f,r3
	jsr	@r3
	mov	r8,r4
	shll16	r0
	or	r0,r13
__intrend:
	mov	r12,r0
	mov.w	r0,@(_CPU_AF,gbr)
	mov	r13,r0
	mov.l	r0,@(_CPU_SP,gbr)
	mov	r14,r0
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov	r10,r0
	mov.l	r0,@(_CPU_IFF,gbr)
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
__intrim0:
	mov.l	201f,r5
	mov	r4,r0
	mov.l	202f,r6
	mov.b	@(r0,r5),r7
	shll2	r0
	mov.l	@(r0,r6),r5
	mov.l	203f,r11
	jmp	@r5
	sub	r7,r14

	.align	2
10000:	.long	mem_read16	
10001:	.long	mem_write16
201:	.long	clktbl
202:	.long	optbl
203:	.long	__intrend
	
_z80h_execute:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	200f,r0
	mov.l	201f,r1
	ldc	r0,gbr
	mov.l	s_dma,r4
	mov.w	@(_CPU_AF,gbr),r0
	extu.w	r0,r12
	mov.l	@(_CPU_SP,gbr),r0
	mov	r0,r13
	mov.b	@r4,r5
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov.l	@(_CPU_IFF,gbr),r0
	tst	r5,r5
	bf/s	__z80hwdma
	mov	r0,r10
__z80hlp:
	cmp/pl	r14
	bf	__z80hed
	GETPC8
	mov.l	202f,r5
	mov	#24,r3
	mov.l	203f,r6
	mov.b	@(r0,r5),r7
	shll2	r0
	mov.l	@(r0,r6),r5
	mov	#((1 << 24) >> 24),r0
	shld	r3,r0
	mov.l	204f,r11
	add	r0,r10
	jmp	@r5
	sub	r7,r14
	
	.align	2
200:	.long	_z80core
201:	.long	_z80core + CPU_SIZE
s_dma:	.long	_dma
202:	.long	clktbl
203:	.long	optbl
204:	.long	__z80hlp

__z80hwdma:	GETPC8
	mov.l	202f,r5
	mov.l	203f,r6
	mov.b	@(r0,r5),r7
	shll2	r0
	mov	#24,r3
	mov.l	@(r0,r6),r5
	mov	#((1 << 24) >> 24),r0
	shld	r3,r0
	sub	r7,r14
	add	r0,r10
	jsr	@r5
	sts	pr,r11
	mov.l	204f,r3
	jsr	@r3
	mov.l	r1,@-r15
	cmp/pl	r14
	bt/s	__z80hwdma
	mov.l	@r15+,r1
__z80hed:
	mov	r12,r0
	mov.w	r0,@(_CPU_AF,gbr)
	mov	r13,r0
	mov.l	r0,@(_CPU_SP,gbr)
	mov	r14,r0
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov	r10,r0
	mov.l	r0,@(_CPU_IFF,gbr)
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8

	.align	2
201:	.long	_z80core + CPU_SIZE
202:	.long	clktbl
203:	.long	optbl
204:	.long	_z80dmap
	
_z80h_step:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	200f,r0
	mov.l	201f,r1
	ldc	r0,gbr
	mov.w	@(_CPU_AF,gbr),r0
	extu.w	r0,r12
	mov.l	@(_CPU_SP,gbr),r0
	mov	r0,r13
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
	mov.l	@(_CPU_IFF,gbr),r0
	mov	r0,r10
	GETPC8
	mov.l	202f,r5
	mov	#((1 << 24) >> 24),r7
	mov.l	203f,r6
	mov	#24,r3
	shld	r3,r7
	add	r7,r10
	mov.b	@(r0,r5),r7
	shll2	r0
	mov.l	@(r0,r6),r5
	sub	r7,r14
	jsr	@r5
	sts	pr,r11
	mov	r12,r0
	mov.w	r0,@(_CPU_AF,gbr)
	mov	r13,r0
	mov.l	r0,@(_CPU_SP,gbr)
	mov	r14,r0
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov	r10,r0
	mov.l	r0,@(_CPU_IFF,gbr)
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	mov.l	204f,r3
	jmp	@r3
	nop

	.align	2
200:	.long	_z80core
201:	.long	_z80core + CPU_SIZE
202:	.long	clktbl
203:	.long	optbl
204:	.long	_z80dmap
	

clktbl:	.byte	 4,10, 7, 6, 4, 4, 7, 4, 4,11, 7, 6, 4, 4, 7, 4
				.byte	 8,10, 7, 6, 4, 4, 7, 4, 7,11, 7, 6, 4, 4, 7, 4
				.byte	 7,10,16, 6, 4, 4, 7, 4, 7,11,16, 6, 4, 4, 7, 4
				.byte	 7,10,13, 6,11,11,10, 4, 7,11,13, 6, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 7, 7, 7, 7, 7, 7, 4, 7, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4
				.byte	 5,10,10,10,10,11, 7,11, 5, 4,10, 0,10,10, 7,11
				.byte	 5,10,10,11,10,11, 7,11, 5, 4,10,11,10, 0, 7,11
				.byte	 5,10,10,19,10,11, 7,11, 5, 4,10, 4,10, 0, 7,11
				.byte	 5,10,10, 4,10,11, 7,11, 5, 6,10, 4,10, 0, 7,11

optbl:	.long		_ld_nop,		_ld_bc_word,	_ld_xbc_a,		_inc_bc
		.long		_inc_b,			_dec_b,			_ld_b_byte,		_rlca
		.long		_ex_af_af,		_add_hl_bc,		_ld_a_xbc,		_dec_bc
		.long		_inc_c,			_dec_c,			_ld_c_byte,		_rrca

		.long		_djnz,			_ld_de_word,	_ld_xde_a,		_inc_de
		.long		_inc_d,			_dec_d,			_ld_d_byte,		_rla
		.long		_jr,			_add_hl_de,		_ld_a_xde,		_dec_de
		.long		_inc_e,			_dec_e,			_ld_e_byte,		_rra

		.long		_jr_nz,			_ld_hl_word,	_ld_xword_hl,	_inc_hl
		.long		_inc_h,			_dec_h,			_ld_h_byte,		_daa
		.long		_jr_z,			_add_hl_hl,		_ld_hl_xword,	_dec_hl
		.long		_inc_l,			_dec_l,			_ld_l_byte,		_cpl

		.long		_jr_nc,			_ld_sp_word,	_ld_xbyte_a,	_inc_sp
		.long		_inc_xhl,		_dec_xhl,		_ld_xhl_byte,	_scf
		.long		_jr_c,			_add_hl_sp,		_ld_a_xbyte,	_dec_sp
		.long		_inc_a,			_dec_a,			_ld_a_byte,		_ccf

		.long		_ld_nop,		_ld_b_c,		_ld_b_d,		_ld_b_e
		.long		_ld_b_h,		_ld_b_l,		_ld_b_xhl,		_ld_b_a
		.long		_ld_c_b,		_ld_nop,		_ld_c_d,		_ld_c_e
		.long		_ld_c_h,		_ld_c_l,		_ld_c_xhl,		_ld_c_a

		.long		_ld_d_b,		_ld_d_c,		_ld_nop,		_ld_d_e
		.long		_ld_d_h,		_ld_d_l,		_ld_d_xhl,		_ld_d_a
		.long		_ld_e_b,		_ld_e_c,		_ld_e_d,		_ld_nop
		.long		_ld_e_h,		_ld_e_l,		_ld_e_xhl,		_ld_e_a

		.long		_ld_h_b,		_ld_h_c,		_ld_h_d,		_ld_h_e
		.long		_ld_nop,		_ld_h_l,		_ld_h_xhl,		_ld_h_a
		.long		_ld_l_b,		_ld_l_c,		_ld_l_d,		_ld_l_e
		.long		_ld_l_h,		_ld_nop,		_ld_l_xhl,		_ld_l_a

		.long		_ld_xhl_b,		_ld_xhl_c,		_ld_xhl_d,		_ld_xhl_e
		.long		_ld_xhl_h,		_ld_xhl_l,		_halt,			_ld_xhl_a
		.long		_ld_a_b,		_ld_a_c,		_ld_a_d,		_ld_a_e
		.long		_ld_a_h,		_ld_a_l,		_ld_a_xhl,		_ld_nop

		.long		_add_a_b,		_add_a_c,		_add_a_d,		_add_a_e
		.long		_add_a_h,		_add_a_l,		_add_a_xhl,		_add_a_a
		.long		_adc_a_b,		_adc_a_c,		_adc_a_d,		_adc_a_e
		.long		_adc_a_h,		_adc_a_l,		_adc_a_xhl,		_adc_a_a

		.long		_sub_b,			_sub_c,			_sub_d,			_sub_e
		.long		_sub_h,			_sub_l,			_sub_xhl,		_sub_a
		.long		_sbc_a_b,		_sbc_a_c,		_sbc_a_d,		_sbc_a_e
		.long		_sbc_a_h,		_sbc_a_l,		_sbc_a_xhl,		_sbc_a_a

		.long		_and_b,			_and_c,			_and_d,			_and_e
		.long		_and_h,			_and_l,			_and_xhl,		_and_a
		.long		_xor_b,			_xor_c,			_xor_d,			_xor_e
		.long		_xor_h,			_xor_l,			_xor_xhl,		_xor_a

		.long		_or_b,			_or_c,			_or_d,			_or_e
		.long		_or_h,			_or_l,			_or_xhl,		_or_a
		.long		_cp_b,			_cp_c,			_cp_d,			_cp_e
		.long		_cp_h,			_cp_l,			_cp_xhl,		_cp_a

		.long		_ret_nz,		_pop_bc,		_jp_nz,			_jp
		.long		_call_nz,		_push_bc,		_add_a_byte,	_rst_00
		.long		_ret_z,			_ret,			_jp_z,			z80h_cb
		.long		_call_z,		_call,			_adc_a_byte,	_rst_08

		.long		_ret_nc,		_pop_de,		_jp_nc,			_out_byte_a
		.long		_call_nc,		_push_de,		_sub_byte,		_rst_10
		.long		_ret_c,			_exx,			_jp_c,			_in_a_byte
		.long		_call_c,		z80h_ix,		_sbc_a_byte,	_rst_18

		.long		_ret_po,		_pop_hl,		_jp_po,			_ex_xsp_hl
		.long		_call_po,		_push_hl,		_and_byte,		_rst_20
		.long		_ret_pe,		_jp_hl,			_jp_pe,			_ex_de_hl
		.long		_call_pe,		z80h_sb,		_xor_byte,		_rst_28

		.long		_ret_p,			_pop_af,		_jp_p,			_di
		.long		_call_p,		_push_af,		_or_byte,		_rst_30
		.long		_ret_m,			_ld_sp_hl,		_jp_m,			_ei
		.long		_call_m,		z80h_iy,		_cp_byte,		_rst_38


	.end
