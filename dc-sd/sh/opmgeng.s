
FMDIV_BITS		=		9
FMDIV_ENT		=		(1 << FMDIV_BITS)
FMVOL_SFTBIT	=		5

SIN_BITS		=		8
EVC_BITS		=		7
ENV_BITS		=		16
KF_BITS			=		6
FREQ_BITS		=		20
ENVTBL_BIT		=		14
SINTBL_BIT		=		14

TL_BITS			=		(FREQ_BITS + 2)
OPM_OUTSB		=		(TL_BITS + 2 - 16)

SIN_ENT			=		(1 << SIN_BITS)
EVC_ENT			=		(1 << EVC_BITS)

EC_ATTACK		=		0
EC_DECAY		=		(EVC_ENT << ENV_BITS)
EC_OFF			=		((2 * EVC_ENT) << ENV_BITS)

EM_ATTACK		=		4
EM_DECAY1		=		3
EM_DECAY2		=		2
EM_RELEASE		=		1
EM_OFF			=		0


! s_detune1			=		0
! s_detune2			=		4
S1_TOTALLEVEL		=		8
S1_DECAYLEVEL		=		12
! s_attack			=		16
! s_decay1			=		20
! s_decay2			=		24
! s_release			=		28
S1_FREQ_CNT			=		32
S1_FREQ_INC			=		36
! s_keyscale		=		40
! s_multiple		=		41
S1_ENV_MODE			=		42
! s_envraito		=		43
S1_ENV_CNT			=		44
S1_ENV_END			=		48
S1_ENV_INC			=		52
! s_env_inc_attack	=		56
S1_ENVINCDECAY1		=		60
S1_ENVINCDECAY2		=		64
! s_env_inc_release	=		68
S_SIZE				=		72

! c_algorithm		=		(S_SIZE * 4 + 0)
C_FEEDBACK			=		(S_SIZE * 4 + 1)
C_PLAYING			=		(S_SIZE * 4 + 2)
C_OUTSLOT			=		(S_SIZE * 4 + 3)
C_OP1FB				=		(S_SIZE * 4 + 4)
C_CONNECT1			=		(S_SIZE * 4 + 8)
C_CONNECT3			=		(S_SIZE * 4 + 12)
C_CONNECT2			=		(S_SIZE * 4 + 16)
C_CONNECT4			=		(S_SIZE * 4 + 20)
! c_keynote			=		(S_SIZE * 4 + 24)
! c_keyfunc			=		(S_SIZE * 4 + 28)
! c_kcode			=		(S_SIZE * 4 + 29)
! c_pan				=		(S_SIZE * 4 + 30)
! c_padding1		=		(S_SIZE * 4 + 31)
C_SIZE				=		(S_SIZE * 4 + 32)

G_PLAYING			=		0
! G_mode			=		4
G_FEEDBACK2			=		8
G_FEEDBACK3			=		12
G_FEEDBACK4			=		16
G_OUTDL				=		20
G_OUTDC				=		24
G_OUTDR				=		28
G_CALCREMAIN		=		32
! G_keyreg			=		36

T_ORG				=		0
T_CALC1024			=		(0 - T_ORG)
T_FMVOL				=		(4 - T_ORG)
T_sintable			=		(8 - T_ORG)
T_envtable			=		(8 - T_ORG + SIN_ENT * 4)
T_envcurve			=		(8 - T_ORG + SIN_ENT * 4 + EVC_ENT * 4)

	.extern	_opmch
	.extern	_opmgen
	.extern	_opmcfg

	.globl	_opmgen_getpcm

	.text
	.align	2

! r0	Temporary Register
! r1	Offset
! r2	Counter
! r3	Temporary Register
! r4	Temporary Register
! r5	channel counter
! r6	OPMCH
! r7	OPMCH base
! r8	L
! r9	R
! r10	opmgen Fix
! r11	opmcfg Fix
! r12	Temporary Register


.macro SLTFREQ	o, upd
	mov.w	100f,r0
	mov.l	@(r0,r10),r7
	mov.w	101f,r0
	mov.l	@(r0,r10),r8
	mov.w	102f,r0
	mov.l	@(r0,r10),r2
	add	r8,r7
	cmp/hs	r2,r7
	bf	2000f
	! cs
	bra	\upd
	nop

100:	 .word (\o + S1_ENV_INC)
101:	 .word (\o + S1_ENV_CNT)
102:	 .word (\o + S1_ENV_END)
2000:	
.endm

