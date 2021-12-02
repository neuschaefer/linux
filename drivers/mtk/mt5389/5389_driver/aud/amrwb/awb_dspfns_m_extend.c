#ifdef LINUX_TURNKEY_SOLUTION

#include    "awb_dspfns_m_extend.h"


INLINE Word32 AWB_saturate_e(Word32 x)
{
    if (x >= (ARMDSP_WORD16_MAX+1))
        x = ARMDSP_WORD16_MAX;
    else if (x < ARMDSP_WORD16_MIN)
        x = ARMDSP_WORD16_MIN;
    return x;
}


/*
 * Extended version of AWB_L_shl(Word32 x, Word16 shift)
 * Direct shift left
 */

/*
 * Extended version of AWB_L_shr(Word32 x, Word16 shift)
 * Direct shift right
 */

INLINE void _awb_check_overflow_start(void)
{
#if defined(__ARM_INTR__)
  armdsp_flagdata_AWB.armdsp_bitfields.armdsp_q = 0;
#elif __TARGET_FEATURE_DSPMUL
  __asm {
    msr CPSR_f, #0;
  }
#else
  armdsp_flagdata_AWB.armdsp_bitfields.armdsp_q = 0;
#endif
}
INLINE Word32 _awb_check_overflow_end(void)
{
  Word32 ret;
#if defined(__ARM_INTR__)
  ret = armdsp_flagdata_AWB.armdsp_bitfields.armdsp_q;
#elif __TARGET_FEATURE_DSPMUL
  __asm {
    mrs ret, CPSR;
    and ret, ret, #(1<<27);
  }
#else
  ret = armdsp_flagdata_AWB.armdsp_bitfields.armdsp_q;
#endif
  return ret;
}


INLINE void _awb_write_overflow_flag(Word32 flag)
{
#if defined(__ARM_INTR__)
  armdsp_flagdata_AWB.armdsp_bitfields.armdsp_q = flag;
#elif __TARGET_FEATURE_DSPMUL

	if(flag == 0)
	{
		__asm { msr CPSR_f, #0; }
	}
	else
	{
		__asm { msr CPSR_f , 0x10000000 ; }
	}
#else
  armdsp_flagdata_AWB.armdsp_bitfields.armdsp_q = flag;
#endif

}


INLINE Word32 _awb_check_word_align(   /* return 0 if aligned */
	void *ptr
){
  return((Word32)ptr & 3);
}


#endif

