/*----------------------------------------------------------------------------*
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
#if !defined(DRV_T32_H)
#define DRV_T32_H

enum
{
	E_T32_NOP,
	E_T32_EXEC,
	E_T32_RESTART,
	E_T32_EXIT,
	E_T32_EXEC_RELOAD_SYM,
	E_T32_DETECT_HOST,
	E_T32_END
};

//low level commands
INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
INT32 T32_HostExec2(INT32 i4TargetFlag,CHAR *puFilePath,UINT32 u4Address,UINT32 u4Size);
INT32 T32_HostDetect(void);
INT32 T32_HostSaveData(CHAR *szFileName, void *pvMemoryStart, UINT32 u4ByteSize);
INT32 T32_HostLoadData(CHAR *szFileName, void *pvMemoryStart);
INT32 T32_HostRestart(void);
INT32 T32_HostExit(void);

//dcc file mode
#define T32_TERM_O_OPEN_EXISTING	0x00
#define T32_TERM_O_CREATE_TRUNC		0x04
#define T32_TERM_O_CREATE_APPEND	0x08
#define T32_TERM_O_RDONLY			0x00
#define T32_TERM_O_WRONLY			0x02
#define T32_TERM_O_RDWR				0x02
#define T32_TERM_O_ASCII			0x00
#define T32_TERM_O_BINARY			0x01

//dcc commands
INT32 ARM_GetChar(void);
void ARM_PutChar(INT32 i4Ch);
INT32 ARM_GetLine(UINT8 *pu1Buf, INT32 i4Len);
void ARM_Puts(UINT8 *pu1Buf);
INT32 ARM_OpenFile(CHAR *szName, INT32 i4Mode);
INT32 ARM_CloseFile(INT32 i4Handle);
INT32 ARM_ReadFile(INT32 i4Handle, UINT8 pu1Buf, INT32 i4Len);
INT32 ARM_WriteFile(INT32 i4Handle, UINT8 *pu1Buf, INT32 i4Len);
INT32 ARM_SeekFile(INT32 i4Handle, INT32 i4Pos);
INT32 ARM_TellFile(INT32 i4Handle);
INT32 ARM_TellSizeFile(INT32 i4Handle);

#endif //DRV_T32_H



