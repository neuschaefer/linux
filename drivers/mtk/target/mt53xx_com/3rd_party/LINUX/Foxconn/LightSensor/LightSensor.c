
#include "x_debug.h"
#include "sif_if.h"		
#include "CustomCtrl.h"

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------

//mapping to 300 Lux
#define SensorMax_E		184//276 // E type
#define SensorMax_C		184//230 // C type
#define SensorMax_P		46	// P type
#define SensorMax_L		184//230	// L type
#define LS_SIF_CLK_DIV     300
UINT16 ui2MaxStep=46;  //Ap access the range 0~46

UINT32 u4ModelIndex;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
VOID CM3217_Init(void)
{
    //atm-liang add for light sensor : begin
    UINT8 buff;
    UINT16 uiRet;

    u4ModelIndex = CustomModelOptionQueryFunc();
	
    switch (u4ModelIndex)
    {
        case DRV_CUSTOM_MODEL_OPTION_P:
            buff = 0x2E;
    	    uiRet = SIF_WriteNoSubAddr(LS_SIF_CLK_DIV,0x20,&buff,1);
            if(uiRet)
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor enter normal mode - successful!!\n");
            else
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor enter normal mode - failed!!\n");

            buff = 0x60;
            uiRet = SIF_WriteNoSubAddr(LS_SIF_CLK_DIV,0x22,&buff,1);
            if(uiRet)
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor write 0x22 successful!!\n");
            else
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor write 0x22 error!!\n");
            break;

        default:
            buff = 0x26;
            uiRet = SIF_WriteNoSubAddr(LS_SIF_CLK_DIV,0x20,&buff,1);
            if(uiRet)
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor enter normal mode - successful!!\n");
            else
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor enter normal mode - failed!!\n");

            buff = 0x20;
            uiRet = SIF_WriteNoSubAddr(LS_SIF_CLK_DIV,0x22,&buff,1);
            if(uiRet)
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor write 0x22 successful!!\n");
            else
                Printf("[GPIO_IbizaPowerOnFunc] Light sensor write 0x22 error!!\n");
            break;
    }
}

UINT8 Get_CM3217_Step(void)
{
    static BOOL bInit = TRUE;
    UINT8 CM3217_value_MSB, CM3217_value_LSB, Step;	
    UINT16 CM3217_value_16bit, uiRet;
			
    if (bInit)
    {
        CM3217_Init();
        bInit = FALSE;
    }

    //Read CM3217 LSB data	
    uiRet = SIF_ReadNoSubAddr(LS_SIF_CLK_DIV,0x23,&CM3217_value_LSB,1);	
    if(uiRet==0)        return 0xff;
					
    //Read CM3217 MSB data	
    uiRet = SIF_ReadNoSubAddr(LS_SIF_CLK_DIV,0x21,&CM3217_value_MSB,1);	
    if(uiRet==0)        return 0xff;

    CM3217_value_16bit = (UINT16)(CM3217_value_MSB<<8)|((UINT16)CM3217_value_LSB);	
    //Printf("[LightSensor] Light Sensor Value = %d\n",CM3217_value_16bit);		

    switch (u4ModelIndex)
    {
        case DRV_CUSTOM_MODEL_OPTION_E:
            if(CM3217_value_16bit < SensorMax_E) 
            {	
                Step = CM3217_value_16bit/(SensorMax_E/ui2MaxStep);
                return Step;
            }
            break;
        case DRV_CUSTOM_MODEL_OPTION_C:
            if(CM3217_value_16bit < SensorMax_C) 
            {
                Step = CM3217_value_16bit/(SensorMax_C/ui2MaxStep);
                return Step;
            }
            break;
        case DRV_CUSTOM_MODEL_OPTION_P:
            if(CM3217_value_16bit < SensorMax_P) 
            {
                Step = CM3217_value_16bit/(SensorMax_P/ui2MaxStep);
                return Step;
            }
            break;
        case DRV_CUSTOM_MODEL_OPTION_L:
            if(CM3217_value_16bit < SensorMax_L) 
            {
                Step = CM3217_value_16bit/(SensorMax_L/ui2MaxStep);
                return Step;
            }
            break;	
        default:
            if(CM3217_value_16bit < 184) //Ibiza default max value  = 184, mapping to 300 Lux
            {
                Step = CM3217_value_16bit/4;
                return  Step;
            }
            break;
 }
    		
    return ui2MaxStep;
}

