/*=============================================================================
	UnVcWin32.h: Unreal definitions for Visual C++ SP2 running under Win32.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

/*----------------------------------------------------------------------------
	Platform compiler definitions.
----------------------------------------------------------------------------*/

#define __WIN32__				1
#define __INTEL__				1
#define __INTEL_BYTE_ORDER__	1

/*----------------------------------------------------------------------------
	Platform specifics types and defines.
----------------------------------------------------------------------------*/

// Undo any Windows defines.
#undef BYTE
#undef WORD
#undef DWORD
#undef INT
#undef FLOAT
#undef MAXBYTE
#undef MAXWORD
#undef MAXDWORD
#undef MAXINT
#undef CDECL

// Make sure HANDLE is defined.
#ifndef _WINDOWS_
	#define HANDLE void*
	#define HINSTANCE void*
#endif

// Sizes.
enum {DEFAULT_ALIGNMENT = 8 }; // Default boundary to align memory allocations on.
enum {CACHE_LINE_SIZE   = 32}; // Cache line size.

// Optimization macros (preceeded by #pragma).
#define DISABLE_OPTIMIZATION optimize("",off)
#ifdef _DEBUG
	#define ENABLE_OPTIMIZATION  optimize("",off)
#else
	#define ENABLE_OPTIMIZATION  optimize("",on)
#endif

// Function type macros.
#define DLL_IMPORT	__declspec(dllimport)	/* Import function from DLL */
#define DLL_EXPORT  __declspec(dllexport)	/* Export function to DLL */
#define DLL_EXPORT_CLASS	__declspec(dllexport)	/* Export class to DLL */
#define VARARGS     __cdecl					/* Functions with variable arguments */
#define CDECL	    __cdecl					/* Standard C function */
#define STDCALL		__stdcall				/* Standard calling convention */
#define FORCEINLINE __forceinline			/* Force code to be inline */
#define ZEROARRAY                           /* Zero-length arrays in structs */

// Variable arguments.
#define GET_VARARGS(msg,len,lastarg,fmt) appGetVarArgs(msg,len,fmt)
#define GET_VARARGS_ANSI(msg,len,lastarg,fmt) appGetVarArgsAnsi(msg,len,fmt)

// Compiler name.
#ifdef _DEBUG
	#define COMPILER "Compiled with Visual C++ Debug"
#else
	#define COMPILER "Compiled with Visual C++"
#endif

// Unsigned base types.
typedef unsigned char		BYTE;		// 8-bit  unsigned.
typedef unsigned short		_WORD;		// 16-bit unsigned.
typedef unsigned long		DWORD;		// 32-bit unsigned.
typedef unsigned __int64	QWORD;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			SBYTE;		// 8-bit  signed.
typedef signed short		SWORD;		// 16-bit signed.
typedef signed int  		INT;		// 32-bit signed.
typedef signed __int64		SQWORD;		// 64-bit signed.

// Character types.
typedef char				ANSICHAR;	// An ANSI character.
typedef unsigned short      UNICHAR;	// A unicode character.
typedef unsigned char		ANSICHARU;	// An ANSI character.
typedef unsigned short      UNICHARU;	// A unicode character.

// Other base types.
typedef signed int			UBOOL;		// Boolean 0 (false) or 1 (true).
typedef float				FLOAT;		// 32-bit IEEE floating point.
typedef double				DOUBLE;		// 64-bit IEEE double.
typedef unsigned long       SIZE_T;     // Corresponds to C SIZE_T.

// Bitfield type.
typedef unsigned long       BITFIELD;	// For bitfields.

