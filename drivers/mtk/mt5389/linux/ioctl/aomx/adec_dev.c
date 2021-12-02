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

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_AOMX)

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include "x_os.h"
#include "x_assert.h"
//#include "x_module.h" //DANIEL_HACK
//#include "x_major.h"  //DANIEL_HACK
#include "x_printf.h"

#include "aud_if.h" //DANIEL_HACK
#include "aud_omx.h"

//DANIEL_HACK
#ifndef SYS_Printf
//#define SYS_Printf Printf
#define SYS_Printf(fmt...)
#endif

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

//static BOOL _fgAdecModInit = FALSE;

static long _i4AdecIoctl(struct file *filp, unsigned int cmd, unsigned long param);
static int _i4AdecOpen(struct inode *inode, struct file *file);
static int _i4AdecRelease(struct inode *inode, struct file *file);

//static struct cdev _adec_cdev; //DANIEL_HACK
static int _adec_opencnt = 0;

#if 1 //DANIEL_HACK
struct file_operations aomx_fops =
#else
static const struct file_operations _adec_fops =
#endif
{
    .owner          = THIS_MODULE,
    .open           = _i4AdecOpen,
    .release        = _i4AdecRelease,
    .unlocked_ioctl = _i4AdecIoctl,
};

//DANIEL_HACK
static void _k_adec_omx_event_nfy(VOID* u4Info, VOID* u4Tag)
{
    DEC_OMX_CB_EVENT_T rEvent;
    rEvent.u4Info = *((PARAMETER_OMX_CB_EVENTHANDLER*)u4Info);;
    rEvent.u4Tag = (UINT32)u4Tag;
    _CB_PutEvent(CB_MTAL_ADEC_OMX_EVENT_MESSAGE, sizeof(DEC_OMX_CB_EVENT_T), (void*)&rEvent);
}

static void _k_adec_omx_empty_buffer_done_nfy(VOID* u4Info, VOID* u4Tag)
{
    DEC_OMX_CB_BUF_T rEmptyBufDone;
    rEmptyBufDone.u4Info = *((PARAMETER_OMX_CB_BUFFER*)u4Info);
    rEmptyBufDone.u4Tag = (UINT32)u4Tag;
    _CB_PutEvent(CB_MTAL_ADEC_OMX_EMPTYBUFFERDONE_MESSAGE, sizeof(DEC_OMX_CB_BUF_T), (void*)&rEmptyBufDone);
}

static void _k_adec_omx_fill_buffer_done_nfy(VOID* u4Info, VOID* u4Tag)
{
    DEC_OMX_CB_BUF_T rFillBufDone;
    rFillBufDone.u4Info = *((PARAMETER_OMX_CB_BUFFER*)u4Info);
    rFillBufDone.u4Tag = (UINT32)u4Tag;
    _CB_PutEvent(CB_MTAL_ADEC_OMX_FILLBUFFERDONE_MESSAGE, sizeof(DEC_OMX_CB_BUF_T), (void*)&rFillBufDone);
}
//DANIEL_HACK

