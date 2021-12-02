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

#ifndef __JDEC_H
#define __JDEC_H

#include "x_common.h"
#include "drv_img_dec.h"
#include "jdec_com_jfif.h"

#define JdecCopyMemory(destination, source, length) memcpy((destination), (source), (length))
#define JdecZeroMemory(destination, length) memset((destination), 0, (length))

#define JDEC_INVALID64      0xFFFFFFFFFFFFFFFF
#define JDEC_INVALID32      0xFFFFFFFF
#define JDEC_INVALID16      0xFFFF
#define JDEC_INVALID8       0xFF

#define JDEC_INST_NUM               3                   ///< the instance number
#define JDEC_PIC_INFO_CACHE_NUM     10                  ///< the picture info cache number
#define JDEC_EXTRA_COMP_ID          (JDEC_INST_NUM - 1) ///< extra jpeg component id
#ifdef JPEG_AUTO_TEST
#define JDEC_FIFO_SIZE              10*1024*1024    ///< fifo size, 2MB
#else
#define JDEC_FIFO_SIZE              512*1024    ///< fifo size, 2MB
#endif
#define JDEC_CMD_NONE               0x4A504700  ///< JPG0
#define JDEC_CMD_FRAME_START        0x4A504701  ///< JPG1
#define JDEC_CMD_DECODE             0x4A504702  ///< JPG2
#define JDEC_CMD_FRAME_END          0x4A504703  ///< JPG3
#define JDEC_CMD_STOP               0x4A504704  ///< JPG4
#define JDEC_CMD_VPD_TURN_ON        0x4A504705  ///< JPG5
#define JDEC_CMD_VPD_TURN_OFF       0x4A504706  ///< JPG6
#define JDEC_CMD_FILL_BUF           0x4A504707  ///< JPG7
#define JDEC_CMD_MAX                0x4A504708  ///< JPG8

/// Image Resizer State Machine
typedef enum 
{
    JDEC_STATE_NONE,
    JDEC_STATE_START,
    JDEC_STATE_PRS_HDR,
    JDEC_STATE_DECODING,
    JDEC_STATE_IDLE,
    JDEC_STATE_MAX
} JDEC_STATE;

/// Image Resizer Decodeing State Machine
typedef enum 
{
    JDEC_DECODE_STATE_NONE,
    JDEC_DECODE_STATE_WAIT_DATA,
    JDEC_DECODE_STATE_DECODING,
    JDEC_DECODE_STATE_END,
    JDEC_DECODE_STATE_MAX
} JDEC_DECODE_STATE;

/// Image Resizer Decodeing State Machine
typedef enum 
{
    JDEC_PRSHDR_STATE_NONE,
    JDEC_PRSHDR_STATE_WAIT_DATA,
    JDEC_PRSHDR_STATE_PARSING,
    JDEC_PRSHDR_STATE_END,
    JDEC_PRSHDR_STATE_MAX
} JDEC_PRSHDR_STATE;

/// Image Resizer Usage State
typedef enum 
{
    JDEC_USAGE_NONE,
    JDEC_USAGE_NORMAL,
    JDEC_USAGE_EXTRA,
    JDEC_USAGE_MJPG,
    JDEC_USAGE_MAX
} JDEC_USAGE;

/// Image Resizer Cache State
typedef enum 
{
    JDEC_CACHE_INVALID,
    JDEC_CACHE_INUSING,
    JDEC_CACHE_VALID,
    JDEC_CACHE_DEC_FAIL
} JDEC_CACHE_STATE;

typedef struct _JDEC_MESSAGE_T
{
    UINT32      ui4_command;
    UINT16      ui2_id;         /* For driver it is the comp_id. */
    UINT32      ui4_arg1;
} JDEC_MESSAGE_T;

