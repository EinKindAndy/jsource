/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Global Definitions                                                      */


#ifndef SYS // include js.h only once - dtoa.c
#include "js.h"
#endif

// If you are porting to a new compiler or architecture, see the bottom of this file
// for instructions on defining the CTTZ macros

#if SY_WINCE
#include "..\cesrc\cecompat.h"
#endif

#if (SYS & SYS_PCWIN)
#define HEAPCHECK  heapcheck()
#else
#define HEAPCHECK
#endif

#if (SYS & SYS_ATARIST)
#define __NO_INLINE__           1
#endif

#if (SYS & SYS_UNIX - SYS_SGI)
#include <memory.h>
#include <sys/types.h>
#endif

#if 1
#include <stdint.h>
#include <float.h>
#include <limits.h>
#define link unused_syscall_link
#define qdiv unused_netbsd_qdiv
#include <stdlib.h>
#undef link
#undef qdiv
#else
#define const /*nothing*/   /* blame rx.h */
#endif

#if ! SY_WINCE
#include <errno.h>
#include <stdio.h>
#endif

#include <math.h>
#include <string.h>  

#ifdef ANDROID
#include <android/log.h>
#define logcat_d(msg) __android_log_write(ANDROID_LOG_DEBUG,(const char*)"libj",msg)
#endif

#if SY_64
#define IMAX            9223372036854775807LL
#define IMIN            (~9223372036854775807LL)   /* ANSI C LONG_MIN is  -LONG_MAX */
#define FMTI            "%lli"
#define FMTI02          "%02lli"
#define FMTI04          "%04lli"
#define FMTI05          "%05lli"

#if defined(_MSC_VER)  // SY_WIN32
#define strtoI         _strtoi64
#else
#define strtoI          strtoll
#endif

#else
#define IMAX            2147483647L
#define IMIN            (~2147483647L)   /* ANSI C LONG_MIN is  -LONG_MAX */
#define FMTI            "%li"
#define FMTI02          "%02li"
#define FMTI04          "%04li"
#define FMTI05          "%05li"
#define strtoI          strtol
#endif

#define NEGATIVE0       0x8000000000000000LL   // IEEE -0 (double precision)

#define C4MAX           0xffffffffUL
#define C4MIN           0L

#if (SYS & SYS_AMIGA)
#define XINF            "\177\377\000\000\000\000\000\000"
#define XNAN            "\177\361\000\000\000\000\000\000"
#endif

#if (SYS & SYS_ARCHIMEDES)
#define XINF            "\000\000\360\177\000\000\000\000"
#define XNAN            "\000\000\370\377\000\000\000\000"
#endif

#if (SYS & SYS_DEC5500) || SY_WINCE_SH
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\000\000\000\000\000\000\370\377"
#endif

#if (SYS & SYS_MACINTOSH)
/* for old versions of ThinkC */
/* #define XINF         "\177\377\000\000\000\000\000\000\000\000\000\000" */
/* #define XNAN         "\377\377\100\000\100\000\000\000\000\000\000\000" */
/* for ThinkC 7.0 or later */
#define XINF            "\177\377\177\377\000\000\000\000\000\000\000\000"
#define XNAN            "\377\377\377\377\100\000\000\000\000\000\000\000"
#endif

#if (SYS & SYS_SUN4+SYS_SUNSOL2)
#define XINF            "\177\360\000\000\000\000\000\000"
#define XNAN            "\177\377\377\377\377\377\377\377"
#endif

#if (SYS & SYS_VAX)
#define XINF            "\377\177\377\377\377\377\377\377"
#define XNAN            "\377\177\377\377\377\377\377\376" /* not right */
#endif

#if (SY_WINCE_MIPS || SY_WINCE_SH)
#if WIN32_PLATFORM_PSPC
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\377\377\377\377\377\377\367\177"
#else
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\001\000\000\000\000\000\360\177"
#endif
#endif

#if SY_WINCE_ARM
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\000\000\000\000\000\000\370\177"
#endif

#if C_LE
#ifndef XINF
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\000\000\000\000\000\000\370\377"
#endif
#endif

#ifndef XINF
#define XINF            "\177\360\000\000\000\000\000\000"
#define XNAN            "\177\370\000\000\000\000\000\000"
#endif


#ifndef PI
#define PI              ((D)3.14159265358979323846)
#endif
#define P2              ((D)6.28318530717958647693)
#ifndef OVERFLOW
#define OVERFLOW        ((D)8.988465674311578e307)
#endif
#ifndef UNDERFLOW
#define UNDERFLOW       ((D)4.450147717014403e-308)
#endif
// RESTRICT causes the compiler to generate better code by assuming no overlap of regions pointed to by pointers
// We use RESTRICT for routines that operate in-place on an argument.  This is strictly speaking a violation of the rule,
// but normally something like *z = *x + *y will not cause trouble because there is no reason to refetch an input after
// the result has been written.  On 32-bit machines, registers are so short that sometimes the compilers refetch an input
// after writing to *z, so we don't turn RESTRICT on for 32-bit
#if defined(_MSC_VER)  // SY_WIN32
// RESTRICT is an attribute of a pointer, and indicates that no other pointer points to the same area
#define RESTRICT __restrict
// RESTRICTF is an attribute of a function, and indicates that the object returned by the function is not aliased with any other object
#define RESTRICTF __declspec(restrict)
#define PREFETCH(x) _mm_prefetch((x),_MM_HINT_T0)
#define PREFETCH2(x) _mm_prefetch((x),_MM_HINT_T1)   // prefetch into L2 cache but not L1
#endif
#ifdef __GNUC__
#define RESTRICT __restrict
// No RESTRICTF on GCC
#define PREFETCH(x) __builtin_prefetch(x)
#define PREFETCH2(x) __builtin_prefetch((x),0,2)   // prefetch into L2 cache but not L1
#endif

#ifdef __MINGW32__
#ifndef _SW_INVALID
#define _SW_INVALID    0x00000010 /* invalid */
#endif
#ifndef _EM_ZERODIVIDE
#define _EM_ZERODIVIDE  0x00000008
#endif
#define EM_INVALID    _SW_INVALID
#define EM_ZERODIVIDE _EM_ZERODIVIDE
#if defined(__STRICT_ANSI__)
extern int __cdecl _isnan (double);
extern unsigned int __cdecl _clearfp (void);
#endif
#ifndef _MAX_PATH
#define _MAX_PATH  (260)
#endif
#endif

#if SY_64
#if defined(_MSC_VER)  // SY_WIN32
// RESTRICTI (for in-place) is used for things like *z++=*x++ - *y++;  Normally you wouldn't store to a z unless you were done reading
// the x and y, so it would be safe to get the faster loop that RESTRICT generates, even though strictly speaking if x or y is the
// same address as z the terms of the RESTRICT are violated.  But on 32-bit machines, registers are so tight that sometimes *z is used
// as a temp, which means we can't take the liberties there
#define RESTRICTI __restrict
#endif
#ifdef __GNUC__
#define RESTRICTI __restrict
#endif
#endif  // SY_64

