
.macro MADC16	r
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	mov.w	@(\r,gbr),r0
	mov	r12,r2
	extu.w	r0,r5
	shlr	r2
	mov	r4,r6
	addc	r5,r6
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	mov	r4,r7
	xor	r6,r7
	mov	r5,r2
	xor	r6,r2
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r4
	mov	#15,r3
	shld	r3,r4
	shll	r4
	mov	r4,r3
	mov	r5,r4
	xor	r7,r4
	mov	r12,r0
	bf/s	1f
	tst	r3,r3
	or	#C_FLAG,r0
1:	
	and	r2,r7
	bf/s	2f
	cmp/pz	r3
	or	#Z_FLAG,r0
2:
	mov	#H_FLAG,r12
	shll8	r12
	bt/s	3f
	and	r12,r4
	or	#S_FLAG,r0
3:
	mov	#13,r3
	mov	#V_FLAG,r12
	shld	r3,r12
	and	r12,r7
	shlr8	r4
	mov	r0,r12
	or	r4,r12
	mov	#-13,r3
	shld	r3,r7
	jmp	@r11
	or	r7,r12
.endm

.macro MADCHL2
	mov.w	@(CPU_HL,gbr),r0
	mov	r12,r2
	extu.w	r0,r4
	shlr	r2
	mov	r4,r5
	addc	r4,r5
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	mov	r5,r2
	mov	#15,r3
	shld	r3,r2
	shll	r2
	mov	r5,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r12,r0
	bf/s	1f
	tst	r2,r2
	or	#C_FLAG,r0
1:	
	mov	r4,r6
	xor	r5,r6
	bf/s	2f
	cmp/pz	r2
	or	#Z_FLAG,r0
2:
	mov	#13,r3
	mov	#V_FLAG,r7
	shld	r3,r7
	and	r6,r7
	mov	#H_FLAG,r6
	shll8	r6
	bt/s	3f
	and	r5,r6
	or	#S_FLAG,r0
3:
	mov	#-13,r3
	mov	r0,r12
	shld	r3,r7
	or	r7,r12
	shlr8	r6
	jmp	@r11
	or	r6,r12
.endm

.macro MADCSP
	mov.w	@(CPU_HL,gbr),r0
	mov	r13,r5
	extu.w	r0,r4
	shll16	r5
	mov	#C_FLAG,r6
	and	r12,r6
	mov	r5,r0
	add	r4,r6
	shlr16	r0
	add	r0,r6
	mov	r4,r7
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	xor	r6,r7
	mov	r13,r2
	xor	r6,r2
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r6,r4
	mov	#15,r3
	shld	r3,r4
	shll	r4
	mov	r4,r3
	mov	r13,r4
	xor	r7,r4
	mov	r12,r0
	bf/s	1f
	tst	r3,r3
	or	#C_FLAG,r0
1:	
	and	r2,r7
	bf/s	2f
	cmp/pz	r3
	or	#Z_FLAG,r0
2:
	mov	#H_FLAG,r12
	shll8	r12
	bt/s	3f
	and	r12,r4
	or	#S_FLAG,r0
3:
	mov	#13,r3
	mov	#V_FLAG,r12
	shld	r3,r12
	shlr8	r4
	and	r12,r7
	mov	r0,r12
	or	r4,r12
	mov	#-13,r3
	shld	r3,r7
	jmp	@r11
	or	r7,r12
.endm


.macro MSBC16	r
	mov.w	@(CPU_HL,gbr),r0
	extu.w	r0,r4
	mov.w	@(\r,gbr),r0
	mov	#C_FLAG,r6
	extu.w	r0,r5
	and	r12,r6
	mov	r4,r0
	sub	r6,r0
	mov	r0,r6
	mov	r4,r7
	xor	r5,r7
	sub	r5,r6
	xor	r6,r4
	mov	r7,r5
	xor	r6,r5
	mov	r6,r2
	mov	#15,r3
	shld	r3,r2
	shll	r2
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r12,r0
	or	#N_FLAG,r0
	and	r7,r4
	bf/s	1f
	tst	r2,r2
	or	#C_FLAG,r0