// Unwanted VC++ level 4 warnings to disable.
#pragma warning(disable : 4244) /* conversion to float, possible loss of data							*/
#pragma warning(disable : 4699) /* creating precompiled header											*/
#pragma warning(disable : 4200) /* Zero-length array item at end of structure, a VC-specific extension	*/
#pragma warning(disable : 4100) /* unreferenced formal parameter										*/
#pragma warning(disable : 4514) /* unreferenced inline function has been removed						*/
#pragma warning(disable : 4201) /* nonstandard extension used : nameless struct/union					*/
#pragma warning(disable : 4710) /* inline function not expanded											*/
#pragma warning(disable : 4714) /* __forceinline function not expanded									*/  
#pragma warning(disable : 4702) /* unreachable code in inline expanded function							*/
#pragma warning(disable : 4711) /* function selected for autmatic inlining								*/
#pragma warning(disable : 4725) /* Pentium fdiv bug														*/
#pragma warning(disable : 4127) /* Conditional expression is constant									*/
#pragma warning(disable : 4512) /* assignment operator could not be generated                           */
#pragma warning(disable : 4530) /* C++ exception handler used, but unwind semantics are not enabled     */
#pragma warning(disable : 4245) /* conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch */
#pragma warning(disable : 4389) /* signed/unsigned mismatch (gam)                                       */
#pragma warning(disable : 4238) /* nonstandard extension used : class rvalue used as lvalue             */
#pragma warning(disable : 4251) /* needs to have dll-interface to be used by clients of class 'ULinker' */
#pragma warning(disable : 4275) /* non dll-interface class used as base for dll-interface class         */
#pragma warning(disable : 4511) /* copy constructor could not be generated                              */
#pragma warning(disable : 4284) /* return type is not a UDT or reference to a UDT                       */
#pragma warning(disable : 4355) /* this used in base initializer list                                   */
#pragma warning(disable : 4097) /* typedef-name '' used as synonym for class-name ''                    */
#pragma warning(disable : 4291) /* typedef-name '' used as synonym for class-name ''                    */
#pragma warning(disable : 4731) /* frame pointer register 'ebp' modified by inline assembly code        */
#pragma warning(disable : 4718) /* recursive call has no side effects, deleting (gam)                   */

// If C++ exception handling is disabled, force guarding to be off.
#ifndef _CPPUNWIND
	#error "Bad VCC option: C++ exception handling must be enabled"
#endif

// Make sure characters are unsigned.
#ifdef _CHAR_UNSIGNED
	#error "Bad VC++ option: Characters must be signed"
#endif

// No asm if not compiling for x86.
#if !(defined _M_IX86)
	#undef ASM
	#define ASM 0
#endif

// Strings.
#define LINE_TERMINATOR TEXT("\r\n")
#define LINE_TERMINATOR_INLINE \r\n
#define PATH_SEPARATOR TEXT("\\")

// DLL file extension.
#define DLLEXT TEXT(".dll")

// Pathnames.
#define PATH(s) s

// NULL.
#define NULL 0

// Package implementation.
#ifdef __STATIC_LINK
#define IMPLEMENT_PACKAGE_PLATFORM(pkgname)
#else
#define IMPLEMENT_PACKAGE_PLATFORM(pkgname) \
	extern "C" {HINSTANCE hInstance;} \
	INT DLL_EXPORT STDCALL DllMain( HINSTANCE hInInstance, DWORD Reason, void* Reserved ) \
	{ hInstance = hInInstance; return 1; }
#endif

// Platform support options.
#define PLATFORM_NEEDS_ARRAY_NEW 1
#define FORCE_ANSI_LOG           1

// OS unicode function calling.
#if defined(NO_UNICODE_OS_SUPPORT) || !defined(UNICODE)
	#define TCHAR_CALL_OS(funcW,funcA) (funcA)
	#define TCHAR_TO_ANSI(str) str
	#define ANSI_TO_TCHAR(str) str
	inline CORE_API TCHAR* winAnsiToTCHAR( char* str ) { return str; }
#elif defined(NO_ANSI_OS_SUPPORT)
	#define TCHAR_CALL_OS(funcW,funcA) (funcW)
	#define TCHAR_TO_ANSI(str) str
	#define ANSI_TO_TCHAR(str) str
	inline CORE_API TCHAR* winAnsiToTCHAR( char* str ) { return str; }
#else
	CORE_API ANSICHAR* winToANSI( ANSICHAR* ACh, const UNICHAR* InUCh, INT Count );
	CORE_API INT winGetSizeANSI( const UNICHAR* InUCh );
	CORE_API UNICHAR* winToUNICODE( UNICHAR* Ch, const ANSICHAR* InUCh, INT Count );
	CORE_API INT winGetSizeUNICODE( const ANSICHAR* InACh );
	#define TCHAR_CALL_OS(funcW,funcA) (GUnicodeOS ? (funcW) : (funcA))
	#define TCHAR_TO_ANSI(str) winToANSI((ANSICHAR*)appAlloca(winGetSizeANSI(str)),str,winGetSizeANSI(str))
	#define TCHAR_TO_OEM(str) winToOEM((ANSICHAR*)appAlloca(winGetSizeANSI(str)),str,winGetSizeANSI(str))
	#define ANSI_TO_TCHAR(str) winToUNICODE((TCHAR*)appAlloca(winGetSizeUNICODE(str)),str,winGetSizeUNICODE(str))
	inline CORE_API UNICHAR* winAnsiToTCHAR( char* str );
