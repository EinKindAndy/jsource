/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: Suffix and Outfix                                              */

#include "j.h"
#include "vasm.h"
#include "ve.h"
#define ZZDEFN
#include "result.h"


#define SUFFIXPFX(f,Tz,Tx,pfx)  \
 AHDRS(f,Tz,Tx){I i;Tz v,*y;                                        \
  x+=m*d*n; z+=m*d*n;                                              \
  if(d==1)DO(m, *--z=v=    *--x; DO(n-1, --x; --z; *z=v=pfx(*x,v);))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DO(d, *--z=    *--x;);                                        \
   DO(n-1, DO(d, --x; --y; --z; *z=pfx(*x,*y);));                     \
 }}}

#define SUFFIXNAN(f,Tz,Tx,pfx)  \
 AHDRS(f,Tz,Tx){I i;Tz v,*y;                                        \
  NAN0;                                                               \
  x+=m*d*n; z+=m*d*n;                                              \
  if(d==1)DO(m, *--z=v=    *--x; DO(n-1, --x; --z; *z=v=pfx(*x,v);))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DO(d, *--z=    *--x;);                                        \
   DO(n-1, DO(d, --x; --y; --z; *z=pfx(*x,*y);));                     \
  }}                                                                   \
  NAN1V;                                                              \
 }

#define SUFFICPFX(f,Tz,Tx,pfx)  \
 AHDRS(f,Tz,Tx){I i;Tz v,*y;                                        \
  x+=m*d*n; z+=m*d*n;                                              \
  if(d==1)DO(m, *--z=v=(Tz)*--x; DO(n-1, --x; --z; *z=v=pfx(*x,v);))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DO(d, *--z=(Tz)*--x;);                                        \
   DO(n-1, DO(d, --x; --y; --z; *z=pfx(*x,*y);));                     \
 }}}

#define SUFFIXOVF(f,Tz,Tx,fs1,fvv)  \
 AHDRS(f,I,I){C er=0;I i,*xx,*y,*zz;                      \
  xx=x+=m*d*n; zz=z+=m*d*n;                              \
  if(d==1){                                                 \
   if(1==n)DO(m, *--z=*--x;)                                \
   else    DO(m, z=zz-=d*n; x=xx-=d*n; fs1(n,z,x); RER;)        \
  }else{for(i=0;i<m;++i){                                   \
   DO(d, *--zz=*--xx;);                                     \
   DO(n-1, x=xx-=d; y=zz; z=zz-=d; fvv(d,z,x,y); RER;);     \
 }}}

#if SY_ALIGN
#define SUFFIXBFXLOOP(T,pfx)  \
 {T* RESTRICT xx=(T*)x,* RESTRICT yy,* RESTRICT zz=(T*)z;   \
  q=d/sizeof(T);              \
  DO(m, yy=zz; DO(q, *--zz=*--xx;); DO(n-1, DO(q, --xx; --yy; --zz; *zz=pfx(*xx,*yy);)));  \
 }
  
#define SUFFIXBFX(f,pfx,ipfx,spfx,bpfx,vexp)  \
 AHDRP(f,B,B){B v,* RESTRICT y;I q;                                        \
  x+=m*d*n; z+=m*d*n;                                           \
  if(1==d){DO(m, *--z=v=*--x; DO(n-1, --x; --z; *z=v=vexp;)); R;}  \
  if(0==(d&(sizeof(UI  )-1))){SUFFIXBFXLOOP(UI,   pfx); R;}              \
  if(0==(d&(sizeof(UI4)-1))){SUFFIXBFXLOOP(UINT,ipfx); R;}              \
  if(0==(d&(sizeof(US  )-1))){SUFFIXBFXLOOP(US,  spfx); R;}              \
  DO(m, y=z; DO(d, *--z=*--x;); DO(n-1, DO(d, --x; --y; --z; *z=bpfx(*x,*y);)));  \
 }
