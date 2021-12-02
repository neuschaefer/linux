#ifdef LINUX_TURNKEY_SOLUTION

#include "wmalslif.h"
#ifdef CC_AUD_RISC_WMALSL_SUPPORT
#include "wmaconfig.h"
#include "wmaudio_type.h"
#include "asfparser.h"
#include "msaudiodec.h"
#include "macros.h"
#include "wmaudio.h"
#include "wmamalloc.h"
#endif
//=====================================
#include "aud_if.h" //AUD_OK
#include "drv_common.h"
//=====================================
#include "aud_debug.h" //LOG //this monkey define BYTE
#include "x_assert.h"

#ifdef CC_AUD_RISC_WMALSL_SUPPORT
#define CALLBACK_BUFFERSIZE WMA_MAX_DATA_REQUESTED*2
#define MAX_SAMPLES 16384
WMA_U16 g_pLeft [MAX_SAMPLES * 2]; // expand the container for 24-bit samples
//WMA_U16 g_pRight [MAX_SAMPLES * 2]; // expand the container for 24-bit samples
//WMA_U16 g_pout [MAX_SAMPLES * 2];

tWMAFileStateInternal g_state;             //WMA decoder structure
CAudioObjectDecoder   h_WMA;
CAudioObject h_AU; //a.k.a. pau
WMAPlayerInfo _PlayerInfo;
tWMAFileHeader g_hdr;  //ian FIXME this should be removed
#endif
static void vAudWMALSLThread(void* pvArg) //vAudOmxThread
{
    INT32 i4Ret = 0;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize;
    AUD_DEC_MSG_T rMsg;
    UINT32 u4OutNum;

    AUD_DEC_HANDLE_T *pHandle = *((AUD_DEC_HANDLE_T **)pvArg);
    UNUSED(u4OutNum);
    LOG(1,"[AUD][WMALSL] vAudWMALSLThread: pHandle = 0x%X.\n", pHandle);

    while (TRUE)
    {
        if ((pHandle->u4decstate == AUD_DEC_STATE_INIT) ||
            (pHandle->u4decstate == AUD_DEC_STATE_IDLE))
        {
           i4Ret = x_msg_q_receive_timeout(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, 10);
        }
        else
        {
           i4Ret = x_msg_q_receive(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, X_MSGQ_OPTION_NOWAIT);
        }
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_NO_MSG)|| (i4Ret == OSR_TIMEOUT));
       
        if (OSR_OK == i4Ret)
        {
/*
            if (AUD_DEC_CMD_TERMINATE == rMsg.eCmd)
            {
                pHandle->u4decstate = AUD_DEC_STATE_STOP;
                // exiting
                break;
            }
*/
            switch (rMsg.eCmd)
            {
            case AUD_DEC_CMD_SET_STATE:
                if((pHandle->u4decstate == AUD_DEC_STATE_INIT)&&
                        (rMsg.eStateTo == AUD_DEC_STATE_PLAY))
                {
                    if (pHandle->eFormat == AUD_FMT_WMA10LOSSLESS)
                    {
#ifdef CC_AUD_RISC_WMALSL_SUPPORT
                        u4DSP_WMALSLinit(pHandle);  //ian wmalsl don't need data while init
#endif
                    }
                    else if (pHandle->eFormat == AUD_FMT_WMA10SPEECH)
                    {
                        //todo: add WMA Voice initialization
                    }
                }
                pHandle->u4decstate = rMsg.eStateTo;
                break; //break out the switch
            default:
                LOG(1,"[AUD][WMALSL] UNKNOWN COMMAND!!");
                break; //break out the switch
            }
        }

        if((pHandle->u4decstate == AUD_DEC_STATE_PLAY) ||
            (pHandle->u4decstate == AUD_DEC_STATE_IDLE))
        {
             if (pHandle->eFormat == AUD_FMT_WMA10LOSSLESS)
             {
#ifdef CC_AUD_RISC_WMALSL_SUPPORT
                    i4Ret = u4DSP_WMALSLdecode(pHandle,&u4OutNum);
                    if(i4Ret == cWMA_NoMoreFrames){ //
                            pHandle->u4decstate = AUD_DEC_STATE_STOP;
                    }
                    else if (i4Ret == cWMA_ChkFrmSizeFail){ //
                            pHandle->u4decstate = AUD_DEC_STATE_IDLE;
                    }
                    else {
                            pHandle->u4decstate = AUD_DEC_STATE_PLAY;
                    }
#endif
             }
             else
             {
                    //todo : Add WMA Voice decoding and state change
             }

        }
        if (pHandle->u4decstate == AUD_DEC_STATE_STOP){
            break;
        }
        //x_thread_delay(10);
    }

    VERIFY(pHandle->u4decstate == AUD_DEC_STATE_STOP);

    // delete message queue
    x_msg_q_delete(pHandle->hCommandQ);

    LOG(0,"[AUD][WMALSL] vAudWMALSLThread: pHandle = 0x%X. Task terminated.\n", pHandle);

    x_thread_exit();
}