#endif

// Bitfield alignment.
#define GCC_PACK(n)

/*----------------------------------------------------------------------------
	Globals.
----------------------------------------------------------------------------*/

// System identification.
extern "C"
{
	extern HINSTANCE      hInstance;
	extern CORE_API UBOOL GIsMMX;
	extern CORE_API UBOOL GIsSSE;
	extern CORE_API UBOOL GIsPentiumPro;
	extern CORE_API UBOOL GIsK6;
	extern CORE_API UBOOL GIs3DNow;
}

/*----------------------------------------------------------------------------
	Functions.
----------------------------------------------------------------------------*/

//
// JP: Commented out these functions as they don't work reliably for some numbers
// eg appFloor(113) returns 112!
// This is due to the rounding for multiples of 0.5 depending on other bits of the number!
//
/*
//
// Round a floating point number to an integer.
// Note that (int+.5) is rounded to (int+1).
//
#if ASM || (defined _XBOX)
#define DEFINED_appRound 1
inline INT appRound( FLOAT F )
{
	INT I;
	__asm fld [F]
	__asm fistp [I]
	return I;
}
#endif

//
// Converts to integer equal to or less than.
//
#if ASM || (defined _XBOX)
#define DEFINED_appFloor 1
inline INT appFloor( FLOAT F )
{
	static FLOAT Half=0.5;
	INT I;
	__asm fld [F]
	__asm fsub [Half]
	__asm fistp [I]
	return I;
}
#endif
*/


// sjs ---
typedef union _LARGEINT // a win32 LARGE_INTEGER
{ 
    struct
    {
        DWORD LowPart; 
        DWORD HighPart; 
    };
    QWORD QuadPart;
} LARGEINT; 

#if ASM || (defined _XBOX)
#pragma warning (push)
#pragma warning (disable : 4035)
//extern CORE_API QWORD GBaseCyles;
#define DEFINED_appResetTimer 1
inline void appResetTimer()
{
	//LARGEINT li;
	//__asm
	//{
	//	xor   eax,eax	// Required so that VC++ realizes EAX is modified.
	//	xor   edx,edx	// Required so that VC++ realizes EDX is modified.
	//	_emit 0x0F		// RDTSC  -  Pentium+ time stamp register to EDX:EAX.
	//	_emit 0x31		// Use only 32 bits in EAX - even a Ghz cpu would have a 4+ sec period.
	//	mov   [li.LowPart],eax   // Save low value.
	//	mov   [li.HighPart],edx   // Save high value.
	//}
    //GBaseCyles = li.QuadPart;
	int SimulateCrash = *(int*)NULL;
}
#pragma warning (pop)
#endif
// --- sjs

//
// CPU cycles, related to GSecondsPerCycle.
//
#if ASM
#define DEFINED_appCycles 1
#pragma warning (push)
#pragma warning (disable : 4035)
#pragma warning (disable : 4715)
inline DWORD appCycles()
{
	__asm
	{
		rdtsc				// RDTSC  -  Pentium+ time stamp register to EDX:EAX
							// Use only 32 bits in EAX - a Ghz cpu has a 4+ sec period.
	}
}
#pragma warning (pop)
#endif

//
// Seconds, arbitrarily based.
//
#if ASM || (defined _XBOX)
#define DEFINED_appSeconds 1
#pragma warning (push)
#pragma warning (disable : 4035)
extern CORE_API DOUBLE GSecondsPerCycle;
CORE_API DOUBLE appSecondsSlow();
inline DOUBLE appSeconds()
{
    LARGEINT cur; // sjs DWORD L,H;
	__asm
	{
		xor   eax,eax	// Required so that VC++ realizes EAX is modified.
		xor   edx,edx	// Required so that VC++ realizes EDX is modified.
		_emit 0x0F		// RDTSC  -  Pentium+ time stamp register to EDX:EAX.
		_emit 0x31		// Use only 32 bits in EAX - even a Ghz cpu would have a 4+ sec period.
		mov   [cur.LowPart],eax   // Save low value. // sjs
		mov   [cur.HighPart],edx   // Save high value. // sjs
	}
	// sjs rem'd return ((DOUBLE)L +  4294967296.0 * (DOUBLE)H) * GSecondsPerCycle;
    //QWORD delta = cur.QuadPart - GBaseCyles;
	//return (double)delta * GSecondsPerCycle;
	return ((DOUBLE)cur.LowPart + 4294967296.0 * (DOUBLE)cur.HighPart) * GSecondsPerCycle;
}

