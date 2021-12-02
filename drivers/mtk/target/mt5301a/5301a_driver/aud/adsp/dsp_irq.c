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
/***************    MTK CONFIDENTIAL & COPYRIGHTED     ****************/
/***************                                       ****************/
/***************  $Modtime:: 04/12/06 3:31p    $       ****************/
/***************  $Revision: #1 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Jack Hsu              ****************/
/***************                 Peichen Chang         ****************/
/**********************************************************************/
#define _DSP_IRQ_C

#include "dsp_general.h"
#include "dsp_reg.h"
#include "dsp_func.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "d2rc_shm.h"
#include "drv_adsp.h"
#include "dsp_rg_ctl.h"
#include "aud_drv.h"
#include "dsp_intf.h"

#include "aud_debug.h"

#include "x_hal_926.h"
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
void vDspIntSvc (void);
UINT32 u4IntFlowControl(UINT32 u4DspRIntData);

#ifdef ADSP_JPEG_DEC2
#include "../../jpg_dsp/dsp_jpg.h"
#include "../../jpg_dsp/img_lib_if.h"
#include "../audio/aud_hw.h"

UINT32 _color_buf[3];
UINT32 _free_buf_addr;
UINT32 _num_components ;
UINT16 _image_width, _image_height ;
UINT16 uYformat, uCbformat, uCrformat;
UINT8  _u1Ns,_u1Cmid,_u1Ss,_u1Se,_u1Ah,_u1Al;
UINT32 _u4Temp;
    
BOOL fgProgressive = 0;
extern  UINT16 pic_width_in_buf;

#ifdef JPEG_DBG
UINT16 uJPEG_Tmp, uJPEG_Line;
#endif

#define JPEG_4_4_4     0x0101
#define JPEG_4_2_0     0x0202
#define JPEG_4_2_2     0x0201
#define JPEG_4_2_2V    0x0102

extern  UINT16 pic_width_in_buf;
extern BOOL fgIsDspJpg(void);
extern BOOL bSetDspJpgHeaderInfo(UINT16 _image_width, UINT16 _image_height, BOOL fgProgressive, UINT16  uYformat, UINT16  uCformat );
extern void vDspJpgStart(void );
extern BOOL fgDspJpgHeadDone(void);
extern void vCopyDspJpgColorBuf(UINT32 *_color_buf);
extern void vSetFetchDspColorBufFlag(void);
extern void vDspJpgErrStatus(void);
extern void vGetProgressiveSos(UINT8 n, UINT8 cc);
extern void vDspSetComID(UINT8 Com1, UINT8 Com2, UINT8 Com3);
extern void _AUD_BitStrPtrSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom);
extern BOOL _AUD_DMX_SetAudFifo(UINT8 u1DecId, UINT32 u4FifoStr, UINT32 u4FifoEnd);
extern void vSetColorSz(UINT32 *_color_buf);
extern void vSetDspJpgState(UINT8 bState );
extern UINT32 dwGetDJpgAreaSa(void);


void vSetJpgWp(UINT32 u4JpgWp)
{
    PARSER_WRITE32(REG_VWP, u4JpgWp & 0xFFFFFFF);
}

UINT32 vReadJpgRp(void)
{
    return(PARSER_READ32(REG_VRP) & 0xFFFFFFF);
}

void vSetDspJpgDecodeType(UINT32 u4StartAdr, UINT32 u4EndAdr)
{
    UINT32 u4Temp;

    u4Temp = dwGetDJpgAreaSa();
    if (_u4DspDramBuf[TYPE_DEC2_DRAM_IDX] != u4Temp)
    {
        _u4DspDramBuf[TYPE_DEC2_DRAM_IDX] = u4Temp;
        _u4DspCmptBuf[TYPE_DEC2_DRAM_IDX] = u4Temp;

        u4Temp = (_u4DspDramBuf[0] >> 8) & 0xFFFF;
        u4Temp = u4Temp | ((_u4DspDramBuf[1] << 8) & 0xFFFF0000);
        WriteREG (REG_DSP_DRAM_SBLK01, u4Temp);

        u4Temp = (_u4DspCmptBuf[0] >> 8) & 0xFFFF;
        u4Temp = u4Temp | ((_u4DspCmptBuf[1] << 8) & 0xFFFF0000);
        WriteREG (REG_DSP_CMPT_DRAM_SBLK01, u4Temp);  // Temp hack for cmpt mode
    }

    _u4AFIFODec2[0] = u4StartAdr;
    _u4AFIFODec2[1] = u4EndAdr;
    AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_MULTI_MEDIA, AUD_FMT_JPEG);
    _AUD_BitStrPtrSel(AUD_DEC_AUX, AUD_STREAM_FROM_MULTI_MEDIA);
    ///Select PTR_SEL_BANK1_PSR and need set REG_A0STA, REG_A0END
    _AUD_DMX_SetAudFifo(AUD_DEC_AUX, u4StartAdr & 0x1FFFFF00, u4EndAdr & 0x1FFFFF00);   
}

