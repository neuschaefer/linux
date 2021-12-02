#ifdef LINUX_TURNKEY_SOLUTION

#include    "typedef.h"
#include    "armdsp_gcc.h"

/*
 * Saturating 32-bit integer addition.
 */
_PREFIX Word32_gcc AWB_qadd(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "qadd %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    Word64_gcc r64;
    r64 = (Word64_gcc)x + (Word64_gcc)y;
    ret = AWB_saturate(r64);
#endif
    return ret;
}

/*
 * Saturating 32-bit integer subtraction.
 */
_PREFIX Word32_gcc AWB_qsub(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "qsub %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    Word64_gcc r64;
    r64 = (Word64_gcc)x - (Word64_gcc)y;
    ret = AWB_saturate(r64);
#endif
    return ret;
}

/*
 * Saturating 32-bit integer addition, with the second operand
 * multiplied by two. (i.e. return x + 2 * y)
 */
_PREFIX Word32_gcc AWB_qdadd(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "qdadd %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    Word64_gcc r64;
    r64 = (Word64_gcc)x + 2*(Word64_gcc)y;
    ret = AWB_saturate(r64);
#endif
    return ret;
}

/*
 * Saturating 32-bit integer subtraction, with the second operand
 * multiplied by two. (i.e. return x - 2 * y)
 */
_PREFIX Word32_gcc AWB_qdsub(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "qdsub %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    Word64_gcc r64;
    r64 = (Word64_gcc)x - 2*(Word64_gcc)y;
    ret = AWB_saturate(r64);
#endif
    return ret;
}

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words. For example, smulbt(x,y) multiplies
 * the bottom 16 bits of x with the top 16 bits of y.
 */

/* smulbb - multiply bottom half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smulbb(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smulbb %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    x = (x << 16) >> 16;
    y = (y << 16) >> 16;
    ret = x * y;
#endif
    return ret;
}

/* smulbt - multiply bottom half of x and top half of y */
_PREFIX Word32_gcc AWB_smulbt(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smulbt %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    x = (x << 16) >> 16;
    y = y >> 16;
    ret = x * y;
#endif
    return ret;
}

/* smultb - multiply top half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smultb(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smultb %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    x = x >> 16;
    y = (y << 16) >> 16;
    ret = x * y;
#endif
    return ret;
}

/* smultt - multiply top half of x and top half of y */
_PREFIX Word32_gcc AWB_smultt(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smultt %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    x = x >> 16;
    y = y >> 16;
    ret = x * y;
#endif
    return ret;
}

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words x and y, and add to the input word a.
 * For example, smlabt(x,y,a) multiplies the bottom 16 bits of x with
 * the top 16 bits of y, and then adds a.
 */

/* smlabb - multiply bottom half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smlabb(Word32_gcc x, Word32_gcc y, Word32_gcc a)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlabb %[ret], %[x], %[y], %[a] \n"
    : [ret]"=r"(ret)
    : [x]"%r"(x), [y]"r"(y), [a]"r"(a) );
#else
    Word64_gcc L64;
    x = (x << 16) >> 16;
    y = (y << 16) >> 16;
    L64 = (Word64_gcc)x * (Word64_gcc)y + (Word64_gcc)a;
    (void)AWB_saturate( L64 );
    ret = (Word32_gcc)L64;
#endif
    return ret;
}

/* smlabt - multiply bottom half of x and top half of y */
_PREFIX Word32_gcc AWB_smlabt(Word32_gcc x, Word32_gcc y, Word32_gcc a)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlabt %[ret], %[x], %[y], %[a] \n"
    : [ret]"=r"(ret)
    : [x]"%r"(x), [y]"r"(y), [a]"r"(a) );
#else
    Word64_gcc L64;
    x = (x << 16) >> 16;
    y = y >> 16;
    L64 = (Word64_gcc)x * (Word64_gcc)y + (Word64_gcc)a;
    (void)AWB_saturate( L64 );
    ret = (Word32_gcc)L64;
#endif
    return ret;
}

