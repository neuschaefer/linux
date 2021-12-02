#ifdef LINUX_TURNKEY_SOLUTION

#include    "awb_armdsp_m.h"
#include    "awb_dspfns_m.h"

/*
 * Convert a 32-bit signed integer into a 16-bit signed integer by
 * saturation.
 */
INLINE Word16 AWB_sature(Word32 x)
{
    if (x >= (ARMDSP_WORD16_MAX+1))
        /*Overflow = 1, */x = ARMDSP_WORD16_MAX;
    else if (x < ARMDSP_WORD16_MIN)
        /*Overflow = 1, */x = ARMDSP_WORD16_MIN;
    return (Word16) x;
}

/*
 * Add two 16-bit signed integers with saturation.
 */
INLINE Word16 AWB_add(Word32 x, Word32 y)
{
#ifdef __NO_ARM_ASM__
    return( AWB_qadd( x << 16, y << 16 ) >> 16 );
#else
    Word32 xs, ys, rs;
    __asm {
        mov xs, x, lsl #16;
        mov ys, y, lsl #16;
        qadd rs, xs, ys;
    }
    return (Word16) (rs >> 16);
#endif
}

/*
 * Subtract one 16-bit signed integer from another with saturation.
 */
INLINE Word16 AWB_sub(Word32 x, Word32 y)
{
#ifdef __NO_ARM_ASM__
    return( AWB_qsub( x << 16, y << 16 ) >> 16 );
#else
    Word32 xs, ys, rs;
    __asm {
        mov xs, x, lsl #16;
        mov ys, y, lsl #16;
        qsub rs, xs, ys;
    }
    return (Word16) (rs >> 16);
#endif
}

/*
 * Absolute value of a 16-bit signed integer. Saturating, so
 * abs(-0x8000) becomes +0x7FFF.
 */
INLINE Word16 AWB_abs_s(Word32 x)
{
    if (x >= 0)
        return x;
    else if (x == ARMDSP_WORD16_MIN)
        return ARMDSP_WORD16_MAX;
    else
        return (Word16) -x;
}

/*
 * Shift a 16-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word16 AWB_shl(Word32 x, Word32 shift)
{
    if (shift <= 0 || x == 0)
        return (Word16) (x >> (-shift));
    if (shift > 15)
        shift = 16;
    return AWB_sature(x << shift);
}

/*
 * Shift a 16-bit signed integer right (or left, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word16 AWB_shr(Word32 x, Word32 shift)
{
    if (shift >= 0 || x == 0)
        return (Word16) (x >> shift);
    if (shift < -15)
        shift = -16;
    return AWB_sature(x << (-shift));
}

/*
 * Multiply two 16-bit signed integers, shift the result right by
 * 15 and saturate it. (Saturation is only necessary if both inputs
 * were -0x8000, in which case the result "should" be 0x8000 and is
 * saturated to 0x7FFF.)
 */
INLINE Word16 AWB_mult(Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = AWB_smulbb(x, y);
    return( AWB_qadd(product, product) >> 16 );
#else
    __asm {
        smulbb product, x, y;
        qadd product, product, product;
    }
    return (Word16) (product >> 16);   /* the qadd added one to the 15 */
#endif
}

/*
 * Multiply two 16-bit signed integers to give a 32-bit signed
 * integer. Shift left by one, and saturate the result. (Saturation
 * is only necessary if both inputs were -0x8000, in which case the
 * result "should" be 0x40000000 << 1 = +0x80000000, and is
 * saturated to +0x7FFFFFFF.)
 */
INLINE Word32 AWB_L_mult(Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = AWB_smulbb(x, y);
    return( AWB_qadd(product, product) );
#else
    __asm {
        smulbb product, x, y;
        qadd product, product, product;
    }
    return product;
#endif
}

/*
 * Negate a 16-bit signed integer, with saturation. (Saturation is
 * only necessary when the input is -0x8000.)
 */
INLINE Word16 AWB_negate(Word32 x)
{
    if (x == ARMDSP_WORD16_MIN)
        return ARMDSP_WORD16_MAX;
    return (Word16) -x;
}

/*
 * Return the top 16 bits of a 32-bit signed integer.
 */
INLINE Word16 AWB_extract_h(Word32 x)
{
    return (Word16)(x >> 16);
}

/*
 * Return the bottom 16 bits of a 32-bit signed integer, with no
 * saturation, just coerced into a two's complement 16 bit
 * representation.
 */
