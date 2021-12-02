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

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_VOMX)

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/cdev.h>
//fix89 #include "x_module.h"
//fix89 #include "x_major.h"
#include <linux/fs.h>          //For file_operations
#include "vomx_if.h"
#include "x_debug.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"

#include "vdec_mod.h"
#include <linux/cb_data.h>

//#include "vdec_drv.h"
//fix89 #include "x_os.h"
#include <asm/uaccess.h>
//fix89 #include "x_rtos.h"

#include "omx_vdec_vconv.h"


extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//HANDLE_T hVdecSetFifoSema;				///< the semapfore id
static int _vdec_opencnt = 0;
bool _fgVOmxLog = 0;

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

static void _k_vdec_omx_event_nfy(VOID* pInfo, VOID* pTag)
{
    DEC_OMX_CB_EVENT_T rEvent;
    rEvent.u4Info = *((PARAMETER_OMX_CB_EVENTHANDLER*)pInfo);
    rEvent.u4Tag = (unsigned int)pTag;
    _CB_PutEvent(CB_MTAL_VDEC_OMX_EVENT_MESSAGE, (INT32)sizeof(DEC_OMX_CB_EVENT_T), (void*)&rEvent);
}

static void _k_vdec_omx_empty_buffer_done_nfy(VOID* pInfo, VOID* pTag)
{
    DEC_OMX_CB_BUF_T rEmptyBufDone;
    rEmptyBufDone.u4Info = *((PARAMETER_OMX_CB_BUFFER*)pInfo);
    rEmptyBufDone.u4Tag = (unsigned int)pTag;
    _CB_PutEvent(CB_MTAL_VDEC_OMX_EMPTYBUFFERDONE_MESSAGE, (INT32)sizeof(DEC_OMX_CB_BUF_T), (void*)&rEmptyBufDone);
}

static void _k_vdec_omx_fill_buffer_done_nfy(VOID* pInfo, VOID* pTag)
{
    DEC_OMX_CB_BUF_T rFillBufDone;
    rFillBufDone.u4Info = *((PARAMETER_OMX_CB_BUFFER*)pInfo);
    rFillBufDone.u4Tag = (unsigned int)pTag;
    _CB_PutEvent(CB_MTAL_VDEC_OMX_FILLBUFFERDONE_MESSAGE, (INT32)sizeof(DEC_OMX_CB_BUF_T), (void*)&rFillBufDone);
}

int k_vdec_omx_open(struct inode *inode, struct file *flip)
{
    //VOMX_Printf ("[Vdec_mode]vdec driver opened successfully!\n");
    if (0 == _vdec_opencnt)
    {
        i4VDOOmxCompInit();
        vVDOOmxBufferInit();
        i4VDOOmxTaskSemInit();
    }

    _vdec_opencnt ++;

    return 0;
}


int k_vdec_omx_release(struct inode *inode, struct file *flip)
{
    //VOMX_Printf("[Vdec_mode]vdec driver closed sucessfully!\n");
    _vdec_opencnt --;

    if (0 == _vdec_opencnt)
    {
        i4VDOOmxCompUninit();
        i4VDOOmxTaskSemUninit();
        vVDOOmxBufferUninit();
    }

    return 0;
}



//-----------------------------------------------------------------------------
/** vdec_ioctl.
 */