#else
#define SUFFIXBFX(f,pfx,ipfx,spfx,bpfx,vexp)  \
 AHDRS(f,B,B){B v;I i,q,r,t,*xi,*yi,*zi;                         \
  x+=m*d*n; z+=m*d*n;                                           \
  if(1==d){DO(m, *--z=v=*--x; DO(n-1, --x; --z; *z=v=vexp;)); R;}  \
  q=d>>LGSZI; r=d&(SZI-1); xi=(I*)x; zi=(I*)z;                            \
  if(0==r)for(i=0;i<m;++i){                                        \
   yi=zi; DO(q, *--zi=*--xi;);                                     \
   DO(n-1, DO(q, --xi; --yi; --zi; *zi=pfx(*xi,*yi);));            \
  }else for(i=0;i<m;++i){                                          \
   yi=zi; DO(q, *--zi=*--xi;);                                     \
   x=(B*)xi; z=(B*)zi; DO(r, *--z=*--x;); xi=(I*)x; zi=(I*)z;      \
   DO(n-1, DO(q, --xi; --yi; --zi; *zi=pfx(*xi,*yi););             \
    xi=(I*)((B*)xi-r);                                             \
    yi=(I*)((B*)yi-r);                                             \
    zi=(I*)((B*)zi-r); t=pfx(*xi,*yi); MC(zi,&t,r););              \
 }}
#endif

SUFFIXBFX(   orsfxB, OR,  IOR,  SOR,  BOR,  *x||v   ) 
SUFFIXBFX(  andsfxB, AND, IAND, SAND, BAND, *x&&v   )
SUFFIXBFX(   eqsfxB, EQ,  IEQ,  SEQ,  BEQ,  *x==v   )
SUFFIXBFX(   nesfxB, NE,  INE,  SNE,  BNE,  *x!=v   )
SUFFIXBFX(   ltsfxB, LT,  ILT,  SLT,  BLT,  *x< v   )
SUFFIXBFX(   lesfxB, LE,  ILE,  SLE,  BLE,  *x<=v   )
SUFFIXBFX(   gtsfxB, GT,  IGT,  SGT,  BGT,  *x> v   )
SUFFIXBFX(   gesfxB, GE,  IGE,  SGE,  BGE,  *x>=v   )
SUFFIXBFX(  norsfxB, NOR, INOR, SNOR, BNOR, !(*x||v))
SUFFIXBFX( nandsfxB, NAND,INAND,SNAND,BNAND,!(*x&&v))

SUFFIXOVF( plussfxI, I, I,  PLUSS, PLUSVV)
SUFFIXOVF(minussfxI, I, I, MINUSS,MINUSVV)
SUFFIXOVF(tymessfxI, I, I, TYMESS,TYMESVV)

SUFFICPFX( plussfxO, D, I, PLUS  )
SUFFICPFX(minussfxO, D, I, MINUS )
SUFFICPFX(tymessfxO, D, I, TYMES )

SUFFIXPFX( plussfxB, I, B, PLUS  )
SUFFIXNAN( plussfxD, D, D, PLUS  )
SUFFIXNAN( plussfxZ, Z, Z, zplus )
SUFFIXPFX( plussfxX, X, X, xplus )
SUFFIXPFX( plussfxQ, Q, Q, qplus )

SUFFIXPFX(minussfxB, I, B, MINUS )
SUFFIXNAN(minussfxD, D, D, MINUS )
SUFFIXNAN(minussfxZ, Z, Z, zminus)

SUFFIXPFX(tymessfxD, D, D, TYMES )
SUFFIXPFX(tymessfxZ, Z, Z, ztymes)
SUFFIXPFX(tymessfxX, X, X, xtymes)
SUFFIXPFX(tymessfxQ, Q, Q, qtymes)

SUFFIXNAN(  divsfxD, D, D, DIV   )
SUFFIXNAN(  divsfxZ, Z, Z, zdiv  )

SUFFIXPFX(  maxsfxI, I, I, MAX   )
SUFFIXPFX(  maxsfxD, D, D, MAX   )
SUFFIXPFX(  maxsfxX, X, X, XMAX  )
SUFFIXPFX(  maxsfxQ, Q, Q, QMAX  )
SUFFIXPFX(  maxsfxS, SB,SB,SBMAX )

SUFFIXPFX(  minsfxI, I, I, MIN   )
SUFFIXPFX(  minsfxD, D, D, MIN   )
SUFFIXPFX(  minsfxX, X, X, XMIN  )
SUFFIXPFX(  minsfxQ, Q, Q, QMIN  )
SUFFIXPFX(  minsfxS, SB,SB,SBMIN )

