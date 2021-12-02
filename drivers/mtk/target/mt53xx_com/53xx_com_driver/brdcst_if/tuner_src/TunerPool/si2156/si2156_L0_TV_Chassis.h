/***************************************************************************************/
/* FILE: Si2170_L0_TV_Chassis.h                                                        */
/*                                                                                     */
/*                                                                                     */
/*                                                                                     */
/***************************************************************************************/
#ifndef SI2156_L0_TV_CHASSIS_H
#define SI2156_L0_TV_CHASSIS_H

typedef struct L0_Context {
    unsigned char   address;
     int             indexSize;
} L0_Context;

typedef struct L1_Context {
  L0_Context *i2c;
  void       *specific;
} L1_Context;

void	L0_Init                  (L0_Context *pContext);
void	system_wait              (int time_ms);
int     L0_SetAddress        (L0_Context* i2c, unsigned int add, int addSize);
int		L0_ReadCommandBytes      (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer) ;
int		L0_WriteCommandBytes     (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer) ;
int     L0_ReadBytes             (L0_Context* i2c, unsigned int iI2CIndex, int iNbBytes, unsigned char *pucDataBuffer);
int     L0_WriteBytes          (L0_Context* i2c, unsigned int iI2CIndex, int iNbBytes, unsigned char *pucDataBuffer) ;
#endif
