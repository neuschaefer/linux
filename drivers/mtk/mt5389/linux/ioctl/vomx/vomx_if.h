


#ifndef VOMX_IF_H
#define VOMX_IF_H

#define VCONV_THREAD_NAME			"VCONV Thread"
#define VCONV_STACK_SIZE		    2048
#define VCONV_THREAD_PRIORITY		60//PRIORITY(PRIORITY_CLASS_NORMAL, PRIORITY_LAYER_DRIVER, 0)

#define VDOOMX_THREAD_NAME			"VDOOmx Thread"
#define VDOOMX_STACK_SIZE		    2048
#define VDOOMX_THREAD_PRIORITY		60//PRIORITY(PRIORITY_CLASS_NORMAL, PRIORITY_LAYER_DRIVER, 0)

#define S_FBM_OK 0

#define DTV_VOMX 1
#define GST_OPENMAX 1


typedef enum
{
  VC_UNKNOW = 0,       ///< unknow type, used for debug
  VC_MPEG2,                ///< mpeg 1/2
  VC_MPEG4,                ///< mpeg 4
  VC_DIVX311,             ///< Divx 3.11
  VC_DIVX4,                  ///< Divx 4
  VC_DIVX6,                  ///< Divx 5/6
  VC_WMV1,                    ///< WMV7
  VC_WMV2,                    ///< WMV8
  VC_WMV3,                    ///< WMV9
  VC_VC1,                    ///< VC1
  VC_H263,                    ///< H.263
  #if 1 //DRV_SUPPORT_SORENSON_H263
  VC_H263_SORENSON,		      ///< H.263 Sorenson version
  #endif
  VC_H264,                    ///< H.264
  VC_RV30,                    ///< Real Video 8
  VC_RV40,                    ///< Real Video 9,10
  VC_MJPG,                    ///<motion jpeg
  VC_VP6,                    ///VP6
  VC_VP8,                    ///VP6
  VC_AVS,                    ///VP6
  VC_JPG,                    ///<jpeg
  VC_PNG  
}VCodeC;

typedef enum 
{
  P_MAIN = 0,                ///< Main Path
  P_SUB,                        ///< Sub Path
} PATH_TYPE;

typedef enum 
{
  FBM_PREPARE_SWAP = 0,            ///< notify prepare to swap frame buffer, pvArg = point of struct FBM_FBInfo
  FBM_FINISH_SWAP,                     ///< notify finish to swap frame buffer, pvArg = point of struct FBM_FBInfo
  FBM_FB_AVAIL,                            ///< Presentation unit (frame buffer) available for get, pvArg = NULL
  FBM_QUEUE_AVAIL,                     ///< Output queue available for insert presentation unit (frame buffer), pvArg = NULL
  FBM_CC_AVAIL,                            ///< Closed Caption data available for get, pvArg = NULL
  FBM_CCQUEUE_AVAIL,                  ///< Output queue available for insert Closed Caption data, pvArg = NULL
  FBM_CAPTURE_DONE,                   ///< capture frame buffer done, pvArg = point of struct FBM_FBInfo
  FBM_DSFB_AVAIL,                     ///< down scale frame buffer available for alloc, pvArg = NULL
} FBM_CBID;

typedef enum
{
    VOMX_IO_SET_OUTPUT_DELAY = 0,
    VOMX_IO_MAX
} VOMX_IO_TYPE;

#endif // VOMX_IF_H

