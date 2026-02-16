/*
 *
 * csl_defs.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


/** \file   csl_defs.h
    \brief  Generic, Platform independent CSL definitions header

    This file defines macros and data types that are generic or common to
    all SoC platforms.

 
    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __CSL_DEFS_H__
#define __CSL_DEFS_H__

#include "_tistdtypes.h" /* Get the primitive C data types */

/**
 * \defgroup CSLDefines CSL Defines
 * 
 *  CSL Generic Defines - for use with any CSL module.
 */
/*@{*/

/* CSL IOCTL defines */
#define CSL_IOCTL_SHIFT     (0)

#define CSL_IOCTL_MASK      (0xFF)

/* Extract CSL relevant IOCTL code from input value */
#define CSL_IOCTL(ioctl) ((ioctl & CSL_IOCTL_MASK) >> CSL_IOCTL_SHIFT)

typedef void *      CSL_ResHandle;  /**< arbitrary resource handle */
typedef Int16       CSL_ModuleId;   /**< numeric identifier of CSL module */
typedef Uint16      CSL_BitMask16;  /**< 16bit binary mask */
typedef Uint32      CSL_BitMask32;  /**< 32bit binary mask */

/**
  \note CSL_Reg16 & CSL_Reg32 may need to be further adorned by compiler
        specific qualifiers in SoCs where I/O address space is distinct
        from that of program/data. For example on TMS320C54xx, an "ioport"
        qualifier is in order ahead of volatile to refer to peripheral
        memory mapped registers.
 */
typedef volatile Uint16 CSL_Reg16;      /* 16bit register */
typedef volatile Uint32 CSL_Reg32;      /* 32bit register */

typedef Int16       CSL_Status;     /* CSL API execution status or result */
typedef Int16       CSL_InstNum;    /* numeric instance number of the device */
typedef Int16       CSL_ChaNum;     /* numeric channel number, local to a device instance */

typedef enum 
{
  CSL_FAIL = 0,
  CSL_PASS = 1
} CSL_Test;

#ifndef NULL
#define NULL            ((void*)0)  /**< Definition of NULL */
#endif

#ifdef CFG_INCLUDE_CSL_RESOURCE_CHECK

typedef Int16  CSL_Uid; /* CSL unique identifier for peripheral resources */
typedef Uint32 CSL_Xio; /* SoC pin multiplex mask */

typedef struct 
{
    CSL_Uid uid;
    CSL_Xio xio;
} CSL_ResAttrs;

#endif /* CFG_INCLUDE_CSL_RESOURCE_CHECK */

/**
 * \brief   Following utility macros allow CSL register bitfield 
 *          manipulations. The memory mapped register could be local 
 *          to the CPU and/or related to a specific h/w peripheral device
 *          associated with the CPU.
 * \note    The macros are written as normal math-expressions and therefore
 *          assume "native endianness" MSB:LSB of parameters passed.
 * \warning These macros DO NOT perform any swaps to deal with Endianess
 *          missmatch between the addressed peripheral device register
 *          and the CPU. Typically, there'd exist h/w circuitry on the SoC
 *          that adjusts the byte ordering such that CPU always sees
 *          the values read/written to its memory mapped peripherals in
 *          its own "native endianness". In the rare event where s/w is
 *          required to make necessary endianess corrections, user is expected
 *          to do so outside these macros. Refer to <pal_defs.h> file for
 *          some utility macros that perform endianness corrections.
 * \note    The following notational conventions apply while using these
 *          macros. User is refered to cslr_<module>.h device specific
 *          header file to know the predefined register fields and symbolic
 *          tokens for use w/ these macros.
 *
 * CSL_<MODULE>_<REG>_<FIELD>_<SYMBOL>    Where..
 * CSL_        : Standard prefix to avert namespace pollution by CSL
 * <MODULE>    : CSL Peripheral module Ex: CPMAC
 * <REG>       : The peripheral device register Ex: TX_CONTROL
 * <FIELD> : Bit-field of interest Ex: EN (enable)
 * <SYMBOL>    : Operational symbol for constant being defined Ex: SHIFT
 *
 * The cslr_<module>.h file mandatorily includes following 3 defines
 * for each peripheral register bitfield --
 *
 *  [1] CSL_<MODULE>_<REG>_<FIELD>_SHIFT #of left shift positions to
 *          reach the register bit-field of interest
 *  [2] CSL_<MODULE>_<REG>_<FIELD>_MASK binary *and* mask bounding the
 *          register bitfield of interest
 *  [3] CSL_<MODULE>_<REG>_<FIELD>_RESETVAL The power-on reset value
 *          assumed by the register bit-field of interest
 *      Other symbolic tokens are defined along same lines as RESETVAL
 */

