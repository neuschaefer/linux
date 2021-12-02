/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dsp_jpg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_jpg.c
 *  Image library command state machine.
 */

/******************************************************************************
* Include files
******************************************************************************/
#include "dsp_jpg.h"
#include "img_lib_if.h"
#include "img_flw.h"

#include "fbm_drvif.h"
#include "vdp_if.h"
#include "jpg_if.h"
#include "jpg_common.h" //return value define
#include "jpg_debug.h"
#include "../aud/adsp/include_API/dsp_uop.h"
#include "../aud/adsp/include_API/drv_adsp.h"
#include "../aud/include/aud_if.h"

/******************************************************************************
* Local	Macros,	defines, typedefs, enums
******************************************************************************/
//invalid define
#define JPGLogS(...) 			LOG(3,__VA_ARGS__)
#define JPGLogB(a,b,c,d)
#define JPGLogD(d)



#define DRAMA_NONCACH_BASE_ADDRESS 	0
#define DRAMA_CACH_BASE_ADDRESS 		0x3000000

#define MM_ALIGN(align, addr)    ((addr + align - 1) & ~(align - 1))
#define dwAbsDramANc(dwPtr)    ((UINT32)(dwPtr) & 0xfffffff)
#define vDrvOsdDramCopy(dwSrc,  dwDst,  dwLen)  x_memcpy(dwDst, dwSrc, dwLen)

#define MAX_COMPS_IN_SCAN   3 /* JPEG limit on # of components in one scan */
#define UP_SCALE    1024
#define UP_SCALE_Q  10     /* / UP_SCALE right shift bit number */
#define UP_SCALE_Q2 20     /* / UP_SCALE right shift bit number */
#define UP_SCALE_R  0x3FF  /* % UP_SCALE remainder mask */
#define MAXSAMPLE       255
#define CENTERSAMPLE    128

#define DCTSIZE  8      /* The basic DCT block is 8x8 samples */
//#define DCTSIZE2 64     /* DCTSIZE squared; # of elements in a block */
#define UP_SCALE_R  0x3FF  /* % UP_SCALE remainder mask */
#define MAX_iMCU_ROW 305

#define JPG_SOFTWARE_RESIZER_BUFFER_WIDTH_FULLSCREEN     1024//PANEL_WIDTH todo
#define JPG_SOFTWARE_RESIZER_BUFFER_WIDTH_PREVIEW            728

#ifdef JPG_SOFTWARE_SCALER_LAYOUT
#define JPG_SOFTWARE_RESIZER_SCALER_Y_UNIT_START_ADDR   15
#define JPG_SOFTWARE_RESIZER_SCALER_CB_UNIT_START_ADDR  14
#define JPG_SOFTWARE_RESIZER_SCALER_CR_UNIT_START_ADDR  12
#endif

#ifdef JPG_SOFTWARE_OSD_LAYOUT
// 2 1 0 means little endian, 1 2 3 means big endian
#define JPG_SOFTWARE_RESIZER_OSD_Y_UNIT_START_ADDR   2 // 2 1
#define JPG_SOFTWARE_RESIZER_OSD_CB_UNIT_START_ADDR  1// 1  2
#define JPG_SOFTWARE_RESIZER_OSD_CR_UNIT_START_ADDR  0// 0  3
#endif


#define VFIFO_SIZE 0x80000 //500*1024//BS_SIZE


#define IO_BASE_ADDRESS    0x20000000
#define PRS_REG_OFFSET   0x000
#define RW_PRS_VSTART    0x0C
#define RW_PRS_VEND      0x10
#define RW_PRS_V_RP      0x54
#define RW_PRS_V_WP      0x58
#define vWritePRS(dAddr, dVal)  *(volatile UINT32 *)(IO_BASE_ADDRESS + PRS_REG_OFFSET + dAddr) = dVal
#define dReadPRS(dAddr)         *(volatile UINT32 *)(IO_BASE_ADDRESS + PRS_REG_OFFSET + dAddr)

#define RW_PRS_VRP_OFST  0x1C0
//efine PRS_VLD_SEL    ((unsigned)0x1 << 21)
#define PRS_VRP_SEL_HOST    ((unsigned)0x1 << 21)

#define RW_PRS_STC_A       0x1C4
#define RW_PRS_PARCTL      0x1F0
  #define PRS_RST          ((unsigned)0x1 << 0)
  #define PRS_CKE          ((unsigned)0x1 << 1)

#define vPREnableAll()   vWritePRS(RW_PRS_PARCTL, PRS_RST + PRS_CKE)


/******************************************************************************
* Function prototype
******************************************************************************/
INT32 decode_dsp_jpg(UINT16 display_buf_height,
                     UINT16 *display_image_width, UINT16 *display_image_height,
                     resample_mode_enum resample_mode, UINT8 *display_Y_base_addr,
                     UINT8 *display_CbCr_base_addr, UINT32 *x_offset,
                     UINT32 *y_offset, IMG_LIB_INFO_T* prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree);

/******************************************************************************
* Local	variables
******************************************************************************/
#ifdef CHECK_COLOR_BUF
volatile UINT32 DSP_crc_value;
#endif

/* Allow big component number*/
UINT8 jpg_component_0;
UINT8 jpg_component_1;
UINT8 jpg_component_2;

//Extended fifo or not
UINT8   ex_vfifo_done;

UINT8 *_pbDecYaddr, *_pbDecCbCraddr;//, *_pbDisYaddr, *_pbDisCbCraddr;

component_info *cur_comp_info[MAX_COMPS_IN_SCAN];
UINT8 Ss, Se, Ah, Al; /* progressive JPEG parameters for scan */
UINT8 next_restart_num; /* next restart number expected (0-7) */
BOOL has_multiple_scans; /* True if file has multiple scans */
BOOL progressive_mode; /* TRUE if scan script uses progressive mode */
UINT8 max_h_samp_factor; /* largest h_samp_factor */
UINT8 max_v_samp_factor; /* largest v_samp_factor */
UINT16 comps_in_scan; /* number of components encoded in this scan */
UINT8 input_scan_number;
#ifdef Enhance_PJPEG
BOOL first_scan = FALSE;    /* use for Enhance_PJPEG */
#endif
UINT32 _dwJPGReadByteNum;   /* number of byte read from VLD */
UINT32 _dwJPGFileSizeInByte;   /* jpeg file size in byte */

component_info *comp_info;
UINT8 *color_buf[MAX_COMPS_IN_SCAN][2];
UINT16 num_components;
UINT16 pic_width_in_buf;
BYTE _bJPGErrorType;  // - indicates JPEG decoding error type
JPGErrorLevelEnum  _eJPGErrorLevel;  // - indicates JPEG decoding error level
BOOL use_second_color_buf;
UINT16 display_image_width;
UINT16 display_image_height;
UINT32 x_offset, y_offset;

/* record the number of image lines outputted to display buffer */
UINT16 output_image_lines[MAX_COMPS_IN_SCAN];
UINT16 pre_output_image_lines[MAX_COMPS_IN_SCAN];
/* number of iMCU row resampling */
UINT16 num_of_resampling;
/* We have decoded many times of decoding a progressive scan image */
//UINT16 p_scan_decode_num;
/* Number of iMCU rows processed */
UINT16 _iOutputIMCURow;
/* # of iMCU rows to be input to coef ctlr */
UINT16 _iTotalIMCURows;
/* last decode action of progressive_scan ? */
BOOL last_p_scan_decode;
BOOL _fgEnhance_PJPEG = 0;
/* define input image width and height */
UINT16 image_width, image_height;
BOOL _fgEnhance_FastPJPEG = 0;
UINT32 downsized_idctsize;
UINT32 _dwDisplayRegionWidth; // - Define display region width
UINT32 _dwDisplayRegionHeight; // -Define display region height
UINT32 _dwDisplayBufferPitch; // - Define display buffer pitch
UINT32  _dw_h_pixel_ratio_ajust_upscale;
UINT32  _dw_h_pixel_ratio_ajust_downscale;
IDCT_SIZE_ENUM idct_size; /* perfom 8x8, 4x4, 2x2 or 1x1 IDCT operation */

//static JPG_PLAYER  _rJpegPlyr;
static JPG_PLAYER  _rDspJpgPlyr;
static BOOL fgFetchDspColorBuf = FALSE;

UINT32 _dwDspColorAddr;
extern UINT32 _free_buf_addr;
UINT32 _dwColorYSz, _dwColorCSz;

UINT32 h_pixel_ratio, v_pixel_ratio;

UINT8 jpg_SoftResizer_format;
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
UINT32 jpg_software_resizer_buffer_width;
UINT32 jpg_pscan_format_mirror_mode;
UINT32 jpg_pscan_format_row_pages;
#endif

SCALING_INPUT_MODE scaling_input_mode;

//#define JPG_DECODING_ANALYSE
#ifdef JPG_DECODING_ANALYSE
UINT32 dwLinearResampleSTC_0, dwLinearResampleSTC_1, dwLinearResampleSTC_2;
UINT32  stc_jpg_main_decoding_0, stc_jpg_main_decoding_1, stc_jpg_main_decoding_2;
UINT32  stc_dsp_decoding_0, stc_dsp_decoding_1, stc_dsp_decoding_2;
#endif
/******************************************************************************
* externl	variable
******************************************************************************/
//dsp_irq.c
extern UINT8   _u1Ss, _u1Se, _u1Ah, _u1Al;
extern UINT32 _color_buf[3];
//img_flw.c
extern IMG_DEBUG_INFO_T _rImgDecDbg;
/******************************************************************************
* externl	functinons
******************************************************************************/
//dsp_irq.c
extern void vDspCmd (UINT32 dwCmd);
extern void vSetJPEGFileSize(UINT32 dwFileSize);
extern void vDspHoldStartFlag (void);
extern void vDspClrMCUDoneFlag (void);
extern void vSetDspJpgDecodeType(UINT32 u4StartAdr, UINT32 u4EndAdr);
extern void vSetDspThumbnailmode (BOOL flag);
extern BOOL bGetDspThumbnailmode(void);
extern void vDspJpegFifoFull(void);
extern void vSetJpgWp(UINT32 u4JpgWp);
extern UINT32 vReadJpgRp(void);
//extern void vPrSetVFifo(UINT32 dSa, UINT32 dEa);

//misc
void HalFlushInvalidateDCache(void);

/******************************************************************************
* Global Function definition
******************************************************************************/

#ifdef JPG_DECODING_ANALYSE
void reset_jpg_main_stc(void)
{
    stc_jpg_main_decoding_0=0;
    stc_jpg_main_decoding_1=0;
    stc_jpg_main_decoding_2=0;
    dwLinearResampleSTC_0 =0;
    dwLinearResampleSTC_1 =0;    
    dwLinearResampleSTC_2 =0;    
}

void reset_dsp_stc(void)
{
    stc_dsp_decoding_0=0;
    stc_dsp_decoding_1=0;
    stc_dsp_decoding_2=0;
}

UINT32 dwReadStc(void)
{
    return (dReadPRS(RW_PRS_STC_A));
}
#endif

void vPicPlayerSetErrorType(UINT8 bErrType, JPGErrorLevelEnum eErrLevel)
{

    _bJPGErrorType = bErrType;
    _eJPGErrorLevel = eErrLevel;

    return;
}


INT8 jpg_get_componet_num(component_info *compptr)
{
    if ((compptr->component_id >= 1) && (compptr->component_id <= 3))
    {
        return compptr->component_id - 1;
    }
    else if (compptr->component_id == jpg_component_0)
    {
        return Y;
    }
    else if (compptr->component_id == jpg_component_1)
    {
        return Cb;
    }
    else if (compptr->component_id == jpg_component_2)
    {
        return Cr;
    }
    else
    {
        vPicPlayerSetErrorType(0x3B, JPG_FATAL_ERR);
        return 0xff;
    }
}


UINT32 get_x_pixel_value(UINT32 x, UINT8 ci, UINT8 format, INT32 x_dw_offset_tab2[8])
{
    if(format==COLOR_BUFFER_FORMAT)
    {
        return x;
    }
    else
    {
        if(ci==0)
        {
            return (x/8)*16 + x_dw_offset_tab2[(x& 0x7)];
        }
        else if(ci==1)
        {
            x = x/2;
            return (x/4)*16 + x_dw_offset_tab2[(x& 0x3)];
        }
        else if(ci==2)
        {
            x = x/2;        
            return (x/4)*16 + x_dw_offset_tab2[(x& 0x3)];
        }
        else
        {
            //error
            return 0;
        }

    }
}

void process_X_element0( UINT32 component_image_width,
                         UINT32 x_multiple,
                         UINT32 org_comp_image_width_sub_1,
                         UINT8  *color_buf_with_y_addr,
                         UINT8  *color_buf_add_ii_y_X_pic_buf_width,
                         UINT32 ci,
                         UINT32 a,
                         UINT8  *DRAM_col_start_addr,
                         UINT32 x_dw_offset,
                         INT32 x_inc,
#if defined(MEMORY_128BIT_ARRANGE)
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                         INT32 x_dw_offset_tab[8] )
#else
                         INT32 x_dw_offset_tab[16] )
#endif
#elif defined(MEMORY_64BIT_ARRANGE)

                         INT32 x_dw_offset_tab[8] )
#else
                         INT32 x_dw_offset_tab[4] )