.macro SLTOUT	o, fd, cn
	mov	r7,r8
	shlr16	r8 ! ENV_BITS
	mov.w	100f,r0
	cmp/hs	r0,r8
	mov	r8,r2
	bt/s	00f
	sub	r0,r2
	! cc
	mov.w	101f,r0
	mov	r1,r2
	add	r0,r2 ! r12 = opmtbl.envcurve
	!cc
	mov	r8,r0
	shll2	r0
	mov.l	@(r0,r2),r2
00:
	mov.w	102f,r0
	mov.l	@(r0,r10),r4
	!
	mov.w	103f,r0
	mov.l	r7,@(r0,r10)
	mov.w	104f,r0
	mov.l	@(r0,r10),r8
	mov.w	105f,r0
	mov.l	@(r0,r10),r7 ! freq
	cmp/hi	r2,r4
	sub	r2,r4
	mov.l	@(\fd,r14),r2
	add	r8,r7
	mov.w	104f,r0
	! ls
	bf/s	2000f
	mov.l	r7,@(r0,r10)
	add	r2,r7
	shll2	r4
	add	r1,r4
	!!
	mov	#(32 - FREQ_BITS),r0
	shld	r0,r7
	mov	r1,r2
	add	#T_sintable,r2 ! r12 = opmtbl.sintable
	mov	#-(32 - SIN_BITS),r0
	shld	r0,r7
	mov.w	106f,r0
	mov.l	@(r0,r10),r8
	mov.w	107f,r0
	mov.l	@(r0,r4),r4
	mov	r7,r0
	shll2	r0
	mov.l	@(r0,r2),r7
	mul.l	r4,r7
	mov.l	@r8,r2
	mov	#-(ENVTBL_BIT + SINTBL_BIT - TL_BITS),r0
	sts	macl,r4
	shad	r0,r4
	add	r4,r2
	sts	macl,r4
	bra	2000f
	mov.l	r2,@r8
	
100:	.word	EVC_ENT
101:	.word	T_envcurve
102:	.word	(\o + S1_TOTALLEVEL)
103:	.word	(\o + S1_ENV_CNT)
104:	.word	(\o + S1_FREQ_CNT)
105:	.word	(\o + S1_FREQ_INC)
106:	.word	\cn
107:	.word	T_envtable
2000:	
.endm


.macro SLTUPD	r, o, m
	mov.w	100f,r0
	mov.b	@(r0,r10),r7
	extu.b	r7,r7
	add	#-1,r7
	mov	#EM_ATTACK,r0
	cmp/hs	r0,r7
	bt/s	55f	! EM_OFF
	mov	r7,r0
	cmp/eq	#0,r0
	bt/s	33f
	cmp/eq	#1,r0
	bt/s	44f
	cmp/eq	#2,r0
	bt	22f
11:	!.att
	mov.w	100f,r0
	mov.b	r7,@(r0,r10)
	mov.w	101f,r0
	mov.l	@(r0,r10),r4
	mov.w	102f,r0
	mov.l	@(r0,r10),r8
	mov.l	200f,r7
	mov.w	103f,r0
	mov.l	r4,@(r0,r10)
	mov.w	104f,r0
	bra	\r
	mov.l	r8,@(r0,r10)

100:	.word	(\o + S1_ENV_MODE)
101:	.word	(\o + S1_DECAYLEVEL)
102:	.word	(\o + S1_ENVINCDECAY1)
103:	.word	(\o + S1_ENV_END)
104:	.word	(\o + S1_ENV_INC)
	
	.align	2
200:	.long	EC_DECAY

22:	!.dc1
	mov.w	100f,r0
	mov.b	r7,@(r0,r10)
	mov.l	200f,r4
	mov.w	101f,r0
	mov.l	@(r0,r10),r8
	mov.w	102f,r0
	mov.l	@(r0,r10),r7
	mov.w	103f,r0
	mov.l	r4,@(r0,r10)
	mov.w	104f,r0
	bra	\r
	mov.l	r8,@(r0,r10)

100:	.word	(\o + S1_ENV_MODE)
101:	.word	(\o + S1_ENVINCDECAY2)
102:	.word	(\o + S1_DECAYLEVEL)
103:	.word	(\o + S1_ENV_END)
104:	.word	(\o + S1_ENV_INC)
	
	.align	2
200:	.long	EC_OFF

33:	!.rel
	mov.w	100f,r0
	mov.b	r7,@(r0,r10)
44:	!.dc2
	mov	r2,r7
	add	#1,r7
	mov.w	101f,r0
	mov.b	@(r0,r10),r8
	mov	#0,r4
	extu.b	r8,r8
	mov.w	102f,r0
	mov.l	r7,@(r0,r10)
	mov.w	103f,r0
	mov.l	r4,@(r0,r10)
	mov	#\m,r0
	and	r0,r8
	mov.w	101f,r0
	mov.l	200f,r7
	bra	\r
	mov.b	r8,@(r0,r10)

