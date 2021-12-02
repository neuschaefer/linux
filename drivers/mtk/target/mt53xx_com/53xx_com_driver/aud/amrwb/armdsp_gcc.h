#ifndef ARMDSP_ARMDSP_GCC_H
#define ARMDSP_ARMDSP_GCC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CC_MTK_PLATFORM
#include "x_typedef.h"
#include "x_assert.h"
#endif

#define _PREFIX inline

#ifdef CC_MTK_PLATFORM
typedef INT8     Word8_gcc;
typedef UINT8   UWord8_gcc;
typedef INT16    Word16_gcc;
typedef UINT16  UWord16_gcc;
typedef INT32      Word32_gcc;
typedef UINT32    UWord32_gcc;
typedef INT64       Word64_gcc;
#else
typedef signed char     Word8_gcc;
typedef unsigned char   UWord8_gcc;
typedef signed short    Word16_gcc;
typedef unsigned short  UWord16_gcc;
typedef signed int      Word32_gcc;
typedef unsigned int    UWord32_gcc;
typedef long long       Word64_gcc;
#endif

#define ARMDSP_Word32_gcc_MAX ((Word32_gcc)0x7FFFFFFF)
#define ARMDSP_Word32_gcc_MIN ((Word32_gcc)0x80000000)
#define ARMDSP_Word16_gcc_MAX ((Word16_gcc)0x7FFF)
#define ARMDSP_Word16_gcc_MIN ((Word16_gcc)0x8000)

#define AWB_saturate(a) (a > ARMDSP_Word32_gcc_MAX ?  ARMDSP_Word32_gcc_MAX : a < ARMDSP_Word32_gcc_MIN ? ARMDSP_Word32_gcc_MIN : a)

/*
 * Saturating 32-bit integer addition.
 */
_PREFIX Word32_gcc AWB_qadd(Word32_gcc x, Word32_gcc y);

/*
 * Saturating 32-bit integer subtraction.
 */
_PREFIX Word32_gcc AWB_qsub(Word32_gcc x, Word32_gcc y);

/*
 * Saturating 32-bit integer addition, with the second operand
 * multiplied by two. (i.e. return x + 2 * y)
 */
_PREFIX Word32_gcc AWB_qdadd(Word32_gcc x, Word32_gcc y);

/*
 * Saturating 32-bit integer subtraction, with the second operand
 * multiplied by two. (i.e. return x - 2 * y)
 */
_PREFIX Word32_gcc AWB_qdsub(Word32_gcc x, Word32_gcc y);

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words. For example, smulbt(x,y) multiplies
 * the bottom 16 bits of x with the top 16 bits of y.
 */

/* smulbb - multiply bottom half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smulbb(Word32_gcc x, Word32_gcc y);

/* smulbt - multiply bottom half of x and top half of y */
_PREFIX Word32_gcc AWB_smulbt(Word32_gcc x, Word32_gcc y);

/* smultb - multiply top half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smultb(Word32_gcc x, Word32_gcc y);

/* smultt - multiply top half of x and top half of y */
_PREFIX Word32_gcc AWB_smultt(Word32_gcc x, Word32_gcc y);

/*
 * Multiply two signed 16-bit numbers, taken from the bottom or top
 * halves of the input words x and y, and add to the input word a.
 * For example, smlabt(x,y,a) multiplies the bottom 16 bits of x with
 * the top 16 bits of y, and then adds a.
 */

/* smlabb - multiply bottom half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smlabb(Word32_gcc x, Word32_gcc y, Word32_gcc a);

/* smlabt - multiply bottom half of x and top half of y */
_PREFIX Word32_gcc AWB_smlabt(Word32_gcc x, Word32_gcc y, Word32_gcc a);

/* smlatb - multiply top half of x and bottom half of y */
_PREFIX Word32_gcc AWB_smlatb(Word32_gcc x, Word32_gcc y, Word32_gcc a);

/* smlatt - multiply top half of x and top half of y */
_PREFIX Word32_gcc AWB_smlatt(Word32_gcc x, Word32_gcc y, Word32_gcc a);

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
_PREFIX void AWB_smlalbb(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y);

/* smlalbt - multiply bottom half of x and top half of y */
_PREFIX void AWB_smlalbt(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y);

/* smlaltb - multiply top half of x and bottom half of y */
_PREFIX void AWB_smlaltb(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y);

/* smlaltt - multiply top half of x and top half of y */
_PREFIX void AWB_smlaltt(Word32_gcc *lo, Word32_gcc *hi, Word32_gcc x, Word32_gcc y);

/*
 * Multiply a 32-bit signed integer (the input word x) with a
 * 16-bit signed integer (taken from either the top or the bottom
 * half of the input word y). Discard the bottom 16 bits of the
 * 48-bit product.
 */

/* smulwb - multiply x by the bottom half of y */
_PREFIX Word32_gcc AWB_smulwb(Word32_gcc x, Word32_gcc y);

/* smulwt - multiply x by the top half of y */
_PREFIX Word32_gcc AWB_smulwt(Word32_gcc x, Word32_gcc y);

/*
 * Multiply a 32-bit signed integer (the input word x) with a
 * 16-bit signed integer (taken from either the top or the bottom
 * half of the input word y). Discard the bottom 16 bits of the
 * 48-bit product, and add to the input word a.
 */

/* smlawb - multiply x by the bottom half of y */
_PREFIX Word32_gcc AWB_smlawb(Word32_gcc x, Word32_gcc y, Word32_gcc a);

/* smlawt - multiply x by the top half of y */
_PREFIX Word32_gcc AWB_smlawt(Word32_gcc x, Word32_gcc y, Word32_gcc a);

INLINE Word32_gcc AWB_smull(Word32_gcc x, Word32_gcc y);

INLINE Word32_gcc AWB_umull(UWord32_gcc x,UWord32_gcc y);

_PREFIX Word64_gcc AWB_smlal(Word64_gcc LL_acc, Word32_gcc x, Word32_gcc y);

_PREFIX Word32_gcc AWB_clz(Word32_gcc absx);

_PREFIX unsigned int AWB_EndianByteReverse(unsigned int temp);

#ifdef __cplusplus
}
#endif

#endif /* ARMDSP_ARMDSP_GCC_H */