#endif
{
    INT32 x;
    UINT32 d_x;
    UINT32 ii_x, i_x;
    UINT32 b, pix1, pix2, pix3, pix4;
#ifdef Enhance_PJPEG
    UINT32 dwTempValue = 0;
    INT32 dwTempValue0 = 0;
    INT32 dwTempValue1 = 0;
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
    UINT8  *DRAM_col_start_addr_tmp = DRAM_col_start_addr;
#endif

    INT32 x_dw_offset_tab2[8]; 
    UINT8 input_mode;    

#ifndef JPG_SOFTWARE_SPECIAL_LAYOUT
    //JPG_SOFTWARE_SPECIAL_LAYOUT will calculate addr by x_dw_offset, it doesn't need to change DRAM_col_start_addr
        DRAM_col_start_addr = DRAM_col_start_addr + x_dw_offset;
#endif


    input_mode = scaling_input_mode;
#ifdef JPG_CAPTURE_LOGO_BY_SCALER_FORMAT
    vScalerLogo_Scaling_init(ci, input_mode, x_dw_offset_tab2);
#endif


    if ( (num_components == 1) && (ci != 0) )
    {
        for (x = 0; x < component_image_width; x = x + x_inc)
        {
            /* output resample row to DRAM display buffer */
            *(DRAM_col_start_addr) = 0x80;
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
            if ( jpg_SoftResizer_format == RASTER_420_MODE )
            {
                DRAM_col_start_addr = DRAM_col_start_addr + 1;
            }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
            {
                DRAM_col_start_addr = DRAM_col_start_addr + x_dw_offset_tab[((x+x_dw_offset) & 0x7)];
            }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
            else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
            {
                    //8223 pscan format
                if (jpg_pscan_format_mirror_mode == 1)
                    {
                    DRAM_col_start_addr = DRAM_col_start_addr_tmp + (jpg_pscan_format_row_pages - 1 - ((x + x_dw_offset) / JPG_DI_BUFFER_ROW_ALIGN_SIZE) ) * JPG_DI_BUFFER_PAGE_SIZE
                                          + ((x + x_dw_offset) % JPG_DI_BUFFER_ROW_ALIGN_SIZE);
                    }
                    else
                    {
                        //8225 pscan format
                        DRAM_col_start_addr = DRAM_col_start_addr_tmp + ((x + x_dw_offset) / JPG_DI_BUFFER_ROW_ALIGN_SIZE) * JPG_DI_BUFFER_PAGE_SIZE + ((x + x_dw_offset) % JPG_DI_BUFFER_ROW_ALIGN_SIZE);
                    }
            }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
            else if ( jpg_SoftResizer_format == OSD_444_MODE )
            {
                DRAM_col_start_addr = DRAM_col_start_addr + (x + x_dw_offset)*4;
            }
#endif
#else
            DRAM_col_start_addr = DRAM_col_start_addr + x_dw_offset_tab[((x+x_dw_offset) & 0xf)];
#endif

        } /* x for loop */
    }
    else
    {

        for (x = 0; x <= component_image_width; x = x + x_inc)
        {

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
            //If it's in PSCAN_420_MODE, pixel addr must be calculated before given value
        if ( jpg_SoftResizer_format == PSCAN_420_MODE)
        {
                //8223 pscan format
                if (jpg_pscan_format_mirror_mode == 1)
                {
                    DRAM_col_start_addr = DRAM_col_start_addr_tmp + (jpg_pscan_format_row_pages - 1 - ((x + x_dw_offset) / JPG_DI_BUFFER_ROW_ALIGN_SIZE) ) * JPG_DI_BUFFER_PAGE_SIZE
                                          + ((x + x_dw_offset) % JPG_DI_BUFFER_ROW_ALIGN_SIZE);
                }
                else
                {
                    //8225 pscan format
                    DRAM_col_start_addr = DRAM_col_start_addr_tmp + ((x + x_dw_offset) / JPG_DI_BUFFER_ROW_ALIGN_SIZE) * JPG_DI_BUFFER_PAGE_SIZE + ((x + x_dw_offset) % JPG_DI_BUFFER_ROW_ALIGN_SIZE);
                }
        }
#endif
#endif
            if (x_multiple < (UP_SCALE*UP_SCALE)) //to prevent from overflow
            {
                d_x = (x * x_multiple) >> UP_SCALE_Q;
            }
            else
            {
                d_x = x * (x_multiple >> UP_SCALE_Q);
            }
            i_x = d_x >> UP_SCALE_Q;


            if (i_x > org_comp_image_width_sub_1)
                i_x = org_comp_image_width_sub_1;
            b = d_x & UP_SCALE_R;

            /* get the four most neighboring pixels in original image */
            /* pix1 pix2                                        */
            /*    O                                           */
            /* pix3 pix4                                        */
            /* We should use pic_width_in_buf instead of image_width to calculate pixel actual address */
            pix1 = *(color_buf_add_ii_y_X_pic_buf_width + get_x_pixel_value(i_x, ci, input_mode, x_dw_offset_tab2));
            pix3 = *(color_buf_with_y_addr + get_x_pixel_value(i_x, ci, input_mode, x_dw_offset_tab2));

            if (i_x  ==  org_comp_image_width_sub_1)
                ii_x = i_x;
            else
                ii_x = i_x + 1;

            pix2 = *(color_buf_add_ii_y_X_pic_buf_width + get_x_pixel_value(ii_x, ci, input_mode, x_dw_offset_tab2));
            pix4 = *(color_buf_with_y_addr + get_x_pixel_value(ii_x, ci, input_mode, x_dw_offset_tab2));

            if(ci==0)
            {
                //add 0xff for alpha value, because this osd plane adopts pixel-alpha mode.
                *(DRAM_col_start_addr+1)=0xff;
            }
            
#ifdef Enhance_PJPEG
            if ((_fgEnhance_PJPEG == TRUE) && (has_multiple_scans))
            {
                //dwTempValue = (UINT32)pix1;
                dwTempValue = (UINT32) ( (UP_SCALE - a) *  ((pix1 << UP_SCALE_Q) + b * (pix2 - pix1) ) +
                                         a * ((pix3 << UP_SCALE_Q) + b * (pix4 - pix3) ) ) >> (UP_SCALE_Q2);
                if (first_scan)
                {
                    *(DRAM_col_start_addr) = (UINT32) dwTempValue;
                }
                else
                {
                    dwTempValue0 = dwTempValue - 128;
                    dwTempValue = *(DRAM_col_start_addr) & 0xff;
                    dwTempValue1 = dwTempValue - 128;
                    dwTempValue1 = (dwTempValue0 + dwTempValue1);
                    dwTempValue1 += 128;
                    if (dwTempValue1 < 0)
                        dwTempValue1 = 0;
                    if (dwTempValue1 > 255)
                        dwTempValue1 = 255;
                    dwTempValue1 = dwTempValue1 & 0xff;
                    *(DRAM_col_start_addr) = (UINT32) (dwTempValue1);
                }
            }
            else
#endif
            {
                
                *(DRAM_col_start_addr) = (UINT32) ( (UP_SCALE - a) *  ((pix1 << UP_SCALE_Q) + b * (pix2 - pix1) ) +
                                                    a * ((pix3 << UP_SCALE_Q) + b * (pix4 - pix3) ) ) >> (UP_SCALE_Q2);
            }

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
            if ( jpg_SoftResizer_format == RASTER_420_MODE )
            {
                if (ci == 0)
                {
                    DRAM_col_start_addr = DRAM_col_start_addr + 1;
                }
                else
                {
                    DRAM_col_start_addr = DRAM_col_start_addr + 2;  //CbCrCbCrCbCr
                }
            }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
            {
                if (ci == 0)
                {
                    DRAM_col_start_addr = DRAM_col_start_addr + x_dw_offset_tab[((x+x_dw_offset) & 0x7)];
                }
                else
                {
                    DRAM_col_start_addr = DRAM_col_start_addr + x_dw_offset_tab[((x+x_dw_offset) & 0x7)];
                }
            }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
            else if ( jpg_SoftResizer_format == OSD_444_MODE )
            {
                DRAM_col_start_addr = DRAM_col_start_addr + 4;
            }
#endif
#else
            DRAM_col_start_addr = DRAM_col_start_addr + x_dw_offset_tab[((x+x_dw_offset) & 0xf)];
#endif
        } /* x for loop */
    }
}

/*****************************************************************
* Function void bilinear_resampling()
* bilinear interpolation to resample the image to desired size
*
****************************************************************/
void bilinear_resampling( UINT16 display_image_width,
                          UINT16 display_image_height,
                          UINT32 x_offset,
                          UINT32 y_offset,
                          UINT8 *display_Y_base_addr,
                          UINT8 *display_CbCr_base_addr,
                          ENUM_IMG_ROTATE_PHASE eRotateDegree,
                          UINT32 dwWhichComp )
{
    UINT32 y;
    UINT32 ci;
    UINT32 d_y;
    UINT32 ii_y, i_y;
    UINT32 a;
    UINT32 y_multiple = 0, x_multiple = 0;
    //INT32  x_off, y_off;
    INT32  y_off;
    component_info *compptr;
    UINT8 *DRAM_col_start_addr = NULL;
    UINT32 component_image_width = 0, component_image_height = 0;
    UINT32 org_comp_image_width_sub_1, org_comp_image_height_sub_1;
    UINT32 cur_x_offset = 0, cur_y_offset;
    INT32 x_inc=0;
    //UINT32 col_size=0;
    UINT32 x_dw_offset = 0;
    UINT32 v_samp_factor_X_DCTSIZE, resamp_num_X_v_samp_X_DCTSIZE;
    UINT8 *color_buf_add_ii_y_X_pic_buf_width, *color_buf_add_ii_y_1_X_pic_buf_width;
    UINT8 *color_buf_with_y_addr;
    //INT32 UP_SCALE_sub_a;
    INT32 x_dw_offset_tab[8];
    UINT16 previous_output_image_lines=0;

#ifdef JPG_DECODING_ANALYSE
    dwLinearResampleSTC_0 = dwReadStc();
#endif

    //if _iTotalIMCURows of picture is only one(resolution:16x16), we don't change use_second_color_buf and do bilinear_resampling_E() immediately
    if (_iTotalIMCURows != 1)
    {
        if (use_second_color_buf == TRUE)
            use_second_color_buf = FALSE;
        else
            use_second_color_buf = TRUE;
    }
    
    //flush&invalid cache
    HalFlushInvalidateDCache();

    if(eRotateDegree!=CLOCKWISE_0)    
    {
        previous_output_image_lines = pre_output_image_lines[0];
    }

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
    if ( jpg_SoftResizer_format == PSCAN_420_MODE)
        {
            if (bSharedInfo(SI_PREVIEW_MODE) == TRUE)
            {
            //preview mode
                jpg_software_resizer_buffer_width = JPG_DI_BUFFER_PREVIEW_PAGE_ROW_SIZE;
                jpg_pscan_format_row_pages = JPG_DI_BUFFER_PREVIEW_ROW_PAGES;
            }
            else
            {
            //digest mode
                jpg_software_resizer_buffer_width = JPG_DI_BUFFER_PAGE_ROW_SIZE;
                jpg_pscan_format_row_pages = JPG_DI_BUFFER_ROW_PAGES;
            }
        }
        else
#endif
        {
            #ifdef JPG_CAPTURE_LOGO_BY_SCALER_FORMAT
            if(slogoenable==TRUE)
            {
                jpg_software_resizer_buffer_width = jpg_software_resizer_buffer_width_by_scaler_logo;
            }
            else 
            #endif
            {
                if(0) //(bSharedInfo(SI_PREVIEW_MODE) == TRUE)
                {
                    jpg_software_resizer_buffer_width = JPG_SOFTWARE_RESIZER_BUFFER_WIDTH_PREVIEW;
                }
                else
                {
                    jpg_software_resizer_buffer_width = _dwDisplayBufferPitch;//JPG_SOFTWARE_RESIZER_BUFFER_WIDTH_FULLSCREEN;
                }
            }
        }
#endif

    for (ci = 0, compptr = comp_info; ci < 3; ci++, compptr++)
    {
        if ( ( (dwWhichComp == Y) && (ci != 0) ) || ( (dwWhichComp == Cb) && (ci != 1) ) || ( (dwWhichComp == Cr) && (ci != 2) ) )
        {
            continue;
        }

        /* calculate the DRAM start address of the interpolation line */
        if (ci == 0) /* Y line */
        {
            /* MB row height is 32 in Y; 16 in CrCb */
            //col_size = 32;
            component_image_height = display_image_height;
            component_image_width = display_image_width;
            x_inc = 1;
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            x_dw_offset_tab[0] = -2;
            x_dw_offset_tab[1] = -2;
            x_dw_offset_tab[2] = -2;
            x_dw_offset_tab[3] = -2;
            x_dw_offset_tab[4] = -2;
            x_dw_offset_tab[5] = -2;
            x_dw_offset_tab[6] = -2;
            x_dw_offset_tab[7] = 30;
#endif            
        }
        else /* CbCr line */
        {
            /* MB row height is 32 in Y; 16 in CrCb */
            //col_size = 16;
            /* in 4:2:0 image format, the CbCr width is half the Y's */
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
            if ( jpg_SoftResizer_format == RASTER_420_MODE )
            {
                component_image_height = (display_image_height + 1) >> 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 2;
            }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
            {
                component_image_height = (display_image_height + 1) >> 1 << 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 2;
            }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
            else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
            {
                component_image_height = (display_image_height + 1) >> 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 2;
            }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
            else if ( jpg_SoftResizer_format == OSD_444_MODE)
            {
                component_image_height = (display_image_height + 1) >> 1 << 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 1;
            }
#endif

#else
            component_image_height = (display_image_height + 1) >> 1;
            component_image_width = (display_image_width + 1) >> 1 << 1;
            x_inc = 2;
#endif
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            x_dw_offset_tab[0] = -4;
            x_dw_offset_tab[1] = 0;
            x_dw_offset_tab[2] = -4;
            x_dw_offset_tab[3] = 0;
            x_dw_offset_tab[4] = -4;
            x_dw_offset_tab[5] = 0;
            x_dw_offset_tab[6] = 28;
            x_dw_offset_tab[7] = 0;
#endif                
        }

#ifdef JPG_CAPTURE_LOGO_BY_SCALER_FORMAT
        vScalerLogoSetFinalOutputWH(component_image_width, component_image_height);
#endif

        //x_off = 0;
        y_off = 0;
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
        if ( jpg_SoftResizer_format == RASTER_420_MODE )
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#ifdef  JPG_SOFTWARE_SCALER_LAYOUT
        else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((ci == 0) ? 1 : 1)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
        else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
        else if ( jpg_SoftResizer_format == OSD_444_MODE)
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((ci == 0) ? 1 : 1)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((ci == 0) ? 1 : 1)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#endif

#else
        x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
        y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((ci == 0) ? 1 : 2)) << UP_SCALE_Q) / max_v_samp_factor;