static long _i4AdecIoctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    INT32 i4Ret = 0;

    SYS_Printf("[AUD][OMX] _i4AdecIoctl: cmd = 0x%X/%X.\n", cmd, OMX_PARAM_IndexParamAudioAmr);

    switch (cmd)
    {
    case OMX_GETHANDLE:
        // return within 20ms
        #if 1 //DANIEL_HACK
        {
            //Create instance
            PARAMETER_OMX_CB rOmxCb;
            rOmxCb.pEventHandler = _k_adec_omx_event_nfy;
            rOmxCb.pEmptyBufferDone = _k_adec_omx_empty_buffer_done_nfy;
            rOmxCb.pFillBufferDone = _k_adec_omx_fill_buffer_done_nfy;
            i4Ret = i4AudOmxCompGetHandle((PARAMETER_OMX_GETHANDLE*)arg, &rOmxCb);
        }
        #else
        SYS_Printf("[AUD][OMX] OMX_GETHANDLE.\n");
        i4Ret = i4AudOmxCompGetHandle((PARAMETER_OMX_GETHANDLE *)arg);
        #endif
        break;
    case OMX_FREEHANDLE:
        // return within 20ms
        SYS_Printf("[AUD][OMX] OMX_FREEHANDLE.\n");
        i4Ret = i4AudOmxCompFreeHandle((PARAMETER_OMX_FREEHANDLE *)arg);
        break;
    case OMX_ALLOCATEBUFFER:
        // blocking, return within 5ms
        SYS_Printf("[AUD][OMX] OMX_ALLOCATEBUFFER.\n");
        i4Ret = i4AudOmxCompAllocateBuffer((PARAMETER_OMX_ALLOCATEBUFFER *)arg);
        break;
    case OMX_FREEBUFFER:
        // blocking, return within 20ms
        SYS_Printf("[AUD][OMX] OMX_FREEBUFFER.\n");
        i4Ret = i4AudOmxCompFreeBuffer((PARAMETER_OMX_FREEBUFFER *)arg);
        break;
    case OMX_EMPTYTHISBUFFER:
        // non-blocking
        SYS_Printf("[AUD][OMX] OMX_EMPTYTHISBUFFER.\n");
        i4Ret = i4AudOmxCompEmptyBuffer((PARAMETER_OMX_EMPTYTHISBUFFER *)arg);
        break;
    case OMX_FILLTHISBUFFER:
        // non-blocking
        SYS_Printf("[AUD][OMX] OMX_FILLTHISBUFFER.\n");
        i4Ret = i4AudOmxCompFillBuffer((PARAMETER_OMX_FILLTHISBUFFER *)arg);
        break;
    case OMX_GETSTATE:
        // return within 5ms
        SYS_Printf("[AUD][OMX] OMX_GETSTATE.\n");
        i4Ret = i4AudOmxCompGetState((PARAMETER_OMX_GETSTATE *)arg);
        break;
    case OMX_COMMAND_CommandStateSet:
        // non-blocking
        SYS_Printf("[AUD][OMX] OMX_COMMAND_CommandStateSet.\n");
        i4Ret = i4AudOmxCompSetStateAsync((PARAMETER_OMX_COMMAND_STATESET *)arg);
        break;
    case OMX_COMMAND_CommandFlush:
        // non-blocking
        SYS_Printf("[AUD][OMX] OMX_COMMAND_CommandFlush.\n");
        i4Ret = i4AudOmxCompFlushAsync((PARAMETER_OMX_COMMAND_PORT *)arg);
        break;
    case OMX_COMMAND_CommandPortDisable:
        // non-blocking
        SYS_Printf("[AUD][OMX] OMX_COMMAND_CommandPortDisable.\n");
        i4AudOmxCompPortDisableAsync((PARAMETER_OMX_COMMAND_PORT *)arg);
        break;
    case OMX_COMMAND_CommandPortEnable:
        // non-blocking
        SYS_Printf("[AUD][OMX] OMX_COMMAND_CommandPortEnable.\n");
        i4AudOmxCompPortEnableAsync((PARAMETER_OMX_COMMAND_PORT *)arg);
        break;
    case OMX_PARAM_IndexParamAudioInit:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioInit.\n");
        i4Ret = i4AudOmxCompGetParamAudioInit((PARAMETER_OMX_PORT_PARAM_TYPE *)arg);
        break;
    case OMX_PARAM_IndexParamPortDefinition:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamPortDefinition.\n");
        i4Ret = i4AudOmxCompParamPortDef((PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioPcm:
        // blocking, return within 20ms (for both set and get)
        // currently not supported
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioPcm.\n");
        return i4AudOmxCompParamPCM((PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE*)arg);
        break;
    case OMX_PARAM_IndexParamAudioAac:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioAac.\n");
        i4Ret = i4AudOmxCompParamAAC((PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioMp3:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioMp3.\n");
        i4Ret = i4AudOmxCompParamMP3((PARAMETER_OMX_AUDIO_PARAM_MP3TYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioWma:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioWma.\n");
        i4Ret = i4AudOmxCompParamWMA((PARAMETER_OMX_AUDIO_PARAM_WMATYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioWmaPro:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioWmaPro.\n");
        i4Ret = i4AudOmxCompParamWMAPRO((PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioRa:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioRa.\n");
        i4Ret = i4AudOmxCompParamRA((PARAMETER_OMX_AUDIO_PARAM_RATYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioAdpcm:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioAdpcm.\n");
        i4Ret = i4AudOmxCompParamADPCM((PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioVorbis:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioVorbis.\n");
        i4Ret = i4AudOmxCompParamVORBIS((PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE *)arg);
        break;
    case OMX_PARAM_IndexParamAudioAmr:
        // blocking, return within 20ms (for both set and get)
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexParamAudioAmr.\n");
        i4Ret = i4AudOmxCompParamAMR((PARAMETER_OMX_AUDIO_PARAM_AMRTYPE *)arg);
        break;
    case OMX_PARAM_IndexConfigAudioVolume:
        // blocking, return within 20ms (for both set and get)
        // currently not supported
        SYS_Printf("[AUD][OMX] OMX_PARAM_IndexConfigAudioVolume not implemented.\n");
        break;
    default:
        SYS_Printf("[AUD][OMX]%s, UNKNOWN COMMAND\n",__FUNCTION__);
        return -1;
    }

    return i4Ret;
}

static int _i4AdecOpen(struct inode *inode, struct file *file)
{
    SYS_Printf("[ADEC][OMX] _i4AdecOpen.\n");

    if (0 == _adec_opencnt)
    {
        AUD_Init(); //DANIEL_HACK
        _AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
        _AUD_OutPadEnable(AUD_DEC_AUX, TRUE);
        ADAC_Mute(FALSE);
        i4AudOmxCompInit();
        vAudOmxBufferInit();
        i4AudOmxTaskSemInit();
    }

    _adec_opencnt ++;

    return 0;
}

static int _i4AdecRelease(struct inode *inode, struct file *file)
{
    SYS_Printf("[ADEC][OMX] _i4AdecRelease.\n");

    _adec_opencnt --;

    if (0 == _adec_opencnt)
    {
        i4AudOmxCompUninit();

        i4AudOmxTaskSemUninit();
        vAudOmxBufferUninit();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0 //DANIEL_HACK
extern void * pfn_AudOpenMAXCmdCli;

static INT32 _AudOpenMAXCmdCli(INT32 i4Argc, const CHAR ** szArgv)
{
    SYS_Printf("[AUD][OMX] Test CLI.\n");

    return 0;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
// *********************************************************************
// export interface function
// *********************************************************************
void adec_mod_init(void)
{
    INT32 i4Ret;

    if (_fgAdecModInit)
    {
        return;
    }

#if 0 //DANIEL_HACK
    // TODO: temp solution: register CLI command callback
    pfn_AudOpenMAXCmdCli = (void *)_AudOpenMAXCmdCli;

    // register device
    cdev_init(&_adec_cdev, &_adec_fops);
    _adec_cdev.owner = THIS_MODULE;
    i4Ret = cdev_add(&_adec_cdev, MKDEV(ADEC_OMX_MAJOR, 0), 1);

    if (i4Ret != 0)
    {
        SYS_Printf(KERN_ERR "[ADEC][OMX] adec_mod_init(), unable to add device!\n");
        return;
    }
#endif

    _fgAdecModInit = TRUE;
}

void adec_mod_uninit(void)
{
    if (!_fgAdecModInit)
    {
        return;
    }

    _fgAdecModInit = FALSE;
}

EXPORT_SYMBOL(adec_mod_init);
EXPORT_SYMBOL(adec_mod_uninit);
#endif

#endif