//-----------------------------------------------------------------------------
int k_vdec_omx_ioctl(struct inode *inode,
                struct file *file, 
                unsigned int cmd,
                 unsigned long arg)
{
	INT32  i4Ret = OMX_ErrorNone;

       OMX_CMD_VDEC_T u4VDecOMXCmd;
       u4VDecOMXCmd.rOMXVDECCMD = (OMX_COMMAND)cmd;

    switch (u4VDecOMXCmd.rOMXVDECCMD)
    {
    case OMX_GETHANDLE:
        {
        	 //Create instance 
        	PARAMETER_OMX_CB rOmxCb;
            //VOMX_Printf("[Vdec_mode]OMX_GETHANDLE\n");  
            
        	rOmxCb.pEventHandler = _k_vdec_omx_event_nfy;
        	rOmxCb.pEmptyBufferDone = _k_vdec_omx_empty_buffer_done_nfy;
        	rOmxCb.pFillBufferDone = _k_vdec_omx_fill_buffer_done_nfy;
            i4Ret = i4VDOOmxCompGetHandle((PARAMETER_OMX_GETHANDLE*)arg, &rOmxCb);
        }
        break;
	
    case OMX_FREEHANDLE:
    	//Release instance
        //VOMX_Printf("[Vdec_mode]OMX_FREEHANDLE\n");	
    	 i4Ret = i4VDOOmxCompFreeHandle((PARAMETER_OMX_FREEHANDLE*)arg);
        break;

    case OMX_ALLOCATEBUFFER:
       //VOMX_Printf("[Vdec_mode]OMX_ALLOC_BUFFER\n");  
    	i4Ret = i4VDOOmxCompAllocateBuffer((PARAMETER_OMX_ALLOCATEBUFFER*)arg);
        break;
        
    case OMX_FREEBUFFER:
       //VOMX_Printf("[Vdec_mode]OMX_FREE_BUFFER\n");  
    	i4Ret = i4VDOOmxCompFreeBuffer((PARAMETER_OMX_FREEBUFFER*)arg);
	 break;     

    case OMX_EMPTYTHISBUFFER:
        //VOMX_Printf("[Vdec_mode]OMX_EMPTY_THIS_BUFFER\n");  
    	i4Ret = i4VDOOmxCompEmptyBuffer((PARAMETER_OMX_EMPTYTHISBUFFER*)arg);
	 break;     
	 
    case OMX_FILLTHISBUFFER:
        //VOMX_Printf("[Vdec_mode]OMX_FILL_THIS_BUFFER\n");  
    	i4Ret = i4VDOOmxCompFillBuffer((PARAMETER_OMX_FILLTHISBUFFER*)arg);
	 break;     
	 
    case OMX_GETSTATE:
        //VOMX_Printf("[Vdec_mode]OMX_GET_STATE\n");  
    	i4Ret = i4VDOOmxCompGetState((PARAMETER_OMX_GETSTATE*)arg);
	 break;     

    case OMX_PARAM_IndexParamVideoInit:
    	 //To this instance, get/set parameter of certain type
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoInit \n");  
        i4Ret = i4VDOOmxCompGetParamVideoInit((PARAMETER_OMX_PORT_PARAM_TYPE*)arg);
        break;
        
    case OMX_PARAM_IndexParamPortDefinition:
    	 //To this instance, get/set parameter of certain type
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamPortDefinition\n");  
        i4Ret = i4VDOOmxCompParamPortDef((PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE*)arg);
        break;
    	
    case OMX_COMMAND_CommandStateSet:
        //VOMX_Printf("[Vdec_mode]OMX_COMMAND_CommandStateSet\n");  
    	i4Ret = i4VDOOmxCompSetStateAsync((PARAMETER_OMX_COMMAND_STATESET*)arg);
        break;    
    case OMX_COMMAND_CommandFlush:
        //VOMX_Printf("[Vdec_mode]OMX_COMMAND_CommandFlush\n");  
    	i4Ret = i4VDOOmxCompFlushAsync((PARAMETER_OMX_COMMAND_PORT*)arg);
        break;    
    	
    case OMX_COMMAND_CommandPortDisable:
        //VOMX_Printf("[Vdec_mode]OMX_COMMAND_CommandPortDisable\n");  
        i4VDOOmxCompPortDisableAsync((PARAMETER_OMX_COMMAND_PORT *)arg);
    	 break;
    case OMX_COMMAND_CommandPortEnable:
        //VOMX_Printf("[Vdec_mode]OMX_COMMAND_CommandPortEnable\n");  
        i4VDOOmxCompPortEnableAsync((PARAMETER_OMX_COMMAND_PORT *)arg);
    	 break;
        
    case OMX_PARAM_IndexParamVideoProfileLevelQuerySupported:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoProfileLevelQuerySupported\n");  
        i4VDOOmxCompParamVideoProfileLevelQuerySupported((PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE *)arg);
    	 break;

    case OMX_PARAM_IndexParamVideoAvc:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoAvc\n");  
        i4VDOOmxCompParamVideoAvc((PARAMETER_OMX_VIDEO_PARAM_AVC *)arg);
    	 break;

    case OMX_PARAM_IndexParamVideoRv:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoRv\n");  
        i4VDOOmxCompParamVideoRv((PARAMETER_OMX_VIDEO_PARAM_RV *)arg);
    	 break;
    	 
    case OMX_PARAM_IndexParamVideoWmv:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoWmv\n");  
        i4VDOOmxCompParamVideoWmv((PARAMETER_OMX_VIDEO_PARAM_WMV *)arg);
    	 break;

    case OMX_PARAM_IndexParamVideoPortFmt:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoPortFmt\n");  
        i4VDOOmxCompParamVideoPortFmt((PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE *)arg);
    	 break;

    case OMX_PARAM_IndexParamVideoSeamless:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoSeamless\n");  
        i4VDOOmxCompParamVideoSeamlessFmt((PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE *)arg);
    	 break;

    case OMX_PARAM_IndexConfigPictureMode:
        i4Ret = i4VDOOmxCompConfigPictureMode((PARAMETER_OMX_IMAGE_MODE *)arg);
        break;

    case OMX_PARAM_IndexConfigClearImageFrame:
        i4Ret = i4VDOOmxCompConfigClearImageFrame((PARAMETER_OMX_IMAGE_PROCESSING *)arg);
        break;

    case OMX_PARAM_IndexConfigCommonOutputSize:
        i4Ret = i4VDOOmxCompConfigCommonOutputSize((PARAMETER_OMX_FRAMESIZETYPE *)arg);
        break;

    case OMX_PARAM_IndexConfigCommonRotate:
        i4Ret = i4VDOOmxCompConfigCommonRotate((PARAMETER_OMX_CONFIG_ROTATIONTYPE *)arg);
        break;

    case OMX_PARAM_IndexConfigCommonScale:
        i4Ret = i4VDOOmxCompConfigCommonScale((PARAMETER_OMX_CONFIG_SCALEFACTORTYPE *)arg);
        break;

    case OMX_PARAM_IndexConfigCommonOutputPosition:
        i4Ret = i4VDOOmxCompConfigCommonOutputPosition((PARAMETER_OMX_CONFIG_POINTTYPE *)arg);
        break;

    case OMX_PARAM_IndexParamExpandedSize:
        i4Ret = i4VDOOmxCompParamExpandedSize((PARAMETER_OMX_FRAMESIZETYPE *)arg);
        break;
    case OMX_PARAM_IndexParamOriginalSize:
        i4Ret = i4VDOOmxCompParamOriginalSize((PARAMETER_OMX_FRAMESIZETYPE *)arg);
        break;
    case OMX_PARAM_IndexParamImageFreeBuffer:
        i4Ret = i4VDOOmxCompParamImageFreeBuffer((PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER*)arg);
        break;

    case OMX_PARAM_IndexConfigOutputDevice:
        //do nothing
        break;

    case OMX_PARAM_IndexParamImageFileSize:
        i4VDOOmxCompParamImageUsrDefineData((PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED *)arg);
        break;

    case OMX_PARAM_IndexConfigTimeScale:
        i4VDOOmxCompParamTimeScale((PARAMETER_OMX_TIME_CONFIG_SCALETYPE *)arg);
        break;

#if 0//todo
    case OMX_VIDEO_PARAM_RVTYPE:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoPortFmt\n");  
        i4VDOOmxCompParamVideoPortFmt((PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE *)arg);
    	 break;
 
    case OMX_VIDEO_PARAM_WMVTYPE:
        //VOMX_Printf("[Vdec_mode]OMX_PARAM_IndexParamVideoPortFmt\n");  
        i4VDOOmxCompParamVideoPortFmt((PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE *)arg);
    	 break;
#endif

    default:
       break;
    }

    return i4Ret;    
}