#ifndef RESTRICT
#define RESTRICT
#endif
#ifndef RESTRICTF
#define RESTRICTF
#endif
#ifndef RESTRICTI
#define RESTRICTI
#endif
// If PREFETCH is not defined, we won't generate prefetch instrs

// If the user switch C_NOMULTINTRINSIC is defined, suppress using it
#ifdef C_NOMULTINTRINSIC
#define C_USEMULTINTRINSIC 0
#else
#define C_USEMULTINTRINSIC 1
#endif

// disable C_USEMULTINTRINSIC if un-available
#if C_USEMULTINTRINSIC
#if !defined(_MSC_VER)
#if defined(__clang__)
#if !__has_builtin(__builtin_smull_overflow)
#undef C_USEMULTINTRINSIC
#define C_USEMULTINTRINSIC 0
#endif
#elif __GNUC__ < 5
#undef C_USEMULTINTRINSIC
#define C_USEMULTINTRINSIC 0
#endif
#endif
#endif

#if C_AVX
#include <immintrin.h>
#endif

#if defined(__aarch64__)
#include <arm_neon.h>
#endif

/* msvc does not define __SSE2__ */
#if !defined(__SSE2__)
#if defined(_MSC_VER)
#if _M_IX86_FP==2
#define __SSE2__ 1
#include <xmmintrin.h>   /* header file for _mm_prefetch() */
#endif
#endif
#endif

#define NALP            256             /* size of alphabet                */
#define NETX            2000            /* size of error display buffer    */
#define NPP             20              /* max value for quad pp           */
#define RMAXX           16              // number of bits in rank
#define RMAX            ((1LL<<RMAXX)-1)   // max rank
#define NPATH           1024            /* max length for path names,      */
                                        /* including trailing 0 byte       */
#define NFDEP           (8000L+12000L*SY_64)             // fn call depth
#define NFCALL          (MAX(40,NFDEP/10)) // call depth for named calls - can be expensive

#define NTSTACK         16384L          // number of BYTES in an allocated block of tstack - pointers to allocated blocks

// modes for indexofsub()
#define IIOPMSK         0xf     // operation bits
#define IIDOT           0        // IIDOT and IICO must be 0-1
#define IICO            1
#define INUBSV          2
#define INUB            3
#define ILESS           4
#define INUBI           5
#define IEPS            6
// the I...EPS values below are wired into the function table at the end of vcompsc.c
#define II0EPS          7  // this must come first; other base on it
#define II1EPS          8
#define IJ0EPS          9
#define IJ1EPS          10
#define ISUMEPS         11
#define IANYEPS         12
#define IALLEPS         13
#define IIFBEPS         14
#define IIMODFIELD      0x70  // bits used to indicate processing options
#define IIMODPACK       0x10  // modifier for type.  (small-range search except i./i:) In IIDOT/IICO, indicates reflexive application.  In others, indicates that the
                              // bitmask should be stored as packed bits rather than bytes

#define IIMODREFLEXX    4
#define IIMODREFLEX     (1LL<<IIMODREFLEXX)  // (small-range i. and i:) this is i.~/i:~ (hashing) this is i.~/i:~/~./~:/I.@:~.
#define IIMODFULL       0x20  // (small-range search) indicates that the min/max values cover the entire range of possible inputs, so no range checking is required.  Always set for hashing
#define IIMODBASE0      0x40  // set in small-range i./i: (which never use BITS) to indicate that the hashtable starts at index 0 and has m in the place of unused indexes.  Set in hashing always, with same meaning
#define IIMODBITS       0x80  // set if the hash field stores bits rather than indexes.  Used only for small-range and not i./i:.  IIMODPACK qualifies this, indicating that the bits are packed
#define IIMODFORCE0X    8
#define IIMODFORCE0     (1LL<<IIMODFORCE0X)  // set to REQUIRE a (non-bit) allocation to reset to offset 0 and clear
#define IPHCALC         0x200   // set when we are calculating a prehashed table
#define IINOTALLOCATED  0x400  // internal flag, set when the block has not been allocated
#define IIOREPSX        11
#define IIOREPS         (1LL<<IIOREPSX)  // internal flag, set if mode is i./i:/e., but not if prehashing
#define IREVERSED       0x1000   // set if we have decided to reverse the hash in a small-range situation
#define IPHOFFSET       0x2000              /* offset for prehashed versions - set when we are using a prehashed table   */
#define IPHIDOT         (IPHOFFSET+IIDOT)
#define IPHICO          (IPHOFFSET+IICO)
#define IPHEPS          (IPHOFFSET+IEPS)
#define IPHI0EPS        (IPHOFFSET+II0EPS)
#define IPHI1EPS        (IPHOFFSET+II1EPS)
#define IPHJ0EPS        (IPHOFFSET+IJ0EPS)
#define IPHJ1EPS        (IPHOFFSET+IJ1EPS)
#define IPHSUMEPS       (IPHOFFSET+ISUMEPS)
#define IPHANYEPS       (IPHOFFSET+IANYEPS)
#define IPHALLEPS       (IPHOFFSET+IALLEPS)
#define IPHIFBEPS       (IPHOFFSET+IIFBEPS)

#define jfloor          floor

#define BB              8      /* # bits in a byte */
#define LGBB 3    // lg(BB)
#if SY_64
#define BW              64     /* # bits in a word */
#define LGSZI 3    // lg(#bytes in an I)
#else
#define BW              32
#define LGSZI 2
#endif
#define LGBW (LGSZI+LGBB)  // lg (# bits in a word)

// nominal cache sizes for current processors
#define L1CACHESIZE (1LL<<15)
#define L2CACHESIZE (1LL<<18)
#define L3CACHESIZE (1LL<<22)

#define TOOMANYATOMS 0x01000000000000LL  // more atoms than this is considered overflow (64-bit)

#define MEMCPYTUNE 8192  // (bytes) blocks shorter than this should just use MCISxx


// Debugging options

// Use MEMAUDIT to sniff out errant memory alloc/free
#define MEMAUDIT 0x00   // Bitmask for memory audits: 1=check headers 2=full audit of tpush/tpop 4=write garbage to memory before freeing it 8=write garbage to memory after getting it
                     // 16=audit freelist at every alloc/free (starting after you have run 6!:5 (1) to turn it on)
 // 13 (0xD) will verify that there are no blocks being used after they are freed, or freed prematurely.  If you get a wild free, turn on bit 0x2
 // 2 will detect double-frees before they happen, at the time of the erroneous tpush

