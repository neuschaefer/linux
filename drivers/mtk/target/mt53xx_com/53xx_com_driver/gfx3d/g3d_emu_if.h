#include "x_os.h"

typedef struct 
{
    BOOL fgMemoryStartFromZero;
    UINT32 u4GPMMUMemoryStart;
    UINT32 u4PPMMUMemoryStart;
    CHAR szEmuListName[256]; 
} G3D_EMU_CONF_T;

INT32 i4Graphics3D_Drv_Emu(void);
INT32 i4Graphics3D_Drv_Init(G3D_EMU_CONF_T rConf);
INT32 i4Graphics3D_Display(void);

