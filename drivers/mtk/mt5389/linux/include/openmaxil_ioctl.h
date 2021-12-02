#ifndef OPENMAXIL_IOCTL_H
#define OPENMAXIL_IOCTL_H

#include "OMX_Component.h"
#include "x_kmem.h"
#include <linux/ioctl.h>
#include <OMX_IndexExt.h>
#include <OMX_VideoExt.h>

#define OMX_KO 1
/**omx data callback function.
*/
typedef void (*MTVDEC_PFN_OMX_IL_CB)
(
    void* pv_cb_info, void* pv_nfy_tag
);

/**omx data callback function.
*/
typedef int (*DEC_PFN_OMX_IL_CB)    //dtv callback style
(
    void* pvTag
);
typedef enum {
    OMX_UNKNOWN,
    
    OMX_GETHANDLE = _IOWR('c', 0, long),
    OMX_FREEHANDLE = _IOWR('c', 1, long),
    OMX_ALLOCATEBUFFER = _IOWR('c', 2, long),
    OMX_FREEBUFFER = _IOWR('c', 3, long),
    OMX_EMPTYTHISBUFFER = _IOWR('c', 4, long),
    OMX_FILLTHISBUFFER = _IOWR('c', 5, long),
    OMX_GETSTATE = _IOWR('c', 6, long),
    
    OMX_PARAM_IndexParamAudioInit = _IOWR('c', 7, long),
    OMX_PARAM_IndexParamVideoInit = _IOWR('c', 8, long),
    OMX_PARAM_IndexParamPortDefinition = _IOWR('c', 9, long),
    OMX_PARAM_IndexParamAudioPcm = _IOWR('c', 10, long),
    OMX_PARAM_IndexParamAudioAac = _IOWR('c', 11, long),
    OMX_PARAM_IndexParamAudioMp3 = _IOWR('c', 12, long),
    OMX_PARAM_IndexConfigAudioVolume = _IOWR('c', 13, long),
    OMX_PARAM_IndexConfigTimeScale = _IOWR('c', 14, long),
    OMX_PARAM_IndexConfigTimeClockState = _IOWR('c', 15, long),
    OMX_PARAM_IndexConfigTimeActiveRefClock = _IOWR('c', 16, long),
    OMX_PARAM_IndexConfigTimeCurrentMediaTime = _IOWR('c', 17, long),
    
    OMX_COMMAND_CommandStateSet = _IOWR('c', 18, long),
    OMX_COMMAND_CommandFlush = _IOWR('c', 19, long),
    OMX_COMMAND_CommandPortDisable = _IOWR('c', 20, long),
    OMX_COMMAND_CommandPortEnable = _IOWR('c', 21, long),
    
    OMX_PARAM_IndexParamVideoProfileLevelQuerySupported = _IOWR('c', 22, long),
    OMX_PARAM_IndexParamVideoAvc = _IOWR('c', 23, long),
    OMX_PARAM_IndexParamAudioWma = _IOWR('c', 24, long),
    OMX_PARAM_IndexParamAudioRa = _IOWR('c', 25, long),
    OMX_PARAM_IndexParamAudioAdpcm = _IOWR('c', 26, long),
    OMX_PARAM_IndexParamVideoPortFmt = _IOWR('c', 27, long),
    OMX_PARAM_IndexParamVideoRv = _IOWR('c', 28, long),
    OMX_PARAM_IndexParamVideoWmv = _IOWR('c', 29, long),
    OMX_PARAM_IndexParamAudioWmaPro = _IOWR('c', 30, long),
    OMX_PARAM_IndexParamAudioVorbis = _IOWR('c', 31, long),

    OMX_PARAM_IndexConfigPictureMode = _IOWR('c', 32, long),       /**< reference: OMX_IMAGE_MODE */
    OMX_PARAM_IndexConfigClearImageFrame = _IOWR('c', 33, long),     /**< reference: OMX_IMAGE_PROCESSING */
    OMX_PARAM_IndexConfigCommonOutputSize = _IOWR('c', 34, long),        /**< reference: OMX_FRAMESIZETYPE */
    OMX_PARAM_IndexConfigCommonRotate = _IOWR('c', 35, long),           /**< reference: OMX_CONFIG_ROTATIONTYPE */
    OMX_PARAM_IndexConfigCommonScale = _IOWR('c', 36, long),            /**< reference: OMX_CONFIG_SCALEFACTORTYPE */
    OMX_PARAM_IndexConfigCommonOutputPosition = _IOWR('c', 37, long),            /**< reference: OMX_CONFIG_POINTTYPE */
    OMX_PARAM_IndexParamExpandedSize = _IOWR('c', 38, long),            /**< reference: OMX_FRAMESIZETYPE */
    OMX_PARAM_IndexParamOriginalSize = _IOWR('c', 39, long),            /**< reference: OMX_FRAMESIZETYPE */
    OMX_PARAM_IndexParamImageFreeBuffer = _IOWR('c', 40, long),            /**< reference: OMX_BUFFERHEADERTYPE */
    OMX_PARAM_IndexConfigOutputDevice = _IOWR('c', 41, long),            /**< reference: OMX_IMAGE_USER_DEFINED */
    OMX_PARAM_IndexParamAudioAmr = _IOWR('C', 42, long),
    OMX_PARAM_IndexParamImageFileSize = _IOWR('c', 43, long),            /**< reference: OMX_IMAGE_USER_DEFINED */
    OMX_PARAM_IndexParamVideoSeamless = _IOWR('c', 44, long),            /**< reference: OMX_SEAMLESSFORMATTYPE */
    OMX_COMMAND_MAX = _IOWR('c', 45, long)

} OMX_COMMAND;


