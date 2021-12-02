#ifdef LINUX_TURNKEY_SOLUTION

//=====================================
#include "aud_if.h" //AUD_OK
#include "drv_common.h"
//=====================================
#include "aud_debug.h" //LOG //this monkey define BYTE
//=====================================
#include "typedef.h"
#include "amr_exp.h"
//#include "audip_exp.h"
#include "amrif.h"
#include "riscdec.h"
#include "x_hal_5381.h"

//#define TURN_ON_DBG_MSG
#ifdef  TURN_ON_DBG_MSG
#define     AMR_DBG_MSG     Printf
#else
#define     AMR_DBG_MSG(fmt...)
#endif

/* frame size in serial bitstream file (frame type + serial stream + flags) */
#define SERIAL_FRAMESIZE    (1+MAX_SERIAL_SIZE+5)
#define AMRNB_OUTPCM_SMP_NUM    160

const UWord16 dpacked_size[16] = {12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0};
static AMR_DEC_CTRL_T    amr_dec_ctrol;

void*  amrMalloc(const size_t iSize)
{
    return (void *)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(iSize, 8));
}

static void vAudAMRThread(void* pvArg) //vAudAMRThread
{
    INT32 i4Ret = 0;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize;
    AUD_DEC_MSG_T rMsg;
    AMRDEC_STATE AMRDecRet;
    UINT32 u4OutNum=0, u4CopiedPCM=0, u4RemainPCM=0;

    AUD_DEC_HANDLE_T *pHandle = *((AUD_DEC_HANDLE_T **)pvArg);

    LOG(1,"[AUD][AMR] vAudAMRThread: pHandle = 0x%X.\n", pHandle);

    do
    {   // waiting for omx send start decoding message
        i4Ret = x_msg_q_receive_timeout(&u2MsgIdx, (void *)&rMsg, &zMsgSize, &(pHandle->hCommandQ), 1, 10);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_NO_MSG)|| (i4Ret == OSR_TIMEOUT));
    }while(i4Ret != OSR_OK);

    AMR_DBG_MSG("[AMR] decoder thread starts!\n");

    while (TRUE)
    {
        if (OSR_OK == i4Ret)    // new message received
        {
            AMR_DBG_MSG("[AMR] msg cmd %d\n", rMsg.eCmd);
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
                LOG(1,"[AUD][AMR] UNKNOWN COMMAND!!");
                break; //break out the switch
            }
        }

        if((pHandle->u4decstate == AUD_DEC_STATE_INIT)/*&&(rMsg.eStateTo == AUD_DEC_STATE_PLAY) */)
        {
            AMRDecRet = i4DSP_AMRinit(pHandle);
            if (AMRDecRet == AMRDEC_FAIL)
            {
                pHandle->u4decstate = AUD_DEC_STATE_STOP;
            }
            else if (AMRDecRet == AMRDEC_CHKFRMSZ_FAIL)
            {
                // polling for filling bitstream
                x_thread_delay(10);
            }
            else //if (AMRDecRet == AMRDEC_OK)
            {
                pHandle->u4decstate = AUD_DEC_STATE_PLAY;
            }
        }
        else if(pHandle->u4decstate == AUD_DEC_STATE_PLAY)
        {
            AMRDecRet = i4DSP_AMRdecode(pHandle,&u4OutNum);

            if(AMRDecRet == AMRDEC_FAIL || AMRDecRet == AMRDEC_EOF)
            {
                pHandle->u4decstate = AUD_DEC_STATE_STOP;
            }
            else if (AMRDecRet == AMRDEC_CHKFRMSZ_FAIL)
            {
                x_thread_delay(10);
            }
            else if (AMRDecRet == AMRDEC_OK)
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

        //AMR_DBG_MSG("[AMR] msg ret %d\n", i4Ret);


    }

    VERIFY(pHandle->u4decstate == AUD_DEC_STATE_STOP);

    // delete message queue
    x_msg_q_delete(pHandle->hCommandQ);

    LOG(0,"[AUD][AMRNB] vAudAMRThread: pHandle = 0x%X. Task terminated.\n", pHandle);

    x_thread_exit();
}