INT32 i4AudWMALSLTaskCreate(AUD_DEC_HANDLE_T *pHandle)
{
    pHandle->u4decstate = AUD_DEC_STATE_INIT;
    pHandle->i4decerrcode = 0;
    pHandle->u4frmcnt = 0;
    pHandle->u4errfrmcnt = 0;
    pHandle->u4InBufSize = pHandle->pInBufENDBLK-pHandle->pInBufSTRBLK;
    pHandle->pInBufPRSPNT = pHandle->pInBufSTRBLK;
    pHandle->pInBufBUFPNT = pHandle->pInBufSTRBLK;
    pHandle->pInBufCURPNT = pHandle->pInBufSTRBLK;


        LOG(1,"[AUD][DEC] i4AudWMALSLTaskCreate = inbuf: 0x%X. 0x%X. 0x%X.\n", 
                pHandle->pInBufSTRBLK,pHandle->pInBufENDBLK,pHandle->u4InBufSize);
        LOG(1,"[AUD][DEC] i4AudWMALSLTaskCreate = outbuf: 0x%X. 0x%X. 0x%X.\n", 
                pHandle->pOutBufSTRBLK,pHandle->pOutBufSTRBLK+pHandle->u4InBufSize,pHandle->u4InBufSize);



    // create command queue
    x_msg_q_create(&(pHandle->hCommandQ), "AudWMALSLCmdQ", sizeof(AUD_DEC_MSG_T), 8);

    LOG(1,"[AUD][DEC] i4AudWMALSLTaskCreate: pHandle = 0x%X.\n", pHandle);

    // create audio risc decoder main thread
    x_thread_create(&(pHandle->hMainTask), "AudWMALSLTask",
                    AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                    vAudWMALSLThread, sizeof(void *), &pHandle);

    return AUD_OK;
}

INT32 i4AudWMALSLTaskSendCommand(AUD_DEC_HANDLE_T *pHandle, AUD_DEC_MSG_T *prMsg)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != pHandle->hCommandQ)
    {
        i4Ret = x_msg_q_send(pHandle->hCommandQ, prMsg, sizeof(AUD_DEC_MSG_T), 0/*not used*/);
    }

    return i4Ret;
}

INT32 i4AudWMALSLTaskDestroy(AUD_DEC_HANDLE_T *pHandle)
{
    AUD_DEC_MSG_T rMsgTerminate;

    rMsgTerminate.eCmd = AUD_DEC_CMD_TERMINATE;
    i4AudWMALSLTaskSendCommand(pHandle, &rMsgTerminate);

    return AUD_OK;
}
#ifdef CC_AUD_RISC_WMALSL_SUPPORT
INT32 u4DSP_WMALSLinit(AUD_DEC_HANDLE_T * pHandle){
        WMAERR u4ret = WMAERR_OK;
        u4ret = u4DSP_WMAFileDecodeInitEx(pHandle);
        if(u4ret != WMAERR_OK){
                LOG(1,"u4DSP_WMAFileDecodeInitEx ERROR: %x\n",u4ret);
                return u4ret;
        }

        u4ret = WMAFileIsLosslessWMA (&g_state);
        if(u4ret != WMAERR_OK){
                LOG(1,"WMAFileIsLosslessWMA ERROR: %x\n",u4ret);
                return u4ret;
        }

        u4ret = WMAFileDecodeInfo (&g_state, &g_hdr);
        if(u4ret != WMAERR_OK){
                LOG(1,"WMAFileDecodeInfo ERROR: %x\n",u4ret);
                return u4ret;
        }
        
        return u4ret;
}


