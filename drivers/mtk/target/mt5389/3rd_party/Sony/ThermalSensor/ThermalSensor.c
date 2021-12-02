
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_debug.h"
#include "x_pdwnc.h"

//-----------------------------------------------------------------------------
// Constant definitions & Macro
//-----------------------------------------------------------------------------
#define CUSTOM_THERMAL_SIF_CLK_DIV          0x100
#define CUSTOM_THERMAL_SIF_DEV_ADDR         0x90
#define CUSTOM_THERMAL_SIF_SUB_ADDR         0x0
#define CUSTOM_THERMAL_DATA_LEN             2

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
INT32 CustomQueryTemperature(void);
INT32 CustomReadTemperature(void);

//-----------------------------------------------------------------------------
// Imported funtions 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static INT32 _i4MainTemperature = 0;
static UINT32 _u4RawTemperature = 0;


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------
UINT32 CustomQueryTempRawData(void)
{
    static BOOL bIsInit = TRUE;
    
    if (bIsInit)
    {
        UNUSED(CustomReadTemperature());
        bIsInit = FALSE;
    }
    
    return _u4RawTemperature;
}

INT32 CustomQueryTemperature(void)
{
    static BOOL bIsInit = TRUE;
    INT32 i4Temperature;
    CRIT_STATE_T rCritState;
    
    if (bIsInit)
    {
        i4Temperature = CustomReadTemperature();
        bIsInit = FALSE;
    }
    
    rCritState = x_crit_start();
    i4Temperature = _i4MainTemperature;
    x_crit_end(rCritState);
    
    return i4Temperature;
}

INT32 CustomReadTemperature(void)
{
    UINT8  au1Temperature[CUSTOM_THERMAL_DATA_LEN] = {0, 0};
    UINT32 u4Temperature;
    INT32  i4Temperature;
    CRIT_STATE_T rCritState;
    
    UNUSED(SIF_Read(CUSTOM_THERMAL_SIF_CLK_DIV, 
                    CUSTOM_THERMAL_SIF_DEV_ADDR,
                    CUSTOM_THERMAL_SIF_SUB_ADDR,
                    au1Temperature, CUSTOM_THERMAL_DATA_LEN));
    u4Temperature = au1Temperature[0] << 1;
    u4Temperature |= au1Temperature[1] >> 7;
    _u4RawTemperature = u4Temperature;
    
    i4Temperature = (u4Temperature & 0x00FF);
    if (u4Temperature & (1 << 8))
    {
        i4Temperature |= 0xFFFFFF00;
    }

    if (i4Temperature != _i4MainTemperature)
    {
        rCritState = x_crit_start();
        _i4MainTemperature = i4Temperature;
        x_crit_end(rCritState);
    }
    
    return i4Temperature;
}
