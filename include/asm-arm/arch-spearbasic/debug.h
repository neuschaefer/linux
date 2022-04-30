/*
 *
 *  Copyright (C) 2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
  * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */

#ifndef __INC_DBG_H
#define __INC_DBG_H

/*--------------------------------------------------------------------------*
 * Includes	
 *--------------------------------------------------------------------------*/

#include <asm/arch/hcl_defs.h>	// As debug is not included
#ifdef __DEBUG
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/*--------------------------------------------------------------------------*
 * Constants and new types
 *--------------------------------------------------------------------------*/

/*Defines for Version */
#define DBG_HCL_VERSION_ID 1
#define DBG_HCL_MAJOR_ID   0
#define DBG_HCL_MINOR_ID   0


/* define the public fifo line internal size */
#define DBG_STRING_LINE_SIZE	30

/* define number of lines in public FiFo */
#define DBG_PUBLIC_FIFO_SIZE_IN_LINES  400


/* define the maximum number of private fifo */
#define DBG_NB_MAX_PRIVATE_FIFO   10


/* define max string size returned by getPrints function */
/* this line contains HCL name, time stamp, and internal line */
#define DBG_RETURNED_LINE_MAX_SIZE (DBG_STRING_LINE_SIZE + 1 + 10 + 4)

/* define minimum size for debug purposes */
#define DBG_MIN_SIZE_TO_ALLOCATE_IN_BYTE (((DBG_PUBLIC_FIFO_SIZE_IN_LINES+1) * (DBG_STRING_LINE_SIZE+1+8)) + (DBG_NB_MAX_PRIVATE_FIFO*50))


/* Store a private fifo ID, unique for each fifo */

    typedef t_uint8 t_dbg_fifo_id;

/* Optional value format type */
    typedef enum
    {
	NO = 0,
	HEX = 1,
	DEC = 2
    } t_dbg_val_format;

/* Errors related values */


    typedef enum
    {
	DBG_OK = 0,
	DBG_ERROR_RELEASE_MODE = -1,
	DBG_ERROR_FATAL = -2,
	DBG_ERROR_NOT_ENOUGH_MEMORY = -3,
	DBG_ERROR = -4,
	DBG_ERROR_STRING_TOO_SMALL = -5,
	DBG_ERROR_INVALID_ID = -6,
	DBG_ERROR_NO_MORE_FIFO = -7,
	DBG_ERROR_INIT_FAILED = -8,
	DBG_BAD_PARAMETER = -9
    } t_dbg_error;


/* Store a submitter ID, unique for each HCL. */

    typedef enum
    {
	UNKNOWN_HCL_DBG_ID,
	APPLI_DBG_ID,
	TEST_DBG_ID,
	DEBUG_HCL_DBG_ID,
	UART_HCL_DBG_ID,
	VIC_HCL_DBG_ID,
	DMA_HCL_DBG_ID,
	HA_HCL_DBG_ID,
	RTC_HCL_DBG_ID,
	TIMER_HCL_DBG_ID,
	WATCHDOG_HCL_DBG_ID,
	I2C_HCL_DBG_ID,
	AUDIOCODEC_HCL_DBG_ID,
	MSP_HCL_DBG_ID,
	HV_HCL_DBG_ID,
	FLASH_HCL_DBG_ID,
	SDRAM_HCL_DBG_ID,
	GPIO_HCL_DBG_ID,
	POWER_HCL_DBG_ID,
	PLL_HCL_DBG_ID,
	SSI_HCL_DBG_ID,
	DIF_HCL_DBG_ID,
	SDMM_HCL_DBG_ID,
	FIRDA_HCL_DBG_ID,
	SSP_HCL_DBG_ID,
	CLCD_HCL_DBG_ID,
	SRC_HCL_DBG_ID,
	RTT_HCL_DBG_ID,
	USB_HCL_DBG_ID,
	PWL_HCL_DBG_ID,
	OWM_HCL_DBG_ID,
	XTI_HCL_DBG_ID,
	SSM_HCL_DBG_ID,
	SECR_HCL_DBG_ID,
	TDES_HCL_DBG_ID,
	SHA_HCL_DBG_ID,
	RNG_HCL_DBG_ID
    } t_dbg_id;


    typedef enum
    {
	DBG_STATUS_EMPTY,
	DBG_STATUS_OK,
	DBG_STATUS_WARNING,
	DBG_STATUS_ALERT,
	DBG_STATUS_FULL
    } t_dbg_status_fifo;