SUFFIXPFX(bw0000sfxI, UI,UI, BW0000)
SUFFIXPFX(bw0001sfxI, UI,UI, BW0001)
SUFFIXPFX(bw0010sfxI, UI,UI, BW0010)
SUFFIXPFX(bw0011sfxI, UI,UI, BW0011)
SUFFIXPFX(bw0100sfxI, UI,UI, BW0100)
SUFFIXPFX(bw0101sfxI, UI,UI, BW0101)
SUFFIXPFX(bw0110sfxI, UI,UI, BW0110)
SUFFIXPFX(bw0111sfxI, UI,UI, BW0111)
SUFFIXPFX(bw1000sfxI, UI,UI, BW1000)
SUFFIXPFX(bw1001sfxI, UI,UI, BW1001)
SUFFIXPFX(bw1010sfxI, UI,UI, BW1010)
SUFFIXPFX(bw1011sfxI, UI,UI, BW1011)
SUFFIXPFX(bw1100sfxI, UI,UI, BW1100)
SUFFIXPFX(bw1101sfxI, UI,UI, BW1101)
SUFFIXPFX(bw1110sfxI, UI,UI, BW1110)
SUFFIXPFX(bw1111sfxI, UI,UI, BW1111)


static DF1(jtsuffix){DECLF;I r;
 RZ(w);
 r=(RANKT)jt->ranks; RESETRANK; if(r<AR(w))R rank1ex(w,self,r,jtsuffix);
 R eachl(IX(IC(w)),w,atop(fs,ds(CDROP)));
}    /* f\."r w for general f */

static DF1(jtgsuffix){A h,*hv,z,*zv;I m,n,r;
 RZ(w);
 r=(RANKT)jt->ranks; RESETRANK; if(r<AR(w))R rank1ex(w,self,r,jtgsuffix);
 n=IC(w); 
 h=VAV(self)->fgh[2]; hv=AAV(h); m=AN(h);
 GATV(z,BOX,n,1,0); zv=AAV(z); I imod=0;
 DO(n, imod=(imod==m)?0:imod; RZ(zv[i]=df1(drop(sc(i),w),hv[imod])); ++imod;);
 R ope(z);
}    /* g\."r w for gerund g */

#define SSGULOOP(T)  \
 {T*v=(T*)zv;                      \
  for(i=0;i<n1;++i){               \
   RZ(q=CALL2(f2,x,y,fs)); if(!(TYPESEQ(t,AT(q))&&!AR(q)))R A0; /* error if error; abort if not compatible scalar */ \
   *v--=*(T*)AV(q);                \
   AK(x)-=k; AK(y)-=k; tpop(old);  \
 }}