void vSetDspThumbnailmode (BOOL flag)
{
    if (flag)
    {
        vWriteDspShmWORD(W_JPG_CMD_FLAG, (u2ReadDspShmWORD(W_JPG_CMD_FLAG) |0x0080));
    }
    else
    {
        vWriteDspShmWORD(W_JPG_CMD_FLAG, (u2ReadDspShmWORD(W_JPG_CMD_FLAG) & ~0x0080));
    }
}
BOOL bGetDspThumbnailmode(void)
{
    if ((u2ReadDspShmWORD(W_JPG_CMD_FLAG) & 0x0080) == 0x0080)
    {
        return TRUE;
    }
    return FALSE;
}

void vDspHoldStartFlag (void)
{
    WriteDspCommDram(ADDR_D2RC_JPEG_D2RC_MCULINE_DONE,1);
}


/***************************************************************************
     Function : vDspClrMCUDoneFlag
  Description : DSP Dsp copy color buffer done
    Parameter : None
    Return    : None
***************************************************************************/
void vDspClrMCUDoneFlag (void)
{
    WriteDspCommDram(ADDR_D2RC_JPEG_D2RC_MCULINE_DONE, 0);
}
/***************************************************************************
     Function : vDspClrMCUDoneFlag
  Description : DSP Dsp copy color buffer done
    Parameter : None
    Return    : None
***************************************************************************/

void vDspReParsingDoneFlag (void)
{
    WriteDspCommDram(ADDR_D2RC_JPEG_D2RC_REPARSE_REQ, 0);
}

/***************************************************************************
     Function : vDspJpegFifoFull
  Description : Fifo was full.
    Parameter : None
    Return    : None
***************************************************************************/
///[20100201]
void vDspJpegFifoFull(void)
{
    vWriteDspWORD (ADDR_RC2D_JPG_FIFO_FULL,              0x01);
}

#endif