INT32 i4AudAMRTaskCreate(AUD_DEC_HANDLE_T *pHandle)
{
    pHandle->u4decstate = AUD_DEC_STATE_INIT;
    pHandle->i4decerrcode = 0;
    pHandle->u4frmcnt = 0;
    pHandle->u4errfrmcnt = 0;
    pHandle->u4InBufSize = pHandle->pInBufENDBLK-pHandle->pInBufSTRBLK;
    pHandle->pInBufPRSPNT = pHandle->pInBufSTRBLK;
    pHandle->pInBufBUFPNT = pHandle->pInBufSTRBLK;
    pHandle->pInBufCURPNT = pHandle->pInBufSTRBLK;


        LOG(1,"[AUD][DEC] i4AudAMRTaskCreate = inbuf: 0x%X. 0x%X. 0x%X.\n",
                pHandle->pInBufSTRBLK,pHandle->pInBufENDBLK,pHandle->u4InBufSize);
        LOG(1,"[AUD][DEC] i4AudAMRTaskCreate = outbuf: 0x%X. 0x%X. 0x%X.\n",
                pHandle->pOutBufSTRBLK,pHandle->pOutBufSTRBLK+pHandle->u4InBufSize,pHandle->u4InBufSize);



    // create command queue
    x_msg_q_create(&(pHandle->hCommandQ), "AudAMRCmdQ", sizeof(AUD_DEC_MSG_T), 8);

    LOG(1,"[AUD][DEC] i4AudAMRTaskCreate: pHandle = 0x%X.\n", pHandle);

    // create audio risc decoder main thread
    x_thread_create(&(pHandle->hMainTask), "AudAMRTask",
                    AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                    vAudAMRThread, sizeof(void *), &pHandle);

    return AUD_OK;
}

INT32 i4AudAMRTaskSendCommand(AUD_DEC_HANDLE_T *pHandle, AUD_DEC_MSG_T *prMsg)
{
    INT32 i4Ret = 0;

    if (NULL_HANDLE != pHandle->hCommandQ)
    {
        i4Ret = x_msg_q_send(pHandle->hCommandQ, prMsg, sizeof(AUD_DEC_MSG_T), 0/*not used*/);
    }

    return i4Ret;
}

INT32 i4AudAMRTaskDestroy(AUD_DEC_HANDLE_T *pHandle)
{
    AUD_DEC_MSG_T rMsgTerminate;

    rMsgTerminate.eCmd = AUD_DEC_CMD_TERMINATE;
    i4AudAMRTaskSendCommand(pHandle, &rMsgTerminate);

    return AUD_OK;
}

#define AMR_MAGIC_NUMBER "#!AMR\n"
AMRDEC_STATE i4DSP_AMRinit(AUD_DEC_HANDLE_T * pHandle){

    AMR_DEC_CTRL_T*  p_amr_dec_ctrl_s = &amr_dec_ctrol;
    AMR_DEC_HANDLE*  p_amr_dec_handle;
    UWord32 int_buf_size;
	UWord32 tmp_buf_size;
	UWord32 pcm_buf_size;
	UWord32 bs_buf_size;
    UWord32 u4OutByteNum;
    UWord8 magic[6];

    if (u4DSP_DecChkFrmSz(pHandle,(UINT32)pHandle->pInBufCURPNT,sizeof (AMR_MAGIC_NUMBER)-1) == FALSE)
    {
        AMR_DBG_MSG("[AMR] Init check frame size fail\n");
        return AMRDEC_CHKFRMSZ_FAIL;
    }

    u4OutByteNum = u4DSP_GetBits(pHandle, sizeof (AMR_MAGIC_NUMBER)-1, magic);

    if (x_memcmp(magic, AMR_MAGIC_NUMBER, sizeof(AMR_MAGIC_NUMBER)-1))
    {
        AMR_DBG_MSG("[AMR][WARNING]invalid magic number %x %x %x %x %x %x\n", magic[0]
                                                            , magic[1]
                                                            , magic[2]
                                                            , magic[3]
                                                            , magic[4]
                                                            , magic[5]);
        pHandle->pInBufCURPNT = pHandle->pInBufBUFPNT;

        //return AMRDEC_FAIL;
    }
    else
    {
        AMR_DBG_MSG("[AMR] magic number found\n");
        vDSP_DecMovFIFOPnt((const UINT32)pHandle->pInBufSTRBLK, (const UINT32)pHandle->pInBufENDBLK, (UINT32**)&(pHandle->pInBufBUFPNT), sizeof (AMR_MAGIC_NUMBER)-1);
    }

    AMR_DBG_MSG("[AMR] decoder init start\n");

    // AMR Decoder: Step 1
	// To know the required buffer size
	AMRDec_GetBufferSize(&int_buf_size ,&tmp_buf_size,&bs_buf_size, &pcm_buf_size);
    p_amr_dec_ctrl_s->int_buf_size = int_buf_size;
    p_amr_dec_ctrl_s->tmp_buf_size = tmp_buf_size;
    p_amr_dec_ctrl_s->bs_buf_size = bs_buf_size;
    p_amr_dec_ctrl_s->pcm_buf_size = pcm_buf_size;

    p_amr_dec_ctrl_s->int_buffer = amrMalloc(int_buf_size);
	p_amr_dec_ctrl_s->tmp_buffer = amrMalloc(tmp_buf_size);
	p_amr_dec_ctrl_s->pcm_buffer = amrMalloc(pcm_buf_size);
	p_amr_dec_ctrl_s->bs_buffer  = amrMalloc(bs_buf_size);

    memset(p_amr_dec_ctrl_s->int_buffer, 0, int_buf_size*sizeof(UChar));
    memset(p_amr_dec_ctrl_s->tmp_buffer, 0, tmp_buf_size*sizeof(UChar));
    memset(p_amr_dec_ctrl_s->pcm_buffer, 0, pcm_buf_size*sizeof(UChar));
    memset(p_amr_dec_ctrl_s->bs_buffer, 0, bs_buf_size*sizeof(UChar));

    AMR_DBG_MSG("[AMR] init bufsize, int/tmp/bs/pcm sz %x %x %x %x\n", int_buf_size
                                                                , tmp_buf_size
                                                                , bs_buf_size
                                                                , pcm_buf_size);


    // AMR Decoder: Step 2
	// Assign buffer and initialize
    p_amr_dec_handle = AMRDec_Init(p_amr_dec_ctrl_s->int_buffer);

    if ((UINT32)p_amr_dec_handle == (UINT32)NULL)
    {
        AMR_DBG_MSG("[AMR] decoder init fail\n");
        return AMRDEC_FAIL;
    }

    p_amr_dec_ctrl_s->amr_dec_handle = *p_amr_dec_handle;

    pHandle->u4frmcnt = pHandle->u4errfrmcnt = 0;

    AMR_DBG_MSG("[AMR] init pass\n");

    return AMRDEC_OK;
}