INLINE Word16 AWB_extract_l(Word32 x)
{
    return (Word16) x;
}

/*
 * Divide a 32-bit signed integer by 2^16, rounding to the nearest
 * integer (round up on a tie). Equivalent to adding 0x8000 with
 * saturation, then shifting right by 16.
 */
INLINE Word32 AWB_round(Word32 x)
{
#ifdef __NO_ARM_ASM__
    //x = qadd(x, 0x8000);
    Word16 var_out;
    Word32 L_rounded;

    L_rounded = AWB_L_add (x, (Word32) 0x00008000L);
    var_out = AWB_extract_h (L_rounded);
    return (var_out);
#else
    __asm {
        qadd x, x, 0x8000;
    }
#endif
    return AWB_extract_h(x);
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and add to
 * another 32-bit integer with saturation.
 *
 * Note the intermediate saturation operation in the definition:
 *
 *    L_mac(-1, -0x8000, -0x8000)
 *
 * will give 0x7FFFFFFE and not 0x7FFFFFFF:
 *    the unshifted product is:   0x40000000
 *    shift left with saturation: 0x7FFFFFFF
 *    add to -1 with saturation:  0x7FFFFFFE
 */
INLINE Word32 AWB_L_mac(Word32 accumulator, Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = AWB_smulbb(x, y);
    accumulator = AWB_qdadd(accumulator, product);
#else
    __asm {
        smulbb product, x, y;
        qdadd accumulator, accumulator, product;
    }
#endif
    return accumulator;
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and subtract
 * from another 32-bit integer with saturation.
 *
 * Note the intermediate saturation operation in the definition:
 *
 *    L_msu(1, -0x8000, -0x8000)
 *
 * will give 0x80000002 and not 0x80000001:
 *    the unshifted product is:         0x40000000
 *    shift left with saturation:       0x7FFFFFFF
 *    subtract from 1 with saturation:  0x80000002
 */
INLINE Word32 AWB_L_msu(Word32 accumulator, Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = AWB_smulbb(x, y);
    accumulator = AWB_qdsub(accumulator, product);
#else
    __asm {
        smulbb product, x, y;
        qdsub accumulator, accumulator, product;
    }
#endif
    return accumulator;
}

/*
 * Add two 32-bit signed integers with saturation.
 */
INLINE Word32 AWB_L_add(Word32 x, Word32 y)
{
    Word32 result;
#ifdef __NO_ARM_ASM__
    //result = qadd(x, y);
    result = x + y;

    if (((x ^ y) & ARMDSP_WORD32_MIN) == 0)
    {
        if ((result ^ x) & ARMDSP_WORD32_MIN)
        {
            result = (x < 0) ? ARMDSP_WORD32_MIN : ARMDSP_WORD32_MAX;
        }
    }
#else
    __asm {
        qadd result, x, y;
    }
#endif
    return result;
}

/*
 * Subtract one 32-bit signed integer from another with saturation.
 */
INLINE Word32 AWB_L_sub(Word32 x, Word32 y)
{
    Word32 result;
#ifdef __NO_ARM_ASM__
    result = AWB_qsub(x, y);
#else
    __asm {
        qsub result, x, y;
    }
#endif
    return result;
}

#if 0
/*
 * Add together the Carry variable and two 32-bit signed integers,
 * without saturation.
 */
__inline Word32 AWB_L_add_c(Word32 x, Word32 y)
{
    Word32 result, flags;

    __asm {
        msr CPSR_f, armdsp_flagdata_G729.armdsp_flags_word;
        adcs result, x, y;
        mrs flags, CPSR;
        orrvs flags, flags, #0x08000000; // set Q if V is set
        mov armdsp_flagdata_G729.armdsp_flags_word, flags;
    }
    return result;
}

/*
 * Subtract one 32-bit signed integer, together with the Carry
 * variable, from another 32-bit signed integer, without
 * saturation.
 */
__inline Word32 AWB_L_sub_c(Word32 x, Word32 y)
{
    Word32 result, flags;

    __asm {
        msr CPSR_f, armdsp_flagdata_G729.armdsp_flags_word;
        sbcs result, x, y;
        mrs flags, CPSR;
        orrvs flags, flags, #0x08000000; // set Q if V is set
        mov armdsp_flagdata_G729.armdsp_flags_word, flags;
    }
    return result;
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one _with_ saturation, and add
 * with carry to another 32-bit integer _without_ saturation.
 */
__inline Word32 AWB_L_macNs(Word32 accumulator, Word32 x, Word32 y)
{
    return AWB_L_add_c(accumulator, AWB_L_mult(x, y));
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one _with_ saturation, and
 * subtract with carry from another 32-bit integer _without_
 * saturation.
 */
__inline Word32 AWB_L_msuNs(Word32 accumulator, Word32 x, Word32 y)
{
    return AWB_L_sub_c(accumulator, AWB_L_mult(x, y));
}
#endif

/*
 * Negate a 32-bit signed integer, with saturation. (Saturation is
 * only necessary when the input is -0x80000000.)
 */
INLINE Word32 AWB_L_negate(Word32 x)
{
    if (x == ARMDSP_WORD32_MIN)
        return ARMDSP_WORD32_MAX;
    return -x;
}

/*
 * Multiply two 16-bit signed integers, shift the result right by
 * 15 with rounding, and saturate it. (Saturation is only necessary
 * if both inputs were -0x8000, in which case the result "should"
 * be 0x8000 and is saturated to 0x7FFF.)
 */
INLINE Word16 AWB_mult_r(Word32 x, Word32 y)
{
    Word32 product;
#ifdef __NO_ARM_ASM__
    product = AWB_qdadd(0x8000, AWB_smulbb(x, y));
#else
    __asm {
        smulbb product, x, y;
        qdadd product, 0x8000, product;
    }
#endif
    return (Word16) (product >> 16);   /* qdadd adds one to the 15 */
}

/*
 * Shift a 32-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word32 AWB_L_shl(Word32 x, Word32 shift)
{
    int lz;
    int absx;
    if (shift <= 0)
        return x >> (-shift);

///    absx = (x < 0 ? -x : x);
		absx = x ^ (x>>31);

#ifdef __NO_ARM_ASM__
    lz = AWB_clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (shift < lz || x == 0)
        return x << shift;
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}

/*
 * Shift a 32-bit signed integer left (or right, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word32 AWB_L_shl_p(Word32 x, Word32 shift)
{
    int lz;
    int absx;

	absx = x ^ (x>>31);

#ifdef __NO_ARM_ASM__
    lz = AWB_clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (shift < lz || x == 0)
        return x << shift;
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}


/*
 * Shift a 32-bit signed integer right (or left, if the shift count
 * is negative). Saturate on overflow.
 */
INLINE Word32 AWB_L_shr(Word32 x, Word32 shift)
{
    int lz;
    int absx;
#if __CC_ARM
    if (shift >= 0)
        return x >> shift;
#else
    if (shift >= 31) {
        return 0;
    } else if (shift >= 0) {
        return x >> shift;
    }
#endif

///    absx = (x < 0 ? -x : x);
		absx = x ^ (x>>31);

#ifdef __NO_ARM_ASM__
    lz = AWB_clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (-shift < lz || x == 0)
        return x << (-shift);
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}

/*
 * Shift a 16-bit signed integer right, with rounding. Shift left
 * with saturation if the shift count is negative.
 */
INLINE Word16 AWB_shr_r(Word32 x, Word32 shift)
{
    if (shift == 0 || x == 0)
        return (Word16) (x >> shift);
    if (shift > 0)
        return (Word16) (((x >> (shift-1)) + 1) >> 1);
    if (shift < -15)
        shift = -16;
    return AWB_sature(x << (-shift));
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and add to
 * another 32-bit integer with saturation (like AWB_L_mac). Then shift
 * the result right by 15 bits with rounding (like round).
 */
INLINE Word16 AWB_mac_r(Word32 accumulator, Word32 x, Word32 y)
{
    return AWB_round(AWB_L_mac(accumulator, x, y));
}

/*
 * Multiply two 16-bit signed integers together to give a 32-bit
 * signed integer, shift left by one with saturation, and subtract
 * from another 32-bit integer with saturation (like AWB_L_msu). Then
 * shift the result right by 15 bits with rounding (like round).
 */
INLINE Word16 AWB_msu_r(Word32 accumulator, Word32 x, Word32 y)
{
    return AWB_round(AWB_L_msu(accumulator, x, y));
}

/*
 * Shift a 16-bit signed integer left by 16 bits to generate a
 * 32-bit signed integer. The bottom 16 bits are zeroed.
 */
INLINE Word32 AWB_L_deposit_h(Word32 x)
{
    return x << 16;
}

/*
 * Sign-extend a 16-bit signed integer by 16 bits to generate a
 * 32-bit signed integer.
 */
INLINE Word32 AWB_L_deposit_l(Word32 x)
{
    return x;
}

/*
 * Shift a 32-bit signed integer right, with rounding. Shift left
 * with saturation if the shift count is negative.
 */
INLINE Word32 AWB_L_shr_r(Word32 x, Word32 shift)
{
    int lz;
    int absx;
    if (shift == 0 || x == 0)
        return x >> shift;
    if (shift > 0) {
        Word32 x2 = x >> (shift-1);
        return (x2 >> 1) + (x2 & 1);
    }

///    absx = (x < 0 ? -x : x);
		absx = x ^ (x>>31);


#ifdef __NO_ARM_ASM__
    lz = AWB_clz(absx);
#else
    __asm {
        clz lz, absx;
    }
#endif
    if (-shift < lz || x == 0)
        return x << (-shift);
    if (x < 0)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}

/*
 * Absolute value of a 32-bit signed integer. Saturating, so
 * abs(-0x80000000) becomes +0x7FFFFFFF.
 */
INLINE Word32 AWB_L_abs(Word32 x)
{
    if (x >= 0)
        return x;
    else if (x == ARMDSP_WORD32_MIN)
        return ARMDSP_WORD32_MAX;
    else
        return -x;

}

/*
 * Return a saturated value appropriate to the most recent carry-
 * affecting operation (L_add_c, L_macNs, L_sub_c, L_msuNs).
 *
 * In other words: return the argument if the Q flag is clear.
 * Otherwise, return -0x80000000 or +0x7FFFFFFF depending on
 * whether the Carry flag is set or clear (respectively).
 */
 #if 0
__inline Word32 L_sat(Word32 x)
{
    int qflag;
    __asm {
        mrs qflag, CPSR;
    }
    if (!(qflag & 0x08000000))
        return x;
    if (Carry)
        return ARMDSP_WORD32_MIN;
    else
        return ARMDSP_WORD32_MAX;
}
 #endif

/*
 * Return the number of bits of left shift needed to arrange for a
 * 16-bit signed integer to have value >= 0x4000 or <= -0x4001.
 *
 * Returns zero if x is zero.
 */
INLINE Word32 AWB_norm_s(Word32 x)
{
    int lz;

    if (x == 0)
        return 0;                      /* 0 is a special case */

    x = (Word16) (x ^ (x >> 15));      /* invert x if it's negative */
#ifdef __NO_ARM_ASM__
    lz = AWB_clz(x);
#else
    __asm {
        clz lz, x;
    }
#endif
    return (lz - 17);
}

/*
 * Divide one 16-bit signed integer by another, and produce a
 * 15-bit fixed point fractional result (by multiplying the true
 * mathematical result by 0x8000). The divisor (denominator) is
 * assumed to be non-zero and also assumed to be greater or equal
 * to the dividend (numerator). Hence the (unscaled) result is
 * necessarily within the range [0,1].
 *
 * Both operands are assumed to be positive.
 *
 * After division, the result is saturated to fit into a 16-bit
 * signed integer. (The only way this can happen is if the operands
 * are equal, so that the result would be 1, i.e. +0x8000 in 15-bit
 * fixed point.)
 */
INLINE Word16 AWB_div_s(Word16 x, Word16 y)
{
    Word32 quot=0;
    if (y!=0) quot = (Word32)(0x8000 * (Word32)x) / y;
    if (y==0 || quot > (ARMDSP_WORD16_MAX))
        return ARMDSP_WORD16_MAX;
    else
        return (Word16)quot;
}

/*
 * Return the number of bits of left shift needed to arrange for a
 * 32-bit signed integer to have value >= 0x40000000 (if
 *
 * Returns zero if x is zero.
 */
INLINE Word32 AWB_norm_l(Word32 x)
{
    int lz;

    if (x == 0)
        return 0;                      /* 0 is a special case */

    x ^= (x >> 31);                    /* invert x if it's negative */
#ifdef __NO_ARM_ASM__
    lz = AWB_clz(x);
#else
    __asm {
        clz lz, x;
    }
#endif
    return (lz - 1);
}

#endif