//static TV_AUD_SYS_T _eTvSys = SV_NONE_DETECTED;
UINT32 u4DspIRQSvc (void)
{
    UINT32 u4DspRIntData;
    UINT8 bDspRIntAddr;
    UINT32 u4Msg = ADSPTASK_MSG_INTERRUPT;

    //u4DspRIntData = ReadREG(RO_D2R_SDA);
    u4DspRIntData = u4DspRegR_D2rSda();
    bDspRIntAddr = (u4DspRIntData & 0xFF);
    u4DspRIntData = (u4DspRIntData >> 8);

    switch (bDspRIntAddr)
    {
    case INT_D2RC_WAKEUP_OK:
        _fgDspWakeupOK = TRUE;
        u4Msg = ADSPTASK_MSG_NO_COMMAND; // Refine ADSP control
	  break;     
    case INT_D2RC_FLOW_CONTROL:  /* Just for LOG */  
        u4DspRIntData = (u4DspRIntData >> 8);
        HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE) 
        {
            if (_bDspStrTyp == AC3_STREAM)
            {
                LOG(7, "Receive AC3 ACMOD change Int, ACMOD = %x\n",dReadDspCommDram(ADDR_D2RC_AC3_ACMOD)>>8);
            }
            else if ((_bDspStrTyp == MPEG12_STREAM) || (_bDspStrTyp == MPEG3_STREAM))
            {
                LOG(7, "Receive MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MPEG_MODE)>>8);
            }
        }
        else if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2) 
        {
            if (_bDspStrTyp == AC3_STREAM)
            {
                LOG(7, "Receive DEC2 AC3 ACMOD change Int, ACMOD = %x\n",dReadDspCommDram(ADDR_D2RC_AC3_ACMOD_DEC2)>>8);
            }
            else if ((_bDspStrTyp == MPEG12_STREAM) || (_bDspStrTyp == MPEG3_STREAM))
            {
                LOG(7, "Receive DEC2 MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MPEG_MODE_DEC2)>>8);
            }
        }
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
        else if (u4DspRIntData==D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3) 
        {
            if ((_bDspStrTyp == MPEG12_STREAM) || (_bDspStrTyp == MPEG3_STREAM))
            {
                LOG(7, "Receive DEC3 MPEG mode change Int, Mode = %x\n",dReadDspCommDram(ADDR_D2RC_MPEG_MODE_DEC3)>>8);
            }
        }
#endif
        vLogFlowControl(u4DspRIntData);
        u4Msg = u4IntFlowControl(u4DspRIntData); // refine ADSP control       
        break;
#ifdef DSP_SUPPORT_NPTV
    // refine ADSP control
    case INT_DSP_MTS_DETECTED_MODE:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_MTS_MODE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    case INT_DSP_PAL_DETECTED_MODE:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_PAL_MODE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);        
        break;
    case INT_DSP_JPN_DETECTED_MODE:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_JPN_MODE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);        
        break;
    case INT_DSP_MINER_NOTIFY:
        // we put (u4DspRIntData>>8) in high word of message
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_MINER_NOTIFY)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    case INT_DSP_DETECTOR_NOTIFY:
        _fgDspDetected = TRUE;		
        // Notify middleware
        //_eTvSys = (TV_AUD_SYS_T)wReadDspWORD(ADDR_DETECTED_TYPE);
        // we put wReadDspWORD(ADDR_DETECTED_TYPE) in high word of message
        u4Msg = wReadDspWORD(ADDR_DETECTED_TYPE);
        u4Msg = u4Msg<<16;
        u4Msg = u4Msg|ADSPTASK_MSG_INT_DETECTOR_NOTIFY;
	  break;        
    case INT_DSP_CHANGE_ATV_STANDARD:
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_ATV_CHANGE)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        LOG(7, "LOG FOR DECODER DETECTS STANDARD CHANGE\n");
	  break;  
    case INT_DSP_HDEV_AUTO_SWITCH:
        LOG(7, "High deviation mode auto switch...\n");
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_ATV_HDEV_SWITCH)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    case INT_D2RC_FM_RDO_DET_FINISH:
        LOG(7, " FM Radio channel detection finished..\n");
        u4Msg = (UINT32)(ADSPTASK_MSG_INT_FM_RADIO_DET)|(u4DspRegR_D2rSda() & 0xFFFF0000L);
        break;
    #if 0 // refine ADSP control
    case INT_DSP_MTS_DETECTED_MODE:  //MTS mode changed INT
        /*
        MTS_MONO      0x01
        MTS_STEREO    0x02
        MTS_SAP       0x04
        */
        switch ((u4DspRIntData >> 8))
        {
        case MTS_DETECTED_STEREO_SAP:
            //Notify UI the detected MTS modes.
            //((MTS_STEREO | MTS_SAP | MTS_MONO));
            break;
        case MTS_DETECTED_STEREO:
            //((MTS_STEREO | MTS_MONO));
            break;
        case MTS_DETECTED_SAP:
            //((MTS_SAP | MTS_MONO));
            break;
        case MTS_DETECTED_MONO:
            //(MTS_MONO);
            break;
        default: //exception
            break;
        }
        break;
#endif
#endif
    case INT_D2RC_PTS_1ST_REFRESH:
        _fgDspPtsSet = TRUE;
    	LOG(7, "DSP interrupt RISC - PTS 1st refresh\n"); 
        u4Msg = ADSPTASK_MSG_NO_COMMAND; //Refine ADSP control 
        break;
    case INT_D2RC_MP3_TYPE:
        switch ((u4DspRIntData >> 8))
        {
        case TYPE_INVALID:
        case TYPE_MP2:
             LOG(5, "Can not play MP2 bitstream\n");
            //_fgDspMpgTypGot = TRUE;
            //_fgDspGetMpgTyp = FALSE;
            //AUD_DSPCmdStop(AUD_DEC_MAIN);
            // notify audio driver about format conflict
            AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN, TRUE);
            break;
        case TYPE_MP3:
            LOG(5, "Can not play MP3 bitstream\n");            
            // Stop at aout reinit
            // notify audio driver about format conflict
            AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN, TRUE);
            break;
        default:
            break;
        }
        _dwDspMpgTyp = (u4DspRIntData >> 8);
        u4Msg = ADSPTASK_MSG_NO_COMMAND;
        break;        
    case INT_D2RC_PRINT_DATA:
            LOG(7, "%.6x\n",u4DspRIntData);
        u4Msg = ADSPTASK_MSG_NO_COMMAND; // Refine ADSP control
    	break;