//WMAFileDecodeData
INT32 u4DSP_WMALSLdecode(AUD_DEC_HANDLE_T * pHandle, UINT32* u4outlen)
{
        WMA_U32 nDecodedSamples;
        WMA_U32 cFetchedSamples;
        tWMAFileStatus rc;
        WMA_U16* pTempLeft;
        static WMA_U32 u4OutNum=0;

//        LOG(1,"=================u4DSP_WMALSLdecode====================\n");


#if 1
    if (u4OutNum ==0)
    {
        nDecodedSamples = 0;
        while (nDecodedSamples ==0)
        {
        rc = WMAFileDecodeData(&g_state, &nDecodedSamples);
            if (rc == cWMA_ChkFrmSizeFail)
                return rc;
        }
        if ( rc != cWMA_NoErr ){
                LOG(1,"DECODING ERR: decoding failed %x##%d\n",rc,nDecodedSamples);
        }
        
        cFetchedSamples = 0;
        pTempLeft = g_pLeft;
        if( 0 < nDecodedSamples ) {
                //LOG(1,"DECODING OUTPUT: %d\n",nDecodedSamples);
                do{ 
                        u4OutNum = WMAFileGetPCM ((void*)&g_state, pTempLeft, NULL, 
                        sizeof(g_pLeft), nDecodedSamples - cFetchedSamples);
                        cFetchedSamples += u4OutNum;
                        pTempLeft += (u4OutNum * g_state.hdr_parse.nChannels);
                } while(cFetchedSamples <  nDecodedSamples);

        }
        *u4outlen = nDecodedSamples;
        

        //u4AudOmxFillFIFOWMA2PCM
        u4OutNum = nDecodedSamples*2*g_state.hdr_parse.nChannels;
     }
/*     
            for (i=0;i<128;i++)
            {
                if (i%4 ==0)
                    LOG(1,"\n%04x\t%08x ",i*4,g_pLeft[i]);
                else
                    LOG(1,"%08x ",g_pLeft[i]);

            }
*/            
           u4OutNum -= pHandle->pu4AudDecFillFIFO(pHandle, (UINT32)g_pLeft, u4OutNum);
        if (u4OutNum != 0)
        {
           LOG(7, "u4OutNum FAILED\n");
           return cWMA_ChkFrmSizeFail;

        } 
#else
        if (u4OutNum == 0)
        {
            LOG(1," b4 ChkFrmSz : buf 0x%X. RP 0x%X. WP 0x%X.\n", 
                        (WMA_U32) pHandle->pInBufBUFPNT,
                        (WMA_U32) pHandle->pInBufCURPNT,
                        (WMA_U32) pHandle->pInBufPRSPNT);

            if (!u4DSP_DecChkFrmSz(
                                        pHandle,
                                        pHandle->pInBufCURPNT, 
                                        0x1000))
           {
                return cWMA_NoMoreDataThisTime;
        }
            u4DSP_GetBits(pHandle, 0x1000, g_pLeft);

                vDSP_DecMovFIFOPnt(
                        (WMA_U32) pHandle->pInBufSTRBLK, 
                        (WMA_U32) pHandle->pInBufENDBLK, 
                        (WMA_U32 **)&(pHandle->pInBufBUFPNT),
                        (WMA_I32)0x1000);
                u4OutNum = 0x1000;                
         }                 
        u4OutNum -= pHandle->pu4AudDecFillFIFO(pHandle, (UINT32)g_pLeft, u4OutNum);
        if (u4OutNum != 0)
           LOG(1, "u4OutNum FAILED\n");

#endif



#if 0


   do{
        rc = WMAFileDecodeData(&g_state, &nDecodedSamples);

        if(rc != cWMA_NoErr)
        {
            //wchen: I don't understand why failed is normal
            if ( rc == cWMA_NoMoreFrames || rc == cWMA_Failed )
            {
            }else {
                LOG(1,"DECODING ERR: decoding failed %x##%d\n",rc,nDecodedSamples);
//                return 0;
            }            
        }
    }while(nDecodedSamples==0);
        cFetchedSamples = 0;

        if( 0 < nDecodedSamples ) {
                //output here
        }                        
        LOG(1,"=======u4DSP_WMALSLdecode %x##%d======\n",rc,nDecodedSamples);

        for(i=0;i<(2048/5);i++){
                LOG(1," 0x%08x # 0x%08x # 0x%08x # 0x%08x # 0x%08x \n",  
                h_AU.m_rgpcinfo[0].m_rgiCoefRecon[i],
                h_AU.m_rgpcinfo[0].m_rgiCoefRecon[i+1],
                h_AU.m_rgpcinfo[0].m_rgiCoefRecon[i+2],
                h_AU.m_rgpcinfo[0].m_rgiCoefRecon[i+3],
                h_AU.m_rgpcinfo[0].m_rgiCoefRecon[i+4]
                );
        }
#endif
//        LOG(1,"u4DSP_WMALSLdecode_end : %d#%d\n",u4inbufaddr,*u4inbuflen);
        return rc;
}