/* smlatb - multiply top half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smlatb(Word32_gcc x, Word32_gcc y, Word32_gcc a)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlatb %[ret], %[x], %[y], %[a] \n"
    : [ret]"=r"(ret)
    : [x]"%r"(x), [y]"r"(y), [a]"r"(a) );
#else
    Word64_gcc L64;
    x = x >> 16;
    y = (y << 16) >> 16;
    L64 = (Word64_gcc)x * (Word64_gcc)y + (Word64_gcc)a;
    (void)AWB_saturate( L64 );
    ret = (Word32_gcc)L64;
#endif
    return ret;
}

/* smlatt - multiply top half of x and top half of y */
_PREFIX Word32_gcc AWB_smlatt(Word32_gcc x, Word32_gcc y, Word32_gcc a)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlatt %[ret], %[x], %[y], %[a] \n"
    : [ret]"=r"(ret)
    : [x]"%r"(x), [y]"r"(y), [a]"r"(a) );
#else
    Word64_gcc L64;
    x = x >> 16;
    y = y >> 16;
    L64 = (Word64_gcc)x * (Word64_gcc)y + (Word64_gcc)a;
    (void)AWB_saturate( L64 );
    ret = (Word32_gcc)L64;
#endif
    return ret;
}

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words x and y, and add to the 64-bit integer
 * stored in the input words lo and hi. For example,
 * smlalbt(&lo,&hi,x,y) multiplies the bottom 16 bits of x with the
 * top 16 bits of y, and then adds the result to (hi,lo).
 *
 * WARNING: if you use these primitives too often inside the same
 * function, they may fail to be inlined.
 */

/* smlalbb - multiply bottom half of x and bottom half of y */
_PREFIX void AWB_smlalbb(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y)
{
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlalbb %[lo], %[hi], %[x], %[y] \n"
    : [lo]"=r"(*lo), [hi]"=r"(*hi)
    : [x]"%r"(x), [y]"r"(y) );
#else
    Word64_gcc ret;
    ret = *lo;
    ret+= (long long)*hi<<32;
    x = (x << 16) >> 16;
    y = (y << 16) >> 16;
    ret = x * y + ret;
    *lo = (ret<<32)>>32;
    *hi = ret>>32;
#endif
}

/* smlalbt - multiply bottom half of x and top half of y */
_PREFIX void AWB_smlalbt(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y)
{
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlalbt %[lo], %[hi], %[x], %[y] \n"
    : [lo]"=r"(*lo), [hi]"=r"(*hi)
    : [x]"%r"(x), [y]"r"(y) );
#else
    Word64_gcc ret;
    ret = *lo;
    ret+= (long long)*hi<<32;
    x = (x << 16) >> 16;
    y = y >> 16;
    ret = x * y + ret;
    *lo = (ret<<32)>>32;
    *hi = ret>>32;
#endif
}

/* smlaltb - multiply top half of x and bottom half of y */
_PREFIX void AWB_smlaltb(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y)
{
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlaltb %[lo], %[hi], %[x], %[y] \n"
    : [lo]"=r"(*lo), [hi]"=r"(*hi)
    : [x]"%r"(x), [y]"r"(y) );
#else
    Word64_gcc ret;
    ret = *lo;
    ret+= (long long)*hi<<32;
    x = x >> 16;
    y = (y << 16) >> 16;
    ret = x * y + ret;
    *lo = (ret<<32)>>32;
    *hi = ret>>32;
#endif
}

/* smlaltt - multiply top half of x and top half of y */
_PREFIX void AWB_smlaltt(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y)
{
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlaltt %[lo], %[hi], %[x], %[y] \n"
    : [lo]"=r"(*lo), [hi]"=r"(*hi)
    : [x]"%r"(x), [y]"r"(y) );
#else
    Word64_gcc ret;
    ret = *lo;
    ret+= (long long)*hi<<32;
    x = x >> 16;
    y = y >> 16;
    ret = x * y + ret;
    *lo = (ret<<32)>>32;
    *hi = ret>>32;
#endif
}