#define AUDITEXECRESULTS 0   // When set, we go through all execution results to verify recursive and virtual bits are OK
#define FORCEVIRTUALINPUTS 0  // When 1 set, we make all non-inplaceable noun inputs to executions VIRTUAL.  Tests should still run
                           // When 2 set, make all outputs from RETF() virtual.  Tests for inplacing will fail; that's OK if nothing crashes

// set FINDNULLRET to trap when a routine returns 0 without having set an error message
#define FINDNULLRET 0


#if BW==64
#define ALTBYTES 0x00ff00ff00ff00ffLL
// t has totals per byte-lane, result combines them into single total.  t must be an lvalue
#define ADDBYTESINI(t) (t=(t&ALTBYTES)+((t>>8)&ALTBYTES), t = (t>>32) + t, t = (t>>16) + t, t&=0xffff) // sig in 01ff01ff01ff01ff, then xxxxxxxx03ff03ff, then xxxxxxxxxxxx07ff, then 00000000000007ff
#else
#define ALTBYTES 0x00ff00ffLL
#define ADDBYTESINI(t) (t=(t&ALTBYTES)+((t>>8)&ALTBYTES), t = (t>>16) + t, t&=0xffff) // sig in 01ff01ff, then xxxx03ff, then 000003ff
#endif

#define A0              0   // a nonexistent A-block
#define ABS(a)          (0<=(a)?(a):-(a))
#define ASSERT(b,e)     {if(!(b)){jsignal(e); R 0;}}
#define ASSERTD(b,s)    {if(!(b)){jsigd((s)); R 0;}}
#define ASSERTMTV(w)    {RZ(w); ASSERT(1==AR(w),EVRANK); ASSERT(!AN(w),EVLENGTH);}
#define ASSERTN(b,e,nm) {if(!(b)){jt->curname=(nm); jsignal(e); R 0;}}
#define ASSERTSYS(b,s)  {if(!(b)){jsignal(EVSYSTEM); jtwri(jt,MTYOSYS,"",(I)strlen(s),s); R 0;}}
#define ASSERTW(b,e)    {if(!(b)){if((e)<=NEVM)jsignal(e); else jt->jerr=(e); R;}}
#define CALL1(f,w,fs)   ((f)(jt,    (w),(A)(fs)))
#define CALL2(f,a,w,fs) ((f)(jt,(a),(w),(A)(fs)))
#define CALL1IP(f,w,fs)   ((f)(jtinplace,    (w),(A)(fs)))
#define CALL2IP(f,a,w,fs) ((f)(jtinplace,(a),(w),(A)(fs)))
#define RETARG(z)       (z)   // These places were ca(z) in the original JE
#define CLEARZOMBIE     {jt->assignsym=0; jt->zombieval=0;}  // Used when we know there shouldn't be an assignsym, just in case
#define DF1(f)          A f(J jt,    A w,A self)
#define DF2(f)          A f(J jt,A a,A w,A self)
#define DO(n,stm)       {I i=0,_n=(n); for(;i<_n;i++){stm}}  // i runs from 0 to n-1
#define DP(n,stm)       {I i=-(n);    for(;i<0;++i){stm}}   // i runs from -n to -1 (faster than DO)
#define DQ(n,stm)       {I i=(I)(n)-1;    for(;i>=0;--i){stm}}  // i runs from n-1 downto 0
#define ds(c)           pst[(UC)(c)]
#define FDEPDEC(d)      {jt->fdepi-=d;}
#define FDEPINC(d)      {ASSERT(jt->fdepn>=d+jt->fdepi,EVSTACK); jt->fdepi+=d;}
#define FCONS(x)        fdef(0,CFCONS,VERB,jtnum1,jtnum2,0L,0L,(x),VFLAGNONE, RMAX,RMAX,RMAX)
#define FEQ(u,v)        (ABS((u)-(v))<=jt->fuzz*MAX(ABS(u),ABS(v)))
#define F1(f)           A f(J jt,    A w)
#define F2(f)           A f(J jt,A a,A w)
#define FPREF           
#define F1PREF          FPREF
#define F2PREF          FPREF
#define FPREFIP         J jtinplace=jt; jt=(J)(intptr_t)((I)jt&~(JTINPLACEW+JTINPLACEA))
#define F1PREFIP        FPREFIP
#define F2PREFIP        FPREFIP
#define F1RANK(m,f,self)    {RZ(   w); if(m<AR(w)         )R rank1ex(  w,(A)self,(I)m,     f);}  // if there is more than one cell, run rank1ex on them.  m=monad rank, f=function to call for monad cell
// obsolete #define F2RANK(l,r,f,self)  {RZ(a&&w); if(l<AR(a)||r<AR(w))R rank2ex(a,w,(A)self,(I)l,(I)r,(I)l,(I)r,f);}  // If there is more than one cell, run rank2ex on them.  l,r=dyad ranks, f=function to call for dyad cell
#define F2RANK(l,r,f,self)  {RZ(a&&w); if((I)((l-AR(a))|(r-AR(w)))<0)R rank2ex(a,w,(A)self,(I)l,(I)r,(I)l,(I)r,f);}  // If there is more than one cell, run rank2ex on them.  l,r=dyad ranks, f=function to call for dyad cell