1:
	mov	#H_FLAG,r3
	shll8	r3
	and	r3,r5
	bf/s	2f
	cmp/pz	r2
	or	#Z_FLAG,r0
2:
	mov	#13,r3
	mov	#V_FLAG,r12
	shld	r3,r12
	bt/s	3f
	and	r12,r4
	or	#S_FLAG,r0
3:
	mov	r0,r12
	shlr8	r5
	mov	#-13,r3
	or	r5,r12
	shld	r3,r4
	jmp	@r11
	or	r4,r12
.endm

.macro MSBCHL2
	mov	#C_FLAG,r3
	tst	r3,r12
	bt/s	1f
	mov	#0,r0
	add	#-1,r0
1:
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	bf/s	2f
	mov.w	r0,@(CPU_HL,gbr)
	mov	#(Z_FLAG + N_FLAG),r0
	jmp	@r11
	or	r0,r12
2:
	mov	r12,r0
	or	#(S_FLAG + H_FLAG + N_FLAG + C_FLAG),r0
	jmp	@r11
	mov	r0,r12
.endm

.macro MSBCSP
	mov.w	@(CPU_HL,gbr),r0
	mov	r13,r5
	extu.w	r0,r4
	shll16	r5
	mov	#C_FLAG,r6
	and	r12,r6
	mov	r4,r0
	sub	r6,r0
	mov	r13,r7
	mov	r0,r6
	mov	r5,r3
	xor	r4,r7
	shlr16	r3
	sub	r3,r6
	xor	r6,r4
	mov	r7,r5
	xor	r6,r5
	mov	r6,r2
	mov	#15,r3
	shld	r3,r2
	shll	r2
	shlr8	r12
	extu.b	r12,r12
	shll8	r12
	mov	r6,r0
	mov.w	r0,@(CPU_HL,gbr)
	mov	r12,r0
	or	#N_FLAG,r0
	and	r7,r4
	bf/s	1f
	tst	r2,r2
	or	#C_FLAG,r0
1:
	mov	#H_FLAG,r3
	shll8	r3
	and	r3,r5
	bf/s	2f
	cmp/pz	r2
	or	#Z_FLAG,r0
2:
	mov	#13,r3
	mov	#V_FLAG,r12
	shld	r3,r12
	bt/s	3f
	and	r12,r4
	or	#S_FLAG,r0
3:
	mov	r0,r12
	shlr8	r5
	mov	#-13,r3
	or	r5,r12
	shld	r3,r4
	jmp	@r11
	or	r4,r12
.endm


.macro OUT8
	mov.l	500022f,r3
	mov.w	@(CPU_BC,gbr),r0
	extu.w	r0,r4
	mov	r14,r0
	mov.l	r1,@-r15
	jsr	@r3
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	mov.l	@r15+,r1
	mov.l	@(CPU_REMCLOCK,gbr),r0
	jmp	@r11
	mov	r0,r14
	
	.align	2
500022:	.long	_iocore_out
.endm

.macro OUTR8	r
	mov.b	@(\r,gbr),r0
	extu.b	r0,r5
	OUT8
.endm


.macro INP8
	mov.w	@(CPU_BC,gbr),r0
	mov.l	500024f,r3
	extu.w	r0,r4
	mov	r14,r0
	mov.l	r0,@(CPU_REMCLOCK,gbr)
	jsr	@r3
	mov.l	r1,@-r15
	mov.l	@r15+,r1
	mov	r0,r4
	mov.b	@(r0,r1),r5
	mov	r12,r0
	shlr8	r12
	and	#~(0xff - C_FLAG),r0
	shll8	r12
	or	r12,r0
	extu.b	r5,r12
	or	r0,r12
	mov.l	@(CPU_REMCLOCK,gbr),r0
	mov	r0,r14
.endm

.macro _INP8
	.align	2
500024:	.long	_iocore_inp
.endm
	
.macro INPR8	r
	INP8
	mov	r4,r0
	jmp	@r11
	mov.b	r0,@(\r,gbr)

	_INP8
.endm