//Define the debug level.

#define DEBUG_LEVEL0 DBGL_OFF
#define DEBUG_LEVEL1 ((t_uint32)DBGL_PUBLIC_FUNC_IN|(t_uint32)DBGL_PUBLIC_FUNC_OUT|(t_uint32)DBGL_ERROR|(t_uint32)DBGL_WARNING)
#define DEBUG_LEVEL2 ((t_uint32)DBGL_IN_ARGS|(t_uint32)DBGL_OUT_ARGS|(t_uint32)DBGL_RET_CODE)
#define DEBUG_LEVEL3 DBGL_INTERNAL
#define DEBUG_LEVEL4 DBGL_HCL_DEV


    typedef enum
    {
	DBGL_OFF = 0,
	DBGL_PUBLIC_FUNC_IN = MASK_BIT0,
	DBGL_PUBLIC_FUNC_OUT = MASK_BIT1,
	DBGL_ERROR = MASK_BIT2,
	DBGL_WARNING = MASK_BIT3,
	DBGL_IN_ARGS = MASK_BIT4,
	DBGL_OUT_ARGS = MASK_BIT5,
	DBGL_RET_CODE = MASK_BIT6,
	DBGL_INTERNAL = MASK_BIT7,
	DBGL_HCL_DEV = MASK_BIT8,
	DBGL_PRIV_FUNC_IN = MASK_BIT9,
	DBGL_PRIV_FUNC_OUT = MASK_BIT10,
	DBGL_PRIV_IN_ARGS = MASK_BIT11,
	DBGL_PRIV_OUT_ARGS = MASK_BIT12,
	DBGL_USER_1 = MASK_BIT13,
	DBGL_USER_2 = MASK_BIT14,
	DBGL_USER_3 = MASK_BIT15,
	DBGL_USER_4 = MASK_BIT16,
	DBGL_USER_5 = MASK_BIT17,
	DBGL_USER_6 = MASK_BIT18,
	DBGL_USER_7 = MASK_BIT19,
	DBGL_USER_8 = MASK_BIT20,
	DBGL_USER_9 = MASK_BIT21,
	DBGL_RESERVED_0 = MASK_BIT22,
	DBGL_RESERVED_1 = MASK_BIT23,
	DBGL_RESERVED_2 = MASK_BIT24,
	DBGL_RESERVED_3 = MASK_BIT25,
	DBGL_RESERVED_4 = MASK_BIT26,
	DBGL_RESERVED_5 = MASK_BIT27,
	DBGL_RESERVED_6 = MASK_BIT28,
	DBGL_RESERVED_7 = MASK_BIT29,
	DBGL_RESERVED_8 = MASK_BIT30
    } t_dbg_level;


    typedef enum
    {
	UNDEFINED_SIGNAL = 0
    } t_dbg_signame;
/*--------------------------------------------------------------------------*
 * Functions declaration													*
 *--------------------------------------------------------------------------*/

#ifdef __DEBUG

    PUBLIC t_dbg_error DBG_QueryPrivateFifo (char *, t_uint8, t_uint8,
					     t_dbg_fifo_id *,
					     t_logical_address *);
    PUBLIC t_dbg_error DBG_PrivateFifoSend8 (t_dbg_fifo_id, t_uint8);
    PUBLIC t_dbg_error DBG_PrivateFifoSend16 (t_dbg_fifo_id, t_uint16);
    PUBLIC t_dbg_error DBG_PrivateFifoSend32 (t_dbg_fifo_id, t_uint32);
    PUBLIC t_dbg_error DBG_PrivateFifoSend64 (t_dbg_fifo_id, t_uint64);
    PUBLIC t_dbg_error DBG_SetHardwareSignal (t_dbg_signame, t_uint16);

    PUBLIC t_dbg_error DBG_Print (t_dbg_id, char *, t_uint32,
				  t_dbg_val_format);