//WMAFileDecodeData
AMRDEC_STATE i4DSP_AMRdecode(AUD_DEC_HANDLE_T * pHandle, UINT32* pOutlen)
{
    AMR_DEC_CTRL_T *p_amr_dec_ctrl_s = &amr_dec_ctrol;
    AMR_DEC_HANDLE *amr_dec_handle = &(p_amr_dec_ctrl_s->amr_dec_handle);
    void*   tmp_buffer = p_amr_dec_ctrl_s->tmp_buffer;
    Word16*  pcm_buffer = (Word16*)p_amr_dec_ctrl_s->pcm_buffer;
    UChar*   bs_buffer = (UChar*)p_amr_dec_ctrl_s->bs_buffer;
    UChar   quality_bit;
	UChar   frame_type;
    UChar   toc;
    UWord16 packed_size;

    *pOutlen = (UINT32)0;

    if (u4DSP_DecChkFrmSz(pHandle,(UINT32)pHandle->pInBufCURPNT,1) == FALSE)
        return AMRDEC_CHKFRMSZ_FAIL;

    u4DSP_GetBits(pHandle, 1, &toc);

    /* read rest of the frame based on ToC byte */
    quality_bit  = (toc >> 2) & 0x01;
    frame_type = (toc >> 3) & 0x0F;
    packed_size = dpacked_size[frame_type];

    if (u4DSP_DecChkFrmSz(pHandle,(UINT32)pHandle->pInBufCURPNT,packed_size) == FALSE)
        return AMRDEC_CHKFRMSZ_FAIL;

    u4DSP_GetBits(pHandle, packed_size, (UINT8*)bs_buffer);

    vDSP_DecMovFIFOPnt((const UINT32)pHandle->pInBufSTRBLK, (const UINT32)pHandle->pInBufENDBLK, (UINT32**)&(pHandle->pInBufBUFPNT), packed_size+1);

    AMR_DBG_MSG("[AMR] dec frm %d, ft %d, psz 0x%x\n", pHandle->u4frmcnt, frame_type, packed_size);

    // AMR Decoder: Step 3, Decode a frame
    AMR_Decode(amr_dec_handle,
                tmp_buffer,
                pcm_buffer,
                bs_buffer,
                quality_bit,
                frame_type);
    AMR_DBG_MSG("[AMR] dec frm %d: %X %X %X %X\n", pHandle->u4frmcnt
                                                , pcm_buffer[0]
                                                , pcm_buffer[1]
                                                , pcm_buffer[2]
                                                , pcm_buffer[3]);
    pHandle->u4frmcnt++;
    pHandle->pDecOutPCMBuf = (UINT8*)pcm_buffer;

    *pOutlen = (UINT32)AMRNB_OUTPCM_SMP_NUM*2;

    return AMRDEC_OK;
}

#endif