#ifdef ADSP_JPEG_DEC2
        case INT_D2RC_JPEG_NOTIFY:
        {
            u4Msg = ADSPTASK_MSG_NO_COMMAND;

            if (!fgIsDspJpg())
                return (u4Msg);
            switch ((u4DspRIntData >> 8))
            {
                case JPEG_D2RC_NOTIFY_PLAY:
                #ifdef JPEG_DBG
                    LOG(7, "***Start JPG decoder\n");
                    uJPEG_Line = 0;
                #endif
                    vDspJpgStart();
                    break;
                case JPEG_D2RC_NOTIFY_JPEG_INFO:
                #ifdef JPEG_DBG
                    LOG(7, "***Get JPEG INFO from JFIF\n");
                #endif
                    _image_width = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_IMAGE_WIDTH) >> 8;
                    _image_height = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_IMAGE_HEIGHT) >> 8;
                    fgProgressive = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_MODE) >> 8;
                    _num_components = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_COMPONENT) >> 8;

                    uYformat = (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_C1_HV) & 0xff0000)>>16;
                    uCbformat = (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_C2_HV) & 0xff0000)>>16;
                    uCrformat = (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_C3_HV) & 0xff0000)>>16;
                    
                    vDspSetComID(uYformat, uCbformat, uCrformat);
                    
                    uYformat = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_C1_HV) & 0xffff;
                    uCbformat = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_C2_HV) & 0xffff;
                    uCrformat = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_C3_HV) & 0xffff;
                    _color_buf[0] =  ((ReadREG(REG_DSP_CMPT_DRAM_SBLK01) & 0xFFFF0000) >> 8) + (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_Y_COLOR_BUF_SADR) << 2);
                    _color_buf[1] =  ((ReadREG(REG_DSP_CMPT_DRAM_SBLK01) & 0xFFFF0000) >> 8) + (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_Cb_COLOR_BUF_SADR) << 2);
                    _color_buf[2] =  ((ReadREG(REG_DSP_CMPT_DRAM_SBLK01) & 0xFFFF0000) >> 8) + (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_Cr_COLOR_BUF_SADR) << 2);

                    _free_buf_addr = ((ReadREG(REG_DSP_CMPT_DRAM_SBLK01) & 0xFFFF0000) >> 8) + (dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_FREE_ADR) << 2);

                    #ifdef JPEG_DBG
                    if (fgProgressive)
                        LOG(7, "********* PROGRESSIVE!!! *********\n");
                    else
                        LOG(7, "********* BASE-LINE!!! *********\n");
                    LOG(7, "%x\n", _color_buf[0]);    /// Physical DRAM address of Y color buffer.
                    LOG(7, "%x\n", _color_buf[1]);
                    LOG(7, "%x\n", _color_buf[2]);
                    LOG(7, "%x\n", _free_buf_addr);

                    _image_width = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_IMAGE_WIDTH) >> 8;
                    _image_height = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_IMAGE_HEIGHT) >> 8;
                    LOG(7, "%x\n", _image_width);
                    LOG(7, "%x\n", _image_height);
                    #endif

                    if ((uYformat == JPEG_4_2_0) || (uYformat == JPEG_4_2_2))
                    {
                        pic_width_in_buf = ((_image_width + 15) >> 4) << 4;    /// 16 pixel align
                    }
                    else    /// JPEG_4_4_4 and JPEG_4_2_2V:
                    {
                        pic_width_in_buf = ((_image_width + 7) >> 3) << 3;     /// 8 pixel align
                    }

                    if (bSetDspJpgHeaderInfo(_image_width, _image_height, fgProgressive, uYformat, uCbformat ))
                    {
                        vSetColorSz(_color_buf);
                        vSetDspJpgState(DSP_PARSING_HEADER);
                    }

                    break;
                case JPEG_D2RC_NOTIFY_ONE_MCU_DONE:
                #ifdef JPEG_DBG
                    if((uJPEG_Tmp%8 == 0) || fgProgressive)
                        LOG(7, "***One MCU decode done\n");
                    uJPEG_Tmp++;
                #endif
                    if (fgDspJpgHeadDone())
                    {
                        vCopyDspJpgColorBuf(_color_buf);
                        vSetDspJpgState(DSP_JPG_RESIZING);
                    }
                    else
                    {
                        vSetFetchDspColorBufFlag();
                    }
                    break;

                case JPEG_D2RC_NOTIFY_DONE:
                    _image_width = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_IMAGE_WIDTH) >> 8;
                    _image_height = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_IMAGE_HEIGHT) >> 8;
                #ifdef JPEG_DBG
                    LOG(7, "***JPEG DEC FINISH\n");
                    LOG(7, "%x\n", _image_width);
                    LOG(7, "%x\n", _image_height);
                #endif  				
                    vSetDspJpgState(DSP_JPG_EXIT);
             	      vDspCmd(DSP_STOP_DEC2);
                    break;

                case JPEG_D2RC_NOTIFY_ERROR:
                #ifdef JPEG_DBG
                    uCrformat = (UINT16)dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_DEC_ERROR);
                    LOG(7, "*** JPEG DEC ERROR=%x\n", uCrformat);
                #endif
                    vDspJpgErrStatus();
                    break;

                    /// [Joel] For progressive mode big picture use.
                case JPEG_D2RC_NOTIFY_ADR_REPARSE:
                #ifdef JPEG_DBG
                    uJPEG_Tmp = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_REPARSE_REQ) & 0xffff;
                    LOG(7, "*** JPEG REPARSE=%x\n", uJPEG_Tmp);
                #endif
                    /// 加入DSP發interrupt後risc這邊需要設定的flag
                    /// 在DSP state machine中，完成reparse後將下面的Share DRAM清為0
                    //WriteDspCommDram(ADDR_D2RC_JPEG_D2RC_REPARSE_REQ,0);
                    vSetDspJpgState(DSP_REPARSING);
                    break;

                    /// [Joel] Decoder STOP already
                case JPEG_D2RC_NOTIFY_DEC_STOP:
                #ifdef JPEG_DBG
                    LOG(7, "****** STOP STOP STOP ******\n");
                #endif
                    vSetDspJpgState(DSP_JPG_STOP);
                    //vDspCmd(DSP_STOP_DEC2);
                    break;

                case JPEG_D2RC_NON_SUPPORT_WIDTH:
                #ifdef JPEG_DBG
                    LOG(7, "*** Width too large ***\n");
                #endif
                    vDspJpgErrStatus();
                    break;

                case JPEG_D2RC_NOTIFY_PROG_SOS:
                    _u4Temp = dReadDspCommDram(ADDR_D2RC_JPEG_PROG_SOS_NS_CMID);
                    _u1Ns = (_u4Temp & 0xFF00)>>8;
                    _u1Cmid = (_u4Temp & 0xFF);

                    _u4Temp = dReadDspCommDram(ADDR_D2RC_JPEG_PROG_SS_SE_ALAH);
                    _u1Ss = (_u4Temp & 0xFF0000)>>16;
                    _u1Se = (_u4Temp & 0xFF00)>>8;
                    _u1Ah = (_u4Temp & 0xF0)>>4;
                    _u1Al = (_u4Temp & 0xF);
                #ifdef JPEG_DBG
                    LOG(7, "*** Prog SOS ***\n");
                    LOG(7, "%x %x 0x00 0x00\n", _u1Ns, _u1Cmid);
                    LOG(7, "%x %x %x %x\n", _u1Ss, _u1Se, _u1Ah, _u1Al);
                #endif
                    vGetProgressiveSos(_u1Ns,_u1Cmid);    				
                    break;

                case JPEG_D2RC_LOG:
                #ifdef JPEG_DBG
                    LOG(7, "*** JPEG Log ***\n");
                    _u4Temp = dReadDspCommDram(ADDR_D2RC_JPEG_D2RC_LOG_INFO);
                    LOG(7, "D2RC_LOG=%x\n", _u4Temp);
                #endif
                    break;
                default:
                    break;
            }

            IMG_UnLockWaitDspSema();
        }
