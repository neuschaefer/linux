#ifdef LINUX_TURNKEY_SOLUTION

//=====================================
#include "aud_if.h" //AUD_OK
#include "drv_common.h"
//=====================================
#include "aud_debug.h" //LOG //this monkey define BYTE
//=====================================
#include "awb_exp.h"
#include "typedef.h"
#include "awbif.h"
#include "riscdec.h"
#include "x_hal_5381.h"

//#define TURN_ON_DBG_MSG
#ifdef  TURN_ON_DBG_MSG
#define     AWB_DBG_MSG     Printf
#else
#define     AWB_DBG_MSG(fmt...)
#endif

/* frame size in serial bitstream file (frame type + serial stream + flags) */
#define SERIAL_FRAMESIZE    (1+MAX_SERIAL_SIZE+5)
#define AMRWB_OUTPCM_SMP_NUM    320

static AWB_DEC_CTRL_T    awb_dec_ctrol;

void*  awbMalloc(const size_t iSize)
{
    return (void *)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(iSize, 8));
}

static void vAudAWBThread(void* pvArg) //vAudAMRThread
{
    INT32 i4Ret = 0;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize;
    AUD_DEC_MSG_T rMsg;
    AWBDEC_STATE AWBDecRet;
    UINT32 u4OutNum=0, u4CopiedPCM=0, u4RemainPCM=0;

    AUD_DEC_HANDLE_T *pHandle = *((AUD_DEC_HANDLE_T **)pvArg);

    LOG(1,"[AUD][AWB] vAudAWBThread: pHandle = 0x%X.\n", pHandle);

    do
    {   // waiting for omx send start decoding message
        i4Ret = x_msg_q_receive_timeout(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, 10);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_NO_MSG)|| (i4Ret == OSR_TIMEOUT));
    }while(i4Ret != OSR_OK);

    AWB_DBG_MSG("[AWB] decoder thread starts!\n");

    while (TRUE)
    {
        if (OSR_OK == i4Ret)    // new message received
        {
            AWB_DBG_MSG("[AWB] msg cmd %d\n", rMsg.eCmd);
            switch (rMsg.eCmd)
            {
            case AUD_DEC_CMD_SET_STATE:
                pHandle->u4decstate = AUD_DEC_STATE_INIT;
                break;
            case AUD_DEC_CMD_TERMINATE:
                pHandle->u4decstate = AUD_DEC_STATE_STOP;
                // exiting
                break;
            default:
                LOG(1,"[AUD][AWB] UNKNOWN COMMAND!!");
                break; //break out the switch
            }
        }

        if((pHandle->u4decstate == AUD_DEC_STATE_INIT)/*&&(rMsg.eStateTo == AUD_DEC_STATE_PLAY) */)
        {
            AWBDecRet = i4DSP_AWBinit(pHandle);
            if (AWBDecRet == AWBDEC_FAIL)
            {
                pHandle->u4decstate = AUD_DEC_STATE_STOP;
            }
            else if (AWBDecRet == AWBDEC_CHKFRMSZ_FAIL)
            {
                // polling for filling bitstream
                x_thread_delay(10);
            }
            else //if (AMRDecRet == AWBDEC_OK)
            {
                pHandle->u4decstate = AUD_DEC_STATE_PLAY;
            }
        }
        else if(pHandle->u4decstate == AUD_DEC_STATE_PLAY)
        {
            AWBDecRet = i4DSP_AWBdecode(pHandle,&u4OutNum);

            if(AWBDecRet == AWBDEC_FAIL || AWBDecRet == AWBDEC_EOF)
            {
                pHandle->u4decstate = AUD_DEC_STATE_STOP;
            }
            else if (AWBDecRet == AWBDEC_CHKFRMSZ_FAIL)
            {
                x_thread_delay(10); // pollling for buffer ready
            }
            else if (AWBDecRet == AWBDEC_OK)
            {
                u4CopiedPCM = pHandle->pu4AudDecFillFIFO(pHandle, (UINT32)pHandle->pDecOutPCMBuf, u4OutNum);
                u4RemainPCM = u4OutNum - u4CopiedPCM;
                if (u4RemainPCM)
                {
                pHandle->u4decstate = AUD_DEC_STATE_IDLE;
                    pHandle->pDecOutPCMBuf += u4CopiedPCM;
                }
            }
        }
        else if (pHandle->u4decstate == AUD_DEC_STATE_IDLE)
        {
            u4CopiedPCM = pHandle->pu4AudDecFillFIFO(pHandle, (UINT32)pHandle->pDecOutPCMBuf, u4RemainPCM);
            u4RemainPCM = u4RemainPCM - u4CopiedPCM;
            if (u4RemainPCM)
            {
                pHandle->pDecOutPCMBuf += u4CopiedPCM;
                x_thread_delay(10); // pollling for pcm output buffer space available
            }
            else
            {
                pHandle->u4decstate = AUD_DEC_STATE_PLAY;
            }
        }

        if (pHandle->u4decstate == AUD_DEC_STATE_STOP)
        {
            break;
        }

        // check for player message
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

        AWB_DBG_MSG("[AWB] msg ret %d\n", i4Ret);


    }

    VERIFY(pHandle->u4decstate == AUD_DEC_STATE_STOP);

    // delete message queue
    x_msg_q_delete(pHandle->hCommandQ);

    LOG(0,"[AUD][AMRWB] vAudAWBThread: pHandle = 0x%X. Task terminated.\n", pHandle);

    x_thread_exit();
}