100:	.word	(\o + S1_ENV_MODE)
101:	.word	C_PLAYING
102:	.word	(\o + S1_ENV_END)
103:	.word	(\o + S1_ENV_INC)

55:	! EM_OFF
	mov.l	200f,r7
	bra	\r
	nop

	.align	2
200:	.long	EC_OFF
2000:	
.endm

_opmgen_getpcm:
	tst	r6,r6
	bt	.exit
	mov.l	dcd_opmgen,r2
	mov.l	@(G_PLAYING,r2),r7
	tst	r7,r7
	bt	.exit
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	mov.l	dcd_opmch,r11
	mov	r2,r14
	mov.l	dcd_opmcfg,r1
	mov.l	@(G_CALCREMAIN,r14),r3
	mov.l	@(G_OUTDL,r14),r7
	bra	getpcm_lp
	mov.l	@(G_OUTDR,r14),r8
.exit:
	rts
	nop
	
	.align	2
dcd_opmgen:	.long		_opmgen
dcd_opmch:	.long		_opmch
dcd_opmcfg:	.long		_opmcfg
getpcm_lp:
	neg	r3,r4
	mul.l	r7,r4
	mov	#0,r2
	mov	r11,r10
	mov.l	r2,@(G_OUTDL,r14)
	sts	macl,r12
	mul.l	r8,r4
	mov.l	r2,@(G_OUTDC,r14)
	mov.l	200f,r9
	mov.l	r2,@(G_OUTDR,r14)
	mov.w	100f,r0
	add	r0,r3
	bra	slotcalc_lp
	sts	macl,r13
mksmp_lp:
	mov	#0,r2
	mov	r11,r10
	mov.l	r2,@(G_OUTDL,r14)
	mov.l	r2,@(G_OUTDC,r14)
	mov.l	200f,r9
	mov.l	r2,@(G_OUTDR,r14)
slotcalc_lp:
	mov.w	101f,r0
	mov.b	@(r0,r10),r4
	mov.w	102f,r0
	extu.b	r4,r4
	mov.b	@(r0,r10),r2
	extu.b	r2,r2
	tst	r2,r4
	bf/s	1f
	add	#1,r9
	bra	slot5calc
	add	#-1,r9

100:	.word	FMDIV_ENT
101:	.word	C_PLAYING
102:	.word	C_OUTSLOT
	
	.align	2
200:	.long	(0 - (8 << 24))
1:
	mov	#0,r2
	mov.l	r2,@(G_FEEDBACK2,r14)
	mov.l	r2,@(G_FEEDBACK3,r14)
	mov.l	r2,@(G_FEEDBACK4,r14)

slot1calc:	SLTFREQ	0, slot1update
s1calcenv:
	mov	r7,r2
	shlr16	r2 ! ENV_BITS
	mov	#EVC_ENT,r0
	mov	r2,r8
	cmp/hs	r0,r2
	bt/s	1f
	sub	r0,r8
	! cc
	mov.w	100f,r0
	mov	r1,r8
	add	r0,r8 ! r4 = opntbl.envcurve
	! cc
	mov	r2,r0
	shll2	r0
	mov.l	@(r0,r8),r8
1:
	mov.l	@(S1_TOTALLEVEL,r10),r4
	mov.l	r7,@(S1_ENV_CNT,r10)
	mov.l	@(S1_FREQ_CNT,r10),r2
	mov.l	@(S1_FREQ_INC,r10),r7 ! freq
	cmp/hi	r8,r4
	sub	r8,r4
	mov.w	101f,r0
	mov.b	@(r0,r10),r8
	add	r2,r7
	extu.b	r8,r8
	bt/s	2f
	mov.l	r7,@(S1_FREQ_CNT,r10)
	! ls
	bra	slot2calc
	nop

100:	.word	T_envcurve
101:	.word	C_FEEDBACK
2:
	mov.w	100f,r0
	tst	r8,r8
	bt/s	3f
	mov.l	@(r0,r10),r2
	! ne
	extu.b	r8,r0
	lds	r2,macl
	neg	r0,r0
	shad	r0,r2
	add	r2,r7 ! back!
	sts	macl,r2
