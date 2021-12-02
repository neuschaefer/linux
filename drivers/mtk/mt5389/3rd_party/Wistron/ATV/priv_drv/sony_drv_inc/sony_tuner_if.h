#ifndef _SONY_TUNER_IF_
#define _SONY_TUNER_IF_


EXTERN VOID SONY_TUNER_Init(VOID);
EXTERN VOID main_sony_tuner_sreg_read_reg(void* pvArg);
EXTERN VOID main_sony_tuner_sreg_write_reg(void* pvArg);
EXTERN VOID SONY_TUNER_set_agc_reg(VOID);
EXTERN UINT16 main_sony_tuner_sreg_read(UINT16 sreg);
EXTERN VOID main_sony_tuner_sreg_write(UINT16 sreg,UINT16 val);
EXTERN VOID SONY_TUNER_cis_setting(BOOL fgCIS);

#endif
