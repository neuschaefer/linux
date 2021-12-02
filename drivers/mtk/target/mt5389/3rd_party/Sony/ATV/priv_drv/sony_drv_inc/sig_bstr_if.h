#ifndef _SIG_BSTR_IF_
#define _SIG_BSTR_IF_

EXTERN VOID sig_bstr_calc(BOOL setSigBooster);
EXTERN VOID SIG_BSTR_Init(VOID);
EXTERN UINT16 main_sig_bstr_sreg_read(UINT16 sreg);
EXTERN VOID main_sig_bstr_sreg_write(UINT16 sreg,UINT16 val);
EXTERN VOID SIG_BSTR_IP_calc(BOOL IP_status,BOOL SigBstr_status);

#endif
