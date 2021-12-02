/*! \file hal_vdec_mpeg_if.h
*
*  \par LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, USER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* ARE PROVIDED TO USER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS
* ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED
* IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND USER AGREES TO LOOK ONLY TO
* SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL
* ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO USER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* USER HEREBY ACKNOWLEDGES THE CONFIDENTIALITY OF MEDIATEK SOFTWARE AND AGREES
* NOT TO DISCLOSE OR PERMIT DISCLOSURE OF ANY MEDIATEK SOFTWARE TO ANY THIRD
* PARTY OR TO ANY OTHER PERSON, EXCEPT TO DIRECTORS, OFFICERS, EMPLOYEES OF
* USER WHO ARE REQUIRED TO HAVE THE INFORMATION TO CARRY OUT THE PURPOSE OF
* OPENING OR USING THIS FILE.
*
* \par Project
*    MT8520
*
* \par Description
*    Jpeg Decoder HAL interface definition.
*
* \par Author_Name
*    CK Hu
*
* \par Last_Changed
* $Author: dtvbm11 $
* $Modtime: $
* $Revision: #1 $
*
*/

// *********************************************************************
// Memo
// *********************************************************************
/*
*/

#ifndef _HAL_JDEC_IF_H_
#define _HAL_JDEC_IF_H_

#include "jdec_com_common.h"
#ifdef MT8520
#include "x_common.h"
#endif
#include "jdec_com_jfif.h"