typedef struct _JDEC_PIC_INFO_T
{
    UINT32              u4ImgId;                ///< picture id
    JPEG_JFIF_SOS_T     rSosInfo;
    JDEC_JFIF_DQT_T     rQTblInfo;
    JDEC_JFIF_DHT_T     rHuffTblInfo;
    JDEC_JFIF_SOF_T     rSofInfo;
    UINT32              u4Ofst1stSOS;           ///< offset of 1st SOS
	BOOL                fgSOF;			        ///< indicate that already have an SOF marker
	BOOL				fgDQT;                  ///< indicate that already have an DQT marker

    JDEC_CACHE_STATE    eCacheState;            ///< Cache State
    INT32               i4ReplaceCount;         ///< if 0, can be replaced
} JDEC_PIC_INFO_T;

typedef struct _JDEC_FIFO_INFO_T
{
    UINT32              u4FifoTrnsId;           ///< the fifo id for trns
    void                *pvFifoBuf;             ///< the source fifo buffer
    UINT32              u4WrPoint;              ///< current fifo write point
    UINT32              u4RdPoint;              ///< current fifo read point
    UINT32              u4FifoStart;            ///< fifo start address
    UINT32              u4FifoSize;             ///< fifo size
    UINT32              u4RequiredLen;          ///< require to fill buffer length
    UINT32              u4RealFilledLen;        ///< Real Filled length
    BOOL                fgFillingBuf;           ///< Is fillinf buffer
    IMG_DRV_FILL_BUF_T  rNfyFillBuf;
} JDEC_FIFO_INFO_T;

typedef struct _JDEC_INST_T
{
    UINT16              u2CompId;               ///< the Jdec Component Id
    HANDLE_T            hJdecInstId;            ///< the instance id
    HANDLE_T            hJdecMsgQId;            ///< the message queue id

    JDEC_USAGE          eJdecUsage;             ///< jpeg usage
    BOOL                fgExtraCompOn;          ///< extra component turn 1:on 2:off
    IMG_DRV_NFY_FCT_T   pfCbFunc;               ///< the callback function of MW
    VOID                *pvTag;                 ///< the callback function private data
    JDEC_STATE          eJdecState;             ///< state machine
    JDEC_DECODE_STATE   eJdecDecState;          ///< state machine of decoding
    JDEC_PRSHDR_STATE   eJdecPrsState;          ///< state machine of parsing
    UINT32              u4DecProgress;          ///< decode progress

    UINT32              u4JdecHwId;             ///< the Jpeg HW decode Id
    UINT32              u4ImgId;                ///< picture id
    void                *pvImgBuf;              ///< if MW has put whole source data into buffer
    UINT32              u4ImgSize;              ///< image source size

    JDEC_FIFO_INFO_T    rFifoInfo;              ///< fifo infomation
    void*               pvPicCache;             ///< jpeg picture cache

    UINT32              u4FrameIndex;           ///< must be 0 in Jpeg
    UINT32              u4SrcCropPosX;          ///< source picture crop X position
    UINT32              u4SrcCropPosY;          ///< source picture crop Y position
    UINT32              u4SrcCropWidth;         ///< source picture crop width
    UINT32              u4SrcCropHeight;        ///< source picture crop height

    void                *pvDstBuffer;           ///< destination buffer point
    UINT32              u4DstCropPosX;          ///< destination picture crop X position
    UINT32              u4DstCropPosY;          ///< destination picture crop y position
    UINT32              u4DstCropWidth;         ///< destination picture crop width
    UINT32              u4DstCropHeight;        ///< destination picture crop height
    UINT32              u4DstBufPitch;          ///< destination buffer pitch
    GFX_DRV_COLORMODE_T eDstColorMode;          ///< destination picture color mode
    IMG_DRV_ROTATE_T    eDstRotate;             ///< destination picture rotation degree

    ///< decode mode
    UINT32              u4CurrDecodeMCU;        ///< current decode MCU
    UINT32              u4CurrDecodeMCURow;     ///< current decode MCU ROW
    UINT32              u4JdecErrorType;        ///< error handle type
} JDEC_INST_T;

#endif // __JDEC_H