static DF1(jtssg){F1PREFIP;PROLOG(0020);A a,z;I i,k,n,r,wr;
 RZ(w);
 ASSERT(DENSE&AT(w),EVNONCE);
 // loop over rank
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; RESETRANK; if(r<wr)R rank1ex(w,self,r,jtssg);

 // From here on we are doing a single scan
 n=AS(w)[0]; // n=#cells
#define ZZFLAGWORD state
 I state;  // init flags, including zz flags

 A fs=FAV(FAV(self)->fgh[0])->fgh[0]; AF f2=FAV(fs)->valencefns[1]; // self = f/\.   FAV(self)->fgh[0] = f/  FAV(FAV(self)->fgh[0])->fgh[0] = f   fetch dyad for f
 // Set BOXATOP if appropriate.  Since {:y is always the last cell, BOXATOP is allowed only when the rank of w is 1, meaning that
 // {:y is a single box, just like the other results.  Also require that w be boxed, lest we make the first z-cell invalid
 state = ((wr-2)>>(BW-1))&(AT(w)>>(BOXX-ZZFLAGBOXATOPX))&((FAV(fs)->flag2&VF2BOXATOP2)>>(VF2BOXATOP2X-ZZFLAGBOXATOPX));  // If rank OK, extract flag.  Rank cannot be 0.  Don't touch fs yet, since we might not loop
 state &= ~((FAV(fs)->flag2&VF2ATOPOPEN2W)>>(VF2ATOPOPEN2WX-ZZFLAGBOXATOPX));  // We don't handle &.> here; ignore it

 // We cannot honor WILLBEOPENED, because the same box that goes into the result must also be released into the next application of f.
 state |= (-state) & (I)jtinplace & JTCOUNTITEMS; // remember if this verb is followed by ; - only if we BOXATOP, to avoid invalid flag setting at assembly
#define ZZWILLBEOPENEDNEVER 1

 // Allocate virtual block for the running x argument.  UNINCORPABLE, non-inplaceable
 fauxblock(virtafaux); fauxvirtual(a,virtafaux,w,r-1,ACUC1);
 // z will hold the result from the iterations.  Init to value of last cell
 // Since there are multiple cells, z will be in a virtual block (usually)
 RZ(z=tail(w)); k=AN(z)<<bplg(AT(z)); // k=length of input cell in bytes
 // fill in the shape, offset, and item-count of the virtual block
 AN(a)=AN(z); AK(a)+=(n-1)*k; MCISH(AS(a),AS(z),r-1);  // make the virtual block look like the tail, except for the offset.  We start out pointing
   // to the last item; the pointer is unused in the first iteration, and we then back up to the second-last item, which is the first one we
   // process as a

#define ZZPOPNEVER 1   // we mustn't TPOP after copying the result atoms, because they are reused.  This will leave the memory used for type-conversions unclaimed.
   // if we implement the annulment of tpop pointers, we should use that to hand-free results that have been converted
 // We have to dance a bit for BOXATOP verbs, because the result comes back unboxed, but it has to be put into a box
 // to be fed into the next iteration.  This is still a saving, because we can use the same box to point to each successive result.
 // Exception: if the reusable box gets incorporated, it is no longer reusable and must be reallocated.  We will use the original z box,
 // which will NEVER be virtual because it is an atom whenever BOXATOP is set, as the starting pointer to the prev boxed result
 A boxedz = z; z=(state&ZZFLAGBOXATOP)?AAV(z)[0]:z;  // init current pointer for the temp box; if BOXATOP, use >{:y as the first (to-be-boxed) result

#define ZZDECL
#define ZZSTARTATEND 1   // build result from bottom up
#include "result.h"

  ZZPARMS(1,n,2)
#define ZZINSTALLFRAME(optr) *optr++=n;

 AD * RESTRICT zz=0;
 for(i=0;i<n;++i){   // loop through items, noting that the first is the tail itself
  if(i){RZ(z=CALL2(f2,a,z,fs));}   // apply the verb to the arguments (except the first time)
#define ZZBODY
#include "result.h"
  // If BOXATOP, we need to reinstate the boxing around z for the next iteration.
  if(state&ZZFLAGBOXATOP){
   // If boxedz itself has been incorporated into the result, we have to reallocate it.  We don't need the usual check for z==boxedz, because we know we INCORPed z into
   // the boxed result, so if it was the same as boxedz, the usecount of boxedz was incremented then
   if(!ACIPISOK(boxedz))GAT(boxedz,BOX,1,0,0);   // reallocate boxedz if needed
   AAV(boxedz)[0]=z; z=boxedz;  // point boxedz to the previous result, and make that the new argument for next time
  }
  // if result happens to be the same virtual block that we passed in, we have to clone it before we change the pointer
  else if(a==z){RZ(z=virtual(z,0,AR(a))); AN(z)=AN(a); MCISH(AS(z),AS(a),r-1);}

  AK(a)-=k;  // back up to next input
 }
#define ZZEXIT
#include "result.h"
 EPILOG(zz);  // this frees the virtual block, at the least
}    /* f/\."r w for general f and 1<(-r){$w and -.0 e.$w */

A jtscansp(J jt,A w,A self,AF sf){A e,ee,x,z;B*b;I f,m,j,r,t,wr;P*wp,*zp;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; RESETRANK; f=wr-r;
 wp=PAV(w); e=SPA(wp,e); RZ(ee=over(e,e));
 if(!equ(ee,CALL1(sf,ee,self))){
  RZ(x=denseit(w));
  R irs1(x,self,r,sf);
 }else{
  RZ(b=bfi(wr,SPA(wp,a),1));
  if(r&&b[f]){b[f]=0; RZ(w=reaxis(ifb(wr,b),w));}
  j=f; m=0; DO(wr-f, m+=!b[j++];);
 }
 wp=PAV(w); e=SPA(wp,e); x=SPA(wp,x);
 RZ(x=irs1(x,self,m,sf));
 t=maxtype(AT(e),AT(x)); RZ(e=cvt(t,e)); if(TYPESNE(t,AT(x)))RZ(x=cvt(t,x));
 GASPARSE(z,STYPE(t),1,wr+!m,AS(w)); if(!m)*(wr+AS(z))=1;
 zp=PAV(z); 
 SPB(zp,e,e); 
 SPB(zp,x,x); 
 SPB(zp,i,ca(SPA(wp,i))); 
 SPB(zp,a,ca(SPA(wp,a)));
 R z;
}    /* f/\"r or f/\."r on sparse w */