INT32 i4AudAWBTaskCreate(AUD_DEC_HANDLE_T *pHandle)
{
    pHandle->u4decstate = AUD_DEC_STATE_INIT;
    pHandle->i4decerrcode = 0;
    pHandle->u4frmcnt = 0;
    pHandle->u4errfrmcnt = 0;
    pHandle->u4InBufSize = pHandle->pInBufENDBLK-pHandle->pInBufSTRBLK;
    pHandle->pInBufPRSPNT = pHandle->pInBufSTRBLK;
    pHandle->pInBufBUFPNT = pHandle->pInBufSTRBLK;
    pHandle->pInBufCURPNT = pHandle->pInBufSTRBLK;


        LOG(1,"[AUD][DEC] i4AudAWBTaskCreate = inbuf: 0x%X. 0x%X. 0x%X.\n",
                pHandle->pInBufSTRBLK,pHandle->pInBufENDBLK,pHandle->u4InBufSize);
        LOG(1,"[AUD][DEC] i4AudAWBTaskCreate = outbuf: 0x%X. 0x%X. 0x%X.\n",
                pHandle->pOutBufSTRBLK,pHandle->pOutBufSTRBLK+pHandle->u4InBufSize,pHandle->u4InBufSize);



    // create command queue
    x_msg_q_create(&(pHandle->hCommandQ), "AudAWBCmdQ", sizeof(AUD_DEC_MSG_T), 8);

    LOG(1,"[AUD][DEC] i4AudAWBTaskCreate: pHandle = 0x%X.\n", pHandle);

    // create audio risc decoder main thread
    x_thread_create(&(pHandle->hMainTask), "AudAWBTask",
                    AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                    vAudAWBThread, sizeof(void *), &pHandle);

    return AUD_OK;
}

INT32 i4AudAWBTaskSendCommand(AUD_DEC_HANDLE_T *pHandle, AUD_DEC_MSG_T *prMsg)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != pHandle->hCommandQ)
    {
        i4Ret = x_msg_q_send(pHandle->hCommandQ, prMsg, sizeof(AUD_DEC_MSG_T), 0/*not used*/);
    }

    return i4Ret;
}