#endif      ///ADSP_JPEG_DEC2  
    default:
        break;
    }
    
    if(u4Msg !=ADSPTASK_MSG_INTERRUPT) // Refine ADSP control
    {
        DSP_ClearInt();
    }
    return u4Msg;
}

/***************************************************************************
     Function : vDSPIntSvc
  Description : DSP Interrupt Service
    Parameter : None
    Return    : None
***************************************************************************/
void vDspIntSvc (void)
{
    UINT8 bDspRIntAddr;
    UINT32 u4DspRIntData;
    UINT32 u4DspRIntData2;
    UINT32 u4DspRIntType;
    INT32 i4DspData;

#if 0 // refine ADSP control
    UINT32 dwDspReqData;
#endif
#ifdef VORBIS_SUPPORT
    UINT32 dwOfst, dwSz, dwFlashOfst;
    UINT8 *pbDspBuf;
#endif

//Read Int Addr. and Data
    //u4DspRIntData = (UINT32) ReadREG(RO_D2R_SDA);
    u4DspRIntData = u4DspRegR_D2rSda();
    bDspRIntAddr = (u4DspRIntData & 0xFF);
    u4DspRIntData = (u4DspRIntData >> 8);
    i4DspData = 0;
    
//Clear DSP Interrupt for MP Version
    //WriteREG(RO_D2R_SDA,0x0);
    DSP_ClearInt();

//Interrupt Service
    switch (bDspRIntAddr)
    {
#if 0 // refine ADSP control
    case INT_DSP_MTS_DETECTED_MODE:
    case INT_DSP_PAL_DETECTED_MODE:
    case INT_DSP_JPN_DETECTED_MODE:    
    case INT_DSP_MINER_NOTIFY:
        vDspDemodDetModeNotify(bDspRIntAddr, (u4DspRIntData >> 8));
        break;
    case INT_DSP_DETECTOR_NOTIFY:	
        // Notify middleware
        vDspTvSysDetectedNotify(_eTvSys); 
        break;
#endif
    #if 0 // refine ADSP control
    case INT_D2RC_MP3_TYPE:
        switch ((u4DspRIntData >> 8))
        {
        case TYPE_INVALID:
        case TYPE_MP2:
            LOG(5, "Can not play MP2 bitstream\n");
            //_fgDspMpgTypGot = TRUE;
            //_fgDspGetMpgTyp = FALSE;
            //AUD_DSPCmdStop(AUD_DEC_MAIN);
            break;
        case TYPE_MP3:
            // Stop at aout reinit
            break;
        default:
            break;
        }
        _dwDspMpgTyp = (u4DspRIntData >> 8);
        break;
    case INT_D2RC_DEEMPHASIS_NOTIFY:
        switch ((u4DspRIntData >> 8))
        {
        case DEEMPH_ENABLE:
            vSetAdac (ADAC_CMD_EMPH, TRUE, 0);
            break;
        case DEEMPH_DISABLE:
            vSetAdac (ADAC_CMD_EMPH, FALSE, 0);
            break;
        default:
            break;
        }
        break;
    #endif    
    case INT_D2RC_FLOW_CONTROL:
        //Extract Mode
        u4DspRIntData = (u4DspRIntData >> 8);

        UNUSED((fgIntFlowControl (u4DspRIntData)));
        break;
    #if 0 // refine ADSP control       
    case INT_D2RC_PTS_1ST_REFRESH:
        _fgDspPtsSet = TRUE;
    	LOG(9, "DSP interrupt RISC - PTS 1st refresh\n"); 
        break;
        
    case INT_D2RC_WAKEUP_OK:
        //To be added
        //DSP wake up OK
        // ...
        //Set Bonding Options
        _fgDspSInt = TRUE;
        _uDspSIntAddr = INT_RC2D_BONDING_OPTIONS;
/////////////////////////////////////////////////////////////////////////////////////
// Bonding Options
//
//    bit 0: Channel Setting 5.1ch(0)/2ch(1)
//    bit 1: Channel 6 (Center Back) Enable(0)/Disable(1)
//    bit 2: Channel 7 Enable(0)/Disable(1)
//    bit 3: (Reversed)
//    bit 4: DTS Decoding Enable(0)/Disable(1)
//    bit 5: WMA Decoding Enable(0)/Disable(1)
//    bit 6: HDCD Decoding Enable(0)/Disable(1)
//    bit 7: MLP Decoding Enable(0)/Disable(1)
//    bit 8: SACD Decodinng Enable(0)/Disable(1)
//    bit 9~15: (Reserved)
/////////////////////////////////////////////////////////////////////////////////////
        _u4DspSIntSD = ((_u4Bond & 0xFFFF) << 8);
	 LOG(9,"DSP interrupt RISC - wake up ok\n"); // Light added for interrupt log
        break;
    #endif        
#if 0 // refine ADSP control
    case INT_D2RC_READ_REG:
        // Warning !! u4DspRIntData only 3 bytes
        //dwDspReqData = ReadREG (u4DspRIntData);
        dwDspReqData = u4DspRegR_ByArguement(u4DspRIntData);
        vSendDspLongInt (INT_RC2D_RISC_ACK, 0x100, dwDspReqData);
        break;

    case INT_D2RC_WRITE_REG_ADR:
        // Warning !! u4DspRIntData only 3 bytes
        //WriteREG (u4DspRIntData, _u4DspWRiscData);
        vDspRegW_ByArguement(u4DspRIntData, _u4DspWRiscData);
        break;

    case INT_D2RC_WRITE_REG_DATA:
        _u4DspWRiscData = dwReadDspLongData () & 0xFF;
        _u4DspWRiscData = _u4DspWRiscData | (u4DspRIntData << 8);
        break;
    case INT_D2RC_PRINT_DRAM_LEN:
        _u4DspDumpDramLen = u4DspRIntData;
        break;
    case INT_D2RC_PRINT_DRAM_ADR:
        DumpDram (_u4DspDumpDramLen, u4DspRIntData);
        //SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
        {
            UINT32 dwCnt = 0;

            while (1)
            {
                SLEEP (0);
                if (!fgDspBusy ())
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    break;
                }
                dwCnt++;
                if (dwCnt > 0x500)
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    _dwDspSIntFail++;
                    break;
                }
            }
        }
        break;
    case INT_D2RC_COMPARE_RESULT:
        if (fgCheckCompareResult ())
        {
            // Result is correct, continue going
            UINT32 dwCnt = 0;

            while (1)
            {
                SLEEP (0);
                if (!fgDspBusy ())
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    break;
                }
                dwCnt++;
                if (dwCnt > 0x500)
                {
                    SendDSPInt (INT_RC2D_RISC_ACK, 0x100);
                    _dwDspSIntFail++;
                    break;
                }
            }
        }
        break;
