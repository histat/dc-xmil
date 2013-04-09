
.macro LDA_xI	b
	GETIPC	r4, \b
	MEMRD8	r5, r4
	extu.b	r12,r6
	mov	r5,r12
	shll8	r12
	jmp	@r11
	or	r6,r12
.endm

.macro LD8_xI	dst, b
	GETIPC	r4, \b
	MEMRD8	r5, r4
	mov	r5,r0
	jmp	@r11
	mov.b	r0,@(\dst,gbr)
.endm

.macro LDxI_A	b
	GETIPC	r4, \b
	mov	r12,r5
	shlr8	r5
	MEMWR8	r4, r5
	jmp	@r11
	nop
.endm

.macro LDxI_8	b, src
	GETIPC	r4, \b
	mov.b	@(\src,gbr),r0
	extu.b	r0,r5
	MEMWR8	r4, r5
	jmp	@r11
	nop
.endm

.macro LDxI_b	b
	GETIPC	r8, \b
	GETPC8
	mov	r0,r4
	MEMWR8	r8, r4
	jmp	@r11
	nop
.endm


.macro MINCI8	b
	GETIPC	r4, \b
	MEMLEA8	r4, r5, r6
	mov	r4,r0
	mov	#1,r2
	mov.b	@(r0,r5),r7
	shll8	r2	! CPU_INCFLAG
	mov	r12,r0
	add	r1,r2
	extu.b	r7,r7
	shlr8	r12
	add	r7,r2
	and	#~(0xff - C_FLAG),r0
	mov.b	@r2,r5
	shll8	r12
	add	#1,r7
	or	r0,r12
	extu.b	r5,r5
	or	r5,r12
	mov	r4,r0
	jmp	@r11
	mov.b	r7,@(r0,r6)
.endm

.macro MDECI8	b
	GETIPC	r4, \b
	MEMLEA8	r4, r5, r6
	mov	r4,r0
	mov	#1,r2
	mov.b	@(r0,r5),r7
	mov	#9,r3
	mov	r12,r0
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


.macro MADDI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MADD8	r2
.endm

.macro MADCI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MADC8	r2
.endm

.macro MSUBI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MSUB8	r2
.endm

.macro MSBCI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MSBC8	r2
.endm

.macro MANDI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MAND8	r2
.endm

.macro MXORI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MXOR8	r2
.endm

.macro MORI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MOR8	r2
.endm

.macro MCPI8	b
	GETIPC	r4, \b
	MEMRD8	r2, r4
	MCP8	r2
.endm