static DF1(jtsscan){A y,z;I d,f,m,n,r,t,wn,wr,*ws,wt;
 RZ(w);F1PREFIP;
 wt=AT(w);
 if(SPARSE&wt)R scansp(w,self,jtsscan);
 wn=AN(w); wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; ws=AS(w); RESETRANK;
 PROD(m,f,ws); PROD1(d,r-1,f+ws+1); n=r?ws[f]:1;  // will not be used if WN==0, so PROD ok.  n is # items along the selected rank
 y=FAV(self)->fgh[0]; // y is f/
 if(2>n||!wn){if(vaid(FAV(y)->fgh[0])){R r?RETARG(w):reshape(over(shape(w),num[1]),w);}else R irs1(w,self,r,jtsuffix);}  // if empty arg, or just 1 cell in selected axis, convert to f/\ which handles the short arg 

   // note that the above line always takes the r==0 case
 VA2 adocv = vasfx(FAV(y)->fgh[0],wt);  // analyze f
 if(!adocv.f)R irs1ip(w,self,r,jtssg);   // if not supported atomically, go do general suffix
 if((t=atype(adocv.cv))&&TYPESNE(t,wt))RZ(w=cvt(t,w));
 if((I)jtinplace&(adocv.cv>>VIPOKWX)&JTINPLACEW && ASGNINPLACE(w))z=w; else GA(z,rtype(adocv.cv),wn,wr,ws);
 adocv.f(jt,m,d,n,AV(z),AV(w));
 if(jt->jerr)R jt->jerr>=EWOV?irs1(w,self,r,jtsscan):0; else R adocv.cv&VRI+VRD?cvz(adocv.cv,z):z;
}    /* f/\."r w main control */


static F2(jtomask){A c,r,x,y;I m,n,p;
 RZ(a&&w);
 RE(m=i0(a)); p=ABS(m); n=IC(w);
 r=sc(0>m?(n+p-1)/p:MAX(0,1+n-m)); c=tally(w);
 x=reshape(sc(p),  num[0]);
 y=reshape(0>m?c:r,num[1] );
 R reshape(over(r,c),over(x,y));
}

static DF2(jtgoutfix){A h,*hv,x,z,*zv;I m,n;
 RZ(x=omask(a,w));
 n=IC(x);
 h=VAV(self)->fgh[2]; hv=AAV(h); m=AN(h);
 GATV(z,BOX,n,1,0); zv=AAV(z); I imod=0;
 DO(n, imod=(imod==m)?0:imod; RZ(zv[i]=df1(repeat(from(sc(i),x),w),hv[i%m])); ++imod;);
 R ope(z);
}

static AS2(jtoutfix, eachl(omask(a,w),w,atop(fs,ds(CPOUND))),0117)

static DF2(jtofxinv){A f,fs,z;C c;I t;V*v;
 F2RANK(0,RMAX,jtofxinv,self);
 fs=FAV(self)->fgh[0]; f=FAV(fs)->fgh[0]; v=FAV(f); c=v->id; t=AT(w);  // self = f/\. fs = f/  f = f  v = verb info for f
 if(!(c==CPLUS||c==CBDOT&&t&INT||(c==CEQ||c==CNE)&&t&B01))R outfix(a,w,self);
 z=irs2(df1(w,fs),df2(a,w,bslash(fs)),VFLAGNONE, RMAX,-1L,c==CPLUS?(AF)jtminus:v->valencefns[1]);
 if(jt->jerr==EVNAN){RESETERR; R outfix(a,w,self);}else R z;
}    /* a f/\. w where f has an "inverse" */

