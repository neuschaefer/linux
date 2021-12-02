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
#if ! defined(JPG_FLW2_H)
#define JPG_FLW2_H

#define TRY2(X) \
	/* following do {} while (0) should be safe */ \
	LINT_SUPPRESS_NEXT_STATEMENT(717) \
	do \
	{ \
		i4Ret = (X); \
		if (i4Ret) \
		{ \
			goto _catch_err; \
		} \
	} while (0)

#define CATCH_ERROR() \
	_catch_err: \

/* above line is left blanc intendedly */

typedef struct _JPEG_FLOW_DATA_T
{
	/* parent object */
	JPEG_BASE_CLASS_T		rParent;
	/* handles */
	JPEGHANDLE				hJpegHdr;
	JPEGHANDLE				hJpegHw;
	JPEGHANDLE				hJpegMem;
	/* decoding status */
	INT32					i4CurRow;
	INT32					i4CurCol;
	UINT32                  u4FileSize;
	UINT32                  u4DecodedSize;
} JPEG_FLOW_DATA_T;

typedef struct _JPEG_PARTIAL_OUTPUT_WINDOW_T
{
    UINT32 u4Partial_x;
    UINT32 u4Partial_y;    // direct|index color mode
    UINT32 u4Partial_w;
    UINT32 u4Partial_h;
    UINT32 u4Partial_skip_mcu_row;    
} JPEG_PARTIAL_OUTPUT_WINDOW_T;

#define JPGFLW		((JPEG_FLOW_DATA_T*)hFlow)
#define H_JPGHDR	(JPGFLW->hJpegHdr)
#define JPGHDR		((JPEG_JFIF_HEADER_T*)H_JPGHDR)
#define H_JPGHW		(JPGFLW->hJpegHw)
#define JPGHW		((JPEG_VLDJPG_DATA_T*)H_JPGHW)
#define H_JPGMEM	(JPGFLW->hJpegMem)
#define JPGMEM		((JPEG_OUTPUT_T*)H_JPGMEM)
#define JPGMEMBS	((JMEM_BASE_T*)(JPGMEM->pvLocalData))

#define HORI 0
#define VERT 1

JPEGHANDLE JDEC_AllocInstance	(void);
INT32 JDEC_New					(JPEGHANDLE hDec);
void JDEC_FreeInstance			(JPEGHANDLE hDec);
INT32 JDEC_DecodeYUV			(JPEGHANDLE hDec, void *pvImage, SIZE_T zByteSize, JMEM_YUV_LINEAR_T **ppYuvImg);

#endif //JPG_FLW2_H

