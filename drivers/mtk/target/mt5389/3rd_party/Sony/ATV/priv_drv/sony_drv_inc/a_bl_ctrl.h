/* ========================================================================== */
/*                                                                            */
/*   a_bl_ctrl.h                                                              */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _A_BL_CTRL_H_
#define _A_BL_CTRL_H_

typedef enum{
  BL_INIT_STATE = 0,
  BL_FIX_DUTY_STATE,
  BL_ON_STATE,
  BL_SWITCH_OVER_STATE,
  BL_READY_STATE,
  BL_STARTUP_LOGO_STATE,
  BL_OFF_STATE  
} Enum_BacklightState;

VOID Dyn_BacklightCtrl_Init(VOID);
extern void vPwrSavingStatus(UINT8 ui1_value);
extern void vUserBacklightLvl(UINT8 ui1_value);
extern void vLightSensorStatus(UINT8 ui1_value);
extern void vUserPictureModeStatus(UINT8 ui1_value);
extern void vAdvContrastEnhancerStatus(UINT8 ui1_value);

extern void vAutoBacklightInit(void);
extern void vAutoBacklightISR(void);
extern UINT32 vAutoBacklightProc(void);
extern INT32 vAutoBacklightState(Enum_BacklightState u1_state, UINT16* u2_ret_ptr);
extern void main_bl_sreg_write(UINT16 sreg,UINT16 val);
extern UINT16 main_bl_sreg_read(UINT16 sreg);

#endif /* _A_BL_CTRL_H_ */