CORE_API QWORD appCyclesQPC();

#pragma warning (pop)
#endif

//
// Memory copy.
//
#if ASM
#define DEFINED_appMemcpy
/******************************************************************************

 Copyright (c) 2001 Advanced Micro Devices, Inc.

 LIMITATION OF LIABILITY:  THE MATERIALS ARE PROVIDED *AS IS* WITHOUT ANY
 EXPRESS OR IMPLIED WARRANTY OF ANY KIND INCLUDING WARRANTIES OF MERCHANTABILITY,
 NONINFRINGEMENT OF THIRD-PARTY INTELLECTUAL PROPERTY, OR FITNESS FOR ANY
 PARTICULAR PURPOSE.  IN NO EVENT SHALL AMD OR ITS SUPPLIERS BE LIABLE FOR ANY
 DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS,
 BUSINESS INTERRUPTION, LOSS OF INFORMATION) ARISING OUT OF THE USE OF OR
 INABILITY TO USE THE MATERIALS, EVEN IF AMD HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE EXCLUSION OR LIMITATION
 OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE LIMITATION MAY
 NOT APPLY TO YOU.

 AMD does not assume any responsibility for any errors which may appear in the
 Materials nor any responsibility to support or update the Materials.  AMD retains
 the right to make changes to its test specifications at any time, without notice.

 NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 further information, software, technical information, know-how, or show-how
 available to you.

 So that all may benefit from your experience, please report  any  problems
 or  suggestions about this software to 3dsdk.support@amd.com

 AMD Developer Technologies, M/S 585
 Advanced Micro Devices, Inc.
 5900 E. Ben White Blvd.
 Austin, TX 78741
 3dsdk.support@amd.com
******************************************************************************/

/*****************************************************************************
MEMCPY_AMD.CPP
******************************************************************************/

// Very optimized memcpy() routine for AMD Athlon and Duron family.
// This code uses any of FOUR different basic copy methods, depending
// on the transfer size.
// NOTE:  Since this code uses MOVNTQ (also known as "Non-Temporal MOV" or
// "Streaming Store"), and also uses the software prefetch instructions,
// be sure you're running on Athlon/Duron or other recent CPU before calling!

#define TINY_BLOCK_COPY 64       // upper limit for movsd type copy
// The smallest copy uses the X86 "movsd" instruction, in an optimized
// form which is an "unrolled loop".

#define IN_CACHE_COPY 64 * 1024  // upper limit for movq/movq copy w/SW prefetch
// Next is a copy that uses the MMX registers to copy 8 bytes at a time,
// also using the "unrolled loop" optimization.   This code uses
// the software prefetch instruction to get the data into the cache.

#define UNCACHED_COPY 197 * 1024 // upper limit for movq/movntq w/SW prefetch
// For larger blocks, which will spill beyond the cache, it's faster to
// use the Streaming Store instruction MOVNTQ.   This write instruction
// bypasses the cache and writes straight to main memory.  This code also
// uses the software prefetch instruction to pre-read the data.
// USE 64 * 1024 FOR THIS VALUE IF YOU'RE ALWAYS FILLING A "CLEAN CACHE"

#define BLOCK_PREFETCH_COPY  infinity // no limit for movq/movntq w/block prefetch 
#define CACHEBLOCK 80h // number of 64-byte blocks (cache lines) for block prefetch
// For the largest size blocks, a special technique called Block Prefetch
// can be used to accelerate the read operations.   Block Prefetch reads
// one address per cache line, for a series of cache lines, in a short loop.
// This is faster than using software prefetch.  The technique is great for
// getting maximum read bandwidth, especially in DDR memory systems.

