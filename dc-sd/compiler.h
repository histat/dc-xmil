#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ronin/common.h>
#include <ronin/report.h>
#include "ff.h"
#include "dccore.h"

#define __SDCARD__

#define	BYTESEX_LITTLE
#define	OSLANG_SJIS
#define	OSLINEBREAK_CRLF

typedef signed int	SINT;
//typedef	unsigned int	UINT;
typedef signed char	SINT8;
typedef unsigned char	UINT8;
typedef	signed short	SINT16;
typedef	unsigned short	UINT16;
typedef	signed int	SINT32;
typedef	unsigned int	UINT32;
typedef signed long long	SINT64;
typedef unsigned long long	UINT64;

#include "integer.h"

#define	INLINE		inline

#ifndef	max
#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#endif
#ifndef	min
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#ifndef	ZeroMemory
#define	ZeroMemory(d,n)		memset((d), 0, (n))
#endif
#ifndef	CopyMemory
#define	CopyMemory(d,s,n)	memcpy((d),(s),(n))
#endif
#ifndef	FillMemory
#define	FillMemory(a, b, c)	memset((a),(c),(b))
#endif

#define	TRUE	1
#define	FALSE	0

#define	MAX_PATH	255

#define	BRESULT				UINT
#define	OEMCHAR				char
#define	OEMTEXT(string)		string
#define	OEMSPRINTF			sprintf
#define	OEMSTRLEN			strlen

#include "common.h"
#include "milstr.h"
#include "_memory.h"
#include "lstarray.h"
#include "trace.h"
#include "rect.h"

#define	GETTICK()			dccore_gettick()
#define	__ASSERT(s)
#define	SPRINTF				sprintf
#define	STRLEN				strlen
#define Sleep(ms)

#if !defined(UNICODE)
#define	SUPPORT_SJIS
#else
#define	SUPPORT_ANK
#endif

// FIXME: this allows assembler sound even on Dreammcast
#define _WIN32_WCE

#define	SUPPORT_8BPP
//#define	SUPPORT_16BPP
#define	MEMOPTIMIZE		2
#define	SUPPORT_OPM
#define	SUPPORT_X1F