/* Following Macros help compose a numeric value at the specified fields */

/**
 * \def     CSL_FMK(PER_REG_FIELD, val) Macro composes the supplied
 *          val to the bits FIELD within the register REG of the given
 *          module MODULE
 * \note    The macro does NOT write into the register, it merely composes
 *          and returns the resulting value.
 *
 *          \b Example: CSL_FMK(CPMAC_TX_CONTROL_EN, 0) will compose 0 into EN field of
 *          register TX_CONTROL of the device CPMAC
 */
#define CSL_FMK(PER_REG_FIELD, val) \
    (((val) << CSL_##PER_REG_FIELD##_SHIFT) & CSL_##PER_REG_FIELD##_MASK)

/**
 * \brief   CSL_FMKT(MODULE_REG_FIELD, TOKEN) Macro composes the 
 *          supplied symbolic TOKEN to the bits FIELD within the register
 *          REG of the given module MODULE
 * \note    The macro does NOT write into the register, it merely composes
 *          and returns the resulting value.
 *
 *          \b Example: CSL_FMKT(CPMAC_TX_CONTROL_EN, RESETVAL) will compose 
 *          reset value as defined by token RESETVAL into EN field of register
 *          TX_CONTROL of the device CPMAC
 */
#define CSL_FMKT(PER_REG_FIELD, TOKEN) \
    CSL_FMK(PER_REG_FIELD, CSL_##PER_REG_FIELD##_##TOKEN)

/**
 * \brief   CSL_FMKR(msb, lsb, val) Macro composes the supplied val to
 *          to the bit fields in the range msb:lsb
 * \note    The macro does NOT write into any register, it merely composes
 *          and returns the resulting value.
 *
 *          \b Example: CSL_FMKR(7,4, 0xF) will compose binary value '1111'
 *          at the bit-field whose most significant bit is at position 7
 *          and least significant bit is at position 4. The bit poistion
 *          is numbered starting from left most bit as bit #0. This is
 *          according to math ordering of MSB:LSB
 */
#define CSL_FMKR(msb, lsb, val) \
    (((val) & ((1 << ((msb) - (lsb) + 1)) - 1)) << (lsb))

/* Following Macros help extract numeric value of bits residing
 * in the specified fields of the specified register 
 */

/**
 * \brief   CSL_FEXT(reg, MODULE_REG_FIELD) Macro snoops at the
 *          specified bits FIELD of the register REG of the device
 *          MODULE and constructs the numeric value of it by applying
 *          the correct number of arithmetic right shift operations.
 * \note    The macro does NOT write into any register, it merely extracts
 *          and returns the value stored at specified bitfield.
 *
 *          \b Example: CSL_FEXT(rxCtrlReg, CPMAC_RX_CONTROL_EN) will 
 *          extract value of the EN field of CPMAC's RX Control register 
 *          whose addres was passed via rxCtrlReg parameter
 */
#define CSL_FEXT(reg, PER_REG_FIELD) \
    (((reg) & CSL_##PER_REG_FIELD##_MASK) >> CSL_##PER_REG_FIELD##_SHIFT)

/**
 * \brief   CSL_FEXTR(reg, msb, lsb) Macro snoops at the
 *          specified bits b/w msb:lsb of the register reg and constructs
 *          the numeric value of it by applying the correct number of
 *          arithmetic right shift operations.
 * \note    The macro does NOT write into any register, it merely extracts
 *          and returns the value stored at specified bitfield.
 *
 *          \b Example: CSL_FEXTR(dmaCcrReg, 15, 14) will extract value of 
 *          2 bitscorresponding to destination addressing mode field within the
 *          DMA Control register CCR whose address was passed via dmaCcrReg
 */
#define CSL_FEXTR(reg, msb, lsb) \
    (((reg) >> (lsb)) & ((1 << ((msb) - (lsb) + 1)) - 1))

/* Following macros help assign a a value to specified bitfield of a
 * variable. This variable may or maynot be a peripheral register.
 */

/**
 * \brief   CSL_FINS(reg, MODULE_REG_FIELD, val) Macro writes val
 *      in specified bit field MODULE_REG_FIELD of the register
 *      whose address is passed via reg parameter to the macro
 *
 *      Similiar to the Field make macros (FMK) discussed above,
 *      the field insert macros are also provided in 3 flavours:
 *      - CSL_FINS  -- value specified as a numeric quantity
 *      - CSL_FINST -- value specified as a predefined symbolic token
 *      - CSL_FINSR -- same as CSL_FINS, exception being the bitfield
 *        is specified as msb:lsb range rather than symbolically
 *
 * \note    This macro can be used to write to normal C variable whose
 *          address is passed via reg parameter. It is not necessary that
 *          reg always refer to peripheral memory mapped registers.
 *          As can be seen from the macro definition, the specified field
 *          is first wiped clear before 'or'ing with the specified bit
 *          pattern.
 *
 * \warning If access to specified reg from CPU requires explicit endian
 *          corrections by software, user must first construct the
 *          overall value to be assigned in a local C variable and then,
 *          swap it as required for endian corrections before doing
 *          final assignment to the register.
 *
 * \warning In case of special registers where a '1' is written to
 *          clear the specified bit rather than set it, these macros will
 *          NOT work as exspected. Please use the CSL Functional API
 *          support to write to such peripheral registers.
 *
 *          \b Example: Let myNetworkReg be address of a peripheral register 
 *          which when accessed by CPU results in endianness mismatch (ie.,
 *          swapped msb/lsb). Also assume you wish to insert the STANDBY
 *          power settings to the NET_POWER_DOWN bit field of this register
 *          To handle this, do as follows:
 *
 *          Uint16 trueReg;
 *          trueReg = PAL_SWAP_UINT16(myNetworkReg);
 *          CSL_FINST(trueReg, NET_POWER_DOWN, STANDBY);
 *          myNetworkReg = PAL_SWAP_UINT16(trueReg);
 *
 *          The PAL_SWAP_UINT16() used above is a simple macro that swaps
 *          MSB with LSB. It is defined in <pal_defs.h>
 */
#define CSL_FINS(reg, PER_REG_FIELD, val) \
    ((reg) = ((reg) & ~CSL_##PER_REG_FIELD##_MASK)\
    | CSL_FMK(PER_REG_FIELD, val))

#define CSL_FINST(reg, PER_REG_FIELD, TOKEN)\
    CSL_FINS((reg), PER_REG_FIELD, CSL_##PER_REG_FIELD##_##TOKEN)

#define CSL_FINSR(reg, msb, lsb, val)\
    ((reg) = ((reg) &~ (((1 << ((msb) - (lsb) + 1)) - 1) << (lsb)))\
    | CSL_FMKR(msb, lsb, val))


/**
 * \defgroup CSLErrotCodes CSL Error Codes
 * 
 * All CSL modules shall use standard CSL error codes
 * - CSL makes use of 16bit error codes 
 * - All errors carry a negative code. Positive value means no error!
 * - First block of 32 codes are reserved for CSL system alone
 * - Ensuing blocks of 32 distinct numbers can be allocated to individual
 *   peripheral devices.
 * - In order to promote CSL errors up the Driver stack, please use the
 *   utility macro PAL_CSLERROR(cslErrorCode) defined in <pal_defs.h>
 *   
 *  \b Example: PAL_ERROR_CSLSTATUS(CSL_ESYS_INVQUERY) gives a corresponding
 *   32bit error code per the PAL error code format.
 */
/*@{*/


#define CSL_SOK                 (1)     /* Success */
#define CSL_ESYS_FAIL           (-1)    /* Generic failure */
#define CSL_ESYS_INUSE          (-2)    /* Peripheral resource is already in use */
#define CSL_ESYS_XIO            (-3)    /* Encountered a shared I/O(XIO) pin conflict */
#define CSL_ESYS_OVFL           (-4)    /* Encoutered CSL system resource overflow */
#define CSL_ESYS_BADHANDLE      (-5)    /* Handle passed to CSL was invalid */
#define CSL_ESYS_INVPARAMS      (-6)    /* invalid parameters */
#define CSL_ESYS_INVCMD         (-7)    /* invalid command */
#define CSL_ESYS_INVQUERY       (-8)    /* invalid query */
#define CSL_ESYS_NOTSUPPORTED   (-9)    /* action not supported */
#define CSL_ESYS_LAST           (-32)   /* Sentinel error code, end of sys range */

/*@}*/
/*@}*/

#endif /* _CSL_DEFS_H_ */