#endif // EMULATION_FPGA
#ifdef VORBIS_SUPPORT
    case INT_D2RC_VORBIS_CODEBOOK:
        if (_bDspStrTyp == VORBIS_STREAM)
        {
            // Construct the Huffman Tree
            if (fgMakeVorbisCodebook ())
            {
                // set dsp for codebook construction ready
                SendDSPInt (INT_RC2D_VORBIS_TABLE, (UINT32) VORBIS_CODEBOOK_READY);   // short data & interrupt
            }
            else
            {
                // codebook construction fails --> force reset ?
                vDspResetDec1 ();
            }
        }
        break;
#endif
    case INT_D2RC_DSP_INFO:
        //Extract Mode
        u4DspRIntType = (u4DspRIntData >> 8);
        switch (u4DspRIntType)
        {
        case D2RC_DSP_INFO_AIN_UPDATE_ADC_TUNE_CFG:
            DSP_SetAdcTuneCfg();
        	break;
        default:
        	break;
        }
    	break;
    case INT_D2RC_LOG_DUMP:
       HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro    
    	u4DspRIntType = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_TYPE);
    	u4DspRIntData = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA);
    	switch (u4DspRIntType>>8)
    	{
    	case LOG_STC_DIFF:
    		if( u4DspRIntData&0x800000 )
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "Main audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_DIFF_DEC2:
    		if( u4DspRIntData&0x800000 )
    		{
    		    // (-) negative
    	        //u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;
    	        i4DspData = (INT32)(u4DspRIntData | 0xFF000000);
    		}
    		else
    		{
    		    // (+) positive
    		    i4DspData = (INT32)u4DspRIntData;
    		}
   	        LOG(9, "Aux audio stc_diff (stc-pts) = %d\n", i4DspData);
    	    break;
    	case LOG_STC_ADJUST_FRAME:    		
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(7, "Main audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(7, "Main audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
        case LOG_STC_ADJUST_FRAME_DEC2:
    	    if (!(u4DspRIntData&0x800000))
    	    {    	        
    	        // (+) audio drop occurs
    	        LOG(7, "Aux audio drop frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    else
    	    {
    	        u4DspRIntData = (~u4DspRIntData + 1) & 0xffffff;    	            	        
    	        // (-) audio repeat occurs
    	        LOG(7, "Aux audio repeat frame count = %d\n", u4DspRIntData>>8);
    	    }
    	    break;
    	case LOG_AOUT_OK_ENABLE:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "Main audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		#ifdef CHANNEL_CHANGE_LOG
    		HAL_TIME_T dt;
    		HAL_GetTime(&dt);
    		LOG(0, " %u.%06u s [AV SYNC] 4 Main audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", dt.u4Seconds, dt.u4Micros, u4DspRIntData2,u4DspRIntData);
            #endif    		
    		break;
    	case LOG_AOUT_OK_ENABLE_DEC2:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "Aux audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		break;
//#ifdef CC_MT5391_AUD_3_DECODER
#if 0
    	case LOG_AOUT_OK_ENABLE_DEC3:
    		u4DspRIntData <<= 8;
    	    u4DspRIntData |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_2)>>8);
    	    u4DspRIntData2 = dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_3);    	    
    		u4DspRIntData2 <<= 8;
    	    u4DspRIntData2 |= (dReadDspCommDram(ADDR_D2RC_RISC_LOG_DUMP_DATA_4)>>8);
    		LOG(7, "Third audio start output. (PTS, STC)=(0x%08x, 0x%08x)\n", u4DspRIntData2,u4DspRIntData);
    		break;
#endif
    	default:
    		break;
    	}
    	break;
    default:    