INT32 i4AudAWBTaskDestroy(AUD_DEC_HANDLE_T *pHandle)
{
    AUD_DEC_MSG_T rMsgTerminate;

    rMsgTerminate.eCmd = AUD_DEC_CMD_TERMINATE;
    i4AudAWBTaskSendCommand(pHandle, &rMsgTerminate);

    return AUD_OK;
}

#define AWB_MAGIC_NUMBER "#!AMR-WB\n"
AWBDEC_STATE i4DSP_AWBinit(AUD_DEC_HANDLE_T * pHandle){

    AWB_DEC_CTRL_T*  p_awb_dec_ctrl_s = &awb_dec_ctrol;
    AWB_DEC_HANDLE*  p_awb_dec_handle;
    UWord32 int_buf_size;
	UWord32 tmp_buf_size;
	UWord32 pcm_buf_size;
	UWord32 bs_buf_size;
    UWord32 u4OutByteNum;
    UWord8 magic[9];

    if (u4DSP_DecChkFrmSz(pHandle,(UINT32)pHandle->pInBufCURPNT,sizeof (AWB_MAGIC_NUMBER)-1) == FALSE)
    {
        AWB_DBG_MSG("[AWB] Init check frame size fail\n");
        return AWBDEC_CHKFRMSZ_FAIL;
    }

    u4OutByteNum = u4DSP_GetBits(pHandle, sizeof (AWB_MAGIC_NUMBER)-1, magic);

    if (x_memcmp(magic, AWB_MAGIC_NUMBER, sizeof (AWB_MAGIC_NUMBER)-1))
    {
        AWB_DBG_MSG("[AWB][WARNING]invalid magic number %x %x %x %x %x %x %x %x %x %x\n", magic[0]
                                                            , magic[1]
                                                            , magic[2]
                                                            , magic[3]
                                                            , magic[4]
                                                            , magic[5]
                                                            , magic[6]
                                                            , magic[7]
                                                            , magic[8]
                                                            , magic[9]);
        pHandle->pInBufCURPNT = pHandle->pInBufBUFPNT;
        //return AWBDEC_FAIL;
    }
    else
    {
        AWB_DBG_MSG("[AWB] magic number found\n");
        vDSP_DecMovFIFOPnt((const UINT32)pHandle->pInBufSTRBLK, (const UINT32)pHandle->pInBufENDBLK, (UINT32**)&(pHandle->pInBufBUFPNT), sizeof (AWB_MAGIC_NUMBER)-1);
    }

    AWB_DBG_MSG("[AWB] decoder init start\n");

    // AMR Decoder: Step 1
	// To know the required buffer size
	AWBDec_GetBufferSize(&int_buf_size ,&tmp_buf_size,&bs_buf_size, &pcm_buf_size);
    p_awb_dec_ctrl_s->int_buf_size = int_buf_size;
    p_awb_dec_ctrl_s->tmp_buf_size = tmp_buf_size;
    p_awb_dec_ctrl_s->bs_buf_size = bs_buf_size;
    p_awb_dec_ctrl_s->pcm_buf_size = pcm_buf_size;

    p_awb_dec_ctrl_s->int_buffer = awbMalloc(int_buf_size);
	p_awb_dec_ctrl_s->tmp_buffer = awbMalloc(tmp_buf_size);
	p_awb_dec_ctrl_s->pcm_buffer = awbMalloc(pcm_buf_size);
	p_awb_dec_ctrl_s->bs_buffer  = awbMalloc(bs_buf_size);

    memset(p_awb_dec_ctrl_s->int_buffer, 0, int_buf_size*sizeof(UChar));
    memset(p_awb_dec_ctrl_s->tmp_buffer, 0, tmp_buf_size*sizeof(UChar));
    memset(p_awb_dec_ctrl_s->pcm_buffer, 0, pcm_buf_size*sizeof(UChar));
    memset(p_awb_dec_ctrl_s->bs_buffer, 0, bs_buf_size*sizeof(UChar));

    AWB_DBG_MSG("[AWB] init bufsize, int/tmp/bs/pcm sz %x %x %x %x\n", int_buf_size
                                                                , tmp_buf_size
                                                                , bs_buf_size
                                                                , pcm_buf_size);


    // AMR Decoder: Step 2
	// Assign buffer and initialize
    p_awb_dec_handle = AWBDec_Init(p_awb_dec_ctrl_s->int_buffer);

    if ((UINT32) p_awb_dec_handle == (UINT32)NULL)
    {
        AWB_DBG_MSG("[AWB] init fail\n");
        return AWBDEC_FAIL;
    }

    p_awb_dec_ctrl_s->awb_dec_handle = p_awb_dec_handle;
    p_awb_dec_ctrl_s->consume_size= 62;
    p_awb_dec_ctrl_s->remain_size= 0;

    pHandle->pDecOutPCMBuf = (UINT8*)p_awb_dec_ctrl_s->pcm_buffer;
    pHandle->u4frmcnt = pHandle->u4errfrmcnt = 0;

    AWB_DBG_MSG("[Awb] init pass\n");

    return AWBDEC_OK;
}


