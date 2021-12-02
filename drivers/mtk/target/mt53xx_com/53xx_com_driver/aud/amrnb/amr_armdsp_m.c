#ifdef LINUX_TURNKEY_SOLUTION

#include    "typedef.h"
#include    "amr_armdsp_m.h"

#if !__TARGET_FEATURE_DSPMUL

INLINE Word32 _saturate(Word64 ret)
{
    if(ret > ARMDSP_WORD32_MAX)
    {
        ret = ARMDSP_WORD32_MAX;
        armdsp_flagdata_AMR.armdsp_bitfields.armdsp_q = 1;
    }
    else if(ret < ARMDSP_WORD32_MIN)
    {
        ret = ARMDSP_WORD32_MIN;
        armdsp_flagdata_AMR.armdsp_bitfields.armdsp_q = 1;
    }
    return (Word32)ret;
}
#endif

INLINE Word32 qadd(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        qadd ret, x, y;
    }
#else
    Word64 r64;
    r64 = (Word64)x + (Word64)y;
    ret = _saturate(r64);
#endif
    return ret;
}

INLINE Word32 qsub(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        qsub ret, x, y;
    }
#else
    Word64 r64;
    r64 = (Word64)x - (Word64)y;
    ret = _saturate(r64);
#endif
    return ret;
}

INLINE Word32 qdadd(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        qdadd ret, x, y;
    }
#else
    Word64 r64;
    r64 = (Word64)x + 2*(Word64)y;
    ret = _saturate(r64);
#endif
    return ret;
}

INLINE Word32 qdsub(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        qdsub ret, x, y;
    }
#else
    Word64 r64;
    r64 = (Word64)x - 2*(Word64)y;
    ret = _saturate(r64);
#endif
    return ret;
}

INLINE Word32 smulbb(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smulbb ret, x, y;
    }
#else
    x = (x << 16) >> 16;
    y = (y << 16) >> 16;
    ret = x * y;
#endif
    return ret;
}

INLINE Word32 smulbt(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smulbt ret, x, y;
    }
#else
    x = (x << 16) >> 16;
    y = y >> 16;
    ret = x * y;
#endif
    return ret;
}

INLINE Word32 smultb(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smultb ret, x, y;
    }
#else
    x = x >> 16;
    y = (y << 16) >> 16;
    ret = x * y;
#endif
    return ret;
}

INLINE Word32 smultt(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smultt ret, x, y;
    }
#else
    x = x >> 16;
    y = y >> 16;
    ret = x * y;
#endif
    return ret;
}

INLINE Word32 smlabb(Word32 x, Word32 y, Word32 a)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlabb ret, x, y, a;
    }
#else
    Word64 L64;
    x = (x << 16) >> 16;
    y = (y << 16) >> 16;
    L64 = (Word64)x * (Word64)y + (Word64)a;
    (void)_saturate( L64 );
    ret = (Word32)L64;
#endif
    return ret;
}

INLINE Word32 smlabt(Word32 x, Word32 y, Word32 a)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlabt ret, x, y, a;
    }
#else
    Word64 L64;
    x = (x << 16) >> 16;
    y = y >> 16;
    L64 = (Word64)x * (Word64)y + (Word64)a;
    (void)_saturate( L64 );
    ret = (Word32)L64;
#endif
    return ret;
}

INLINE Word32 smlatb(Word32 x, Word32 y, Word32 a)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlatb ret, x, y, a;
    }
#else
    Word64 L64;
    x = x >> 16;
    y = (y << 16) >> 16;
    L64 = (Word64)x * (Word64)y + (Word64)a;
    (void)_saturate( L64 );
    ret = (Word32)L64;
#endif
    return ret;
}

INLINE Word32 smlatt(Word32 x, Word32 y, Word32 a)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlatt ret, x, y, a;
    }
#else
    Word64 L64;
    x = x >> 16;
    y = y >> 16;
    L64 = (Word64)x * (Word64)y + (Word64)a;
    (void)_saturate( L64 );
    ret = (Word32)L64;
#endif
    return ret;
}

INLINE void smlalbb(Word32 *lo, Word32 *hi, Word32 x, Word32 y)
{
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlalbb *lo, *hi, x, y;
    }