#ifdef _cplusplus
extern "C" {
#endif

/*! \name Video Decoder HAL MPEG Interface
* @{
*/



/*
typedef enum {
    JDEC_HAL_PIC_FORMAT_BASELINE,           ///< Jpeg baseline format
    JDEC_HAL_PIC_FORMAT_PROGRESSIVE         ///< Jpeg progressive format
} JDEC_HAL_PIC_FORMAT_T;


typedef struct {
    // These two fields directly represent the contents of a JPEG DHT marker
    UINT8 u1Bits[17];   // bits[k] = # of symbols with codes of
    // length k bits; bits[0] is unused
    UINT8 u1Huffval[256];   // The symbols, in order of incr code length
    // This field is used only during compression.  It's initialized FALSE when
    // the table is created, and set TRUE when it's been output to the file.
    // You could suppress output of a table by setting this to TRUE.
} JDEC_HAL_HUFF_TBL_T;


typedef struct {
    UINT32              u4DcHuffTblNum;           ///< DC Huffman Table Numbers
    UINT32              u4AcHuffTblNum;           ///< DC Huffman Table Numbers
    JDEC_HAL_HUFF_TBL_T arDCHuffTbl[4];           ///< DC Huffman Tables
    JDEC_HAL_HUFF_TBL_T arACHuffTbl[4];           ///< AC Huffman Tables
} JDEC_HAL_HUFFMAN_TABLE_T;


typedef struct {
    JDEC_HAL_PIC_FORMAT_T ePicFormat;             ///< Input Jpeg picture format
    UINT32 u4PicWidth;                            ///< Input Jpeg picture width
    UINT32 u4PicHeight;                           ///< Input Jpeg picture height
    void  *pvQuantTable;                          ///< Input Jpeg picture quantization table
    JDEC_HAL_HUFFMAN_TABLE_T *prHuffmanTable;     ///< Input Jpeg picture Huffman table
} JDEC_HAL_PIC_INFO_T;


typedef struct {
	UCHAR ucCompId;                         ///< Component ID
	UCHAR ucDCTblNum;                       ///< DC Table Number
	UCHAR ucACTblNum;                       ///< AC Table Number
} JDEC_HAL_COMP_INFO_T;


typedef struct {
    UINT32 u4CompNum;                             ///< Component number
    JDEC_HAL_COMP_INFO_T rCompInfo[3];            ///< Component information
    UCHAR ucSS;                                   ///< Input Jpeg scan SS
    UCHAR ucSE;                                   ///< Input Jpeg scan SE
    UCHAR ucAH;                                   ///< Input Jpeg scan AH
    UCHAR ucAL;                                   ///< Input Jpeg scan AL
    BOOL fgFirstScan;                             ///< If this is first scan
    BOOL fgLastScan;                              ///< If this is last scan
} JDEC_HAL_SCAN_INFO_T;
*/
#define CHIPVERIFY

#if 1//def JPG_PROGRASSIVE_V4
typedef enum {
    JDEC_HAL_DEC_MODE_NONE,
    JDEC_HAL_DEC_MODE_BASELINE_PIC,                     ///< Jpeg baseline picture, decode whole picture
    JDEC_HAL_DEC_MODE_BASELINE_MCU,                     ///< Jpeg baseline picture, decode MCU row
    JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW,                 ///< Jpeg baseline picture, decode MCU
    JDEC_HAL_DEC_MODE_BASELINE_VIDEO_OUTPUT,
    JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT,    ///< Jpeg progressive picture, decode MCU milti-collect
    JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE,           ///< Jpeg progressive picture, decode MCU enhance decoding
    JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ROW_MULTI_COLLECT, ///Jpeg progressive MCU-Row Mode
    JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ROW_ENHANCE,
    JDEC_HAL_DEC_MODE_PROGRESSIVE_SCAN_MULTI_COLLECT,
    JDEC_HAL_DEC_MODE_PROGRESSIVE_SCAN_ENHANCE
} JDEC_HAL_DEC_MODE_T;
#else
typedef enum {
    JDEC_HAL_DEC_MODE_NONE,
    JDEC_HAL_DEC_MODE_BASELINE_PIC,                     ///< Jpeg baseline picture, decode whole picture
    JDEC_HAL_DEC_MODE_BASELINE_MCU,                     ///< Jpeg baseline picture, decode MCU row
    JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW,                 ///< Jpeg baseline picture, decode MCU
    JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT,    ///< Jpeg progressive picture, decode MCU milti-collect
    JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE           ///< Jpeg progressive picture, decode MCU enhance decoding
} JDEC_HAL_DEC_MODE_T;
#endif

typedef enum
{
    JDEC_HAL_DEC_PROG_VERIFY,
    JDEC_HAL_DEC_MULTI_COLLECT,
    JDEC_HAL_DEC_ENHANCE
} JDEC_HAL_DEC_PROG_T;


typedef struct {
    UINT32 u4InFifoSa;                       ///< Jpeg input Fifo memory start address
    UINT32 u4InFifoSz;                       ///< Jpeg input Fifo memory size
    UINT32 *pu4RdPoint;                     ///< Jpeg input Fifo Read Point
    UINT32 *pu4WrPoint;                     ///< Jpeg input Fifo Write Point
    UINT32 u4BitstreamSa;                    ///< Jpeg input bitstream start address
} JDEC_HAL_INPUT_INFO_T;


typedef struct {
    UINT32 u4OutYBufSa;                     ///< Jpeg output color buffer Y start address
    UINT32 u4OutCbBufSa;                    ///< Jpeg output color buffer Cb start address
    UINT32 u4OutCrBufSa;                    ///< Jpeg output color buffer Cr start address
    UINT32 u4OutYBufPitch;                  ///< Jpeg output Y color buffer pitch
    UINT32 u4OutCbBufPitch;                 ///< Jpeg output Cb color buffer pitch
    UINT32 u4OutCrBufPitch;                 ///< Jpeg output Cr color buffer pitch
    UINT32 u4OutBank0YBufSa;                ///< Jpeg output Bank0 color buffer Y start address
    UINT32 u4OutBank0CbBufSa;               ///< Jpeg output Bank0 color buffer Cb start address
    UINT32 u4OutBank0CrBufSa;               ///< Jpeg output Bank0 color buffer Cr start address
    UINT32 u4OutBank1YBufSa;                ///< Jpeg output Bank1 color buffer Y start address
    UINT32 u4OutBank1CbBufSa;               ///< Jpeg output Bank1 color buffer Cb start address
    UINT32 u4OutBank1CrBufSa;               ///< Jpeg output Bank1 color buffer Cr start address
} JDEC_HAL_OUTPUT_INFO_T;


typedef struct {
    JDEC_JFIF_SOF_T *prSofInfo;             ///< [IN] Jpeg SOF information
    JDEC_JFIF_DQT_T *prQTblInfo;            ///< [IN] Jpeg dqt information
    JDEC_JFIF_DHT_T *prHuffTblInfo;         ///< [IN] Jpeg dht information
    UINT32 u4RestartInterval;               ///< [IN] The Interval between Restart Marker.
} JDEC_HAL_PIC_INFO_T;


typedef struct {
    UINT32 u4BitsLeftRead;
    UINT32 u4BufPtrRead;
    UINT32 u4ShiftMag;
    UINT32 u4MkrRm2;
    UINT32 u4JpegBufRead;
    UINT32 u4RestartMkr;
    UINT32 u4LastDcValue0;
    UINT32 u4LastDcValue1;
    UINT32 u4LastDcValue2;
    UINT32 u4CurrMCUNum;
    UINT32 u4ReadAddrBytes;
    UINT32 u4ReadAddrBits;
} JDEC_HAL_HW_STATUS_T;


/// Initialize Jpeg decoder HAL
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Init(
    UINT32 u4JDecID                                     ///< [IN] Jpeg decoder hardware ID
);


/// Uninitialize Jpeg decoder HAL
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Uninit(
    UINT32 u4JDecID                                     ///< [IN] Jpeg decoder hardware ID
);


/// Set Jpeg input picture information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Pic_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_PIC_INFO_T *prPicInfo                      ///< [IN] Jpeg input picture information
);


