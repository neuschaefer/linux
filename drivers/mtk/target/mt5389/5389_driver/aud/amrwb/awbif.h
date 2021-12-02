#ifndef _AWBIF_H_
#define _AWBIF_H_

typedef enum
{
    AWBDEC_OK,
    AWBDEC_FAIL,
    AWBDEC_CHKFRMSZ_FAIL,
    AWBDEC_EOF,
}AWBDEC_STATE;

typedef struct
{
    AWB_DEC_HANDLE*  awb_dec_handle;
    UWord32 int_buf_size;
	UWord32 tmp_buf_size;
	UWord32 pcm_buf_size;
	UWord32 bs_buf_size;
    UWord32 consume_size;
    UWord32 remain_size;
    void*   int_buffer;
    void*   tmp_buffer;
    void*   pcm_buffer;
    void*   bs_buffer;
}AWB_DEC_CTRL_T;

//=====================================
AWBDEC_STATE i4DSP_AWBinit(AUD_DEC_HANDLE_T * pHandle);
AWBDEC_STATE i4DSP_AWBdecode(AUD_DEC_HANDLE_T * pHandle, UINT32* u4outlen);

#endif