/* General purpose API */

    PUBLIC t_dbg_error DBG_Init (t_uint32, t_logical_address,
				 t_logical_address);
    PUBLIC t_dbg_error DBG_ResetPrints (void);
    PUBLIC t_dbg_error DBG_GetPrints (char *, t_uint16, t_uint16 *,
				      t_dbg_status_fifo *);
    PUBLIC t_dbg_error DBG_GetPublicFifoStatus (t_dbg_status_fifo *);


    PUBLIC t_dbg_error DBG_SetDbgLevel (t_dbg_level);
    PUBLIC t_dbg_error DBG_GetVersion (t_version *);

/*--------------------------------------------------------------------------*
 * Macro                													*
 *--------------------------------------------------------------------------*/

#define DBG_TEMP_STRING_SIZE 256

/* Begin of Private definitions */

/*
 * Compiler define __ARMCC_VERSION returns PVtbbb where:
 * P is the major version (1 for ADS and 2 for RVCT v2.1)
 * V is the minor version
 * t is the patch release
 * bbb is the build
*/
#if ((__ARMCC_VERSION >= 100000) && (__ARMCC_VERSION < 200000))
/* ADS Compiler */
#define DBGFUNCNAME __func__
#elif (__ARMCC_VERSION < 300000)
/* RVCT Compiler */
#define DBGFUNCNAME __FILE__
#else
/* To be added - depends on the compiler to be used. Currently is left as empty */
#define DBGFUNCNAME ""
#endif


#define DBGENTER__(x)  DBGPRINT(DEBUG_LEVEL1,"Enter "##x);


#define DBGSPBEG {char mylcstring[DBG_TEMP_STRING_SIZE];t_dbg_error dumerr;int nochar;nochar =  (((t_uint32)DBGL_OUT_ARGS  & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? sprintf(mylcstring

