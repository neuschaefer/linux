typedef enum
{
	RGBA8888,
	RGB565,
	FORMAT_NONE = 0xff
}SCPIP_GFX_FORMAT;

typedef struct R3DGfxInf
{
	UINT8 u1PathIn;
	UINT8 u1Sel444;
	//UINT8 u1OutValue;
	UINT8 u1FrameNumber;
	UINT8 u1WriteMode;
	UINT8 u1WriteEnable;
	UINT8 u13D22DEnable;
	
	UINT32 u4BassAddr0;
	UINT32 u4BassAddrOffset;
	
	UINT32 u4WAddrHight;
	UINT32 u4WAddrLow;

	SCPIP_GFX_FORMAT Format;
	UINT8 u1AlphaPos;
	UINT8 u1AlphaValue;
	
	UINT8 u1HBoundEn;
	UINT16 u2HBoundValue;

	UINT8 u1FrameDropEn;
	UINT32 u1DropM;
	UINT32 u1DropN;

	UINT8 u1MuxB;
	UINT8 u1MuxG;
	UINT8 u1MuxR;

}R3DGfxInf;

EXTERN void vScpip_GFX_init(void);
EXTERN UINT8 u1Scpip_GFX_Source_Sel(UINT8 bPath);

EXTERN UINT8 u1Scpip_GFX_Set_Frame_Num(UINT8 u1Number);
EXTERN UINT8 u1Scpip_GFX_Set_Dram_Address(UINT32 u4BaseAddr, UINT32 u4FrameSize, UINT8 u1FrameNum);
EXTERN UINT8 u1Scpip_GFX_Set_Write_Mode(UINT8 u1Select);
EXTERN UINT8 u1Scpip_GFX_Write_Trigger(void);
EXTERN UINT8 u1Scpip_GFX_Write_Enable(UINT8 u1Enable);
EXTERN UINT8 u1Scpip_GFX_Set_Alpha(UINT8 u1Alpha);

EXTERN UINT8 u1Scpip_GFX_Set_3DTo2D_En(UINT8 u1Enable);
EXTERN UINT8 u1Scpip_GFX_Set_Frame_Drop(UINT8 u1Enable,UINT8 u1DropN, UINT8 u1DropM);
EXTERN UINT8 u1Scpip_GFX_Set_Dump_Format(SCPIP_GFX_FORMAT Format);

EXTERN UINT8 u1Scpip_GFX_GetCurrentFrameIdx(void);
EXTERN UINT8 u1Scpip_GFX_GetWriteStatus(void);
EXTERN void vScpip_GFX_Query(void);