#if DTV_VOMX//fix89 (CONFIG_SECTION_BUILD_LINUX_KO)
//static struct cdev _cdev_vdec[2];
struct file_operations vomx_fops = {
    .owner   = THIS_MODULE,
    .open       = k_vdec_omx_open,
    .ioctl      = k_vdec_omx_ioctl,
    .release    = k_vdec_omx_release,
};
#endif
#if 0 //fix89 mkdev at mtk_mod.c
static int __init vdec_omx_init(void)
{
    int ret = OMX_ErrorNone;
    INT32 i4Idx = 1;

    
//    ret = x_sema_create(&hVdecModeResourceSema, X_SEMA_TYPE_BINARY, 1);
//    VERIFY(ret == OSR_OK);

//    ret = register_chrdev(VDEC_MAJOR, "drvvdec", &vdec_fops);
//    ret = x_sema_create(&hVdecSetFifoSema, X_SEMA_TYPE_BINARY, 1);
//    VERIFY(ret == OSR_OK);

#if (CONFIG_SECTION_BUILD_LINUX_KO)
    cdev_init(&_cdev_vdec[i4Idx], & vdec_fops);
    _cdev_vdec[i4Idx].owner = THIS_MODULE;
    if (0 != cdev_add(&_cdev_vdec[i4Idx], MKDEV(VDEC_OMX_MAJOR, 0), 1))
    {
      VOMX_Printf("[VDEC_OMX] Unable to add device!\n");
      ASSERT(0);
      return ret;
    }
#endif
    //VMOD_PRINT("[Vdec_mode]open vdec driver...\n");
    return ret;
    
}
#endif
static void __exit vdec_omx_exit(void)
{
//    INT32 i4Ret =0;
//    INT32 i4Idx = 1;

    //fix89 cdev_del(&_cdev_vdec[i4Idx]);
//    unregister_chrdev(VDEC_MAJOR, "drvvdec");
//    i4Ret = x_sema_delete(hVdecSetFifoSema);
//     i4Ret = x_sema_delete(hVdecModeResourceSema);
//    VERIFY(i4Ret == OSR_OK);  
    //VMOD_PRINT("[Vdec_mode]vdec device uninit sucess!\n");
}

//DECLARE_MODULE(vdec_omx);


#endif