#endif

        if ( jpg_SoftResizer_format != OSD_444_MODE)
        {
            if (ci != 0)
                x_multiple = x_multiple / 2;
        }

        org_comp_image_height_sub_1 = ( (image_height * compptr->v_samp_factor * downsized_idctsize) / (max_v_samp_factor * DCTSIZE)) - 1;
        org_comp_image_width_sub_1 = ( (image_width * compptr->h_samp_factor * downsized_idctsize) / (max_h_samp_factor * DCTSIZE)) - 1;

        v_samp_factor_X_DCTSIZE = compptr->v_samp_factor * downsized_idctsize;
        resamp_num_X_v_samp_X_DCTSIZE = num_of_resampling * v_samp_factor_X_DCTSIZE;

        for (y = output_image_lines[ci]; y <= component_image_height; y++)
        {
            /* Since we will output image to 4:2:0 format YCBCR, we multiple 2 in CBCR case */
            if (y_multiple < (UP_SCALE*UP_SCALE)) //to prevent from overflow
            {
                d_y = (y *  y_multiple) >> UP_SCALE_Q;
            }
            else
            {
                d_y = y *  (y_multiple >> UP_SCALE_Q);
            }
            d_y = d_y + y_off;
            i_y = d_y >> UP_SCALE_Q;
            if (i_y > org_comp_image_height_sub_1)
            {
                i_y = org_comp_image_height_sub_1;
            }
            a = d_y & UP_SCALE_R;
            ii_y = i_y - resamp_num_X_v_samp_X_DCTSIZE;
            color_buf_add_ii_y_X_pic_buf_width = color_buf[ci][use_second_color_buf] + ii_y * pic_width_in_buf;
            color_buf_add_ii_y_1_X_pic_buf_width = color_buf_add_ii_y_X_pic_buf_width + pic_width_in_buf;
            //UP_SCALE_sub_a = UP_SCALE - a;

            if (i_y == org_comp_image_height_sub_1)
            {
                color_buf_with_y_addr = color_buf_add_ii_y_X_pic_buf_width;
            }
            else
            {
                if ( (ii_y + 1) == v_samp_factor_X_DCTSIZE )
                {
                    /* use another color buffer's first line data */

                    color_buf_with_y_addr = color_buf[ci][((use_second_color_buf == 1)? 0 : 1)];

                }
                else
                {
                    color_buf_with_y_addr = color_buf_add_ii_y_1_X_pic_buf_width;
                }
            }

            if (ii_y >= v_samp_factor_X_DCTSIZE ) /* the below interpolation line don't exist in the current color_buf */
            {
                /* we have done all the resampling lines of one component in this iMCU */
                break;
            }
            else  /* the next interpolation lines stored in the current color_buf */
            {
                /* calculate the DRAM start address of the interpolation line */
                if (ci == 0) /* Y line */
                {
                    /* CbCr motion vector is half the Y's */
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                        if ( jpg_SoftResizer_format == RASTER_420_MODE )
                        {
                            cur_x_offset = x_offset;
                            cur_y_offset = y + y_offset;
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width + display_Y_base_addr + cur_x_offset;
                        }
#ifdef  JPG_SOFTWARE_SCALER_LAYOUT
                        else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
                        {
                            cur_x_offset = x_offset & (~0x7);
                            cur_y_offset = y + y_offset;
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 2 + display_Y_base_addr + cur_x_offset * 2 + JPG_SOFTWARE_RESIZER_SCALER_Y_UNIT_START_ADDR;
                        }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
                        else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
                        {
                            cur_x_offset = x_offset;
                            cur_y_offset = y + y_offset;
                            DRAM_col_start_addr = display_Y_base_addr + (cur_y_offset / JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * jpg_software_resizer_buffer_width + (cur_y_offset % JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * JPG_DI_BUFFER_ROW_ALIGN_SIZE;

                            if ((cur_y_offset / JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) % 2 == 1)  //Y change to mirror pages every 8 lines
                            {
                                //page mirror mode for 8223
                                jpg_pscan_format_mirror_mode = 1;
                            }
                            else
                            {
                                jpg_pscan_format_mirror_mode = 0;
                            }
                        }
#endif
#ifdef  JPG_SOFTWARE_OSD_LAYOUT
                        else if ( jpg_SoftResizer_format == OSD_444_MODE )
                        {
                            if(eRotateDegree!=CLOCKWISE_0)
                            {
                                    cur_x_offset = 0;
                                    cur_y_offset = y- previous_output_image_lines;  
                                    DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_Y_UNIT_START_ADDR;                            
                            }            
                            else
                            {
                                cur_x_offset = x_offset;
                                cur_y_offset = y + y_offset;
                                DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_Y_UNIT_START_ADDR;
                            }
                            
                        }
#endif
#endif
                }
                else /* CbCr line */
                {
                    /* CbCr motion vector is half the Y's */

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                        if ( jpg_SoftResizer_format == RASTER_420_MODE )
                        {
                            cur_x_offset = (x_offset >> 1) << 1;
                            cur_y_offset = y + (y_offset >> 1);
                            if (ci == 1)
                            {
                                DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width + display_CbCr_base_addr + cur_x_offset;
                            }
                            else if (ci == 2)
                            {
                                DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width + display_CbCr_base_addr + cur_x_offset + 1;
                            }
                        }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
                        else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
                        {
                            cur_x_offset = x_offset & (~0x7);
                            cur_y_offset = y + y_offset;

                            if (ci == 1)
                            {
                                DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 2 + display_Y_base_addr + cur_x_offset * 2 + JPG_SOFTWARE_RESIZER_SCALER_CB_UNIT_START_ADDR;
                            }
                            else if (ci == 2)
                            {
                                DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 2 + display_Y_base_addr + cur_x_offset * 2 + JPG_SOFTWARE_RESIZER_SCALER_CR_UNIT_START_ADDR;
                            }
                        }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
                        else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
                        {
                            cur_x_offset = (x_offset >> 1) << 1;
                            cur_y_offset = y + (y_offset >> 1);

                            DRAM_col_start_addr = display_CbCr_base_addr + (cur_y_offset / JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * jpg_software_resizer_buffer_width + (cur_y_offset % JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * JPG_DI_BUFFER_ROW_ALIGN_SIZE;

                            if ((cur_y_offset / (JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE)) % 2 == 1) //CbCr change to mirror pages every 8 lines
                            {
                                //page mirror mode for 8223
                                jpg_pscan_format_mirror_mode = 1;
                            }
                            else
                            {
                                jpg_pscan_format_mirror_mode = 0;
                            }

                            if (ci == 2)
                            {
                                cur_x_offset = cur_x_offset + 1;
                            }
                        }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
                        else if ( jpg_SoftResizer_format == OSD_444_MODE )
                        {
                            if(eRotateDegree!=CLOCKWISE_0)
                            {
                                cur_x_offset = 0;
                                cur_y_offset = y- previous_output_image_lines;  

                                if (ci == 1)
                                {
                                    DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_CB_UNIT_START_ADDR;
                                }
                                else if (ci == 2)
                                {
                                    DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_CR_UNIT_START_ADDR;
                                }    
                            }
                            else
                            {
                                cur_x_offset = (x_offset >> 1) << 1;
                                cur_y_offset = y + y_offset;

                                if (ci == 1)
                                {
                                    DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_CB_UNIT_START_ADDR;
                                }
                                else if (ci == 2)
                                {
                                    DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_CR_UNIT_START_ADDR;
                                }
                            }
                        }
#endif

#endif
                }

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                x_dw_offset = cur_x_offset;
#else
                //calculate x_offest in a block
                x_dw_offset = cur_x_offset & 0xf;
#endif

                process_X_element0(component_image_width, x_multiple,
                                    org_comp_image_width_sub_1,
                                    color_buf_with_y_addr,
                                    color_buf_add_ii_y_X_pic_buf_width,
                                    ci, a, DRAM_col_start_addr, x_dw_offset, x_inc,
                                    x_dw_offset_tab);
                /* We have resampled and outputted this line, increment output_image_lines[ci] */
                output_image_lines[ci]++;
            } /* if (i_y) statement */
        } /* y for loop */
    } /* ci for loop */
    /* increases num_of_resampling by 1 */
    num_of_resampling = num_of_resampling + 1;

    #ifdef JPG_DECODING_ANALYSE
        dwLinearResampleSTC_1 = (dwReadStc() - dwLinearResampleSTC_0);
        dwLinearResampleSTC_2 += dwLinearResampleSTC_1;
    #endif    
}

void bilinear_resampling_E(UINT16 display_image_width, UINT16 display_image_height, UINT32 x_offset, UINT32 y_offset, UINT8 *display_Y_base_addr, UINT8 *display_CbCr_base_addr, ENUM_IMG_ROTATE_PHASE eRotateDegree, UINT32 dwWhichComp)
{
    UINT32 y;
    UINT32 ci;
    UINT32 d_y;
    UINT32 ii_y, i_y;
    UINT32 a;
    UINT32 y_multiple = 0, x_multiple = 0;
    //INT32  x_off, y_off;
    INT32  y_off;    
    component_info *compptr;
    UINT8 *DRAM_col_start_addr = NULL;
    UINT32 component_image_width = 0, component_image_height = 0;
    UINT32 org_comp_image_width_sub_1, org_comp_image_height_sub_1;
    UINT32 cur_x_offset = 0, cur_y_offset = 0;
    INT32 x_inc = 0;
    //UINT32 col_size=0;    /* MB row height is 32 in Y; 16 in CrCb */
    UINT32 x_dw_offset;  /* indicate the x_dw_offset in display buffer */
    UINT32 v_samp_factor_X_DCTSIZE, resamp_num_X_v_samp_X_DCTSIZE;
    UINT8 *color_buf_add_ii_y_X_pic_buf_width, *color_buf_add_ii_y_1_X_pic_buf_width;
    UINT8 *color_buf_with_y_addr;
    //INT32 UP_SCALE_sub_a;
    INT8 bCurCompId;
    INT32 x_dw_offset_tab[8];

#ifdef JPG_DECODING_ANALYSE
    dwLinearResampleSTC_0 = dwReadStc();
#endif

    //if _iTotalIMCURows of picture is only one(resolution:16x16), we don't change use_second_color_buf and do bilinear_resampling_E() immediately
    if (_iTotalIMCURows != 1)
    {
        if (use_second_color_buf == TRUE)
            use_second_color_buf = FALSE;
        else
            use_second_color_buf = TRUE;
    }

    //flush&invalid cache
    HalFlushInvalidateDCache();

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
    if ( jpg_SoftResizer_format == PSCAN_420_MODE)
    {
        if (bSharedInfo(SI_PREVIEW_MODE) == TRUE)
        {
            jpg_software_resizer_buffer_width = JPG_DI_BUFFER_PREVIEW_PAGE_ROW_SIZE;
            jpg_pscan_format_row_pages = JPG_DI_BUFFER_PREVIEW_ROW_PAGES;
        }
        else
        {
            jpg_software_resizer_buffer_width = JPG_DI_BUFFER_PAGE_ROW_SIZE;
            jpg_pscan_format_row_pages = JPG_DI_BUFFER_ROW_PAGES;
        }
    }
    else
#endif
    {
        if (0)//(bSharedInfo(SI_PREVIEW_MODE) == TRUE)
        {
            jpg_software_resizer_buffer_width = JPG_SOFTWARE_RESIZER_BUFFER_WIDTH_PREVIEW;
        }
        else
        {
            jpg_software_resizer_buffer_width = _dwDisplayBufferPitch;//JPG_SOFTWARE_RESIZER_BUFFER_WIDTH_FULLSCREEN;
        }
    }
#endif

    for (ci = 0; ci < comps_in_scan; ci++)
    {
        compptr = cur_comp_info[ci];
        //bCurCompId = (compptr->component_id - 1);
        bCurCompId = jpg_get_componet_num(compptr);

        if (((dwWhichComp == Y) && (bCurCompId != 0)) ||
                ((dwWhichComp == Cb) && (bCurCompId != 1)) ||
                ((dwWhichComp == Cr) && (bCurCompId != 2)))
        {
            continue;
        }

        // calculate the DRAM start address of the interpolation line
        // MB row height is 32 in Y; 16 in CrCb
        if (bCurCompId == 0) /* Y line */
        {
            //col_size = 32;
            component_image_height = display_image_height;
            component_image_width = display_image_width;
            x_inc = 1;
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            x_dw_offset_tab[0] = -2;
            x_dw_offset_tab[1] = -2;
            x_dw_offset_tab[2] = -2;
            x_dw_offset_tab[3] = -2;
            x_dw_offset_tab[4] = -2;
            x_dw_offset_tab[5] = -2;
            x_dw_offset_tab[6] = -2;
            x_dw_offset_tab[7] = 30;
#endif
        }
        else /* CbCr line */
        {
            /* MB row height is 32 in Y; 16 in CrCb */
            //col_size = 16;
            /* in 4:2:0 image format, the CbCr width is half the Y's */
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
            if ( jpg_SoftResizer_format == RASTER_420_MODE )
            {
                component_image_height = (display_image_height + 1) >> 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 2;
            }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
            {
                component_image_height = (display_image_height + 1) >> 1 << 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 2;
            }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
            else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
            {
                component_image_height = (display_image_height + 1) >> 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 2;
            }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
            else if ( jpg_SoftResizer_format == OSD_444_MODE)
            {
                component_image_height = (display_image_height + 1) >> 1 << 1;
                component_image_width = (display_image_width + 1) >> 1 << 1;
                x_inc = 1;
            }
#endif
#else
            component_image_height = (display_image_height + 1) >> 1;
            component_image_width = (display_image_width + 1) >> 1 << 1;
            x_inc = 2;
#endif

#ifdef JPG_SOFTWARE_SCALER_LAYOUT
            x_dw_offset_tab[0] = -4;
            x_dw_offset_tab[1] = 0;
            x_dw_offset_tab[2] = -4;
            x_dw_offset_tab[3] = 0;
            x_dw_offset_tab[4] = -4;
            x_dw_offset_tab[5] = 0;
            x_dw_offset_tab[6] = 28;
            x_dw_offset_tab[7] = 0;
#endif
        }

        //x_off = 0;
        y_off = 0;
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
        if ( jpg_SoftResizer_format == RASTER_420_MODE )
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
        else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((bCurCompId == 0) ? 1 : 1)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
        else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
        else if ( jpg_SoftResizer_format == OSD_444_MODE)
        {
            x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((bCurCompId == 0) ? 1 : 1)) << UP_SCALE_Q) / max_h_samp_factor;
            y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((bCurCompId == 0) ? 1 : 1)) << UP_SCALE_Q) / max_v_samp_factor;
        }
