#ifndef _RGB_IF_H_
#define _RGB_IF_H_

extern VOID RGB_Init(VOID);

extern VOID RGB_SetRgbUiData(UINT8 ui1AmbientSensorOnOff, UINT8 ui1CTIndex);

extern void main_rgb_sreg_write(UINT16 sreg,UINT16 val);

extern UINT16 main_rgb_sreg_read(UINT16 sreg);

extern void RGB_SetGaisouSetting(UINT8 data);

extern UINT8 RGB_GetGaisouSetting(void);

#endif
