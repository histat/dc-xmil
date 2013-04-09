
	.include "../z80hdc/z80h.inc"

	.globl	_z80mem_read8
	.globl	_z80mem_write8
	.globl	mem_read16
	.globl	mem_write16

	.extern	_z80core

	.text
	.align	2

	
_z80mem_read8:
	mov.l	z8h_r7,r6
	exts.w	r4,r0
	cmp/pz	r0
	bt	1f
	add	#CPU_MEMBASE,r6
	mov.l	@r6,r0
	mov.b	@(r0,r4),r4
	rts
	extu.b	r4,r0
1:
	add	#CPU_MEMREAD,r6
	mov.l	@r6,r0
	mov.b	@(r0,r4),r4
	rts
	extu.b	r4,r0

_z80mem_write8:
	mov.l	z8h_r7,r6
	exts.w	r4,r0
	cmp/pz	r0
	bt	1f
	add	#CPU_MEMBASE,r6
	bra	2f
	mov.l	@r6,r0
1:
	add	#CPU_MEMWRITE,r6
	mov.l	@r6,r0
2:	
	rts
	mov.b	r5,@(r0,r4)

	.align	2
z8h_r7:	.long		_z80core

	
! ---- .Sから呼ばれる

mem_read16:
	exts.w	r4,r0
	cmp/pz	r0
	bt/s	1f
	tst	#1,r0
	bf/s	_mr16a
	mov.l	@(CPU_MEMBASE,gbr),r0
	mov.w	@(r0,r4),r4
	rts
	extu.w	r4,r0
1:
	bf/s	_mr16a
	mov.l	@(CPU_MEMREAD,gbr),r0
	mov.w	@(r0,r4),r4
	rts
	extu.w	r4,r0
_mr16a:
	mov	r4,r6
	add	#1,r6
	mov.b	@(r0,r4),r4
	mov	r6,r7
	mov	#17,r3
	shld	r3,r7
	tst	r7,r7
	bt/s	_mr16b
	extu.b	r4,r4
	mov.b	@(r0,r6),r5
	extu.b	r5,r0
	shll8	r0
	rts
	or	r4,r0
_mr16b:
	exts.w	r6,r0
	mov	#~(0x10000 >> 16),r3
	cmp/pz	r0
	swap.w	r6,r6
	and	r3,r6
	bt/s	1f
	swap.w	r6,r6
	mov.l	@(CPU_MEMBASE,gbr),r0
	mov.b	@(r0,r6),r5
	extu.b	r5,r0
	shll8	r0
	rts
	or	r4,r0
1:
	mov.l	@(CPU_MEMREAD,gbr),r0
	mov.b	@(r0,r6),r5
	extu.b	r5,r0
	shll8	r0
	rts
	or	r4,r0

mem_write16:
	exts.w	r4,r0
	cmp/pz	r0
	bt/s	1f
	tst	#1,r0
	bf/s	_mw16a
	mov.l	@(CPU_MEMBASE,gbr),r0
	rts
	mov.w	r5,@(r0,r4)
1:	
	bf/s	_mw16a
	mov.l	@(CPU_MEMWRITE,gbr),r0
	rts
	mov.w	r5,@(r0,r4)
_mw16a:
	mov	r4,r6
	add	#1,r6
	mov.b	r5,@(r0,r4)
	mov	r6,r7
	mov	#17,r3
	shld	r3,r7
	tst	r7,r7
	bt/s	_mw16b
	shlr8	r5
	rts
	mov.b	r5,@(r0,r6)
_mw16b:
	exts.w	r6,r0
	mov	#~(0x10000 >> 16),r3
	cmp/pz	r0
	swap.w	r6,r6
	and	r3,r6
	bt/s	1f
	swap.w	r6,r6
	mov.l	@(CPU_MEMBASE,gbr),r0
	rts
	mov.b	r5,@(r0,r6)
1:
	mov.l	@(CPU_MEMWRITE,gbr),r0
	rts
	mov.b	r5,@(r0,r6)
	

	.end