typedef enum {
    OMX_PARAM_GET,
    OMX_PARAM_SET
} OMX_PARAM_DIR;


typedef struct _PLATFORM_PRIVATE {
    HANDLE_T                    hPBufHeader;
    HANDLE_T                    hPBufHeaderBuf;
    OMX_BUFFERHEADERTYPE        *pUserBufHeader;
    OMX_BOOL                    fgUseBuf;
    OMX_BOOL                    fgKernelAddr;         //0: user addr, 1: kernel addr
    OMX_U8                      *pUseBuffer;
    OMX_U8                      *pInternalBuffer;
}PLATFORM_PRIVATE;

typedef struct _PARAMETER_OMX_CB {                  //OMX_GETHANDLE
    MTVDEC_PFN_OMX_IL_CB            pEventHandler;
    MTVDEC_PFN_OMX_IL_CB            pEmptyBufferDone;
    MTVDEC_PFN_OMX_IL_CB            pFillBufferDone;
} PARAMETER_OMX_CB;

typedef struct _PARAMETER_OMX_GETHANDLE {           //OMX_GETHANDLE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_STRING                  cComponentName; 
    OMX_PTR                     pAppData;
} PARAMETER_OMX_GETHANDLE;

typedef struct _PARAMETER_OMX_FREEHANDLE {           //OMX_FREEHANDLE
    OMX_ERRORTYPE           eReturnValue;
    OMX_HANDLETYPE          pHandle;
} PARAMETER_OMX_FREEHANDLE;

typedef struct _PARAMETER_OMX_ALLOCATEBUFFER {           //OMX_ALLOCATEBUFFER
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    HANDLE_T                    hPBufHeader;
    HANDLE_T                    hPBufHeaderBuf;
    OMX_U32                     nPortIndex;
    OMX_PTR                     pAppPrivate;
    OMX_U32                     nSizeBytes;
    HANDLE_T                    hPBufHeaderInternalBuf;
    OMX_BOOL                    fgUseBuf;                // use the assign address
} PARAMETER_OMX_ALLOCATEBUFFER;

typedef struct _PARAMETER_OMX_FREEBUFFER {           //OMX_FREEBUFFER
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_U32                     nPortIndex;
    HANDLE_T                    hPBufHeader;
    HANDLE_T                    hPBufHeaderBuf;
} PARAMETER_OMX_FREEBUFFER;

typedef struct _PARAMETER_OMX_EMPTYTHISBUFFER {           //OMX_EMPTYTHISBUFFER
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    HANDLE_T                    hPBufHeader;
    HANDLE_T                    hPBufHeaderBuf;
} PARAMETER_OMX_EMPTYTHISBUFFER;

typedef struct _PARAMETER_OMX_FILLTHISBUFFER {           //OMX_FILLTHISBUFFER
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    HANDLE_T                    hPBufHeader;
    HANDLE_T                    hPBufHeaderBuf;
} PARAMETER_OMX_FILLTHISBUFFER;

typedef struct _PARAMETER_OMX_GETSTATE {           //OMX_GETSTATE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_STATETYPE               * pState;
} PARAMETER_OMX_GETSTATE;

