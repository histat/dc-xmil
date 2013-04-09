
C_FLAG				=		0x01
N_FLAG				=		0x02
V_FLAG				=		0x04
H_FLAG				=		0x10
Z_FLAG				=		0x40
S_FLAG				=		0x80

IFF_IFLAG			=		0
IFF_NMI				=		1
IFF_HALT			=		2

CPU_SIZE			=		(56 + 16)
CPU_STAT			=		(0)
CPU_EXT				=		(56)

CPU_BC				=		CPU_STAT + 0
CPU_DE				=		CPU_STAT + 2
CPU_HL				=		CPU_STAT + 4
CPU_IX				=		CPU_STAT + 6
CPU_IY				=		CPU_STAT + 8
_CPU_AF				=		CPU_STAT + 10			! shadow
_CPU_SP				=		CPU_STAT + 12			! shadow
_CPU_PC				=		CPU_STAT + 14			! shadow
CPU_AF2				=		CPU_STAT + 16
CPU_BC2				=		CPU_STAT + 18
CPU_DE2				=		CPU_STAT + 20
CPU_HL2				=		CPU_STAT + 22
_CPU_IFF			=		CPU_STAT + 24			! shadow
! CPU_I				=		CPU_STAT + 25
! CPU_IM			=		CPU_STAT + 26
! CPU_R1			=		CPU_STAT + 27
CPU_R2				=		CPU_STAT + 28
CPU_IRQ				=		CPU_STAT + 32
CPU_REQIRQ			=		CPU_STAT + 36
CPU_REMCLOCK		=		CPU_STAT + 40
CPU_BASECLOCK		=		CPU_STAT + 44
! cpu_clock			=		CPU_STAT + 48
! cpu_stat_size		=		52

CPU_C				=		CPU_STAT + 0
CPU_B				=		CPU_STAT + 1
CPU_E				=		CPU_STAT + 2
CPU_D				=		CPU_STAT + 3
CPU_L				=		CPU_STAT + 4
CPU_H				=		CPU_STAT + 5
CPU_IXL				=		CPU_STAT + 6
CPU_IXH				=		CPU_STAT + 7
CPU_IYL				=		CPU_STAT + 8
CPU_IYH				=		CPU_STAT + 9
! CPU_F				=		CPU_STAT + 10
! CPU_A				=		CPU_STAT + 11

CPU_MEMREAD			=		CPU_EXT + 0
CPU_MEMWRITE		=		CPU_EXT + 4
CPU_MEMBASE			=		CPU_EXT + 8
! cpu_ext_size		=		12

CPU_SZPFLAG			=		0
CPU_INCFLAG			=		256
CPU_DECFLAG			=		512


.macro Z80WORK	c
	add	#-\c,r14
.endm

.macro MEMRD8	r, m
	exts.w	\m,r0
	cmp/pz	r0
	bt	800000f
	mov	#(800001f - 800000f),r3
	braf	r3
	mov.l	@(CPU_MEMBASE,gbr),r0
800000:
	mov.l	@(CPU_MEMREAD,gbr),r0
800001:
	mov.b	@(r0,\m),\r
	extu.b	\r,\r
.endm

.macro MEMWR8	m, v
	exts.w	\m,r0
	cmp/pz	r0
	bt	800002f
	mov	#(800003f - 800002f),r3
	braf	r3
	mov.l	@(CPU_MEMBASE,gbr),r0
800002:
	mov.l	@(CPU_MEMWRITE,gbr),r0
800003:	
	mov.b	\v,@(r0,\m)
.endm

.macro MEMLEA8	m, s, d
	exts.w	\m,r0
	cmp/pz	r0
	bt	800004f
	mov.l	@(CPU_MEMBASE,gbr),r0
	mov	#(800005f - 800004f),r3
	mov	r0,\s
	braf	r3
	mov	r0,\d
800004:
	mov.l	@(CPU_MEMREAD,gbr),r0
	mov	r0,\s
	mov.l	@(CPU_MEMWRITE,gbr),r0
	mov	r0,\d
800005:
.endm

.macro GETPC8
	cmp/pz	r13
	mov	r13,r2
	bt/s	800006f
	shlr16	r2
	mov	#(800007f - 800006f),r3
	braf	r3
	mov.l	@(CPU_MEMBASE,gbr),r0
800006:	
	mov.l	@(CPU_MEMREAD,gbr),r0
800007:
	mov	#(0x10000 >> 16),r3
	mov.b	@(r0,r2),r4
	shll16	r3
	add	r3,r13
	extu.b	r4,r0
.endm
	
.macro GETPC8S
	cmp/pz	r13
	mov	r13,r2
	bt/s	800008f
	shlr16	r2
	mov	#(800009f - 800008f),r3
	braf	r3
	mov.l	@(CPU_MEMBASE,gbr),r0
800008:	
	mov.l	@(CPU_MEMREAD,gbr),r0
800009:
	mov	#(0x10000 >> 16),r3
	mov.b	@(r0,r2),r0
	shll16	r3
	add	r3,r13
.endm

.macro GETPC16
	mov	#(0x20000 >> 16),r0
	mov	r13,r4
	shll16	r0
	mov.l	10000f,r3
	shlr16	r4
	jsr	@r3
	add	r0,r13
.endm
	
.macro GETIPC	r, b
	cmp/pz	r13
	bt	800011f
	mov	#(800012f - 800011f),r3
	mov.l	@(CPU_MEMBASE,gbr),r0
	braf	r3
	mov	r0,r2
800011:
	mov.l	@(CPU_MEMREAD,gbr),r0
	mov	r0,r2
800012:
	mov.w	@(\b,gbr),r0
	extu.w	r0,r4
	mov	r13,r0
	shlr16	r0
	mov.b	@(r0,r2),r5
	mov	#(0x10000 >> 16),r3
	extu.b	r5,r5
	mov	r5,r0
	tst	#0x80,r0
	shll16	r3
	bt/s	800013f
	add	r3,r13
	swap.b	r5,r0
	or	#0xff,r0
	swap.b	r0,r5
800013:
	not	r3,r3
	add	r4,r5
	and	r5,r3
	mov	r3,\r
.endm
