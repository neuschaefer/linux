/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
#if ! defined(JPEG_COMMON_H)
#define JPEG_COMMON_H

#include "x_lint.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"

typedef enum _ENUM_JPEG_RET_CODE_T
{
	E_JPG_OK,
	E_JPG_ERR_NONFATAL,
	E_JPG_ERR_FATAL,
	E_JPG_ERR_PARAM,
	E_JPG_ERR_NOT_INITED,
	E_JPG_ERR_ALREADY, //5
	/* markers */
	E_JPG_ERR_NOT_JPEG_IMAGE,
	E_JPG_ERR_NOT_A_MARKER,
	E_JPG_ERR_PASS_END,
	E_JPG_ERR_MULTI_SOI,
	E_JPG_ERR_UNSUPPORT_SOF, //10
	E_JPG_ERR_MULTI_SOF,
	E_JPG_ERR_INVALID_SOF,
	E_JPG_ERR_SOS_BEFORE_SOF,
	E_JPG_ERR_INVALID_SOS,
	E_JPG_ERR_INVALID_DHT, //15
	E_JPG_ERR_INVALID_DRI,
	E_JPG_ERR_MULTI_APP1,
	E_JPG_ERR_INVALID_APP0,
	E_JPG_ERR_INVALID_APP1,
	E_JPG_ERR_PARSE_FAILED, //20
	E_JPG_ERR_NOT_PARSED,
	E_JPG_ERR_MULTI_EOI,
	/* instances */
	E_JPG_ERR_ALLOC_PARSER,
	E_JPG_ERR_ALLOC_DECODER,
	E_JPG_ERR_ALLOC_MEM, //25
	E_JPG_ERR_ALLOC_FLOW,
	/* general */
	E_JPG_ERR_UNSUPPORT,
	E_JPG_ERR_GENERAL,
	E_JPG_ERR_LAST,
	E_JPG_ERR_DISPLAY_ADDR, //30
	E_JPG_ERR_INVALID_COMPONENT	//code sync
} ENUIM_JPEG_RET_CODE_T;

/* MISRA32 enum assign at first only or all assign */
LINT_SUPPRESS_NEXT_STATEMENT(960)
typedef enum _ENUM_HWJPG_RET_CODE
{
	E_HWJPG_OK = 0,
	E_HWJPG_BUSY,

	E_HWJPG_ERR_FETCH_TIMEOIT = E_JPG_ERR_LAST + 1,
	E_HWJPG_ERR_SET_BS,
	E_HWJPG_ERR_LOAD_Q,
	E_HWJPG_ERR_QTBL_INDEX,
	E_HWJPG_ERR_QTBL_NUM,

	E_HWJPG_ERR_PARAM,
	E_HWJPG_ERR_TIMEOUT,

	E_HWJPG_ERR_COMP_RANGE,
	E_HWJPG_ERR_TBL_RANGE,
	E_HWJPG_ERR_BLOCK,
	E_HWJPG_ERR_SIZE,
	E_HWJPG_ERR_OUTBUF,

	E_HWJPG_ERR_NULL_SCAN,

	E_HWJPG_ERR_GLOB_NOT_READY,
	E_HWJPG_ERR_SCAN_NOT_READY,
	E_HWJPG_ERR_ROW_NOT_READY,
	E_HWJPG_ERR_MCU_NOT_READY,
	E_HWJPG_ERR_MISC_NOT_READY,
	E_HWJPG_ERR_HUFF_ADDR,
	/* last */
	E_HWJPG_ERR_GENERAL
} ENUM_HWJPG_RET_CODE;


#define D_BIT(X)			(1 << (X))
#define ALIGN_MASK(BIT)		(((UINT32)(BIT) >> 3) - 1)


#endif //JPEG_COMMON_H
