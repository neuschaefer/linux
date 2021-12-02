/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
*
* Filename:
* ---------
*   $Workfile:$
*
* Project:
* --------
* MT8530
*
* Description:
* ------------
* The operations of USB Audio driver
*
* Author:
* -------
*
* Last changed:
* -------------
*
* $Modtime: $
*
* $Revision: #2 $
****************************************************************************/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER

#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#include "x_common.h"
#include "x_rm.h"
//#include "x_drv_cb.h"     //FIX_BUILD_ERROR
//#include "x_memtype.h"    //FIX_BUILD_ERROR
#include "x_printf.h"
//#include "drv_config.h"   //FIX_BUILD_ERROR
//#include "drv_mem.h"      //FIX_BUILD_ERROR
//#include "x_audin.h"      //FIX_BUILD_ERROR
//#include "drv_thread.h"   //FIX_BUILD_ERROR
#include "oss_pcm.h"

OSS_INSTANCE_T _arOssDevInst[MAX_OSS_INST] = {{0}};

static UINT8 u1OssGetCurInst(VOID)
{
    return 0;
}

INT32 i4OssCreateInstance(UINT16 u2CardNum, E_OP_MODE eMode)
{
    INT16 i2mode = 0;
    INT8 i = 0;
    CHAR OssDevPath[10] ="/dev/dsp1";
    OSS_FILE *ossfile;
    mm_segment_t oldfs;
    
    switch(eMode)
    {
        case READ_ONLY:
            i2mode = O_RDONLY;
            break;
        case WRITE_ONLY:
            i2mode = O_WRONLY;
            break;
        case DUPLEX:
            i2mode = O_RDWR;
            break;   
        default:
            return (-1);         
    }
    
    oldfs = get_fs();
    
    OssDevPath[8]= '0' + u2CardNum;
    OssDevPath[9]= 0 ;

	printk("OssDevPath = %s\n", OssDevPath);
	
    set_fs(KERNEL_DS);
    ossfile = filp_open(OssDevPath,i2mode,0);
     if (IS_ERR(ossfile))
    {
        set_fs(oldfs);
        printk("Open failed, please check the USB device's connection!\n");
        return (-1);
    }
    
    for(i = 0; i < MAX_OSS_INST; i++)
    {
        if(DESTROIED == _arOssDevInst[i].eInstStat)
        {
            _arOssDevInst[i].eInstStat = CREATED;
            _arOssDevInst[i].old_fs = oldfs;
            _arOssDevInst[i].ossfile = ossfile;
            break;
        }
    }
    
    return (0);
}

INT32 i4OssDestroyInstance(VOID)
{
    //INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    
    //SNDCTL_DSP_RESET will trigger oss to restart urb when closing runtime stream,
    //no need here, close and reopen is safe enough
    #if(0)
    printk("SNDCTL_DSP_RESET\n");
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_RESET,0)))
    {
        printk("reset failed\n");
    }
    #endif
    printk("filp_close\n");
    filp_close(_arOssDevInst[u1Cur].ossfile,NULL);
    printk("set_fs\n");
    set_fs(_arOssDevInst[u1Cur].old_fs);
    printk("done\n");
    _arOssDevInst[u1Cur].eInstStat = DESTROIED;
    _arOssDevInst[u1Cur].ossfile = NULL;      
    return (0);
}

INT32 i4OssQueryStatus(VOID)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    int delay;

    // SNDCTL_DSP_GETODELAY
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_GETODELAY,(unsigned long)&delay)))
    {
        printk("SNDCTL_DSP_GETODELAY failed\n");
    }
    else
    {
        printk("SNDCTL_DSP_GETODELAY delay = %d\n", delay);
    }

    return (0);
}

INT32 i4OssGetOutputDelay(VOID)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    int delay;

    // SNDCTL_DSP_GETODELAY
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_GETODELAY,(unsigned long)&delay)))
    {
        printk("SNDCTL_DSP_GETODELAY failed\n");
        return 0;
    }

    return delay;
}

/******************************************************************************
 *Function    : i4OssSetFormat()
 *Purpose     : Set format
 *Returns      :0 means succeuss
                      :-1 means failed
 *****************************************************************************/
INT32 i4OssSetFormat(UINT32 u4Format)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_SETFMT,(unsigned long)&u4Format)))
    {
        printk("set format failed\n");
    }
    return i4Result;
}

/******************************************************************************
 *Function    : i4OssSetChannel()
 *Purpose     : Set channel
 *Returns      :0 means succeuss
                      :-1 means failed
 *****************************************************************************/
INT32 i4OssSetChannel(UINT32 u4ChannelNO)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    //20110128 IR Lian
    //For multichannel output
    //if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_STEREO,(unsigned long)&u4ChannelNO)))
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_CHANNELS,(unsigned long)&u4ChannelNO)))
    {
        printk("set Channel failed\n");
    }
    return i4Result;
}


/******************************************************************************
 *Function    : i4OssSetSpeed()
 *Purpose     : Set sample rate
 *Returns      :0 means succeuss
                      :-1 means failed
 *****************************************************************************/
INT32 i4OssSetSpeed(UINT32 u4SampleRate)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_SPEED,(unsigned long)&u4SampleRate)))
    {
        printk("set speed failed\n");
    }
    else
    {
        printk("set speed, u4SampleRate = %ld\n", u4SampleRate);
    }
    return i4Result;
}


/******************************************************************************
 *Function    : i4OssGetOptimumFragmentSize()
 *Purpose     : OSS driver will compute the optimum fragment size automatically 
                        depending on sampling parameters
                        (speed, bits and number of channels) and amount of available memory

 *****************************************************************************/
INT32 i4OssGetOptimumFragmentSize(UINT32 *pu4size)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    if(!pu4size)
    {
        return (-1);
    }
    i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_GETBLKSIZE,((unsigned long)pu4size));
    return i4Result;
}

/******************************************************************************
 *Function    : i4OssSetFragmentSize()
 *Purpose     : In some cases it may be desirable to select the fragment size explicitly. For example, in real 
                        time applications such as games, it is necessary to use relatively short fragments. Otherwise, 
                       delays between events on the screen and their associated sound effects become too long.
 *Returns      :0 means succeuss
                      :-1 means failed
 *****************************************************************************/
INT32 i4OssSetFragmentSize(UINT32 u4size)
{
    INT32 i4Result;
    UINT8 u1Cur = u1OssGetCurInst();
    i4Result = _arOssDevInst[u1Cur].ossfile->f_op->unlocked_ioctl(_arOssDevInst[u1Cur].ossfile,SNDCTL_DSP_SETFRAGMENT,(unsigned long)&u4size);
    return i4Result;
}

/******************************************************************************
 *Function    : i4OssWrite()
 *Purpose     : Playback the data streaming
 *Returns      :0 means succeuss
                      :-1 means failed
 *****************************************************************************/
INT32 i4OssWrite(UCHAR *pcBuffer, UINT32 u4size)
{
    INT32 i4Result;
    UINT64 u8WriteSize =0;
    UINT8 u1Cur = u1OssGetCurInst();
    if(-1 == (i4Result = _arOssDevInst[u1Cur].ossfile->f_op->write(_arOssDevInst[u1Cur].ossfile,pcBuffer,u4size,&u8WriteSize)))
    {
        printk("write failed\n");
    }
    
    return i4Result;
}

#endif