// Inline assembly syntax for use with Visual C++
inline void appMemcpy( void* Dst, const void* Src, INT Count )
{	
	if( GIsSSE )
	__asm 
	{
		mov		ecx, [Count]	; number of bytes to copy
		mov		edi, [Dst]		; destination
		mov		esi, [Src]		; source
		mov		ebx, ecx		; keep a copy of count

		cld
		cmp		ecx, TINY_BLOCK_COPY
		jb		$memcpy_ic_3	; tiny? skip mmx copy

		cmp		ecx, 32*1024		; don't align between 32k-64k because
		jbe		$memcpy_do_align	;  it appears to be slower
		cmp		ecx, 64*1024
		jbe		$memcpy_align_done
	$memcpy_do_align:
		mov		ecx, 8			; a trick that's faster than rep movsb...
		sub		ecx, edi		; align destination to qword
		and		ecx, 111b		; get the low bits
		sub		ebx, ecx		; update copy count
		neg		ecx				; set up to jump into the array
		add		ecx, offset $memcpy_align_done
		jmp		ecx				; jump to array of movsb's

	align 4
		movsb
		movsb
		movsb
		movsb
		movsb
		movsb
		movsb
		movsb

	$memcpy_align_done:			; destination is dword aligned
		mov		ecx, ebx		; number of bytes left to copy
		shr		ecx, 6			; get 64-byte block count
		jz		$memcpy_ic_2	; finish the last few bytes

		cmp		ecx, IN_CACHE_COPY/64	; too big 4 cache? use uncached copy
		jae		$memcpy_uc_test

	// This is small block copy that uses the MMX registers to copy 8 bytes
	// at a time.  It uses the "unrolled loop" optimization, and also uses
	// the software prefetch instruction to get the data into the cache.
	align 16
	$memcpy_ic_1:			; 64-byte block copies, in-cache copy

		prefetchnta [esi + (200*64/34+192)]		; start reading ahead

		movq	mm0, [esi+0]	; read 64 bits
		movq	mm1, [esi+8]
		movq	[edi+0], mm0	; write 64 bits
		movq	[edi+8], mm1	;    note:  the normal movq writes the
		movq	mm2, [esi+16]	;    data to cache; a cache line will be
		movq	mm3, [esi+24]	;    allocated as needed, to store the data
		movq	[edi+16], mm2
		movq	[edi+24], mm3
		movq	mm0, [esi+32]
		movq	mm1, [esi+40]
		movq	[edi+32], mm0
		movq	[edi+40], mm1
		movq	mm2, [esi+48]
		movq	mm3, [esi+56]
		movq	[edi+48], mm2
		movq	[edi+56], mm3

		add		esi, 64			; update source pointer
		add		edi, 64			; update destination pointer
		dec		ecx				; count down
		jnz		$memcpy_ic_1	; last 64-byte block?

	$memcpy_ic_2:
		mov		ecx, ebx		; has valid low 6 bits of the byte count
	$memcpy_ic_3:
		shr		ecx, 2			; dword count
		and		ecx, 1111b		; only look at the "remainder" bits
		neg		ecx				; set up to jump into the array
		add		ecx, offset $memcpy_last_few
		jmp		ecx				; jump to array of movsd's

	$memcpy_uc_test:
		cmp		ecx, UNCACHED_COPY/64	; big enough? use block prefetch copy
		jae		$memcpy_bp_1

	$memcpy_64_test:
		or		ecx, ecx		; tail end of block prefetch will jump here
		jz		$memcpy_ic_2	; no more 64-byte blocks left

	// For larger blocks, which will spill beyond the cache, it's faster to
	// use the Streaming Store instruction MOVNTQ.   This write instruction
	// bypasses the cache and writes straight to main memory.  This code also
	// uses the software prefetch instruction to pre-read the data.
	align 16
	$memcpy_uc_1:				; 64-byte blocks, uncached copy

		prefetchnta [esi + (200*64/34+192)]		; start reading ahead

		movq	mm0,[esi+0]		; read 64 bits
		add		edi,64			; update destination pointer
		movq	mm1,[esi+8]
		add		esi,64			; update source pointer
		movq	mm2,[esi-48]
		movntq	[edi-64], mm0	; write 64 bits, bypassing the cache
		movq	mm0,[esi-40]	;    note: movntq also prevents the CPU
		movntq	[edi-56], mm1	;    from READING the destination address
		movq	mm1,[esi-32]	;    into the cache, only to be over-written
		movntq	[edi-48], mm2	;    so that also helps performance
		movq	mm2,[esi-24]
		movntq	[edi-40], mm0
		movq	mm0,[esi-16]
		movntq	[edi-32], mm1
		movq	mm1,[esi-8]
		movntq	[edi-24], mm2
		movntq	[edi-16], mm0
		dec		ecx
		movntq	[edi-8], mm1
		jnz		$memcpy_uc_1	; last 64-byte block?

		jmp		$memcpy_ic_2		; almost done

	// For the largest size blocks, a special technique called Block Prefetch
	// can be used to accelerate the read operations.   Block Prefetch reads
	// one address per cache line, for a series of cache lines, in a short loop.
	// This is faster than using software prefetch.  The technique is great for
	// getting maximum read bandwidth, especially in DDR memory systems.
	$memcpy_bp_1:			; large blocks, block prefetch copy

		cmp		ecx, CACHEBLOCK			; big enough to run another prefetch loop?
		jl		$memcpy_64_test			; no, back to regular uncached copy

		mov		eax, CACHEBLOCK / 2		; block prefetch loop, unrolled 2X
		add		esi, CACHEBLOCK * 64	; move to the top of the block
	align 16
	$memcpy_bp_2:
		mov		edx, [esi-64]		; grab one address per cache line
		mov		edx, [esi-128]		; grab one address per cache line
		sub		esi, 128			; go reverse order to suppress HW prefetcher
		dec		eax					; count down the cache lines
		jnz		$memcpy_bp_2		; keep grabbing more lines into cache

		mov		eax, CACHEBLOCK		; now that it's in cache, do the copy
	align 16
	$memcpy_bp_3:
		movq	mm0, [esi   ]		; read 64 bits
		movq	mm1, [esi+ 8]
		movq	mm2, [esi+16]
		movq	mm3, [esi+24]
		movq	mm4, [esi+32]
		movq	mm5, [esi+40]
		movq	mm6, [esi+48]
		movq	mm7, [esi+56]
		add		esi, 64				; update source pointer
		movntq	[edi   ], mm0		; write 64 bits, bypassing cache
		movntq	[edi+ 8], mm1		;    note: movntq also prevents the CPU
		movntq	[edi+16], mm2		;    from READING the destination address 
		movntq	[edi+24], mm3		;    into the cache, only to be over-written,
		movntq	[edi+32], mm4		;    so that also helps performance
		movntq	[edi+40], mm5
		movntq	[edi+48], mm6
		movntq	[edi+56], mm7
		add		edi, 64				; update dest pointer

		dec		eax					; count down

		jnz		$memcpy_bp_3		; keep copying
		sub		ecx, CACHEBLOCK		; update the 64-byte block count
		jmp		$memcpy_bp_1		; keep processing chunks

	// The smallest copy uses the X86 "movsd" instruction, in an optimized
	// form which is an "unrolled loop".   Then it handles the last few bytes.
	align 4
		movsd
		movsd			; perform last 1-15 dword copies
		movsd
		movsd
		movsd
		movsd
		movsd
		movsd
		movsd
		movsd			; perform last 1-7 dword copies
		movsd
		movsd
		movsd
		movsd
		movsd
		movsd

	$memcpy_last_few:		; dword aligned from before movsd's
		mov		ecx, ebx	; has valid low 2 bits of the byte count
		and		ecx, 11b	; the last few cows must come home
		jz		$memcpy_final	; no more, let's leave
		rep		movsb		; the last 1, 2, or 3 bytes

	$memcpy_final: 
		emms				; clean up the MMX state
		sfence				; flush the write buffer
	//	mov		eax, [dest]	; ret value = destination pointer
	}
	else
	__asm
	{
		mov		ecx, Count
		mov		esi, Src
		mov		edi, Dst
		mov     ebx, ecx
		shr     ecx, 2
		and     ebx, 3
		rep     movsd
		mov     ecx, ebx
		rep     movsb
	}
}
#endif

//
// Memory zero.
//
#if ASM
#define DEFINED_appMemzero
inline void appMemzero( void* Dest, INT Count )
{	
	__asm
	{
		mov		ecx, [Count]
		mov		edi, [Dest]
		xor     eax, eax
		mov		ebx, ecx
		shr		ecx, 2
		and		ebx, 3
		rep     stosd
		mov     ecx, ebx
		rep     stosb
	}
}
#endif

#if ASM3DNOW
inline void DoFemms()
{
	__asm _emit 0x0f
	__asm _emit 0x0e
}
#endif

extern "C" void* __cdecl _alloca(size_t);
//#define appAlloca(size) _alloca((size+7)&~7)
#define appAlloca(size) ((size==0) ? 0 : _alloca((size+7)&~7))

/*----------------------------------------------------------------------------
	The End.
-------------
---------------------------------------------------------------*/