#endif
#else
        x_multiple = ((h_pixel_ratio * compptr->h_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_h_samp_factor;
        y_multiple = ((v_pixel_ratio * compptr->v_samp_factor * ((bCurCompId == 0) ? 1 : 2)) << UP_SCALE_Q) / max_v_samp_factor;
#endif

        if ( jpg_SoftResizer_format != OSD_444_MODE)
        {
            if (bCurCompId != 0)
                x_multiple = x_multiple / 2;
        }
        org_comp_image_height_sub_1 = ( (image_height * compptr->v_samp_factor * downsized_idctsize) / (max_v_samp_factor * DCTSIZE)) - 1;
        org_comp_image_width_sub_1 = ( (image_width * compptr->h_samp_factor * downsized_idctsize) / (max_h_samp_factor * DCTSIZE)) - 1;

        v_samp_factor_X_DCTSIZE = compptr->v_samp_factor * downsized_idctsize;
        resamp_num_X_v_samp_X_DCTSIZE = num_of_resampling * v_samp_factor_X_DCTSIZE;

        for (y = output_image_lines[bCurCompId]; y <= component_image_height; y++)
        {
            /* Since we will output image to 4:2:0 format YCBCR, we multiple 2 in CBCR case */
            if (y_multiple < (UP_SCALE*UP_SCALE)) //to prevent from overflow
            {
                d_y = (y *  y_multiple) >> UP_SCALE_Q;
            }
            else
            {
                d_y = y *  (y_multiple >> UP_SCALE_Q);
            }
            d_y = d_y + y_off;
            i_y = d_y >> UP_SCALE_Q;
            if (i_y > org_comp_image_height_sub_1)
            {
                i_y = org_comp_image_height_sub_1;
            }

            a = d_y & UP_SCALE_R;
            ii_y = i_y - resamp_num_X_v_samp_X_DCTSIZE;
            color_buf_add_ii_y_X_pic_buf_width = color_buf[bCurCompId][use_second_color_buf] + ii_y * pic_width_in_buf;
            color_buf_add_ii_y_1_X_pic_buf_width = color_buf_add_ii_y_X_pic_buf_width + pic_width_in_buf;
            //UP_SCALE_sub_a = UP_SCALE - a;


            if (i_y == org_comp_image_height_sub_1)
            {
                color_buf_with_y_addr = color_buf_add_ii_y_X_pic_buf_width;
            }
            else
            {
                if ( (ii_y + 1) == v_samp_factor_X_DCTSIZE )
                {
                    /* use another color buffer's first line data */

                    color_buf_with_y_addr = color_buf[bCurCompId][((use_second_color_buf == 1)? 0 : 1)];

                }
                else
                {
                    color_buf_with_y_addr = color_buf_add_ii_y_1_X_pic_buf_width;
                }
            }

            if (ii_y >= v_samp_factor_X_DCTSIZE ) /* the below interpolation line don't exist in the current color_buf */
            {
                /* we have done all the resampling lines of one component in this iMCU */
                break;
            }
            else  /* the next interpolation lines stored in the current color_buf */
            {
                /* calculate the DRAM start address of the interpolation line */
                if (bCurCompId == 0) /* Y line */
                {
                    /* CbCr motion vector is half the Y's */
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                    if ( jpg_SoftResizer_format == RASTER_420_MODE )
                    {
                        cur_x_offset = x_offset;
                        cur_y_offset = y + y_offset;
                        DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width + display_Y_base_addr + cur_x_offset;
                    }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
                    else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
                    {
                        cur_x_offset = x_offset & (~0x7);
                        cur_y_offset = y + y_offset;
                        DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 2 + display_Y_base_addr + cur_x_offset * 2 + JPG_SOFTWARE_RESIZER_SCALER_Y_UNIT_START_ADDR;
                    }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
                    else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
                    {
                        cur_x_offset = x_offset;
                        cur_y_offset = y + y_offset;
                        DRAM_col_start_addr = display_Y_base_addr + (cur_y_offset / JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * jpg_software_resizer_buffer_width + (cur_y_offset % JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * JPG_DI_BUFFER_ROW_ALIGN_SIZE;

                        if ((cur_y_offset / JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) % 2 == 1)  //Y change to mirror pages every 8 lines
                        {
                            //page mirror mode for 8223
                            jpg_pscan_format_mirror_mode = 1;
                        }
                        else
                        {
                            jpg_pscan_format_mirror_mode = 0;
                        }
                    }
#endif
#ifdef  JPG_SOFTWARE_OSD_LAYOUT
                    else if ( jpg_SoftResizer_format == OSD_444_MODE )
                    {
                        cur_x_offset = x_offset;
                        cur_y_offset = y + y_offset;
                        DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_Y_UNIT_START_ADDR;
                    }
#endif

#endif

                }
                else /* CbCr line */
                {
                    /* CbCr motion vector is half the Y's */
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                    if ( jpg_SoftResizer_format == RASTER_420_MODE )
                    {
                        cur_x_offset = (x_offset >> 1) << 1;
                        cur_y_offset = y + (y_offset >> 1);
                        if (bCurCompId == 1)
                        {
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width + display_CbCr_base_addr + cur_x_offset;
                        }
                        else if (bCurCompId == 2)
                        {
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width + display_CbCr_base_addr + cur_x_offset + 1;
                        }
                    }
#ifdef JPG_SOFTWARE_SCALER_LAYOUT
                    else if ( jpg_SoftResizer_format == SCALER_422_MDOE )
                    {
                        cur_x_offset = x_offset & (~0x7);
                        cur_y_offset = y + y_offset;

                        if (bCurCompId == 1)
                        {
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 2 + display_Y_base_addr + cur_x_offset * 2 + JPG_SOFTWARE_RESIZER_SCALER_CB_UNIT_START_ADDR;
                        }
                        else if (bCurCompId == 2)
                        {
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 2 + display_Y_base_addr + cur_x_offset * 2 + JPG_SOFTWARE_RESIZER_SCALER_CR_UNIT_START_ADDR;
                        }
                    }
#endif
#ifdef JPG_SOFTWARE_PSCAN_LAYOUT
                    else if ( jpg_SoftResizer_format == PSCAN_420_MODE)
                    {
                        cur_x_offset = (x_offset >> 1) << 1;
                        cur_y_offset = y + (y_offset >> 1);

                        DRAM_col_start_addr = display_CbCr_base_addr + (cur_y_offset / JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * jpg_software_resizer_buffer_width + (cur_y_offset % JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE) * JPG_DI_BUFFER_ROW_ALIGN_SIZE;

                        if ((cur_y_offset / (JPG_DI_BUFFER_ROW_COUNTS_IN_PAGE)) % 2 == 1)  //CbCr change to mirror pages every 8 lines
                        {
                            //page mirror mode for 8223
                            jpg_pscan_format_mirror_mode = 1;
                        }
                        else
                        {
                            jpg_pscan_format_mirror_mode = 0;
                        }

                        if (bCurCompId == 2)
                        {
                            cur_x_offset = cur_x_offset + 1;
                        }
                    }
#endif
#ifdef JPG_SOFTWARE_OSD_LAYOUT
                    else if ( jpg_SoftResizer_format == OSD_444_MODE )
                    {
                        cur_x_offset = (x_offset >> 1) << 1;
                        cur_y_offset = y + y_offset;
                        
                        if (bCurCompId == 1)
                        {
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_CB_UNIT_START_ADDR;
                        }
                        else if (bCurCompId == 2)
                        {
                            DRAM_col_start_addr = cur_y_offset * jpg_software_resizer_buffer_width * 4 + display_Y_base_addr + cur_x_offset * 4 + JPG_SOFTWARE_RESIZER_OSD_CR_UNIT_START_ADDR;
                        }
                    }
#endif

#endif
                }

#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
                x_dw_offset = cur_x_offset;
#else
                //calculate x_offest in a block
                x_dw_offset = cur_x_offset & 0xf;
#endif

                process_X_element0(component_image_width, x_multiple, 
                                    org_comp_image_width_sub_1, 
                                    color_buf_with_y_addr, 
                                    color_buf_add_ii_y_X_pic_buf_width, 
                                    bCurCompId, a, DRAM_col_start_addr, x_dw_offset, x_inc, 
                                    x_dw_offset_tab);
                /* We have resampled and outputted this line, increment output_image_lines[ci] */
                output_image_lines[bCurCompId]++;
            } /* if (i_y) statement */
        } /* y for loop */
    } /* ci for loop */
    /* increases num_of_resampling by 1 */
    num_of_resampling = num_of_resampling + 1;

#ifdef JPG_DECODING_ANALYSE
    dwLinearResampleSTC_1 = (dwReadStc() - dwLinearResampleSTC_0);
    dwLinearResampleSTC_2 += dwLinearResampleSTC_1;
#endif

} /* bilinear_resampling() function  */



void vGetProgressiveSos(UINT8 n, UINT8 cc)
{
    INT16 i, ci;
    component_info * compptr;

    JPGLogS("--->ProgressiveSos");

    comps_in_scan = (UINT8) n;

    if (input_scan_number)
    {
#ifdef Enhance_PJPEG
        first_scan = FALSE;
#endif

        for (i = 0; i < n; i++)
        {

            for (ci = 0, compptr = comp_info; ci < num_components; ci++, compptr++)
            {
                if ((cc) == compptr->component_id)
                    goto id_found;
            }

            vPicPlayerSetErrorType(0x11, JPG_FATAL_ERR);
            return;


id_found:

            cur_comp_info[i] = compptr;
        }

    }
    else//first scan
    {
#ifdef Enhance_PJPEG
        first_scan = TRUE;
#endif

        if (comps_in_scan == MAX_COMPS_IN_SCAN)
        {
            cur_comp_info[0] = comp_info;
            cur_comp_info[1] = (comp_info + 1);
            cur_comp_info[2] = (comp_info + 2);

        }
    }

    /* Count another SOS marker */
    (input_scan_number)++;

    use_second_color_buf = 0;

    Ss = _u1Ss;
    Se = _u1Se;
    Ah = _u1Ah;
    Al =   _u1Al;


    if (Ah == 0)   //CJYang, reduce scan number
    {
        _iOutputIMCURow = 0;
        output_image_lines[0] = 0;
        output_image_lines[1] = 0;
        output_image_lines[2] = 0;
        num_of_resampling = 0;
    }
    else
    {  //make sure progressive finsh scan

        _iOutputIMCURow = 0;
        output_image_lines[0] = 0;
        output_image_lines[1] = 0;
        output_image_lines[2] = 0;
        num_of_resampling = 0;
        //unread_marker = 0;  /* processed the marker */
        // return JPEG_REACHED_EOI;
    }
}
void vSetColorSz(UINT32 *_color_buf)
{
    JPGLogS("vSetColorSz\n");

    _dwDspColorAddr = _color_buf[0];
    _dwColorYSz = _color_buf[1] - _color_buf[0];
    _dwColorCSz = _color_buf[2] - _color_buf[1];

    JPGLogD(_dwColorYSz);
    JPGLogD(_dwColorCSz);

    if ((!_dwColorYSz && !_fgEnhance_PJPEG) || !_free_buf_addr || !_dwDspColorAddr)
    {
        vDspJpgErrStatus();
    }

#ifdef REDUCE_COLOR_BUF

    color_buf[0][0] = (UINT8*)_dwDspColorAddr + DRAMA_CACH_BASE_ADDRESS;
    color_buf[1][0] = color_buf[0][0] + _dwColorYSz;
    color_buf[2][0] = color_buf[1][0] + _dwColorCSz;

    color_buf[0][1] =  (UINT8*) _free_buf_addr + DRAMA_CACH_BASE_ADDRESS;
    color_buf[1][1] =  color_buf[0][1] + _dwColorYSz;
    color_buf[2][1] =  color_buf[1][1] + _dwColorCSz;

#endif
}


UINT32 dwGetDJpgAreaSa(void)
{
    UINT32 dTmp = 0;
    //ASSERT(_rImgDecDbg.rBufInfo.pu1WorkingBuf  != NULL);
    dTmp = (UINT32)_rImgDecDbg.rBufInfo.pu1WorkingBuf + VFIFO_SIZE + JPG_ROTATION_TEMP_BUFFER_SIZE + JPG_WORKING_BUFFER_SIZE;
    dTmp = MM_ALIGN(0x100, dTmp);

    return (UINT32)dwAbsDramANc(dTmp);
}

UINT32 dwGetDJpgAreaEa(void)
{
    UINT32 dTmp = 0;
    
    //ASSERT(_rImgDecDbg.rBufInfo.pu1WorkingBuf  != NULL);
    dTmp = (UINT32)_rImgDecDbg.rBufInfo.pu1WorkingBuf + JPG_TOTAL_BUFFER_SIZE;
    dTmp = MM_ALIGN(0x100, dTmp);

    return (UINT32)dwAbsDramANc(dTmp);
}

//because support width maybe exceed 65536, so we declare UINT32
UINT32 dwCheckDspJpgresolution(void)
{
    UINT32 buffer_size;
    UINT32 width;
    buffer_size = dwGetDJpgAreaEa() - dwGetDJpgAreaSa();

    //Y: W*16, Cb: W*8, Cr: W*8, therefore W = size/32;
    width = buffer_size / 32;

    return width;
}
#if defined(CHECK_COLOR_BUF)

/* Polynomial:
 * x^32 + x^26 + x^23 + x^22 + x^16 + + x^12 + x^11 + x^10 + x^8 + x^7
 * + x^5 + x^4 + x^2 + x + 1 */
#define CRC32_POLYNOMIAL        0x04C11DB7
#define CRC32_INIT              0xFFFFFFFF

// Change the definition of CRC_BITS for different number of CRC bits.
#define CRC_BITS                32
#define MSB_BIT                 (CRC_BITS - 1)
#define KOALA_CRC       0x63a8e07d
static UINT32 ComputeCrc(UINT8 *msg, UINT32 msgSize)
{
    UINT32 u4ByteIndex, u4Remainder;
    UINT8 u1Bit, u1BitIndex;

    u4Remainder = CRC32_INIT;

    for (u4ByteIndex = 0; u4ByteIndex < msgSize; u4ByteIndex++)
    {
        for (u1BitIndex = 0; u1BitIndex < 8; u1BitIndex++)
        {
            u1Bit = (msg[u4ByteIndex] & (1 << (7 - u1BitIndex)) ? 1 : 0);
            if ((u4Remainder & (1U << MSB_BIT)) ^ (u1Bit << MSB_BIT))
            {
                u4Remainder = (u4Remainder << 1) ^ CRC32_POLYNOMIAL;
            }
            else
            {
                u4Remainder <<= 1;
            }
        }
    }

    return u4Remainder;
}
#endif

#ifdef REDUCE_COLOR_BUF
void vMoveColorBuf(void)
{
    //component_info *compptr;
    //INT8 bCurCompId;

    // JPGLogS("-->vMoveColorBuf");
#ifdef CHECK_COLOR_BUF
//    extern UINT32 ComputeCrc(UINT8 *msg, UINT32 msgSize);
    DSP_crc_value += ComputeCrc((UINT8*)_dwDspColorAddr, (_dwColorYSz ));
#endif

    vDrvOsdDramCopy((void *)(_dwDspColorAddr+DRAMA_CACH_BASE_ADDRESS), (void *)(_free_buf_addr+DRAMA_CACH_BASE_ADDRESS),  (_dwColorYSz + 2*_dwColorCSz));

#ifdef FAST_ENHANCE_PJPEG_FOR_DSP
    if (_fgEnhance_FastPJPEG)
    {
        if (comps_in_scan > 1)
        {
            vEnhanceMode_RefineFrame((UINT8 *)_dwDspColorAddr, dwGetPictureWorkingBufEnd() + dwGetPictureColorBufSize(ALL)*_iOutputIMCURow, dwGetPictureColorBufSize(ALL));
        }
        else
        {
            compptr = cur_comp_info[0];
            //bCurCompId = (compptr->component_id - 1);
            bCurCompId = jpg_get_componet_num(compptr);

            if (bCurCompId == Y)
            {
                //add Y component data
                vEnhanceMode_RefineFrame((UINT8 *)_dwDspColorAddr, dwGetPictureWorkingBufEnd() + dwGetPictureColorBufSize(ALL)*_iOutputIMCURow, dwGetPictureColorBufSize(Y));
            }
            else if (bCurCompId == Cb)
            {
                //add cb component data
                vEnhanceMode_RefineFrame((UINT8 *)(_dwDspColorAddr + dwGetPictureColorBufSize(Y)), dwGetPictureWorkingBufEnd() + dwGetPictureColorBufSize(ALL)*_iOutputIMCURow + dwGetPictureColorBufSize(Y), dwGetPictureColorBufSize(Cb));
            }
            else
            {
                //add cr component data
                vEnhanceMode_RefineFrame((UINT8 *)(_dwDspColorAddr + dwGetPictureColorBufSize(Y) + dwGetPictureColorBufSize(Cb)), dwGetPictureWorkingBufEnd() + dwGetPictureColorBufSize(ALL)*_iOutputIMCURow + dwGetPictureColorBufSize(Y) + dwGetPictureColorBufSize(Cb), dwGetPictureColorBufSize(Cr));
            }
        }
    }
#endif

    if ((use_second_color_buf) == 1)//even color buf
    {
        color_buf[0][1] = (UINT8*)_dwDspColorAddr + DRAMA_CACH_BASE_ADDRESS;
        color_buf[1][1] = color_buf[0][1] + _dwColorYSz;
        color_buf[2][1] = color_buf[1][1] + _dwColorCSz;

        color_buf[0][0] =  (UINT8*)_free_buf_addr + DRAMA_CACH_BASE_ADDRESS;
        color_buf[1][0] =  color_buf[0][0] + _dwColorYSz;
        color_buf[2][0] =  color_buf[1][0] + _dwColorCSz;

    }
    else//odd
    {
        color_buf[0][0] = (UINT8*)_dwDspColorAddr + DRAMA_CACH_BASE_ADDRESS;
        color_buf[1][0] =  color_buf[0][0] + _dwColorYSz;
        color_buf[2][0] = color_buf[1][0] + _dwColorCSz;

        color_buf[0][1] =   (UINT8*)_free_buf_addr + DRAMA_CACH_BASE_ADDRESS;
        color_buf[1][1] =  color_buf[0][1] + _dwColorYSz;
        color_buf[2][1] =  color_buf[1][1] + _dwColorCSz;
    }
}
#endif



BOOL fgIsDspJpg(void)
{
    return (1);//_eImageFormat == DSP_JPG);
}

void vJpg_DspCmd(UINT32 dwCmd)
{
    if (!fgIsDspJpg())
        return;

    //JPGLogS(3, "vJpg_DspCmd=%d\n", dwCmd);

    switch (dwCmd)
    {
        case DSP_STOP_DEC2:
            //vDspCmd(DSP_STOP_DEC2);
            AUD_DSPCmdStop(AUD_DEC_AUX);
            break;
        case DSP_FLUSH_DEC2:
            vDspCmd(DSP_FLUSH_DEC2);
            break;
        case DSP_PAUSE_DEC2:
            vDspCmd(DSP_PAUSE_DEC2);
            break;
        case DSP_RESUME_DEC2:
            vDspCmd(DSP_RESUME_DEC2);
            break;
        case DSP_PLAY_DEC2:
            //vDspHoldStartFlag();
            //vDspCmd(DSP_PLAY_DEC2);
            AUD_DSPCmdPlay(AUD_DEC_AUX);
            break;

        default:
            break;
    }
    return;


}

void vSetDspJpgState(DSP_JPG_STATE bState )
{
    _rDspJpgPlyr.eDspState = bState;
    //JPGLogS("eDspState=%d\n", _rDspJpgPlyr.eDspState);
}

BOOL bSetDspJpgHeaderInfo(UINT16 _image_width, UINT16 _image_height, BOOL fgProgressive, UINT16  uYformat, UINT16  uCformat )
{

    JPGLogS("DspJpgHeaderInfo\n");
    JPGLogD(uYformat);
    JPGLogD(uCformat);

    if (!uYformat  || !_image_width || !_image_height)
    {
        JPGLogS("!Illeagel HEADER INFO ERROR");
        vDspJpgErrStatus();
        return FALSE;
    }

    if (_image_width > dwCheckDspJpgresolution())
    {
        JPGLogS("!Illeagel Width ERROR, too small buffer");
        vDspJpgErrStatus();
        return FALSE;
    }


    image_width = _image_width;
    image_height = _image_height;
    _fgEnhance_PJPEG = has_multiple_scans = fgProgressive;

#ifdef Enhance_PJPEG
    _fgEnhance_FastPJPEG = FALSE;
#endif

    if (_fgEnhance_PJPEG)
        last_p_scan_decode = FALSE;
    else
        last_p_scan_decode = TRUE;

    comp_info[0].v_samp_factor = uYformat & 0xff;
    comp_info[0].h_samp_factor = (uYformat & 0xff00) >> 8;
    comp_info[2].v_samp_factor = comp_info[1].v_samp_factor = uCformat & 0xff;;
    comp_info[2].h_samp_factor = comp_info[1].h_samp_factor = (uCformat & 0xff00) >> 8;  ;

    return TRUE;

}

void vCopyDspJpgColorBuf(UINT32 *_color_buf)
{

#ifndef REDUCE_COLOR_BUF
    UINT32 uYSz = ((_color_buf[1] > _color_buf[0] ) ? (_color_buf[1] - _color_buf[0]) : 0 );
    UINT32 uCSz = (_color_buf[2] - _color_buf[1] );

    JPGLogS("vCopyDspJpgColorBuf");
    if (uYSz)
        vDrvOsdDramCopy( (UINT32)_color_buf[0], (UINT32)dwAbsDramANc(color_buf[0][use_second_color_buf]),  (uYSz));
    else
    {
        vDspJpgErrStatus();
    }

    if (uCSz)
    {
        vDrvOsdDramCopy((UINT32)_color_buf[1], (UINT32)dwAbsDramANc(color_buf[1][use_second_color_buf]),  (uCSz));
        vDrvOsdDramCopy((UINT32)_color_buf[2], (UINT32)dwAbsDramANc(color_buf[2][use_second_color_buf]),  (uCSz));
    }
    JPGLogS("vCopyColorBuf->Done");
#endif

}


void vDspJpgStart(void )
{
    //_rDspJpgPlyr.eDspState = DSP_JPG_INIT;
    //_rJpegPlyr.eDecMode = DSP_INT_JPG;
    //_fgJpgRotating = FALSE; //for MAL;
    input_scan_number = 0;  //reset counts
    fgFetchDspColorBuf = FALSE;
    //_eImageFormat = DSP_JPG;

    //_iOutputIMCURow = 0;
    max_h_samp_factor = 1;
    max_v_samp_factor = 1;
    num_components = 3;
    num_of_resampling = 0;
    downsized_idctsize = DCTSIZE;
    use_second_color_buf = 0;

    //parser read pointer is from adsp
    //vPrAutoVfifoWP(OFF);
    vWritePRS(RW_PRS_VRP_OFST, dReadPRS(RW_PRS_VRP_OFST)&(~PRS_VRP_SEL_HOST));
    vDspClrMCUDoneFlag();
}
void vDspJpgErrStatus(void)
{
    vPicPlayerSetErrorType(0xA1, JPG_FATAL_ERR);
    _rDspJpgPlyr.eDspState = DSP_JPG_EXIT;

    //vJpg_DspCmd(DSP_STOP_DEC2);

    return;
}
void vInitializeDspJpgItem(void)
{
    _rDspJpgPlyr.eDspState = DSP_JPG_INIT;
    scaling_input_mode = COLOR_BUFFER_FORMAT;    
    ex_vfifo_done = 0;

    vPREnableAll();
    vWritePRS(RW_PRS_VRP_OFST, dReadPRS(RW_PRS_VRP_OFST)|PRS_VRP_SEL_HOST);
    vWritePRS(RW_PRS_V_RP, dReadPRS(RW_PRS_VSTART)<<8);
}
BOOL fgDspJpgHeadDone(void)
{
    return (_rDspJpgPlyr.eDspState == DSP_HEADER_DONE || _rDspJpgPlyr.eDspState == WAIT_DSP_DECODING);
}

void  vSetFetchDspColorBufFlag(void)
{

    JPGLogS("-->After Parsing header CPY MCU Line\n");
    fgFetchDspColorBuf = TRUE;
}

#ifdef PROGRESSIVE_MULTI_COLLECT
void vDspReParsing(void)
{
    FS_FILE *prJpgFile;

    JPGLogS("&&& Reparsing Bitstream");

    prJpgFile = prJpeg_GetMdItem(_rJpegPlyr.wDecExeListIdx);
    fgJpeg_EnableDataTx(prJpgFile->rIns.dwLBA,
                        prMpGetDev()->prConv->dwByte2Blk(prJpgFile->rIns.dwLength),
                        TRUE);
    _rDspJpgPlyr.eDspState = DSP_WAIT_FIFO_READY;
}
BOOL bJPGExistInFifo(void)
{
    UINT8 *ptr =    (UINT8 *) ((dReadPRS(RW_PRS_ASTART) << 8));

    if (((*ptr) == 0xFF) && ((*(ptr + 1)) == 0xD8))
    {
        return TRUE;
    }
    return FALSE;
}

void vDspWaitReParsingRdy(void)
{
    if ( bJPGExistInFifo() || dwAFifoFullness() > 0x500)
    {
        JPGLogS("-->DspWaitReParsingRdy");
        _rDspJpgPlyr.eDspState = WAIT_DSP_DECODING;
        vDspReParsingDoneFlag();
    }

}
#endif

void vHaltDspJpg(void)
{
    if (!fgIsDspJpg())
        return;

    if (!fgDecoderStopped(SECOND_DECODER))
    {
        vJpg_DspCmd(DSP_STOP_DEC2);

        while (!fgDecoderStopped(SECOND_DECODER))
        {
            //vADSPTaskState();//dsp: to do
        }
    }
}


// *********************************************************************
// Function : DWRD dwFifoFullness(void)
// Description : Check The Fullness of Ring FIFO
// Parameter : dwFifoSz: Total Size of Ring FIFO
//             dwRPtr/dwWPtr: Read/Write Pointer of FIFO
// Return    : None
// *********************************************************************
UINT32 dwFifoFullness(UINT32 dwFifoSz, UINT32 dwRPtr, UINT32 dwWPtr)
{
    UINT32 dwFullness;

    if (dwWPtr >= dwRPtr)
    {
        dwFullness = dwWPtr - dwRPtr;
    }
    else
    {
        dwFullness = dwFifoSz - (dwRPtr - dwWPtr);
    }
    return(dwFullness);
}

#define DSP_JPG_EXTENDED_VFIFO_SIZE 0x13000
static void vDspExtendVfifo(JPEGHANDLE hFlow)
{

    UINT32 fileEOF, unused_2=0, unused_3=0;
    JDEC_Get_Param(hFlow, E_JDEC_PARAM_BS_EOF,(UINT32)( &fileEOF),  unused_2, unused_3);	

    //If dsp is playing and bistream is EOF and never extended, so we extend once.
    if (fgDecoderPlaySent(SECOND_DECODER)&&fileEOF && (ex_vfifo_done == 0))
    {
        UINT32 dwTmp, fullness;

        fullness = dwFifoFullness(VFIFO_SIZE, dReadPRS(RW_PRS_V_RP), dReadPRS(RW_PRS_V_WP));

        if ((dwTmp = VFIFO_SIZE - fullness) > DSP_JPG_EXTENDED_VFIFO_SIZE)
        {
            //JPGLogS(" ---> Extend VFIFO");

            dwTmp = dReadPRS(RW_PRS_V_WP);
            dwTmp = dwTmp + DSP_JPG_EXTENDED_VFIFO_SIZE;

            if (dwTmp >= (dReadPRS(RW_PRS_VEND) << 8))
                dwTmp -= VFIFO_SIZE;

            vSetJpgWp(dwTmp);
            vDspJpegFifoFull();
            ///RS232LogS(" ---> Extend VFIFO");
            ex_vfifo_done = 1;
        }
    }


}

void vDspSetComID(UINT8 Com1, UINT8 Com2, UINT8 Com3)
{
    component_info * compptr;

    compptr = comp_info;
    compptr->component_id = Com1;
    compptr->component_index = 0;
    //record the big component number and traslate to 0, 1, 2 later
    if (compptr->component_id != 1)
    {
        jpg_component_0 = compptr->component_id;
    }
    compptr++;

    compptr->component_id = Com2;
    compptr->component_index = 1;
    //record the big component number and traslate to 0, 1, 2 later
    if (compptr->component_id != 2)
    {
        jpg_component_1 = compptr->component_id;
    }
    compptr++;

    compptr->component_id = Com3;
    compptr->component_index = 2;
    //record the big component number and traslate to 0, 1, 2 later
    if (compptr->component_id != 3)
    {
        jpg_component_2 = compptr->component_id;
    }

}
void calculate_display_image_size(UINT16 *display_image_width,
                                  UINT16 *display_image_height,
                                  resample_mode_enum resample_mode,
                                  UINT32 *x_offset, UINT32 *y_offset,
                                  BOOL scale_idct_mode, BOOL fgAspectRatioAjusted)
{
    UINT32 ds_image_height, ds_image_width;
    UINT32 org_image_height, org_image_width;
    UINT32 dwHRatioUpscale, dwHRatioDownScale;
    UINT32 temp_image_height, temp_image_width;

    // - If source image's aspect ratio has been modified for TV distortion
    if ((image_height == _dwDisplayRegionHeight) && (image_width == _dwDisplayRegionWidth) )
    {
        resample_mode = ORIGINAL_SIZE;
    }

    if (fgAspectRatioAjusted)
    {
        dwHRatioUpscale = _dw_h_pixel_ratio_ajust_upscale * _dw_h_pixel_ratio_ajust_upscale;
        dwHRatioDownScale = _dw_h_pixel_ratio_ajust_downscale * _dw_h_pixel_ratio_ajust_downscale;
    }
    else
    {
        dwHRatioUpscale = _dw_h_pixel_ratio_ajust_upscale;
        dwHRatioDownScale = _dw_h_pixel_ratio_ajust_downscale;
    }

    ds_image_height = image_height;
    ds_image_width = image_width;
    idct_size = EIGHT_BY_EIGHT;


     /* display image in original size with source image size smaller than display buffer size */
    if ( (resample_mode == ORIGINAL_SIZE) && ((image_height <= _dwDisplayRegionHeight) && (image_width <= _dwDisplayRegionWidth))  )
    {
        h_pixel_ratio = (ds_image_height * UP_SCALE) / image_height;
        v_pixel_ratio = (ds_image_width * UP_SCALE) / image_width;
        org_image_width = *display_image_width;
        org_image_height = *display_image_height;
        *display_image_height = image_height;
        *display_image_width = image_width;

        if ((image_height == _dwDisplayRegionHeight) && (image_width == _dwDisplayRegionWidth) )
        {
        }
        else
        {
            *display_image_width = 2 * (*display_image_width / 2);
            *display_image_height = 2 * (*display_image_height / 2);

            *x_offset = *x_offset + (org_image_width - *display_image_width) / 2;
            *x_offset = (*x_offset / 2) * 2; /* ensure even number x_offset */
            *y_offset = *y_offset + (org_image_height - *display_image_height) / 2;
            *y_offset = 2 * (*y_offset / 2); /* ensure even number y_offset */
        }
    }
    else if ( (resample_mode == RESAMPLE_FIT) || (resample_mode == ORIGINAL_SIZE) ) /* Resample image to just fix the maximum display width or height in RESAMPLE_FIT mode or ORIGINAL_SIZE mode with large source image */
    {
        if (resample_mode == ORIGINAL_SIZE)
        {
            // _fgTooLargeForScreen = TRUE; /* for resample_mode == ORIGINAL_SIZE and enter rotate mode */
        }

        /* the height extension ratio is smaller than the width extension ratio, the image extension ratio is set to height extension ratio */
        //Mars Wen, [2002/1/17], add considering of TV distortion into maximum extension ratio checking
        if ( ((_dwDisplayRegionHeight * UP_SCALE * dwHRatioDownScale ) / ds_image_height)
                <= ((_dwDisplayRegionWidth * UP_SCALE * dwHRatioUpscale) / (ds_image_width)) )
        {
            /* In order to avoiding float time consuming operation, we change h_ and v_pixel_ratio to DRWD data type */
            v_pixel_ratio = h_pixel_ratio = ((ds_image_height  * UP_SCALE) / _dwDisplayRegionHeight);
            //v_pixel_ratio = ((ds_image_height  * UP_SCALE) / _dwDisplayRegionHeight);
            // ajust h_pixel_ratio to compensate TV distortion
            h_pixel_ratio = (h_pixel_ratio * dwHRatioUpscale) / dwHRatioDownScale;
            org_image_width = *display_image_width;
            org_image_height = *display_image_height;
            //carry unconditionally
            *display_image_height = ((UINT16) ((ds_image_height * UP_SCALE + v_pixel_ratio - 1) / (v_pixel_ratio)));
            *display_image_width = ((UINT16) ((ds_image_width * UP_SCALE + h_pixel_ratio - 1) / (h_pixel_ratio)));
            //peiyu, for sony  #106_2. The H/W maybe overstep the display region.
            temp_image_height = *display_image_height;
            temp_image_width = *display_image_width;
            if (temp_image_height > _dwDisplayRegionHeight )
            {
                *display_image_height  = _dwDisplayRegionHeight;
            }
            if (temp_image_width > _dwDisplayRegionWidth)
            {
                *display_image_width = _dwDisplayRegionWidth;
            }
            //*x_offset = *x_offset + (org_image_width - *display_image_width)/2;
            //to prevent underflow , the variable are all defined as unsigned

            if ((*x_offset + org_image_width / 2) > (UINT32)(*display_image_width) / 2)
                *x_offset = *x_offset + (org_image_width - ((UINT32)(*display_image_width))) / 2;
            else
                *x_offset = 0;
            *x_offset = (*x_offset / 2) * 2; /* ensure even number x_offset */


            if ((*y_offset + org_image_height / 2) > ((UINT32)(*display_image_height) / 2))
                *y_offset = *y_offset + (org_image_height - ((UINT32)(*display_image_height))) / 2;
            else
                *y_offset = 0;
            *y_offset = 2 * (*y_offset / 2); /* ensure even number y_offset */

        }
        else  /* the width extension ratio is smaller than the height extension ratio, the image extension ratio is set to width extension ratio */
        {
            /* In order to avoiding float time consuming operation, we change h_ and v_pixel_ratio to DRWD data type */
            v_pixel_ratio = h_pixel_ratio = ((ds_image_width  * UP_SCALE) / _dwDisplayRegionWidth);
            // v_pixel_ratio = ((ds_image_width  * UP_SCALE) / _dwDisplayRegionWidth);
            //Mars Wen,[2002/1/17], ajust v_pixel_ratio to compensate TV distortion
            v_pixel_ratio = (v_pixel_ratio * dwHRatioDownScale) / dwHRatioUpscale;
            org_image_width = *display_image_width;
            org_image_height = *display_image_height;
            //carry unconditionally
            *display_image_height = ((UINT16) ((ds_image_height * UP_SCALE + v_pixel_ratio - 1) / (v_pixel_ratio)));
            *display_image_width = ((UINT16) ((ds_image_width * UP_SCALE + h_pixel_ratio - 1) / (h_pixel_ratio)));
            //peiyu, for sony  #106_2. The H/W maybe overstep the display region.
            temp_image_height = *display_image_height;
            temp_image_width = *display_image_width;
            if (temp_image_height > _dwDisplayRegionHeight )
            {
                *display_image_height  = _dwDisplayRegionHeight;
            }
            if (temp_image_width > _dwDisplayRegionWidth)
            {
                *display_image_width = _dwDisplayRegionWidth;
            }

            if ((*x_offset + org_image_width / 2) > (UINT32)(*display_image_width) / 2)
                *x_offset = *x_offset + (org_image_width - ((UINT32)(*display_image_width))) / 2;
            else
                *x_offset = 0;
            *x_offset = (*x_offset / 2) * 2; /* ensure even number x_offset */

            if ((*y_offset + org_image_height / 2) > ((UINT32)(*display_image_height) / 2))
                *y_offset = *y_offset + (org_image_height - ((UINT32)(*display_image_height))) / 2;
            else
                *y_offset = 0;

            *y_offset = 2 * (*y_offset / 2); /* ensure even number y_offset */

            /*
            if (*display_image_height == 0)
            {
                *display_image_height = ((ds_image_height + 1) / 2) * 2;
                v_pixel_ratio = UP_SCALE;
                *y_offset = (((480 - *display_image_height) / 2) / 2) * 2;
            }
            */

        }
    }
    else if (resample_mode == FIXED_SIZE)  /* Resample image to fixed size image for digest view */
    {
         /* In order to avoiding float time consuming operation, we change h_ and v_pixel_ratio to DRWD data type */
        h_pixel_ratio = ((ds_image_width  * UP_SCALE) / *display_image_width);
        v_pixel_ratio = ((ds_image_height * UP_SCALE) / *display_image_height);
    }

}  /* calculate_display_image_size() function */

BOOL fgCheckPictureValidInGeneral(void)
{
    if ( image_width < 4 || image_height < 4 ) //Avoid that color displays lossess
    {
        vPicPlayerSetErrorType(0xE1, JPG_UNSUPPORTED_FILE);
        return FALSE;
    }

    //don't return error here, it will be handled by _IMG_DecPrepare()
    /*
    if ( image_width*40 < image_height ) //Avoid that rotation/mirror displays exception
    {
        vPicPlayerSetErrorType(0xE2, JPG_UNSUPPORTED_FILE);
        return FALSE;
    }
    */
    return TRUE;
}

UINT32  vRetDTVJpgUserBreak(JPEGHANDLE hFlow)    	
{
    UINT32 user_cancel_playback, unused_2=0, unused_3=0;
    JDEC_Get_Param(hFlow, E_JDEC_PARAM_CANCELED, (UINT32)&user_cancel_playback,  unused_2, unused_3);
    return user_cancel_playback;    
}


void fgJpeg_Play(void)
{
    vSetDspJpgDecodeType((UINT32)(_rImgDecDbg.rBufInfo.pu1WorkingBuf), (UINT32)(_rImgDecDbg.rBufInfo.pu1WorkingBuf +VFIFO_SIZE));
    vSetJPEGFileSize( _dwJPGFileSizeInByte);
    _rDspJpgPlyr.eState = JPG_START;
}

void vPrSetVFifo(UINT32 dSa, UINT32 dEa)
{
    vWritePRS(RW_PRS_VSTART, (UINT32)(dSa>>8));
    vWritePRS(RW_PRS_VEND, (UINT32)(dEa>>8));
}

void vGetDTVJpgInterface(JPEGHANDLE hFlow)    
{
        UINT32 filesize, unused_2=0, unused_3=0;
        
	JDEC_Get_Param(hFlow, E_JDEC_PARAM_FILESIZE,(UINT32)( &filesize),  unused_2, unused_3);	
	_dwJPGFileSizeInByte = filesize;
	
    	//ASSERT(_rImgDecDbg.rBufInfo.pu1WorkingBuf  != NULL);
	 vPrSetVFifo((UINT32)(_rImgDecDbg.rBufInfo.pu1WorkingBuf), (UINT32)(_rImgDecDbg.rBufInfo.pu1WorkingBuf +VFIFO_SIZE));
        //driver working buffer
	 comp_info = (component_info *) (_rImgDecDbg.rBufInfo.pu1WorkingBuf +VFIFO_SIZE+JPG_ROTATION_TEMP_BUFFER_SIZE);

}

void vGetDTVJpgInterface_2(IMG_LIB_INFO_T *prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree)    	
{
    IMG_LIB_INFO_T *pprImg=prImg;
        
    x_offset = _rImgDecDbg.u4StartX;
    y_offset = _rImgDecDbg.u4StartY;
    _dwDisplayBufferPitch = (_rImgDecDbg.u4Pitch)/4;    //OSD format transfers to pixel units

    
    if((eRotateDegree==CLOCKWISE_0)||(eRotateDegree==CLOCKWISE_0_WITH_FLIP))
    {            
        _dwDisplayRegionWidth = _rImgDecDbg.u4ScW;  //display window of width
        _dwDisplayRegionHeight = _rImgDecDbg.u4ScH;   //display window of height              
        _pbDecYaddr = (UINT8 *)_rImgDecDbg.u4ScImgAddr[0];
        _pbDecCbCraddr = (_pbDecYaddr + _dwDisplayRegionWidth*_dwDisplayRegionHeight);
    }
    else
    {
        //When ratational process by 90 or 270, MW will inverse the (W, H), but driver should get the original (W, H) for decoding.
        if((eRotateDegree==CLOCKWISE_180)||(eRotateDegree==CLOCKWISE_180_WITH_FLIP))
        {
            _dwDisplayRegionWidth = _rImgDecDbg.u4ScW;  //display window of width
            _dwDisplayRegionHeight = _rImgDecDbg.u4ScH;   //display window of height              
        }
        else
        {
            _dwDisplayRegionWidth = _rImgDecDbg.u4ScH;  //display window of width
            _dwDisplayRegionHeight = _rImgDecDbg.u4ScW;   //display window of height                
        }
        _pbDecYaddr = (UINT8 *)(pprImg->rMem.pau1ScDstAddr[0]);
        _pbDecCbCraddr = (UINT8 *)(pprImg->rMem.pau1ScDstAddr[0]);
    }
}

void vSetDTVJpgInterface(IMG_LIB_INFO_T *p)    
{
    	 //driver rotational working buffer
        p->rMem.pau1ScDstAddr[0] = (UINT8 *)(_rImgDecDbg.rBufInfo.pu1WorkingBuf +VFIFO_SIZE);
        p->rMem.pau1ScDstAddr[1] = (UINT8 *)(_rImgDecDbg.rBufInfo.pu1WorkingBuf +VFIFO_SIZE+(JPG_ROTATION_TEMP_BUFFER_SIZE>>1));        	
}

void vRetDTVJpgInterface(void)    	//todo
{

}

UINT32  vRetDTVJpgVfifo_wpointer(JPEGHANDLE hFlow)    	
{
    UINT32 wpointer, unused_2=0, unused_3=0;
    JDEC_Get_Param(hFlow, E_JDEC_PARAM_BS_WPTR, (UINT32)&wpointer,  unused_2, unused_3);
    return wpointer;
    
}



INT32 decode_dsp_jpg(UINT16 display_buf_height,
                     UINT16 *display_image_width, UINT16 *display_image_height,
                     resample_mode_enum resample_mode, UINT8 *display_Y_base_addr,
                     UINT8 *display_CbCr_base_addr, UINT32 *x_offset,
                     UINT32 *y_offset, IMG_LIB_INFO_T* prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree)
{
    UINT16    ci;
    //UINT8    *color_base;
    //UINT32   color_buf_size, i;
    component_info * compptr;
    IMG_LIB_INFO_T* pprImg=prImg;

    switch (_rDspJpgPlyr.eDspState)
    {

        case DSP_JPG_INIT:

            JPGLogS("DSP_JPG_INIT\n");
            #ifdef JPG_DECODING_ANALYSE
            reset_dsp_stc();
            #endif
            
            _iOutputIMCURow = 0;

            comp_info[0].v_samp_factor = 1;
            comp_info[0].h_samp_factor = 1;
            comp_info[1].v_samp_factor = 1;
            comp_info[1].h_samp_factor = 1;
            comp_info[2].v_samp_factor = 1;
            comp_info[2].h_samp_factor = 1;

            max_h_samp_factor = 1;
            max_v_samp_factor = 1;
            num_components = 3;
            num_of_resampling = 0;
            downsized_idctsize = DCTSIZE;
            use_second_color_buf = 0;

            _rDspJpgPlyr.eDspState = WAIT_PARSING_HEADER;
            
            if ( (!fgDecoderPlaySent(SECOND_DECODER)) && (_bJPGErrorType != 0xA1))
            {
                vSetJPEGFileSize(_dwJPGFileSizeInByte);
                vJpg_DspCmd(DSP_PLAY_DEC2);
                //avoid _Reset() in PSR_SoftInit, avoid reset parser then w_ptr will be set to 0x0
                vSetJpgWp(vRetDTVJpgVfifo_wpointer(pprImg->hCurDec));
            }


            break;
        case DSP_HEADER_DONE:
            if (fgFetchDspColorBuf)
            {

                JPGLogS("-->FetchDspColorBuf\n");
                vSetDspJpgState(DSP_JPG_RESIZING);
                vCopyDspJpgColorBuf(_color_buf);

                fgFetchDspColorBuf = FALSE;
            }
            IMG_LockWaitDspSema();
            break;
        case WAIT_DSP_DECODING:
            IMG_LockWaitDspSema();
            //x_thread_delay(10);
            vDspExtendVfifo(pprImg->hCurDec);
            break;
        case WAIT_PARSING_HEADER:
            IMG_LockWaitDspSema();
            //x_thread_delay(10);
            vDspExtendVfifo(pprImg->hCurDec);//DTV0075191
            break;

        case PARSING_EXIF_STATE:
            JPGLogS("PARSING_EXIF_STATE\n");

            vDspClrMCUDoneFlag();
            _rDspJpgPlyr.eDspState = WAIT_PARSING_HEADER;

            break;

        case DSP_PARSING_HEADER:
            JPGLogS("DSP_PARSING_HEADER\n");

            if ( !fgCheckPictureValidInGeneral() )
            {
                return FALSE;
            }

            vDspExtendVfifo(pprImg->hCurDec);

            /* Calculate the resample image height and width */
            calculate_display_image_size(display_image_width, display_image_height, resample_mode, x_offset, y_offset, TRUE, FALSE);

            idct_size = EIGHT_BY_EIGHT;

            for (ci = 0, compptr = comp_info; ci < num_components; ci++, compptr++)
            {
                max_h_samp_factor = MAX(max_h_samp_factor, compptr->h_samp_factor);
                max_v_samp_factor = MAX(max_v_samp_factor, compptr->v_samp_factor);
            }

            //JPGLogD(pic_width_in_buf);
            JPGLogD(image_width);
            JPGLogD(image_height);
            JPGLogB(max_h_samp_factor, max_v_samp_factor, 0, 0);


            _iTotalIMCURows = (UINT16) ((image_height + (max_v_samp_factor * DCTSIZE - 1)) / (max_v_samp_factor * DCTSIZE));

#ifndef REDUCE_COLOR_BUF
            color_buf_size = (pic_width_in_buf / 8) * max_h_samp_factor * max_v_samp_factor * DCTSIZE * DCTSIZE;  //one block has 64 byte
            //LogSD("color_buf_size", color_buf_size);
            color_base = (UINT8 *) _free_buf_addr + DRAMA_NONCACH_BASE_ADDRESS ;
            //JPGLogS("Color buf Addresx");
            for (ci = 0; ci < 3; ci++)
            {
                for (i = 0; i < 2; i++)
                {
                    color_buf[ci][i] = color_base + (ci * 2 + i) * color_buf_size;
                }
            }
            /* Process data */
#endif
            _rDspJpgPlyr.eDspState = DSP_HEADER_DONE;

            JPGLogS("DSP_HEADER_DONE\n");

            if (fgFetchDspColorBuf)
            {
                JPGLogS("--->Process  row\n");

                vSetDspJpgState(DSP_JPG_RESIZING);
                vCopyDspJpgColorBuf(_color_buf);

                fgFetchDspColorBuf = FALSE;
            }

#ifdef FAST_ENHANCE_PJPEG_FOR_DSP

            _bJpgWorkingBufferEnd =  (UINT8 *)((_free_buf_addr + DRAMA_NONCACH_BASE_ADDRESS) + dwGetPictureColorBufSize(ALL));
            //we only enable FAST ENHANCE MODE when 1. resolution is smaller than framebuffer size 2. fullscreen.
            //if (((_bJpgWorkingBufferEnd + dwGetPictureColorBufSize(ALL)*_iTotalIMCURows) < _pbVFifo) && (_fgSlideShow == TRUE) && (image_width*image_height < PANEL_WIDTH*PANEL_HEIGHT))
            if (((_bJpgWorkingBufferEnd + dwGetPictureColorBufSize(ALL)*_iTotalIMCURows) < _pbVFifo) && (_fgSlideShow == TRUE) && (image_width*image_height < (*display_image_width)*(*display_image_height)))
            {
                _fgEnhance_FastPJPEG = TRUE;
            }
#endif

            break;

        case DSP_JPG_RESIZING:

#ifdef JPG_DECODING_ANALYSE
            if (stc_dsp_decoding_0 != 0)
                stc_dsp_decoding_1 += ( dwReadStc() - stc_dsp_decoding_0);
#endif
            
            if(_iOutputIMCURow%8 == 0)
                JPGLogS("DSP_JPG_RESIZING\n");

            if ( _eJPGErrorLevel == JPG_FATAL_ERR )
            {
                _rDspJpgPlyr.eDspState = DSP_JPG_EXIT;
            }

#ifdef Enhance_PJPEG
            if (_fgEnhance_PJPEG)
            {
                if (_fgEnhance_FastPJPEG == FALSE)
                {
                    if (_iOutputIMCURow != 0)
                    {
                        bilinear_resampling_E( *display_image_width, *display_image_height,
                                               *x_offset, *y_offset, display_Y_base_addr,
                                               display_CbCr_base_addr, eRotateDegree, ALL);
                    }
                    else
                    {
                        if (use_second_color_buf == TRUE)
                            use_second_color_buf = FALSE;
                        else
                            use_second_color_buf = TRUE;
                    }
                }
            }
#endif
            else
            {
                if (_iOutputIMCURow != 0)
                {
                    bilinear_resampling( *display_image_width, *display_image_height,
                                         *x_offset, *y_offset, display_Y_base_addr,
                                         display_CbCr_base_addr, eRotateDegree, ALL);
                    
                    pprImg->rDispInfo.u4ScaleRowH = output_image_lines[0] - pre_output_image_lines[0];
                    
                    if((eRotateDegree != CLOCKWISE_0) && (pprImg->rDispInfo.u4ScaleRowH > 0))
                    {                        
                        _IMG_RotateJpg(eRotateDegree, &pprImg->rDispInfo, &pprImg->rMem);
                            
                        for(ci=0;ci<3;ci++)
                        {
                            pre_output_image_lines[ci] = output_image_lines[ci];
                        }
                    }
                }
                else
                {
                    if (use_second_color_buf == TRUE)
                        use_second_color_buf = FALSE;
                    else
                        use_second_color_buf = TRUE;
                }
            }

#ifdef REDUCE_COLOR_BUF
            vMoveColorBuf();
#endif

            _iOutputIMCURow++;

            if (_iOutputIMCURow >= _iTotalIMCURows)
            {
                /* When not using the simplest bilinear resampling algorithm,
                we have to perform additional resampling at last _iOutputIMCURow */
                //if ( (_iOutputIMCURow >= _iTotalIMCURows))// && (last_p_scan_decode == TRUE) )
                {
                    if (_fgEnhance_PJPEG)
                    {
#ifdef Enhance_PJPEG
                        if (_fgEnhance_FastPJPEG == FALSE)
                        {
                            /* last resampling process */
                            bilinear_resampling_E( *display_image_width, *display_image_height,
                                                   *x_offset, *y_offset, display_Y_base_addr,
                                                   display_CbCr_base_addr, eRotateDegree, ALL);
                        }
#endif
                    }
                    else
                    {
                        /* last resampling process */
                        bilinear_resampling(*display_image_width, *display_image_height,
                                            *x_offset, *y_offset, display_Y_base_addr,
                                            display_CbCr_base_addr, eRotateDegree, ALL);
                        
                        pprImg->rDispInfo.u4ScaleRowH = output_image_lines[0] - pre_output_image_lines[0];
                        
                        if((eRotateDegree != CLOCKWISE_0) && (pprImg->rDispInfo.u4ScaleRowH > 0))
                        {                        
                            _IMG_RotateJpg(eRotateDegree, &pprImg->rDispInfo, &pprImg->rMem);
                                
                            for(ci=0;ci<3;ci++)
                            {
                                pre_output_image_lines[ci] = output_image_lines[ci];
                            }
                        }                        
                    }
                }

                //set _iOutputIMCURow to 0, avoid (_iOutputIMCURow >= _iTotalIMCURows) then finish decode before calling vGetProgressiveSos()
                if (_fgEnhance_PJPEG)
                {
                    _iOutputIMCURow = 0;
                }

                if (_fgEnhance_PJPEG)
                {
                    _rDspJpgPlyr.eDspState = WAIT_DSP_DECODING;
                }
                else
                {
                    _rDspJpgPlyr.eDspState = DSP_JPG_EXIT;

                    //set stop to audio driver for baseline jpg.
                    vHaltDspJpg();
                    
                    #ifdef JPG_DECODING_ANALYSE
                    stc_jpg_main_decoding_1 = dwReadStc() -stc_jpg_main_decoding_0;
                    #endif
                }
            }
            else
            {
                _rDspJpgPlyr.eDspState = WAIT_DSP_DECODING;
            }

            //notify dsp, continue decoding
            vDspClrMCUDoneFlag();

#ifdef JPG_DECODING_ANALYSE
            stc_dsp_decoding_0 = dwReadStc();
#endif

            break;
#ifdef PROGRESSIVE_MULTI_COLLECT
        case DSP_REPARSING:
            vDspReParsing();
            break;
        case DSP_WAIT_FIFO_READY:
            vDspWaitReParsingRdy();
            break;
#endif
        case DSP_JPG_EXIT:
        default:
            JPGLogS("DSP_JPG_EXIT\n");
            //set _iOutputIMCURow to _iTotalIMCURows, then go to JPG_TRANSITION state;
            _iOutputIMCURow = _iTotalIMCURows;
            last_p_scan_decode = TRUE;

            if ( _eJPGErrorLevel == JPG_FATAL_ERR )
            {
                JPGLogS("--->Switch Jpg Redecoding\n");
            }
            else
            {

#ifdef CHECK_COLOR_BUF
                JPGLogS("Dsp_jpg CRC:\n");
                JPGLogD(DSP_crc_value);
#endif

#ifdef FAST_ENHANCE_PJPEG_FOR_DSP
                if (_fgEnhance_FastPJPEG == TRUE)
                {
                    _iOutputIMCURow = 0;
                    _rDspJpgPlyr.eState = JPG_PROG_RENDER;
                }
#endif
            }

            break;
    }


    return TRUE;
}


INT32 u4ImageDecode(void*  hCurImg, UINT32 u4ImgID, ENUM_IMG_ROTATE_PHASE eRotateDegree)
{
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)hCurImg;
    //JPEG_HEADER_INFO_T* prJpgHdrInfo;
    IMG_LIB_FILL_RING_BUF_T rStreamBufInfo;
    //INT32 i4Ret, i4Idx;
    UINT32 u4dsp_read_pointer;

    INT32 i;

    static    UINT16 display_buf_height;                           /* define display buffer height */
    static    resample_mode_enum resample_mode;                    /* define resampling mode */
    static    UINT8 *decode_Y_base_addr, *decode_CbCr_base_addr;   /* define output decode buffer memory location */
    //static    UINT8 *display_Y_base_addr, *display_CbCr_base_addr; /* define output display buffer memory location */

    switch (_rDspJpgPlyr.eState)
    {
        case JPG_START:


            #ifdef JPG_DECODING_ANALYSE
            reset_jpg_main_stc();
            stc_jpg_main_decoding_0 = dwReadStc();
            #endif
            
            vInitializeDspJpgItem();

            _iOutputIMCURow = 0;

#ifdef CHECK_COLOR_BUF
            DSP_crc_value = 0;
#endif

            input_scan_number = 0x0;

#ifdef Enhance_PJPEG
            _fgEnhance_PJPEG = FALSE; //@@cjyang, default FALSE
#endif

            //seems that display_buf_width&display_buf_height aren't needed.
            display_buf_height = _dwDisplayRegionHeight;

            
            decode_Y_base_addr = (UINT8 *) _pbDecYaddr;
            decode_CbCr_base_addr = (UINT8 *) _pbDecCbCraddr;

            display_image_width = _dwDisplayRegionWidth; 
            display_image_height = _dwDisplayRegionHeight; 

            //when rotation, w&h will be calculated by mw, so we don't calculated it again.
            if(eRotateDegree==CLOCKWISE_0)
                resample_mode = RESAMPLE_FIT;
            else
                resample_mode = FIXED_SIZE;

             jpg_SoftResizer_format = OSD_444_MODE;


            last_p_scan_decode = FALSE;
            has_multiple_scans = FALSE;
            progressive_mode = FALSE;

            _iTotalIMCURows = MAX_iMCU_ROW; /* a initial big value */

            // - Reset error parameters
            vPicPlayerSetErrorType(0x00, JPG_NO_ERR);

            num_of_resampling = 0;
            _dwJPGReadByteNum = 0;


            // - set horonzontal pixel ration ajust factor to correct TV distortion
            _dw_h_pixel_ratio_ajust_upscale = TV_RESO_Y_FACTOR * TV_SCREEN_WIDTH;
            _dw_h_pixel_ratio_ajust_downscale = TV_RESO_H_FACTOR * TV_SCREEN_HEIGHT;

            // - initialize global variables
            for (i = 0; i < MAX_COMPS_IN_SCAN; i++)
            {
                output_image_lines[i] = 0;
                pre_output_image_lines[i] = 0;
            }

            _rDspJpgPlyr.eState = JPG_BITSTREAM_FETCHING;

            break;

        case JPG_BITSTREAM_FETCHING:
            _rDspJpgPlyr.eState = JPG_DECODING;
            //vWritePRS(RW_PRS_V_WP, vRetDTVJpgVfifo_wpointer(prImg->hCurDec));
            //update system w_pointer to dsp
            if(ex_vfifo_done!=1)
            {
                vSetJpgWp(vRetDTVJpgVfifo_wpointer(prImg->hCurDec));
            }
            
            break;

        case JPG_DECODING:            

            decode_dsp_jpg(display_buf_height,
                           &display_image_width, &display_image_height,
                           resample_mode,
                           decode_Y_base_addr, decode_CbCr_base_addr,
                           &x_offset, &y_offset, prImg, eRotateDegree);

            if (_eJPGErrorLevel == JPG_FATAL_ERR)
            {
                _rDspJpgPlyr.eState = JPG_ERROR_HANDLING;
                return E_IMG_DEC_FAIL;
            }

            if (_eJPGErrorLevel == JPG_UNSUPPORTED_FILE)
            {
                _rDspJpgPlyr.eState = JPG_ERROR_HANDLING;
                return E_IMG_FMT_UNSUPPORT;
            }

            // - decoding correctly.
            if (_iOutputIMCURow >= _iTotalIMCURows)
            {
                if (last_p_scan_decode == TRUE)
                {
#ifdef FAST_ENHANCE_PJPEG
                    if (_fgEnhance_FastPJPEG == TRUE)
                    {
                        _iOutputIMCURow = 0;
                        _rDspJpgPlyr.eState = JPG_PROG_RENDER;

                    }
                    else
#endif
                        _rDspJpgPlyr.eState = JPG_END;

                    return E_IMG_DEC_FINISH;


                }
                else
                {
#if 0//fie reget..?
                    {
                        FS_FILE *prJpgFile;

                        prJpgFile = prJpeg_GetMdItem(_rJpegPlyr.wDecExeListIdx);
                        fgJpeg_EnableDataTx(prJpgFile->rIns.dwLBA,
                                            prMpGetDev()->prConv->dwByte2Blk(prJpgFile->rIns.dwLength),
                                            TRUE);
                    }
#endif
                    _iOutputIMCURow = 0;
                    //p_scan_decode_num++;

                    /* Reset error parameters */
                    vPicPlayerSetErrorType(0x00, JPG_NO_ERR);
                }


            }
            if ( _rDspJpgPlyr.eState == JPG_DECODING )
            {
                _rDspJpgPlyr.eState = JPG_BITSTREAM_FETCHING;
            }

            break;

        case JPG_PROG_RENDER:
#ifdef FAST_ENHANCE_PJPEG
            if ((_fgEnhance_FastPJPEG) && (_iOutputIMCURow <= _iTotalIMCURows))
            {

                //we don't refine scan in process_X_element0()
                has_multiple_scans = FALSE;

                //if ((_iOutputIMCURow != 0) || (_iTotalIMCURows == 1))
                if (_iOutputIMCURow == 0)
                {
#ifdef REDUCE_SOFTDECODER_COLOR_BUF
                    color_buf[0][0] = dwGetPictureWorkingBufEnd();
                    color_buf[1][0] =  color_buf[0][0] + dwGetPictureColorBufSize(Y);
                    color_buf[2][0] =  color_buf[1][0] + dwGetPictureColorBufSize(Cb);
#endif

                    if (use_second_color_buf == TRUE)
                        use_second_color_buf = FALSE;
                    else
                        use_second_color_buf = TRUE;

                }
                else
                {

                    color_buf[0][use_second_color_buf] =   dwGetPictureWorkingBufEnd() + dwGetPictureColorBufSize(ALL) * _iOutputIMCURow;
                    color_buf[1][use_second_color_buf] =  color_buf[0][use_second_color_buf]  + dwGetPictureColorBufSize(Y);
                    color_buf[2][use_second_color_buf] = color_buf[1][use_second_color_buf] + dwGetPictureColorBufSize(Cb);

                    bilinear_resampling(display_image_width, display_image_height,
                                        x_offset, y_offset, (UINT8 *)_pbDecYaddr,
                                        (UINT8 *)_pbDecCbCraddr, eRotateDegree, ALL);

                }

                _iOutputIMCURow++;


            }
            else
#endif
            {
                _rDspJpgPlyr.eState = JPG_END;
                //return E_IMG_DEC_FINISH;

            }

            break;
        case JPG_ERROR_HANDLING:
            //JPGLogS(3, "JPG_ERROR_HANDLING in vImageDecode\n");
            if (_eJPGErrorLevel == JPG_FATAL_ERR)
                _rDspJpgPlyr.eState = JPG_END;

            break;

        case JPG_END:

            if  (_rDspJpgPlyr.eDspState == DSP_JPG_STOP)
            {
                JPGLogS("DSP_JPG_STOP\n");

                if (!fgDecoderStopped(SECOND_DECODER))
                {
                    vHaltDspJpg();
                }

                _rDspJpgPlyr.eDspState = DSP_JPG_EXIT;

            }

            return E_IMG_DEC_FINISH;

            //break;


    }//switch

    if (prImg->bStopDec)
    {
        vJpg_DspCmd(DSP_STOP_DEC2);
        _rDspJpgPlyr.eState = JPG_END;        
        _rDspJpgPlyr.eDspState = DSP_JPG_EXIT;    
        return (INT32)E_IMG_CANCEL;
    }

    //update dsp r_pointer to system
    u4dsp_read_pointer = vReadJpgRp();
    Jpg_UpdateReadPtr(prImg->hCurDec, u4dsp_read_pointer);
    if ((prImg->bIsEOF == FALSE) && (_IMG_GetBSBufInfo(hCurImg, &rStreamBufInfo) > (VFIFO_SIZE - BS_PKT_SIZE)))
    {
        INT32 i4Ret;
		i4Ret = _IMG_BsBufManager(hCurImg, u4ImgID);	//MW callback function
		if(i4Ret == (INT32)E_IMG_OK)
		{
			return (INT32)E_IMG_BSBUF_UNDERFLOW;
		}
		else if(i4Ret != (INT32)E_IMG_CANCEL)
		{
			return i4Ret;			
		}
    }
    return E_IMG_OK;
}