/// Set Huffman Table information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_HuffmanTable_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_JFIF_DHT_T *prHuffTblInfo                      ///< [IN] Huffman Table information
);


/// Set Jpeg decoding mode
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Decoding_Mode(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_DEC_MODE_T eDecodingMode                   ///< [IN] Jpeg decoding mode
);


/// Get Jpeg decoding mode supporting status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_Decoding_Mode_Support_Status(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_DEC_MODE_T eDecodingMode                   ///< [IN] Jpeg decoding mode
);


/// Set Jpeg scan information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Scan_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JPEG_JFIF_SOS_T *prScanInfo                    ///< [IN] Input Jpeg scan information
);


/// Set Jpeg input information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Input_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_INPUT_INFO_T *prInputInfo                  ///< [IN] Input Jpeg input information
);


/// Set Jpeg output information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Output_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_OUTPUT_INFO_T *prOutputInfo                ///< [IN] Input Jpeg output information
);


/// Trigger Jpeg HAL decoding
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Decode(
    UINT32 u4JDecID,                                     ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_DEC_PROG_T eFlow                            ///< [IN] Jpeg flow, driver uses row buffer
);


/// Get Jpeg HAL decode status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_Decode_Status(
    UINT32 u4JDecID                                     ///< [IN] Jpeg decoder hardware ID
);


/// Set Jpeg VLD read pointer
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Read_Pointer(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    UINT32 u4Addr                                       ///< [IN] Read pointer address (bytes)
);


/// Get Jpeg VLD read pointer
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_Read_Pointer(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    UINT32 *pu4AddrBytes,                               ///< [IN] Read pointer address (bytes)
    UINT32 *pu4AddrBits                                 ///< [IN] Read pointer address (bytes)
);


/// Get Jpeg hardware status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_HW_Status(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_HW_STATUS_T *prHwStatus                    ///< [OUT] Hardware status
);


/// Set Jpeg hardware status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_HW_Status(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    const JDEC_HAL_HW_STATUS_T *prHwStatus              ///< [IN] Hardware status
);

UINT32 u4VDecReadVldRPtr(UINT32 u4BSID, UINT32 u4VDecID, UINT32 *pu4Bits, UINT32 u4VFIFOSa);
INT32 i4JDEC_HAL_Set_Buf(UINT32 u4JDecID, VOID **paCoefBuf, VOID **paNzBuf);
INT32 i4JDEC_HAL_Unset_Buf(UINT32 u4JDecID);

//
/*! @} */


#ifdef _cplusplus
}
#endif

#endif //#ifndef _HAL_VDEC_MPEG_IF_H_