/*
 * Multiply a 32-bit signed integer (the input word x) with a
 * 16-bit signed integer (taken from either the top or the bottom
 * half of the input word y). Discard the bottom 16 bits of the
 * 48-bit product.
 */

/* smulwb - multiply x by the bottom half of y */
_PREFIX Word32_gcc AWB_smulwb(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smulwb %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    Word64_gcc ret64;
    y = (y << 16) >> 16;
    ret64 = (Word64_gcc)x * (Word64_gcc)y;
    ret = (Word32_gcc)(ret64 >> 16);
#endif
    return ret;
}

/* smulwt - multiply x by the top half of y */
_PREFIX Word32_gcc AWB_smulwt(Word32_gcc x, Word32_gcc y)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smulwt %[ret], %[x], %[y] \n"
       : [ret]"=r"(ret)
       : [x]"%r"(x), [y]"r"(y));
#else
    Word64_gcc ret64;
    y = y >> 16;
    ret64 = (Word64_gcc)x * (Word64_gcc)y;
    ret = (Word32_gcc)(ret64 >> 16);
#endif
    return ret;
}

/*
 * Multiply a 32-bit signed integer (the input word x) with a
 * 16-bit signed integer (taken from either the top or the bottom
 * half of the input word y). Discard the bottom 16 bits of the
 * 48-bit product, and add to the input word a.
 */

/* smlawb - multiply x by the bottom half of y */
_PREFIX Word32_gcc AWB_smlawb(Word32_gcc x, Word32_gcc y, Word32_gcc a)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlawb %[ret], %[x], %[y], %[a] \n"
    : [ret]"=r"(ret)
    : [x]"%r"(x), [y]"r"(y), [a]"r"(a) );
#else
    Word64_gcc ret64;
    y = (y << 16) >> 16;
    ret64 = (Word64_gcc)x * (Word64_gcc)y;
    ret = (Word32_gcc)(ret64 >> 16) + a;
#endif
    return ret;
}

/* smlawt - multiply x by the top half of y */
_PREFIX Word32_gcc AWB_smlawt(Word32_gcc x, Word32_gcc y, Word32_gcc a)
{
    Word32_gcc ret;
#if defined(__GNUC__) && defined(__arm__)
    asm( "smlawt %[ret], %[x], %[y], %[a] \n"
    : [ret]"=r"(ret)
    : [x]"%r"(x), [y]"r"(y), [a]"r"(a) );
#else
    Word64_gcc ret64;
    y = y >> 16;
    ret64 = (Word64_gcc)x * (Word64_gcc)y;
    ret = (Word32_gcc)(ret64 >> 16) + a;
#endif
    return ret;
}

INLINE Word32_gcc AWB_smull(Word32_gcc x, Word32_gcc y)
{
    Word64_gcc ret = (Word64_gcc)x * (Word64_gcc)y;
    return ret>>32;
}

INLINE Word32_gcc AWB_umull(UWord32_gcc x,UWord32_gcc y)
{
    Word64_gcc ret = (Word64_gcc)x * (Word64_gcc)y;
    return ret>>32;
}

_PREFIX Word64_gcc AWB_smlal(Word64_gcc LL_acc, Word32_gcc x, Word32_gcc y)
{
    LL_acc += (Word64_gcc)x * (Word64_gcc)y;
    return LL_acc;
}

_PREFIX Word32_gcc AWB_clz(Word32_gcc absx)
{
    Word32_gcc lz;
#if defined(__GNUC__) && defined(__arm__)
    asm( "clz %[lz], %[absx] \n"
    : [lz]"=r"(lz)
    : [absx]"r"(absx));
#else
    Word32_gcc j = 1 << 31;
    for(lz=0; lz<32; lz++, j>>=1)
        {  if( absx&j )
                  break;
        }
#endif
    return lz;
}

_PREFIX unsigned int AWB_EndianByteReverse(unsigned int temp)
{
      unsigned int tmp = 0;
      tmp |= temp >> 24;
      tmp |= (temp >> 8) & 0x0000ff00;
      tmp |= (temp << 8) & 0x00ff0000;
      tmp |= temp << 24;
      return tmp;
}

#endif