typedef struct _PARAMETER_OMX_PORT_PARAM_TYPE {           //OMX_PARAM_IndexParamAudioInit, OMX_PARAM_IndexParamVideoInit
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_PORT_PARAM_TYPE         * pPortParamType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_PORT_PARAM_TYPE;

typedef struct _PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE {           //OMX_PARAM_IndexParamPortDefinition
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_PARAM_PORTDEFINITIONTYPE    * pPortDefinitionType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_PARAM_PORTDEFINITIONTYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE {           //OMX_PARAM_IndexParamAudioPcm
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_AUDIO_PARAM_PCMMODETYPE * pPcmModeType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_AUDIO_PARAM_PCMMODETYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE {           //OMX_PARAM_IndexParamAudioAac
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_AUDIO_PARAM_AACPROFILETYPE  * pAacProfFileType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_AUDIO_PARAM_AACPROFILETYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_MP3TYPE {                 //OMX_PARAM_IndexParamAudioMp3
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_AUDIO_PARAM_MP3TYPE         * pMP3Type;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_AUDIO_PARAM_MP3TYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_WMATYPE {                 //OMX_PARAM_IndexParamAudioWma
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_AUDIO_PARAM_WMATYPE         * pWMAType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_AUDIO_PARAM_WMATYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE {                 //OMX_PARAM_IndexParamAudioWma
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_AUDIO_PARAM_WMAPROTYPE      * pWMAPROType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_AUDIO_PARAM_WMAPROTYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_RATYPE {                 //OMX_PARAM_IndexParamAudioRA
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_AUDIO_PARAM_RATYPE         * pRAType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_AUDIO_PARAM_RATYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE {           //OMX_PARAM_IndexParamAudioAdpcm
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_AUDIO_PARAM_ADPCMTYPE * pADPCMType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_AUDIO_PARAM_ADPCMTYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE {           //OMX_PARAM_IndexParamAudioVorbis
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_AUDIO_PARAM_VORBISTYPE * pVORBISType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_AUDIO_PARAM_VORBISTYPE;

typedef struct _PARAMETER_OMX_AUDIO_PARAM_AMRTYPE {           //OMX_PARAM_IndexParamAudioAMR
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_AUDIO_PARAM_AMRTYPE *   pAMRType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_AUDIO_PARAM_AMRTYPE;

typedef struct _PARAMETER_OMX_AUDIO_CONFIG_VOLUMETYPE {           //OMX_PARAM_IndexConfigAudioVolume
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_AUDIO_CONFIG_VOLUMETYPE * pVolumeType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_AUDIO_CONFIG_VOLUMETYPE;

typedef struct _PARAMETER_OMX_TIME_CONFIG_SCALETYPE {           //OMX_PARAM_IndexConfigTimeScale
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_TIME_CONFIG_SCALETYPE   * pScaleType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_TIME_CONFIG_SCALETYPE;

typedef struct _PARAMETER_OMX_TIME_CONFIG_CLOCKSTATETYPE {           //OMX_PARAM_IndexConfigTimeClockState
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_TIME_CONFIG_CLOCKSTATETYPE  * pClockStateType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_TIME_CONFIG_CLOCKSTATETYPE;

typedef struct _PARAMETER_OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE {           //OMX_PARAM_IndexConfigTimeActiveRefClock
    OMX_ERRORTYPE                       eReturnValue;
    OMX_HANDLETYPE                      pHandle;
    OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE  * pActiveRefClockType;
    OMX_PARAM_DIR                       eDir;
} PARAMETER_OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE;

typedef struct _PARAMETER_OMX_TIME_CONFIG_TIMESTAMPTYPE {           //OMX_PARAM_IndexConfigTimeCurrentMediaTime
    OMX_ERRORTYPE                   eReturnValue;
    OMX_HANDLETYPE                  pHandle;
    OMX_TIME_CONFIG_TIMESTAMPTYPE   * ptimeStampType;
    OMX_PARAM_DIR                   eDir;
} PARAMETER_OMX_TIME_CONFIG_TIMESTAMPTYPE;

typedef struct _PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE {           //OMX_PARAM_IndexParamVideoProfileLevelQuerySupported
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_VIDEO_PARAM_PROFILELEVELTYPE * pVideoParamPro;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_VIDEO_PARAM_PROFILELEVELTYPE;

typedef struct _PARAMETER_OMX_VIDEO_PARAM_AVC {           //OMX_PARAM_IndexParamVideoAvc
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_VIDEO_PARAM_AVCTYPE * pVideoParamAvc;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_VIDEO_PARAM_AVC;

typedef struct _PARAMETER_OMX_VIDEO_PARAM_RV {           //OMX_PARAM_IndexParamVideoRv
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_VIDEO_PARAM_RVTYPE * pVideoParamRv;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_VIDEO_PARAM_RV;

typedef struct _PARAMETER_OMX_VIDEO_PARAM_WMV {           //OMX_PARAM_IndexParamVideoWmv
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_VIDEO_PARAM_WMVTYPE * pVideoParamWmv;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_VIDEO_PARAM_WMV;

typedef struct _PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE { //OMX_VIDEO_PARAM_PORTFORMATTYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_VIDEO_PARAM_PORTFORMATTYPE * pVideoPortFmt;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_VIDEO_PARAM_PORTFORMATTYPE;

typedef struct _PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE { //OMX_SEAMLESSFORMATTYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_SEAMLESSFORMATTYPE * pVideoSeamlessFmt;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_VIDEO_PARAM_SEAMLESSFORMATTYPE;

typedef struct _PARAMETER_OMX_COMMAND_STATESET {           //OMX_COMMAND_CommandStateSet
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_STATETYPE               eStateType;
} PARAMETER_OMX_COMMAND_STATESET;

typedef struct _PARAMETER_OMX_COMMAND_PORT {           //OMX_COMMAND_CommandFlush, OMX_COMMAND_CommandPortDisable, OMX_COMMAND_CommandPortEnable
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_U32                     nPorts;
} PARAMETER_OMX_COMMAND_PORT;

typedef struct _PARAMETER_OMX_CB_EVENTHANDLER {     //callback information structure in event handler callback function
    OMX_EVENTTYPE eEvent;
    OMX_U32 nData1;
    OMX_U32 nData2;
    OMX_PTR pEventData;
} PARAMETER_OMX_CB_EVENTHANDLER;

typedef struct _PARAMETER_OMX_CB_BUFFER {     //callback information structure in EmptyBufferDone and FillBufferDone callback function
    HANDLE_T        hPBufHeader;
    OMX_PTR         pPlatformPrivate;
} PARAMETER_OMX_CB_BUFFER;

//DANIEL_HACK
typedef struct
{
    PARAMETER_OMX_CB_EVENTHANDLER u4Info;
    unsigned int u4Tag;
} DEC_OMX_CB_EVENT_T;

typedef struct
{
    PARAMETER_OMX_CB_BUFFER u4Info;
    unsigned int u4Tag;
} DEC_OMX_CB_BUF_T;
//DANIEL_HACK

typedef struct _PARAMETER_OMX_IMAGE_MODE { //OMX_IMAGE_MODE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_IMAGE_MODE * pImageMode;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_IMAGE_MODE;

typedef struct _PARAMETER_OMX_IMAGE_PROCESSING { //OMX_IMAGE_PROCESSING
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_IMAGE_PROCESSING * pImageProcessing;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_IMAGE_PROCESSING;

typedef struct _PARAMETER_OMX_FRAMESIZETYPE { //OMX_FRAMESIZETYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_FRAMESIZETYPE * pFrameSizeType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_FRAMESIZETYPE;

typedef struct _PARAMETER_OMX_CONFIG_ROTATIONTYPE { //OMX_CONFIG_ROTATIONTYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_CONFIG_ROTATIONTYPE * pRotationType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_CONFIG_ROTATIONTYPE;

typedef struct _PARAMETER_OMX_CONFIG_SCALEFACTORTYPE { //OMX_CONFIG_SCALEFACTORTYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_CONFIG_SCALEFACTORTYPE * pScaleFactorType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_CONFIG_SCALEFACTORTYPE;

typedef struct _PARAMETER_OMX_CONFIG_POINTTYPE { //OMX_CONFIG_POINTTYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_CONFIG_POINTTYPE * pPointType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_CONFIG_POINTTYPE;

typedef struct _PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER { //OMX_BUFFERHEADERTYPE
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_BUFFERHEADERTYPE * pBufferHeaderType;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_PARAM_IMAGE_FREEBUFFER;

typedef struct _PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED { //OMX_IMAGE_USER_DEFINED
    OMX_ERRORTYPE               eReturnValue;
    OMX_HANDLETYPE              pHandle;
    OMX_IMAGE_USER_DEFINED * pImageUserDefined;
    OMX_PARAM_DIR               eDir;
} PARAMETER_OMX_PARAM_IMAGE_USER_DEFINED;

#endif      //OPENMAXIL_IOCTL_H
