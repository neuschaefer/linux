#ifndef _AMRIF_H_
#define _AMRIF_H_

typedef enum
{
    AMRDEC_OK,
    AMRDEC_FAIL,
    AMRDEC_CHKFRMSZ_FAIL,
    AMRDEC_EOF,
}AMRDEC_STATE;

typedef struct
{
    AMR_DEC_HANDLE  amr_dec_handle;
    UWord32 int_buf_size;
	UWord32 tmp_buf_size;
	UWord32 pcm_buf_size;
	UWord32 bs_buf_size;
    void*   int_buffer;
    void*   tmp_buffer;
    void*   pcm_buffer;
    void*   bs_buffer;
}AMR_DEC_CTRL_T;

//=====================================
AMRDEC_STATE i4DSP_AMRinit(AUD_DEC_HANDLE_T * pHandle);
AMRDEC_STATE i4DSP_AMRdecode(AUD_DEC_HANDLE_T * pHandle, UINT32* u4outlen);

#endif