// Memory-allocation macros
// Size-of-block calculations.  VSZ when size is constant or variable
// Because the Boolean dyads write beyond the end of the byte area (up to 1 extra word), we add one SZI for islast (which includes B01), rather than adding 1
#define ALLOBYTESVSZ(atoms,rank,size,islast,isname)      ( ((((rank)|(!SY_64))*SZI  + ((islast)? (isname)?(NORMAH*SZI+sizeof(NM)+SZI):(NORMAH*SZI+SZI) : (NORMAH*SZI)) + (atoms)*(size)))  )  // # bytes to allocate allowing only 1 byte for string pad - include mem hdr
// here when size is constant.  The number of bytes, rounded up with overhead added, must not exceed 2^(PMINL+4)
#define ALLOBYTES(atoms,rank,size,islast,isname)      ((size&(SZI-1))?ALLOBYTESVSZ(atoms,rank,size,islast,isname):(SZI*(((rank)|(!SY_64))+NORMAH+((size)>>LGSZI)*(atoms))))  // # bytes to allocate
#define ALLOBLOCK(n) ((n)<=2*PMIN?((n)<=PMIN?PMINL:PMINL+1) : (n)<=8*PMIN?((n)<=4*PMIN?PMINL+2:PMINL+3) : (n)<=32*PMIN?PMINL+4:IMIN)   // lg2(#bytes to allocate)
// value to put into name->bucketx for locale names: number if numeric, hash otherwise
#define BUCKETXLOC(len,s) ((*(s)<='9')?strtoI((s),NULL,10):(I)nmhash((len),(s)))
// GA() is used when the type is unknown.  This routine is in m.c and documents the function of these macros.
// NEVER use GA() for NAME types - it doesn't honor it.
#define GA(v,t,n,r,s)   RZ(v=ga(t,(I)(n),(I)(r),(I*)(s)))
// GAE executes the given expression when there is an error
#define GAE(v,t,n,r,s,erraction)   if(!(v=ga(t,(I)(n),(I)(r),(I*)(s))))erraction;
// When the type and all rank/shape are known, use GAT.  The compiler precalculates almost everything
// For best results declare name as: AD* RESTRICT name;  The number of bytes, rounded up with overhead added, must not exceed 2^(PMINL+4)
#define GAT(name,type,atoms,rank,shaape) \
{ ASSERT(!((rank)&~RMAX),EVLIMIT); \
 I bytes = ALLOBYTES(atoms,rank,type##SIZE,(type)&LAST0,(type)&NAME); \
 name = jtgaf(jt, ALLOBLOCK(bytes)); \
 I akx=AKXR(rank);   \
 RZ(name);   \
 AK(name)=akx; AT(name)=type; AN(name)=atoms;   \
 if(!(type&DIRECT))memset((C*)name+akx,C0,bytes-akx);  \
 else if(type&LAST0){((I*)((C*)name+((bytes-SZI)&(-SZI))))[0]=0; }     \
 AR(name)=(RANKT)(rank);     \
 if((1==(rank))&&!(type&SPARSE))*AS(name)=atoms; else if((shaape)&&(rank)){AS(name)[0]=((I*)(shaape))[0]; DO(rank-1, AS(name)[i+1]=((I*)(shaape))[i+1];)}    \
 /* obsolete AM(name)=((I)1<<ALLOBLOCK(bytes))-mhb-akx; */    \
}
#if 0 // obsolete
// Used when type is known and something else is variable.  ##SIZE must be applied before type is substituted, so we have GATVS to use inside other macros.  Normally use GATV
#define GATVS(name,type,atoms,rank,shaape,size,erraction) \
{ I bytes = ALLOBYTES(atoms,rank,size,(type)&LAST0,(type)&NAME); \
 ASSERT(SY_64?((unsigned long long)(atoms))<TOOMANYATOMS:(I)bytes>(I)(atoms)&&(I)(atoms)>=(I)0,EVLIMIT); \
 AD* ZZz = jtgafv(jt, bytes);   \
 I akx=AKXR(rank);   \
 if(ZZz){   \
  if(!(type&DIRECT))memset((C*)ZZz+akx,C0,bytes-mhb-akx);  \
  else if(type&LAST0){((I*)((C*)ZZz+((bytes-SZI-mhb)&(-SZI))))[0]=0; }     \
  AK(ZZz)=akx; AT(ZZz)=type; AN(ZZz)=atoms; AR(ZZz)=rank;     \
  if((1==(rank))&&!(type&SPARSE))*AS(ZZz)=atoms; else if((shaape)&&(rank)){AS(ZZz)[0]=((I*)(shaape))[0]; DO(rank-1, AS(ZZz)[i+1]=((I*)(shaape))[i+1];)}   \
  /* obsolete AM(ZZz)=((I)1<<((MS*)ZZz-1)->j)-mhb-akx; */     \
  name=ZZz;   \
 }else{erraction;} \
}
#else
// Used when type is known and something else is variable.  ##SIZE must be applied before type is substituted, so we have GATVS to use inside other macros.  Normally use GATV
// Note: assigns name before assigning the components of the array, so the components had better not depend on name, i. e. no GATV(z,BOX,AN(z),AR(z),AS(z))
#define GATVS(name,type,atoms,rank,shaape,size,erraction) \
{ I bytes = ALLOBYTES(atoms,rank,size,(type)&LAST0,(type)&NAME); \
 ASSERT(SY_64?((unsigned long long)(atoms))<TOOMANYATOMS:(I)bytes>(I)(atoms)&&(I)(atoms)>=(I)0,EVLIMIT); \
 ASSERT(!((rank)&~RMAX),EVLIMIT); \
 name = jtgafv(jt, bytes);   \
 I akx=AKXR(rank);   \
 if(name){   \
  if(!(type&DIRECT))memset((C*)name+akx,C0,bytes-akx);  \
  else if(type&LAST0){((I*)((C*)name+((bytes-SZI)&(-SZI))))[0]=0; }     \
  AK(name)=akx; AT(name)=type; AN(name)=atoms; AR(name)=(RANKT)(rank);     \
  if((1==(RANKT)(rank))&&!(type&SPARSE))*AS(name)=atoms; else if((shaape)&&(rank)){AS(name)[0]=((I*)(shaape))[0]; DO(rank-1, AS(name)[i+1]=((I*)(shaape))[i+1];)}   \
  /* obsolete AM(name)=((I)1<<((MS*)name-1)->j)-mhb-akx; */     \
  /* obsolete name=ZZz; */   \
 }else{erraction;} \
}
#endif

// see warnings above under GATVS
#define  GATV(name,type,atoms,rank,shaape) GATVS(name,type,atoms,rank,shaape,type##SIZE,R 0)

#define HN              4L  // number of boxes per valence to hold exp-def info (words, control words, original (opt.), symbol table)
#define IC(w)           (AR(w) ? *AS(w) : 1L)
#define ICMP(z,w,n)     memcmp((z),(w),(n)*SZI)
#define ICPY(z,w,n)     memcpy((z),(w),(n)*SZI)
// Mark a block as incorporated by removing its inplaceability.  The blocks that are tested for incorporation are ones that are allocated by partitioning, and they will always start out as inplaceable
// If a block is virtual, it must be realized before it can be incorporated.  realized blocks always start off inplaceable
// z is an lvalue
// Use INCORPNA if you need to tell the caller that the block e sent you has been incoroprated.  If you created the block being incorporated,
// even by calling a function that returns it, you can get by using rifv() or rifvs().  This may leave an incorporated block marked inplaceable,
// but that's OK as long as you pass it to some place where it can become an argument to another function
#define INCORP(z)       {if(AFLAG(z)&AFVIRTUAL)RZ((z)=realize(z)); ACIPNO(z); }
// same, but for nonassignable argument
#define INCORPNA(z)     incorp(z)
// Tests for whether a result incorporates its argument.  The originator, who is going to check this, always marks the argument inplaceable,
// and we signal incorporation either by returning the argument itself or by marking it non-inplaceable (if we box it)
#define WASINCORP1(z,w)    ((z)==(w)||0<=AC(w))
#define WASINCORP2(z,a,w)  ((z)==(w)||(z)==(a)||0<=(AC(a)|AC(w)))
#define INF(x)          ((x)==inf||(x)==infm)
// true if a has recursive usecount
#define UCISRECUR(a)    (AFLAG(a)&RECURSIBLE)
// Structural verbs (dyads #, $, {., { etc) always inherit the SMNOREL status of their ibput, at least to start with
#define INHERITNOREL(z,w) (AFLAG(z) |= AFLAG(w)&AFNOSMREL)
// if the verb may add fill, we have to check the fill as well
#define INHERITNORELFILL(z,w) (AFLAG(z) |= (jt->fill&&(!(AT(jt->fill)&DIRECT))?AFLAG(jt->fill)&AFLAG(w):AFLAG(w))&AFNOSMREL)
#define INHERITNOREL2(z,a,w) (AFLAG(z) |= AFLAG(a)&AFLAG(w)&AFNOSMREL)
// if the verb may add fill, we have to check the fill as well
#define INHERITNORELFILL2(z,a,w) (AFLAG(z) |= (jt->fill&&(!(AT(jt->fill)&DIRECT))?AFLAG(jt->fill)&AFLAG(a)&AFLAG(w):AFLAG(a)&AFLAG(w))&AFNOSMREL)
// Install new value z into xv[k], where xv is AAV(x).  If x has recursive usecount, we must increment the usecount of z.
// This also guarantees that z has recursive usecount whenever x does, and that z is realized
#define INSTALLBOX(x,xv,k,z) rifv(z); if(UCISRECUR(x)){A zzZ=xv[k]; ra(z); fa(zzZ);} xv[k]=z
#define INSTALLBOXNF(x,xv,k,z) rifv(z); if(UCISRECUR(x)){ra(z);} xv[k]=z   // Don't do the free - if we are installing into known 0 or known nonrecursive
#define INSTALLBOXRECUR(xv,k,z) rifv(z); {I zzK=(k); {A zzZ=xv[zzK]; ra(z); fa(zzZ);} xv[zzK]=z;}  // Don't test - we know we are installing into a recursive block
// Same thing for RAT type.  z is a Q, xv[k] is a Q
#define INSTALLRAT(x,xv,k,z) if(UCISRECUR(x)){Q zzZ=xv[k]; ra(z.n); ra(z.d); fa(zzZ.n); fa(zzZ.d);} xv[k]=z
#define INSTALLRATNF(x,xv,k,z) if(UCISRECUR(x)){ra(z.n); ra(z.d);} xv[k]=z   // Don't do the free - if we are installing into known 0
#define INSTALLRATRECUR(xv,k,z) rifv(z.n); rifv(z.d); {I zzK=(k); {Q zzZ=xv[k]; ra(z.n); ra(z.d); fa(zzZ.n); fa(zzZ.d);} xv[zzK]=z;}  // Don't test - we know we are installing into a recursive block
#define IX(n)           apv((n),0L,1L)
#define JATTN           {if(*jt->adbreakr){jsignal(EVATTN); R 0;}}
#define JBREAK0         {if(2<=*jt->adbreakr){jsignal(EVBREAK); R 0;}}
#define JTINPLACEWX     0   // turn this on in jt to indicate that w can be inplaced
#define JTINPLACEW      (1LL<<JTINPLACEWX)
#define JTINPLACEAX     1   // turn this on in jt to indicate that a can be inplaced.  Must be 1+JTINPLACEWX
#define JTINPLACEA      (1LL<<JTINPLACEAX)
#define MAX(a,b)        ((a)>(b)?(a):(b))
#define MC              memcpy
#define MCL(dest,src,n) memcpy(dest,src,n)  // use when copy is expected to be long
#define MCI(dest,src,n) memcpy(dest,src,(n)*sizeof(*src))   // copy items of source
#define MCIL(dest,src,n) memcpy(dest,src,(n)*sizeof(*src))   // use when copy expected to bo long
#define MCIS(dest,src,n) {I *_d=(dest); I *_s=(src); I _n=~(n); while((_n-=(_n>>(BW-1)))<0)*_d++=*_s++;}  // use for short copies.  the tricky stuff is to confound the compiler so it doesn't produce memcpy
#define MCISd(dest,src,n) {I *_s=(src); I _n=~(n); while((_n-=(_n>>(BW-1)))<0)*dest++=*_s++;}  // ... this version when d increments through the loop
#define MCISs(dest,src,n) {I *_d=(dest); I _n=~(n); while((_n-=(_n>>(BW-1)))<0)*_d++=*src++;}  // ... this when s increments through the loop
#define MCISds(dest,src,n) {I _n=~(n); while((_n-=(_n>>(BW-1)))<0)*dest++=*src++;}  // ...this when both
#define MIN(a,b)        ((a)<(b)?(a):(b))
#define MLEN            (SY_64?63:31)
#define MODVIRTINPLACE(x)  (AFLAG(x) |= (AFLAG(x)&AFVIRTUAL)<<(AFVIRTUALINPLACEX-AFVIRTUALX))

#define NAN0            (_clearfp())
#if defined(_MSC_VER) && _MSC_VER==1800 && !SY_64 // bug in some versions of VS 2013
#define NAN1            {if(_SW_INVALID&_statusfp()){_clearfp();jsignal(EVNAN); R 0;}}
#define NAN1V           {if(_SW_INVALID&_statusfp()){_clearfp();jsignal(EVNAN); R  ;}}
#define NANTEST         (_SW_INVALID&_statusfp())
#else
#define NAN1            {if(_SW_INVALID&_clearfp()){jsignal(EVNAN); R 0;}}
#define NAN1V           {if(_SW_INVALID&_clearfp()){jsignal(EVNAN); R  ;}}
#define NANTEST         (_SW_INVALID&_clearfp())
#endif
#define NUMMIN          (-9)    // smallest number represented in num[]
#define NUMMAX          9    // largest number represented in num[]
// PROD multiplies a list of numbers, where the product is known not to overflow a signed int (for example, it might be part of the shape of a dense array)
// obsolete #define PROD(result,length,ain) {I _i; if((_i=(length)-1)<0)result=1;else{result=*(ain);if(_i>0){I *_ain=(ain); do{result*=*++_ain;}while(--_i);}}}
#define PROD(result,length,ain) {I _i; if((_i=(length))<=0)result=1;else{result=(ain)[0];while(--_i>0){result*=(ain)[_i];}}}
// CPROD is to be used to create a test testing #atoms.  Because empty arrays can have cells that have too many atoms, we can't use PROD if
// we don't know that the array isn't empty or will be checked later
#define CPROD(t,z,x,a)if(t)PROD(z,x,a)else RE(z=prod(x,a))
// PROLOG/EPILOG are the main means of memory allocation/free.  jt->tstack contains a pointer to every block that is allocated by GATV(i. e. all blocks).
// GA causes a pointer to the block to be pushed onto tstack.  PROLOG saves a copy of the stack pointer in _ttop, a local variable in its function.  Later, tpop(_ttop)
// can be executed to free every block that the function allocated, without requiring bookkeeping in the function.  This may be done from time to time in
// long-running definitions, to free memory [for this application it is normal to do some allocating of working memory, then save the tstack pointer in a local name
// other than _ttop, then periodically do tpop(other local name); such a sequence will free up all memory that was allocated after the working memory; the working
// memory itself will be freed by the eventual tpop(_ttop)].
// EPILOG performs the tpop(_ttop), but it has another important function: that of preserving the result of a function.  Of all the blocks that were allocated by a function,
// one (possibly including its descendants) is the result of the function.  It must not be freed, so that it can carry the result back to the caller of this function.
// So, it is preserved by incrementing its usecount before the tpop(_ttop); then after the tpop, it is pushed back onto the tstack, indicating that it will be freed
// by the next-higher-level function.  Thus, when X calls Y inside PROLOG/EPILOG, the result of Y (which is an A block), has the same viability as any other GA executed in X
// (unless its usecount is > 1 because it was assigned elsewhere)
#define PROLOG(x)       I _ttop=jt->tnextpushx
#define EPILOGNORET(z) (gc(z,_ttop))   // protect z and return its address
#define EPILOG(z)       RETF(EPILOGNORET(z))   // z is the result block
#define EPILOGNOVIRT(z)       R rifvsdebug((gc(z,_ttop)))   // use this when the repercussions of allowing virtual result are too severe
#define EPILOGZOMB(z)       if(!gc3(&(z),0L,0L,_ttop))R0; RETF(z);   // z is the result block.  Use this if z may contain inplaceable contents that would free prematurely
// Routines that do little except call a function that does PROLOG/EPILOG have EPILOGNULL as a placeholder
#define EPILOGNULL(z)   R z
// Routines that do not return A
#define EPILOG0         tpop(_ttop)
#define PTO             3L  // Number of prefix entries of ptab[] that are used only for local symbol tables
// Compounds push zombie to begin with and restore before the last operation, which can execute in place.
// zombieval is used as a way of flagging reusable blocks.  They are reused only if they are marked as inplaceable; in other words,
// zombieval is an alternative to AC<0.  We could try to overwrite the zombieval during final assignment, even if it is
// not an argument, but this seems to be more trouble than it's worth, so we don't bother detecting final assignment.
#define PUSHZOMB L*savassignsym = jt->assignsym; A savzombval; if(savassignsym){savzombval=jt->zombieval; if((UI)jt->asgzomblevel-1>=(UI)jt->local){CLEARZOMBIE}}  // test is (jt->asgzomblevel==0||jt->local==0)
#define POPZOMB if(savassignsym){jt->assignsym=savassignsym;jt->zombieval=savzombval;}
// obsolete #define PUSHZOMB L*savassignsym = jt->assignsym; A savzombval; if(savassignsym){if(!jt->asgzomblevel||!jt->local){savzombval=jt->zombieval;CLEARZOMBIE}}
// obsolete #define POPZOMB if(savassignsym){jt->assignsym=savassignsym;jt->zombieval=savzombval;}
#define R               return
#if FINDNULLRET   // When we return 0, we should always have an error code set.  trap if not
#define R0 {if(jt->jerr)R A0;else *(I*)0=0;}
#else
#define R0 R 0;
#endif
// In the original JE many verbs returned a clone of the input, i. e. R ca(w).  We have changed these to avoid the clone, but we preserve the memory in case we need to go back
#define RCA(w)          R w
#define RE(exp)         {if((exp),jt->jerr)R 0;}
#define RER             {if(er){jt->jerr=er; R;}}
#define RESETERR        {jt->etxn=jt->jerr=0;}
#define RESETRANK       (jt->ranks=(RANK2T)~0)
#define RNE(exp)        {R jt->jerr?0:(exp);}
#define RZ(exp)         {if(!(exp))R0}
// RETF is the normal function return.  For debugging we hook into it
#if AUDITEXECRESULTS && (FORCEVIRTUALINPUTS==2)
#define RETF(exp)       A ZZZz = (exp); auditblock(ZZZz,1,1); ZZZz = virtifnonip(jt,0,ZZZz); R ZZZz
#else
#define RETF(exp)       R exp
#endif
#define SBSV(x)         (jt->sbsv+(I)(x))
#define SBUV(x)         (jt->sbuv+(I)(x))
#define SGN(a)          ((0<(a))-(0>(a)))
#define SMAX            65535
#define SMIN            (-65536)
#if SY_64
#define SYMHASH(h,n)    (((UI)(h)*(UI)(n)>>32)+SYMLINFOSIZE)   // h is hash value for symbol; n is number of symbol chains (not including LINFO entries)
#else
#define SYMHASH(h,n)    ((UI)(((D)(h)*(D)(n)*(1.0/4294967296.0))+SYMLINFOSIZE))   // h is hash value for symbol; n is number of symbol chains (not including LINFO entries)
#endif
// obsolete #define SYMHASH(h,n)    ((h)%(n)+(SYMLINFOSIZE-0.49999999999))   // h is hash value for symbol; n is number of symbol chains (not including LINFO entries)
#define SZA             ((I)sizeof(A))
#define LGSZA    LGSZI  // we always require A and I to have same size
#define SZD             ((I)sizeof(D))
#define SZI             ((I)sizeof(I))
#define LGSZD    3  // lg(#bytes in a D)
#define SZI4            ((I)sizeof(I4))
#define LGSZI4   2  // lg (bytes in an I4)
#define SZUI4            ((I)sizeof(UI4))
#define LGSZUI4  2  // lg(#bytes in a UI4)
#define SZUS            ((I)sizeof(US))
#define LGSZUS   1  // lg(bytes in a US)
#define SZS            ((I)sizeof(S))
#define LGSZS   1  // lg (bytes in an S)

#define VAL1            '\001'
#define VAL2            '\002'

#if C_LE
#if BW==64
#define IHALF0  0x00000000ffffffffLL
#else
#define IHALF0  0x0000ffff
#endif
#define B0000   0x00000000
#define B0001   0x01000000
#define B0010   0x00010000
#define B0011   0x01010000
#define B0100   0x00000100
#define B0101   0x01000100
#define B0110   0x00010100
#define B0111   0x01010100
#define B1000   0x00000001
#define B1001   0x01000001
#define B1010   0x00010001
#define B1011   0x01010001
#define B1100   0x00000101
#define B1101   0x01000101
#define B1110   0x00010101
#define B1111   0x01010101
#define BS00    0x0000
#define BS01    0x0100
#define BS10    0x0001
#define BS11    0x0101
#else
#if BW==64
#define IHALF0  0xffffffff00000000LL
#else
#define IHALF0  0xffff0000
#endif
#define B0000   0x00000000
#define B0001   0x00000001
#define B0010   0x00000100
#define B0011   0x00000101
#define B0100   0x00010000
#define B0101   0x00010001
#define B0110   0x00010100
#define B0111   0x00010101
#define B1000   0x01000000
#define B1001   0x01000001
#define B1010   0x01000100
#define B1011   0x01000101
#define B1100   0x01010000
#define B1101   0x01010001
#define B1110   0x01010100
#define B1111   0x01010101
#define BS00    0x0000
#define BS01    0x0001
#define BS10    0x0100
#define BS11    0x0101
#endif



#define CACHELINESIZE 64  // size of processor cache line, in case we align to it



#include "ja.h" 
#include "jc.h" 
#include "jtype.h" 
#include "m.h"
#include "jt.h" 
#include "jlib.h"
#include "je.h" 
#include "jerr.h" 
#include "va.h" 
#include "vq.h" 
#include "vx.h" 
#include "vz.h"
#include "vdx.h"  
#include "a.h"
#include "s.h"

// CTTZ(w) counts trailing zeros in low 32 bits of w.  Result is undefined if w is 0.
// CTTZZ(w) does the same, but returns 32 if w is 0
// CTTZI(w) counts trailing zeros in an argument of type I (32 or 64 bits depending on architecture)
// CTLZ would be a better primitive to support, except that LZCNT executes as BSR on some Intel processors,
// but produces incompatible results! (BSR returns bit# of leading 1, LZCNT returns #leading 0s)
// since we don't require CTLZ yet, we defer that problem to another day

// CTTZ uses the single-instruction count-trailing-zeros instruction to convert
// a 1-bit mask to a bit number.  If this instruction is available on your architecture/compiler,
// you should use the compiler intrinsic to create this instruction, and define the CTTZ and CTTZZ
// macros to use the instruction inline.  It is used enough in the J engine to make a difference.

// If you set AUDITCOMPILER to 1, i.c will include code to test CTTZ (and signed shift) on startup and crash if it
// does not perform properly, as a debugging aid.

// If CTTZ is not defined, the default routine defined in u.c will be used.  You can look there
// for the complete spec for CTTZ and CTTZZ.

#if defined(_MSC_VER)  // SY_WIN32
#include <intrin.h>
#define CTTZ(w) _tzcnt_u32((UINT)(w))
#if SY_64
#define CTTZI(w) _tzcnt_u64((UI)(w))
#define CTLZI(in,out) _BitScanReverse64(&(out),in)
#else
#define CTTZI(w) _tzcnt_u32((UINT)(w))
#define CTLZI(in,out) _BitScanReverse(&(out),in)
#endif
#define CTTZZ(w) ((w)==0 ? 32 : CTTZ(w))
#endif

#ifdef _MSC_VER
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__((noinline))
#ifndef __forceinline
#define __forceinline inline
#endif
#endif
#ifdef __MINGW32__
// original definition
// #define __forceinline extern __inline__ __attribute__((__always_inline__,__gnu_inline__))
#ifdef __forceinline
#undef __forceinline
#endif
#define __forceinline __inline__ __attribute__((__always_inline__,__gnu_inline__))
#endif

#ifdef __GNUC__
#define CTTZ(w) __builtin_ctzl((UINT)(w))
#if SY_64
#define CTTZI(w) __builtin_ctzll((UI)(w))
#define CTLZI(w,out) (out=(63-__builtin_clzll((UI)(w))))
#else
#define CTTZI(w) __builtin_ctzl((UINT)(w))
#define CTLZI(w,out) (out=(31-__builtin_clzl((UI)(w))))
#endif
#define CTTZZ(w) ((w)==0 ? 32 : CTTZ(w))
#endif

// For older processors, TZCNT is executed as BSF, which differs from TZCNT in that it does not
// set the Z flag if the result is 0.  The optimizer sometimes turns a switch into tests rather than a branch
// table, and it expects TSCNT to set the Z flag properly.  We use CTTZNOFLAG to set it right
#define CTTZNOFLAG(w) (CTTZ(w)&31)

#ifdef __GNUC__
#ifndef offsetof
#define offsetof(TYPE, MEMBER)  __builtin_offsetof (TYPE, MEMBER)
#endif
#endif

// Insert other compilers/architectures here

// Insert CTLZ here if CTTZ is not available

// If your machine supports count-leading-zeros but not count-trailing-zeros, you can define the macros
// CTLZ/CTLZI, which returns the number of high-order zeros in the low 32 bits of its argument, and the following
// CTTZ will be defined:
#if defined(CTLZ) && !defined(CTTZ)
#define CTTZ(w) (31-CTLZ((w)&-(w)))
#define CTTZI(w) (63-CTLZI((w)&-(w)))
#define CTTZZ(w) (0xffffffff&(w) ? CTTZ(w) : 32)
#endif

// If CTTZ is not defined, the following code will use the default from u.c:
#if !defined(CTTZ)
extern I CTTZ(I);
extern I CTTZI(I);
extern I CTTZZ(I);
#endif
#if !defined(CTLZI)
extern I CTLZI_(UI,UI4*);
#define CTLZI(in,out) CTLZI_(in,&(out))
#endif

// Set these switches for testing
#define AUDITBP 0  // Verify that bp() returns expected data
#define AUDITCOMPILER 0  // Verify compiler features CTTZ, signed >>


// JPFX("here we are\n")
// JPF("size is %i\n", v)
// JPF("size and extra: %i %i\n", (v,x))
#define JPFX(s)  {char b[1000]; sprintf(b, s);    jsto(gjt,MTYOFM,b);}
#define JPF(s,v) {char b[1000]; sprintf(b, s, v); jsto(gjt,MTYOFM,b);}
extern J gjt; // global for JPF (procs without jt)

#if SY_WINCE_MIPS
/* strchr fails for CE MIPS - neg chars - spellit fails in ws.c for f=.+.  */
#define strchr(a,b)     (C*)strchr((unsigned char*)(a), (unsigned char)(b))
#endif
#if (defined(__arm__)||defined(__aarch64__)) && !defined(__MACH__)
// option -fsigned-char in android and raspberry
#ifdef strchr
#undef strchr
#endif
#define strchr(a,b)     (C*)strchr((unsigned char*)(a), (unsigned char)(b))
#endif

/* workaround clang branch prediction side effect */
#if defined(__clang__) && ( (__clang_major__ > 3) || ((__clang_major__ == 3) && (__clang_minor__ > 5)))
#define dmul2(u,v) ({__asm__("" ::: "cc");(u)*(v);})
#define ddiv2(u,v) ({__asm__("" ::: "cc");(u)/(v);})
#else
#define dmul2(u,v) ((u)*(v))
#define ddiv2(u,v) ((u)/(v))
#endif

/* (hopefully) turn off some re-scheduling optimization  */
#ifdef __GNUC__
#define CCBLOCK __asm__("" ::: "cc")
#else
#define CCBLOCK
#endif

#if SYS & SYS_UNIX
#include <fenv.h>
#define _isnan       isnan
#define _SW_INVALID  FE_INVALID

static inline UINT _clearfp(void){int r=fetestexcept(FE_ALL_EXCEPT);
 feclearexcept(FE_ALL_EXCEPT); return r;
}
#endif

// Define integer multiply, *z=x*y but do something else if integer overflow.
// Depending on the compiler, the overflowed result may or may not have been stored
#if SY_64

#if C_USEMULTINTRINSIC
#if defined(_MSC_VER)  // SY_WIN32
#define DPMULDECLS
// DPMUL: *z=x*y, execute s if overflow
#define DPMUL(x,y,z,s) {I _l,_h; *z=_l=_mul128(x,y,&_h); if(_h+((UI)_l>>(BW-1)))s}
#define DPMULDDECLS
#define DPMULD(x,y,z,s) {I _h; z=_mul128(x,y,&_h); if(_h+((UI)z>>(BW-1)))s}
#else
#define DPMULDECLS
#define DPMUL(x,y,z,s) if(__builtin_smulll_overflow(x,y,z))s
#define DPMULDDECLS
#define DPMULD(x,y,z,s) if(__builtin_smulll_overflow(x,y,&z))s
#endif
#else // C_USEMULTINTRINSIC 0 - use standard-C version (64-bit)
#define DPMULDECLS
#define DPMUL(x,y,z,s) {I _l, _x=(x), _y=(y); D _d; _l=_x*_y; _d=(D)_x*(D)_y-(D)_l; *z=_l; _d=ABS(_d); if(_d>1e8)s}  // *z may be the same as x or y
#define DPMULDDECLS
#define DPMULD(x,y,z,s) {I _l, _x=(x), _y=(y); D _d; _l=_x*_y; _d=(D)_x*(D)_y-(D)_l; z=_l; _d=ABS(_d); if(_d>1e8)s}
#endif

#else  // 32-bit

#if C_USEMULTINTRINSIC
#if defined(_MSC_VER)  // SY_WIN32
// optimizer can't handle this #define SPDPADD(addend, sumlo, sumhi) {C c; c=_addcarry_u32(0,addend,sumlo,&sumlo); _addcarry_u32(c,0,sumhi,&sumhi);}
#define DPMULDECLS unsigned __int64 _p;
#define DPMUL(x,y,z,s) _p = __emul(x,y); *z=(I)_p; if((_p+0x80000000U)>0xFFFFFFFFU)s
#define DPMULDDECLS unsigned __int64 _p;
#define DPMULD(x,y,z,s) _p = __emul(x,y); z=(I)_p; if((_p+0x80000000U)>0xFFFFFFFFU)s
#else
#define DPMULDECLS
#define DPMUL(x,y,z,s) if(__builtin_smull_overflow(x,y,z))s
#define DPMULDDECLS
#define DPMULD(x,y,z,s) if(__builtin_smull_overflow(x,y,&z))s
#endif
#else // C_USEMULTINTRINSIC 0 - use standard-C version (32-bit)
#define DPMULDECLS D _p;
#define DPMUL(x,y,z,s) _p = (D)x*(D)y; *z=(I)_p; if(_p>IMAX||_p<IMIN)s
#define DPMULDDECLS D _p;
#define DPMULD(x,y,z,s) _p = (D)x*(D)y; z=(I)_p; if(_p>IMAX||_p<IMIN)s
#endif

#endif
// end of multiply builtins

// define single+double-recision integer add
#if SY_64
#if defined(_MSC_VER)  // SY_WIN32
#define SPDPADD(addend, sumlo, sumhi) {C c; c=_addcarry_u64(0,addend,sumlo,&sumlo); _addcarry_u64(c,0,sumhi,&sumhi);}
#endif
#endif

#ifndef SPDPADD   // default version for systems without addcarry
#define SPDPADD(addend, sumlo, sumhi) sumlo += addend; sumhi += (addend > sumlo);
#endif
// end of addition builtins

// aligned memory allocation, assume align is power of 2
static __forceinline void* aligned_malloc(size_t size, size_t align) {
 void *result;
 align = (align>=sizeof(void*))?align:sizeof(void*);
#ifdef _WIN32
 result = _aligned_malloc(size, align);
#elif ( !defined(ANDROID) || defined(__LP64__) )
 if(posix_memalign(&result, align, size)) result = 0;
#else
 void *mem = malloc(size+(align-1)+sizeof(void*));
 if(mem){
  result = (void*)((uintptr_t)(mem+(align-1)+sizeof(void*)) & ~(align-1));
  ((void**)result)[-1] = mem;
 } else result = 0;
#endif
 return result;
}

static __forceinline void aligned_free(void *ptr) {
#ifdef _WIN32
 _aligned_free(ptr);
#elif ( !defined(ANDROID) || defined(__LP64__) )
 free(ptr);
#else
 free(((void**)ptr)[-1]);
#endif
}


// Create (x&y) where x and y are signed, so we can test for overflow.
#if defined(_MSC_VER)  // SY_WIN32
#define XANDY(x,y) ((x)&(y))
#else
#define XANDY(x,y) ((I)((UI)(x)&(UI)(y)))
#endif

#if (SY_64 && C_AVX)
#ifdef C_CRC32C
#undef C_CRC32C
#endif
#define C_CRC32C 1
#endif

// Supported in architecture ARMv8.1 and later
#if (C_CRC32C && defined(__aarch64__))
#define CRC32CW(crc, value) __asm__("crc32cw %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(value))
#define CRC32CX(crc, value) __asm__("crc32cx %w[c], %w[c], %x[v]":[c]"+r"(crc):[v]"r"(value))
#define CRC32(crc,value)  ({ uint32_t crci=crc; CRC32CW(crci, value); crci; })
#define CRC32L(crc,value) ({ uint64_t crci=crc; CRC32CX(crci, value); crci; })
#define CRC32LL CRC32L                 // takes UIL (8 bytes), return UI
#endif

// The following definitions are used only in builds for the AVX instruction set
// 64-bit Atom cpu in android has hardware crc32c but not AVX
#if C_CRC32C && (defined(_M_X64) || defined(__x86_64__))
#if defined(_MSC_VER)  // SY_WIN32
// Visual Studio definitions
#define CRC32(x,y) _mm_crc32_u32(x,y)  // takes UI4, returns UI4
#define CRC32L(x,y) _mm_crc32_u64(x,y)  // takes UI, returns UI (top 32 bits 0)
#else
// gcc/clang definition
#define CRC32(x,y) __builtin_ia32_crc32si(x,y)  // returns UI4
#define CRC32L(x,y) __builtin_ia32_crc32di(x,y)  // returns UI
#endif
#define CRC32LL CRC32L                 // takes UIL (8 bytes), return UI
#endif