#ifdef ADSP_DEBUG        
        DBGLogB (bDspRIntAddr, u4DspRIntData >> 16, u4DspRIntData >> 8,
                 u4DspRIntData);  
#endif /* ADSP_DEBUG */
        break;
    }

    // Just fix warning message for -O2
    UNUSED(i4DspData);
}
// Refine ADSP control
/*
 * flow control handler within ADSP interrupt
 * u4DspRIntData: interrupt for flow control
 * return: message if send interupt to main handler
*/
UINT32 u4IntFlowControl(UINT32 u4DspRIntData)
{
    UINT32 u4RetMsg=ADSPTASK_MSG_INTERRUPT;
    switch(u4DspRIntData)
    {
        case D2RC_FLOW_CONTROL_AOUT_ENABLE:
            vDspAOutEnable();
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE;
            LOG(5, "Dec1 Aout Enable\n");
        break;
        case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2:
            vDspAOutEnableDec2();
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE_DEC2;
            LOG(5, "Dec2 Aout Enable\n"); 
            if (AUD_IsDualDecMode())
            {
                AUD_DualDecSetSyncSema(FALSE);
            }            
        break;
        case D2RC_FLOW_CONTROL_FLUSH_DONE:
            vDspFlushDone ();
            u4RetMsg = ADSPTASK_MSG_INT_STOP;
            LOG(5, "Dec1 flush done\n"); 
        break;
        case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2:
            vDspFlushDoneDec2 ();
            u4RetMsg = ADSPTASK_MSG_INT_STOP_DEC2;
            LOG(5, "Dec2 flush done-----\n\n"); 
        break;
        case D2RC_FLOW_CONTROL_UPDATE_EFFECT:
            u4RetMsg = ADSPTASK_MSG_INT_UPDATE_EFFECT;
            LOG(5, "Dec1 flow control update effect\n"); 
        break;
        case D2RC_FLOW_CONTROL_SAMPLING_RATE:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE;
            LOG(5, "Dec1 set sampling rate\n"); 
        break;
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE_DEC2;
            LOG(5, "Dec2 set sampling rate\n"); 
        break;
        case D2RC_FLOW_CONTROL_MODE_CHANGE:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE;
            LOG(5, "Dec1 mode change\n");             
        break;
        case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE_DEC2;
            LOG(5, "Dec2 mode change\n");             
        break;        
        case D2RC_FLOW_CONTROL_PAUSE_OK:
        case D2RC_FLOW_CONTROL_PAUSE_OK_DEC2:
        // none handling
        break;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
        case D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC3:
            vDspAOutEnableDec3();
            u4RetMsg = ADSPTASK_MSG_INT_AOUT_ENABLE_DEC3;
            LOG(5, "Dec3 Aout Enable\n"); 
            //if (AUD_IsDualDecMode())
            //{
            //    AUD_DualDecSetSyncSema(FALSE);
            //}  
        break;   
        case D2RC_FLOW_CONTROL_FLUSH_DONE_DEC3:
            vDspFlushDoneDec3 ();
            u4RetMsg = ADSPTASK_MSG_INT_STOP_DEC3;
            LOG(5, "Dec3 flush done\n"); 
        break;    
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC3:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLE_RATE_DEC3;
            LOG(5, "Dec3 set sampling rate\n");
        break;    
        case D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3:
            u4RetMsg = ADSPTASK_MSG_INT_MODE_CHANGE_DEC3;
            LOG(5, "Dec3 mode change\n");                         
        break;            
#endif
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE;
            LOG(5, "##### DEC1 decoder's sampling rate change !!! #######\n");                         
        break;
        case D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2:
            u4RetMsg = ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE_DEC2;
            LOG(5, "##### DEC2 decoder's sampling rate change !!! #######\n");                         
        break;
	 	
        default:
        break;
    }
    return(u4RetMsg);
}