//WMAFileDecodeData
AWBDEC_STATE i4DSP_AWBdecode(AUD_DEC_HANDLE_T * pHandle, UINT32* pOutlen)
{
    AWB_DEC_CTRL_T *p_awb_dec_ctrl_s = &awb_dec_ctrol;
    AWB_DEC_HANDLE *awb_dec_handle = p_awb_dec_ctrl_s->awb_dec_handle;
    void*   tmp_buffer = p_awb_dec_ctrl_s->tmp_buffer;
    Word16*  pcm_buffer = (Word16*)p_awb_dec_ctrl_s->pcm_buffer;
    UChar*   bs_buffer = (UChar*)p_awb_dec_ctrl_s->bs_buffer;
    Word32  consumed_size = p_awb_dec_ctrl_s->consume_size;
    UWord32  remain_size = p_awb_dec_ctrl_s->remain_size;

    *pOutlen = (UINT32) 0;

    if (u4DSP_DecChkFrmSz(pHandle,(UINT32)pHandle->pInBufCURPNT,consumed_size) == FALSE)
        return AWBDEC_CHKFRMSZ_FAIL;

    u4DSP_GetBits(pHandle, consumed_size, &bs_buffer[remain_size]);
    vDSP_DecMovFIFOPnt((const UINT32)pHandle->pInBufSTRBLK, (const UINT32)pHandle->pInBufENDBLK, (UINT32**)&(pHandle->pInBufBUFPNT), consumed_size);

    AWB_DBG_MSG("[AWB] frm#%d: %X %X %X %X\n", pHandle->u4frmcnt
                                            , bs_buffer[0]
                                            , bs_buffer[1]
                                            , bs_buffer[2]
                                            , bs_buffer[3]);

    consumed_size = AWB_Decode(awb_dec_handle, tmp_buffer, pcm_buffer, bs_buffer);

    AWB_DBG_MSG("[AWB] frm#%d: %X %X %X %X, consumed 0x%x\n", pHandle->u4frmcnt
                                                , pcm_buffer[0]
                                                , pcm_buffer[1]
                                                , pcm_buffer[2]
                                                , pcm_buffer[3]
                                                , consumed_size);

    if (consumed_size > 62)
        return AWBDEC_FAIL;

    remain_size = 62 - consumed_size;

    if (remain_size)
        x_memcpy((void*)bs_buffer, (const void*)&bs_buffer[consumed_size], remain_size);

    pHandle->u4frmcnt++;
    p_awb_dec_ctrl_s->consume_size = consumed_size;
    p_awb_dec_ctrl_s->remain_size = remain_size;
    pHandle->pDecOutPCMBuf = (UINT8*)pcm_buffer;
    *pOutlen = (UINT32)AMRWB_OUTPCM_SMP_NUM*2;

    return AWBDEC_OK;
}

#endif