3:
	mov	r1,r8
	add	#T_sintable,r8  ! r1 = opntbl.sintable
	mov	#(32 - FREQ_BITS),r0
	shld	r0,r7
	shll2	r4
	add	r1,r4
	!!
	mov	#-(32 - SIN_BITS),r0
	shld	r0,r7
	mov.w	101f,r0
	mov.l	@(r0,r4),r4
	mov	r7,r0
	shll2	r0
	mov.l	@(r0,r8),r7
	mov.w	102f,r0
	mov.l	@(r0,r10),r8
	mul.l	r4,r7
	mov	#-(ENVTBL_BIT + SINTBL_BIT - TL_BITS),r0
	sts	macl,r4
	mov	r4,r7
	bt/s	4f
	shad	r0,r7
	! ne
	mov.w	100f,r0
	mov.l	r7,@(r0,r10)
	add	r2,r7
	mov	#-31,r0
	lds	r7,macl
	shad	r0,r7
	mov	r7,r0
	sts	macl,r7
	sub	r0,r7 ! adjust....
	shar	r7
4:
	tst	r8,r8
	bt	5f
	! ne
	mov.l	@r8,r4
	add	r7,r4
	bra	slot2calc
	mov.l	r4,@r8

100:	.word	C_OP1FB
101:	.word	T_envtable
102:	.word	C_CONNECT1
5:	! eq
	mov.l	r7,@(G_FEEDBACK2,r14)
	mov.l	r7,@(G_FEEDBACK3,r14)
	mov.l	r7,@(G_FEEDBACK4,r14)

slot2calc:	SLTFREQ	(S_SIZE * 1), slot2update
s2calcenv:	SLTOUT	(S_SIZE * 1), G_FEEDBACK2, C_CONNECT2

slot3calc:	SLTFREQ	(S_SIZE * 2), slot3update
s3calcenv:	SLTOUT	(S_SIZE * 2), G_FEEDBACK3, C_CONNECT3

slot4calc:	SLTFREQ	(S_SIZE * 3), slot4update
s4calcenv:	SLTOUT	(S_SIZE * 3), G_FEEDBACK4, C_CONNECT4

slot5calc:
	mov.l	200f,r0
	clrt
	addc	r0,r9
	mov.w	100f,r0
	bt/s	1f
	add	r0,r10
	bra	slotcalc_lp
	nop
		
100:	.word	C_SIZE

	.align	2
200:	.long	(1 << 24)	
1:
	mov.l	@(G_OUTDC,r14),r4
	mov.l	@(G_OUTDL,r14),r7
	mov.l	@(G_OUTDR,r14),r8
	mov.l	@(T_CALC1024,r1),r2
	cmp/gt	r2,r3
	add	r4,r7
	bf/s	2f
	sub	r2,r3
	mov	#-FMVOL_SFTBIT,r0
	shad	r0,r7
	mul.l	r7,r2
	add	r4,r8
	shad	r0,r8
	! gt
	sts	macl,r0
	mul.l	r8,r2
	add	r0,r12
	sts	macl,r0
	bra	mksmp_lp
	add	r0,r13
2:
	mov	#-FMVOL_SFTBIT,r0
	shad	r0,r7
	!!
	add	r3,r2
	! le
	mul.l	r7,r2
	add	r4,r8
	shad	r0,r8
	sts	macl,r0
	add	r0,r12
	mul.l	r8,r2
	!!
	mov.l	@(T_FMVOL,r1),r4
	sts	macl,r0
	add	r0,r13
	mov	#-FMDIV_BITS,r0
	shad	r0,r12
	shad	r0,r13
	mul.l	r12,r4
	mov.l	@r5,r2
	mov	#-(OPM_OUTSB + FMDIV_BITS + 1 + 6 - FMVOL_SFTBIT - 8),r0
	sts	macl,r12
	mul.l	r13,r4
	mov.l	@(4,r5),r4
	lds	r12,mach
	shad	r0,r12
	add	r12,r2
	sts	mach,r12
	sts	macl,r13
	shad	r0,r13
	add	r13,r4
	mov.l	r2,@r5
	add	#4,r5
	mov.l	r4,@r5
	dt	r6
	add	#4,r5
	bt/s	3f
	sts	macl,r13
	! ne
	bra	getpcm_lp
	nop
3:
	mov.l	r7,@(G_OUTDL,r14)
	mov.l	r8,@(G_OUTDR,r14)
	mov	r9,r0
	mov.l	r3,@(G_CALCREMAIN,r14)
	mov.b	r0,@(G_PLAYING,r14)
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts
	mov.l	@r15+,r8

slot1update:	SLTUPD	s1calcenv, (S_SIZE * 0), 0xfe
slot2update:	SLTUPD	s2calcenv, (S_SIZE * 1), 0xfd
slot3update:	SLTUPD	s3calcenv, (S_SIZE * 2), 0xfb
slot4update:	SLTUPD	s4calcenv, (S_SIZE * 3), 0xf7

	.end