#define DBGSPEND(cr) : (0) ; dumerr = DBGPRINT(DBGL_OUT_ARGS,mylcstring); \
                    nochar = (((t_uint32)DBGL_PUBLIC_FUNC_OUT & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? sprintf(mylcstring," Exit %s",DBGFUNCNAME) : (0); \
                    dumerr = DBGPRINT(DBGL_PUBLIC_FUNC_OUT,mylcstring); \
                    nochar = (((t_uint32)DBGL_RET_CODE & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? sprintf(mylcstring," return(%d)",(cr)) : (0); \
                    dumerr = DBGPRINT(DBGL_RET_CODE,mylcstring);dumerr = dumerr;nochar =nochar;}

#define DBGSIBEG {char mylcstring[DBG_TEMP_STRING_SIZE];t_dbg_error dumerr;int nochar; \
				 nochar = (((t_uint32)DBGL_PUBLIC_FUNC_IN & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? sprintf(mylcstring," %s()",DBGFUNCNAME) : (0); \
                 dumerr = DBGPRINT(DBGL_PUBLIC_FUNC_IN,mylcstring);  \
                 nochar = (((t_uint32)DBGL_IN_ARGS  & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ?  sprintf(mylcstring

#define DBGSIEND  : (0) ; dumerr = DBGPRINT(DBGL_IN_ARGS,mylcstring);dumerr =dumerr;nochar = nochar;}
/* End of Private definitions */


#define DBGEXIT0(cr)					  DBGSPBEG,"")                   DBGSPEND(cr)
#define DBGEXIT1(cr,ch,p1)                DBGSPBEG,ch,p1)                DBGSPEND(cr)
#define DBGEXIT2(cr,ch,p1,p2)             DBGSPBEG,ch,p1,p2)             DBGSPEND(cr)
#define DBGEXIT3(cr,ch,p1,p2,p3)          DBGSPBEG,ch,p1,p2,p3)          DBGSPEND(cr)
#define DBGEXIT4(cr,ch,p1,p2,p3,p4)       DBGSPBEG,ch,p1,p2,p3,p4)       DBGSPEND(cr)
#define DBGEXIT5(cr,ch,p1,p2,p3,p4,p5)    DBGSPBEG,ch,p1,p2,p3,p4,p5)    DBGSPEND(cr)
#define DBGEXIT6(cr,ch,p1,p2,p3,p4,p5,p6) DBGSPBEG,ch,p1,p2,p3,p4,p5,p6) DBGSPEND(cr)

#define DBGENTER0()						DBGSIBEG,"")	               DBGSIEND
#define DBGENTER1(ch,p1)				DBGSIBEG,ch,p1)                DBGSIEND
#define DBGENTER2(ch,p1,p2)				DBGSIBEG,ch,p1,p2)             DBGSIEND
#define DBGENTER3(ch,p1,p2,p3)			DBGSIBEG,ch,p1,p2,p3)          DBGSIEND
#define DBGENTER4(ch,p1,p2,p3,p4)		DBGSIBEG,ch,p1,p2,p3,p4)       DBGSIEND
#define DBGENTER5(ch,p1,p2,p3,p4,p5)	DBGSIBEG,ch,p1,p2,p3,p4,p5)    DBGSIEND
#define DBGENTER6(ch,p1,p2,p3,p4,p5,p6)	DBGSIBEG,ch,p1,p2,p3,p4,p5,p6) DBGSIEND

#define DBGEXIT DBGEXIT0
#define DBGENTER DBGENTER0

#define DBGPRINT(dbg_level,dbg_string) 	         ((((t_uint32)(dbg_level) & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? DBG_Print(MY_DEBUG_ID,dbg_string,0,NO) : (t_dbg_error)(0))
#define DBGPRINTHEX(dbg_level,dbg_string,uint32) ((((t_uint32)(dbg_level) & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? DBG_Print(MY_DEBUG_ID,dbg_string,uint32,HEX) :(t_dbg_error) (0))
#define DBGPRINTDEC(dbg_level,dbg_string,uint32) ((((t_uint32)(dbg_level) & (t_uint32)MY_DEBUG_LEVEL_VAR_NAME ) != (t_uint32)DBGL_OFF) ? DBG_Print(MY_DEBUG_ID,dbg_string,uint32,DEC) : (t_dbg_error)(0))
#endif

#ifdef __RELEASE

#define DBGEXIT(cr)
#define DBGEXIT0(cr)
#define DBGEXIT1(cr,ch,p1)
#define DBGEXIT2(cr,ch,p1,p2)
#define DBGEXIT3(cr,ch,p1,p2,p3)
#define DBGEXIT4(cr,ch,p1,p2,p3,p4)
#define DBGEXIT5(cr,ch,p1,p2,p3,p4,p5)
#define DBGEXIT6(cr,ch,p1,p2,p3,p4,p5,p6)

#define DBGENTER()
#define DBGENTER0()
#define DBGENTER1(ch,p1)
#define DBGENTER2(ch,p1,p2)
#define DBGENTER3(ch,p1,p2,p3)
#define DBGENTER4(ch,p1,p2,p3,p4)
#define DBGENTER5(ch,p1,p2,p3,p4,p5)
#define DBGENTER6(ch,p1,p2,p3,p4,p5,p6)



#define DBGPRINT(dbg_level,dbg_string)
#define DBGPRINTHEX(dbg_level,dbg_string,uint32)
#define DBGPRINTDEC(dbg_level,dbg_string,uint32)
#endif


/*--------------------------------------------------------------------------*
 * C++       																*
 *--------------------------------------------------------------------------*/
#ifdef __cplusplus
}				/* allow C++ to use these headers */
#endif				/* __cplusplus */


#endif				/* __INC_DBG_H */

// End of file - debug.h