#else
    Word64 ret;
    lo64(ret) = *lo;
    hi64(ret) = *hi;
    x = (x << 16) >> 16;
    y = (y << 16) >> 16;
    ret = x * y + ret;
    *lo = lo64(ret);
    *hi = hi64(ret);
#endif
}

INLINE void smlalbt(Word32 *lo, Word32 *hi, Word32 x, Word32 y)
{
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlalbt *lo, *hi, x, y;
    }
#else
    Word64 ret;
    lo64(ret) = *lo;
    hi64(ret) = *hi;
    x = (x << 16) >> 16;
    y = y >> 16;
    ret = x * y + ret;
    *lo = lo64(ret);
    *hi = hi64(ret);
#endif
}

INLINE void smlaltb(Word32 *lo, Word32 *hi, Word32 x, Word32 y)
{
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlaltb *lo, *hi, x, y;
    }
#else
    Word64 ret;
    lo64(ret) = *lo;
    hi64(ret) = *hi;
    x = x >> 16;
    y = (y << 16) >> 16;
    ret = x * y + ret;
    *lo = lo64(ret);
    *hi = hi64(ret);
#endif
}

INLINE void smlaltt(Word32 *lo, Word32 *hi, Word32 x, Word32 y)
{
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlaltt *lo, *hi, x, y;
    }
#else
    Word64 ret;
    lo64(ret) = *lo;
    hi64(ret) = *hi;
    x = x >> 16;
    y = y >> 16;
    ret = x * y + ret;
    *lo = lo64(ret);
    *hi = hi64(ret);
#endif
}

INLINE Word32 smulwb(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smulwb ret, x, y;
    }
#else
    Word64 ret64;
    y = (y << 16) >> 16;
    ret64 = (Word64)x * (Word64)y;
    ret = (Word32)(ret64 >> 16);
#endif
    return ret;
}

INLINE Word32 smulwt(Word32 x, Word32 y)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smulwt ret, x, y;
    }
#else
    Word64 ret64;
    y = y >> 16;
    ret64 = (Word64)x * (Word64)y;
    ret = (Word32)(ret64 >> 16);
#endif
    return ret;
}

INLINE Word32 smlawb(Word32 x, Word32 y, Word32 a)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlawb ret, x, y, a;
    }
#else
    Word64 ret64;
    y = (y << 16) >> 16;
    ret64 = (Word64)x * (Word64)y;
    ret = (Word32)(ret64 >> 16) + a;
#endif
    return ret;
}

/* smlawt - multiply x by the top half of y */
INLINE Word32 smlawt(Word32 x, Word32 y, Word32 a)
{
    Word32 ret;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlawt ret, x, y, a;
    }
#else
    Word64 ret64;
    y = y >> 16;
    ret64 = (Word64)x * (Word64)y;
    ret = (Word32)(ret64 >> 16) + a;
#endif
    return ret;
}

INLINE Word32 smull(Word32 x, Word32 y)
{
#if __TARGET_FEATURE_DSPMUL
    Word32 a, b;
    __asm {
        smull a, b, x, y;
    }
    return b;
#else
    Word64 ret = (Word64)x * (Word64)y;
    return((Word32)hi64(ret));
#endif
}

INLINE Word64 smlal(Word64 LL_acc, Word32 x, Word32 y)
{
#if __TARGET_FEATURE_DSPMUL
    __asm {
        smlal lo64(LL_acc), hi64(LL_acc), x, y;
    }
#else
    LL_acc += (Word64)x * (Word64)y;
#endif
    return LL_acc;
}

INLINE Word32 clz(Word32 absx)
{
    Word32 lz;
#if __TARGET_FEATURE_DSPMUL
    __asm {
        clz lz, absx;
    }
#else
    Word32 j = 1 << 31;
    for(lz=0; lz<32; lz++, j>>=1)
      if( absx&j ) break;
#endif
    return lz;
}

INLINE void msr_cpsr_f_0(void)
{
#if __TARGET_FEATURE_DSPMUL
    __asm {
        MSR CPSR_f, #0;
    }
#endif
}

#endif