static DF2(jtofxassoc){A f,i,j,p,s,x,z;C id,*zv;I c,d,k,kc,m,r,t;V*v;VA2 adocv;
 F2RANK(0,RMAX,jtofxassoc,self);
 m=IC(w); RE(k=i0(a)); c=ABS(k);  // m = # items in w; k is value of a; c is # items per suffix
 f=FAV(self)->fgh[0]; x=FAV(f)->fgh[0]; v=FAV(x); id=CBDOT==v->id?(C)*AV(v->fgh[0]):v->id;  // self = f/\. f = f/  x = f  v = verb info for f
 if(k==IMIN||m<=c||id==CSTARDOT&&!(B01&AT(w)))R outfix(a,w,self);
 if(-1<=k){d=m-c;     RZ(i=IX(d)); RZ(j=apv(d,c,1L));}
 else     {d=(m-1)/c; RZ(i=apv(d,c-1,c )); RZ(j=apv(d,c,c ));}
 // d is (number of result cells)-1; i is indexes of last item of the excluded infix for cells AFTER the first
 // j is indexes of first item AFTER the excluded infix for cells BEFORE the last
 RZ(p=from(i,df1(w,bslash(f)))); // p is i { u\ w; that is, the totals of the prefixes after the first
 RZ(s=from(j,df1(w,bsdot(f))));  // s is j { u\. w; totals of suffixes except the last
 // We need to make sure that p, s, and (p f s) all have the same type.  This is problematic, since we don't actually see
 // the type of (p f s) which is encoded in cv below.  But since this case is limited to atomic associative verbs, we
 // know that if p and s have the same type, p f s will also, except that it might overflow, which we will detect after we
 // run it.
 //
 // If we modify this code to use this path for other associative verbs, we would need to check the type of (p f s)
 if(!TYPESEQ(AT(p),AT(s))){jt->jerr=EWOV;} else {I klg;  // simulate overflow if different precisions - will convert everything to float
  r=AR(p); c=aii(p); t=AT(p); klg=bplg(t); kc=c<<klg;
  adocv=var(x,t,t); // analyze the u operand
  ASSERTSYS(adocv.f,"ofxassoc");
  GA(z,t,c*(1+d),r,AS(p)); *AS(z)=1+d; zv=CAV(z);  // allocate result assuming no overflow
  MC(zv,     AV(s),          kc);                     // first cell is {.s, i. e. all but the first infix
  if(1<d)adocv.f(jt,1,c*(d-1),1L,zv+kc,AV(p),kc+CAV(s));  /* (}:p) f (}.s), with result stored into the result area */
  MC(zv+kc*d,CAV(p)+kc*(d-1),kc);                     // last cell is {:p, i. e. all but the last infix
  // If there was overflow on the ado, we have to redo the operation as a float.
  // We also have to redo if the types of p and s were different (for example, if one overflowed to float and the other didn't)
 }
 if(jt->jerr>=EWOV){RESETERR; R ofxassoc(a,cvt(FL,w),self);}else R z;
}    /* a f/\. w where f is an atomic associative fn */

static DF1(jtiota1rev){R apv(IC(w),IC(w),-1L);}

F1(jtbsdot){A f;AF f1=jtsuffix,f2=jtoutfix;I flag=FAV(ds(CBSDOT))->flag;C id;V*v;
 RZ(w);
 if(NOUN&AT(w))R fdef(0,CBSLASH,VERB, jtgsuffix,jtgoutfix, w,0L,fxeachv(1L,w), VGERL|VAV(ds(CBSLASH))->flag, RMAX,0L,RMAX);
 v=FAV(w);  // verb info for w
 switch(v->id){
  case CPOUND: f1=jtiota1rev; break;
  case CSLASH:
   f1=jtsscan; flag|=VJTFLGOK1;
   f=v->fgh[0]; id=ID(f); if(id==CBDOT){f=VAV(f)->fgh[0]; if(INT&AT(f)&&!AR(f))id=(C)*AV(f);}
   switch(id){
    case CPLUS:   case CEQ:     case CNE:     case CBW0110:  case CBW1001:               
     f2=jtofxinv;   break;
    case CSTAR:   case CMAX:    case CMIN:    case CPLUSDOT: case CSTARDOT: 
    case CBW0000: case CBW0001: case CBW0011: case CBW0101:  case CBW0111: case CBW1111: 
     f2=jtofxassoc;
 }}
 R ADERIV(CBSDOT,f1,f2,flag,RMAX,0,RMAX);
}