//WMAFileDecodeInitEx 
//ian: whis function will not consume any AFIFO bits
INT32 u4DSP_WMAFileDecodeInitEx(AUD_DEC_HANDLE_T* pHandle){
        WMAERR u4ret = WMAERR_OK;
        WMAPlayerInfo PlayerInfo;
        tWMA_U16 nPlayerOpt = 0;
        WMAFormat WMAFormat;
        PCMFormat  PCMFormat;
        WMA_I32 *rgiMixDownMatrix = NULL; // Can be null to invoke defaults
        WMA_I32 iPeakAmplitudeRef = 0;
        WMA_I32 iRmsAmplitudeRef = 0;
        WMA_I32 iPeakAmplitudeTarget = 0;
        WMA_I32 iRmsAmplitudeTarget = 0;
        WMARESULT wmar = cWMA_NoErr;
        AUD_WMA_CFG_T *pWMAInfo = (AUD_WMA_CFG_T *)(pHandle->pDecInfo);
        //tWMAFileHdrStateInternal g_hdrstate;
/**************************************************************
* Initial Global Variable
**************************************************************/
        memset(&g_state, 0, sizeof(tWMAFileStateInternal));
        //memset(&g_hdrstate, 0, sizeof(tWMAFileHdrStateInternal));
        memset(&WMAFormat, 0, sizeof(WMAFormat));
        memset(&PCMFormat, 0, sizeof(PCMFormat));
        memset(&_PlayerInfo,0,sizeof(WMAPlayerInfo));
        //memset(&h_WMA, 0, sizeof(CAudioObjectDecoder));

/**************************************************************
* Obtain Buffer Pointer
**************************************************************/

/**************************************************************
* ASF parser Initialization   
* Cmodel: WMA_ParseAsfHeader(&pInt->hdr_parse, 1);
**************************************************************/
//        vDSP_WMALSLasfinit();
#if 0
        g_hdrstate.currPacketOffset = 0;     /* not just for packets */
        g_hdrstate.cbHeader = 0;

        g_hdrstate.cbPacketSize = 13406;
        g_hdrstate.cbAudioSize = 0;
        g_hdrstate.cPackets = 15;
        g_hdrstate.msPreroll = 3000;
        g_hdrstate.dwFilePropertiesFlags = 2;
        g_hdrstate.nVersion = 3;
        g_hdrstate.wFormatTag = 355;
        g_hdrstate.nSamplesPerSec = 44100;
        g_hdrstate.nAvgBytesPerSec = 144000;
        g_hdrstate.nBlockAlign = 13375;
        g_hdrstate.nChannels = 2;
        g_hdrstate.nSamplesPerBlock = 0;
        g_hdrstate.nEncodeOpt = 417;
        g_hdrstate.nAdvancedEncodeOpt = 0;
        g_hdrstate.nAdvancedEncodeOpt2 = 0;
        g_hdrstate.wBitsPerSample = 24; // container size
        g_hdrstate.wValidBitsPerSample = 24; // actual valid data depth in the decoded stream
        g_hdrstate.dwChannelMask = 3;
        g_hdrstate.wOriginalBitDepth = 0; // actual valid data depth in the original source (informational)
        g_hdrstate.wAudioStreamId = 1;
        g_hdrstate.wTargetAudioStreamNumber = 1;
        g_hdrstate.cAudioStreams = 1;
        g_hdrstate.u4currentpktcnt = 0;
//#else
        g_hdrstate.currPacketOffset = 0;     /* not just for packets */
        //g_hdrstate.cbHeader = 0;

        g_hdrstate.cbPacketSize = 0x343f;//0x345e;
        //g_hdrstate.cbAudioSize = 0;
        g_hdrstate.cPackets = 0x1cb;
        //g_hdrstate.msPreroll = 0xbb8;
        //g_hdrstate.dwFilePropertiesFlags = 2;
        //g_hdrstate.nVersion = 3;
        g_hdrstate.wFormatTag = 355;
        g_hdrstate.nSamplesPerSec = 0xac44;
        g_hdrstate.nAvgBytesPerSec = 0x1c037;
        g_hdrstate.nBlockAlign = 0x343f;
        g_hdrstate.nChannels = 2;
        g_hdrstate.nSamplesPerBlock = 0;
        g_hdrstate.nEncodeOpt = 0x21;
        g_hdrstate.nAdvancedEncodeOpt = 0;
        g_hdrstate.nAdvancedEncodeOpt2 = 0;
        g_hdrstate.wBitsPerSample = 0x10; // container size
        g_hdrstate.wValidBitsPerSample = 0x10; // actual valid data depth in the decoded stream
        g_hdrstate.dwChannelMask = 3;
        //g_hdrstate.wOriginalBitDepth = 0; // actual valid data depth in the original source (informational)
        g_hdrstate.wAudioStreamId = 1;
        g_hdrstate.wTargetAudioStreamNumber = 1;
        g_hdrstate.cAudioStreams = 1;
        g_hdrstate.u4currentpktcnt = 0;

        //Ian FIXME g_hdrstate should be removed
        memcpy(&g_state.hdr_parse, &g_hdrstate, sizeof(tWMAFileHdrStateInternal));  
        
#else
        //get ASF setting
        g_state.hdr_parse.wFormatTag = 355;
        g_state.hdr_parse.cbPacketSize = pWMAInfo->u4Blocksz;
        g_state.hdr_parse.nSamplesPerSec = pWMAInfo->u4Sampersec;
        g_state.hdr_parse.nAvgBytesPerSec = pWMAInfo->u4Bpersec;
        g_state.hdr_parse.nBlockAlign = pWMAInfo->u4Blocksz;
        g_state.hdr_parse.nChannels = pWMAInfo->u2Numch;
        g_state.hdr_parse.nEncodeOpt = pWMAInfo->u2Encoderopt;
        g_state.hdr_parse.nAdvancedEncodeOpt = pWMAInfo->u2AdvEncoderopt;
        g_state.hdr_parse.nAdvancedEncodeOpt2 = pWMAInfo->u4AdvEncoderopt2;
        g_state.hdr_parse.wBitsPerSample = pWMAInfo->u2BitsPerSample;// container size
        g_state.hdr_parse.wValidBitsPerSample = pWMAInfo->u2BitsPerSample; // actual valid data depth in the decoded stream
        g_state.hdr_parse.dwChannelMask = pWMAInfo->u4ChannelMask;
        g_state.hdr_parse.wAudioStreamId = 1;
        g_state.hdr_parse.wTargetAudioStreamNumber = 1;
        g_state.hdr_parse.cAudioStreams = 1;
/*        
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u4Blocksz);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u4Sampersec);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u4Bpersec);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u2Numch);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u2Encoderopt);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u4AdvEncoderopt2);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u2AdvEncoderopt);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u2BitsPerSample);
        LOG(1,"u4DSP_WMAFileDecodeInitEx %d\n",pWMAInfo->u4ChannelMask);
*/        
#endif
        g_state.hdr_parse.pHandle = pHandle;
        
        //LOG(1,"vDSP_WMALSLasfinit : %d#%d\n",g_hdrstate.cbPacketSize,g_hdrstate.nSamplesPerSec);
/**************************************************************
* WMA decoder initialization
* Cmodel: pInt->hWMA = audecNew(NULL,0);
* initial CAudioObjectDecoder
**************************************************************/
        g_state.hWMA = (&h_WMA); //ian FIXME should access global variable directly
        u4ret = prvWipeCleanDecoder(&h_WMA);
        if(u4ret != WMA_OK){
                u4ret = cWMA_Failed;
         }

        //PlayerInfo initialize
        memset(&PlayerInfo, 0, sizeof(PlayerInfo));
        PlayerInfo.nPlayerOpt = nPlayerOpt;
        WMAFormat.wFormatTag = (WMA_U16) g_state.hdr_parse.wFormatTag;           
        WMAFormat.nChannels = (WMA_U16) g_state.hdr_parse.nChannels;          
        WMAFormat.nSamplesPerSec = (WMA_U32) g_state.hdr_parse.nSamplesPerSec;     
        WMAFormat.nAvgBytesPerSec = (WMA_U32) g_state.hdr_parse.nAvgBytesPerSec;    
        WMAFormat.nBlockAlign = (WMA_U16) g_state.hdr_parse.nBlockAlign;        
        WMAFormat.nValidBitsPerSample  = (WMA_U16) g_state.hdr_parse.wValidBitsPerSample;
        WMAFormat.nChannelMask = (WMA_U32) g_state.hdr_parse.dwChannelMask;      
        WMAFormat.wEncodeOpt = (WMA_U16) g_state.hdr_parse.nEncodeOpt;         
        WMAFormat.wAdvancedEncodeOpt = (WMA_U16) g_state.hdr_parse.nAdvancedEncodeOpt;
        WMAFormat.dwAdvancedEncodeOpt2 = (WMA_U32) g_state.hdr_parse.nAdvancedEncodeOpt2;
        PCMFormat.cbPCMContainerSize = (WMA_U32) g_state.hdr_parse.wBitsPerSample/8;   
        PCMFormat.nSamplesPerSec = (WMA_U32) g_state.hdr_parse.nSamplesPerSec;     
        PCMFormat.nValidBitsPerSample = (WMA_U16) g_state.hdr_parse.wValidBitsPerSample;

        WMAFormat2PCMFormat(&WMAFormat, &PCMFormat);

#ifdef CC_MTK_PLATFORM
    //ian fix 24 bits output    
        PCMFormat.nValidBitsPerSample = 16;
        PCMFormat.cbPCMContainerSize = 2;
#else  //CC_MTK_PLATFORM

#ifdef WMA_16BITS_OUTPUT_SUPPORT
    if (nDecoderFlags & DECOPT_REQUANTTO16) 
    {
        PCMFormat.nValidBitsPerSample = 16;
        PCMFormat.cbPCMContainerSize = 2;
    }
    else 
#endif
    if(nDecoderFlags & DECOPT_PCM24_OUT)
    {
        // valid bits dictated by wma format
        PCMFormat.cbPCMContainerSize = 3;
    }
    else if (nDecoderFlags & DECOPT_PCM32_OUT)
    {
        // valid bits dictated by wma format
        PCMFormat.cbPCMContainerSize = 4;
    }
#endif //CC_MTK_PLATFORM
    PlayerInfo.nDRCSetting = WMA_DRC_HIGH;
    // PlayerInfo needs lot more stuff to be filled: TODO.
    PlayerInfo.iPeakAmplitudeRef = iPeakAmplitudeRef;
    PlayerInfo.iRmsAmplitudeRef = iRmsAmplitudeRef;
    PlayerInfo.iPeakAmplitudeTarget = iPeakAmplitudeTarget;
    PlayerInfo.iRmsAmplitudeTarget = iRmsAmplitudeTarget;
    PlayerInfo.rgiMixDownMatrix = rgiMixDownMatrix;

    wmar = audecInit(&h_WMA, &WMAFormat, &PCMFormat, &PlayerInfo, &g_state.audecState);
    if( WMA_FAILED( wmar ) ) return wmar;

    if (wmar != WMA_OK)
        return cWMA_Failed;

    g_state.pPlayerInfo = &_PlayerInfo;
    memcpy(g_state.pPlayerInfo, &PlayerInfo, sizeof(WMAPlayerInfo));

    /* Set up the decryption if necessary */
    g_state.bHasDRM = (BOOL) 0;
    if(g_state.hdr_parse.cbSecretData > 0)
    {
        /* only for DRM now */
        char *p = (char *)g_state.hdr_parse.pbType;
        if(p[0] == 'D' && p[1] == 'R' && p[2] == 'M' && p[3] == '\0')
        {
            g_state.bHasDRM = (BOOL)( 1 );
        }
        else
            return cWMA_BadDRMType;
    }

    g_state.hdr_parse.u4totalpktcnt = g_state.hdr_parse.cPackets;

    g_state.parse_state = csWMA_NewAsfPacket;
    g_state.pCallBackBuffer = (WMA_U8 *)auMalloc(CALLBACK_BUFFERSIZE);
    if(NULL == g_state.pCallBackBuffer)
        return cWMA_Internal;

     return cWMA_NoErr;
}
#endif

#endif
