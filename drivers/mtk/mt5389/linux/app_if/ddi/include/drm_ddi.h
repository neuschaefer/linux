#ifndef	_DRM_DDI_H_
#define	_DRM_DDI_H_

#include "emp_ddi.h"

#define AES_CBC  0
#define AES_ECB  1
#define AES_ENCRYPT  0
#define AES_DESCRYPT 1
#define true 1
#define false 0

/* PlayReady */
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_SecureStoreGlobalPasswordSeed (char *pszPath, unsigned char *pbSecStorePassword, unsigned long cbSecStorePassword);
DTV_STATUS_T DDI_DRM_PLAYREADY_Set_SecureStoreGlobalPasswordSeed (char *pszPath);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_ZPrivSig (char *KeyPath, unsigned char *pbZPrivSig, unsigned long *cbZPrivSig);
DTV_STATUS_T DDI_DRM_PLAYREADY_Set_ZPrivSig (char *KeyPath, unsigned char *pbZPrivSig, unsigned long cbZPrivSig);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_ZPrivEnc (char *KeyPath, unsigned char *pbZPrivEnc, unsigned long *cbZPrivEnc);
DTV_STATUS_T DDI_DRM_PLAYREADY_Set_ZPrivEnc (char *KeyPath, unsigned char *pbZPrivEnc, unsigned long cbZPrivEnc);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_DeviceCert (char *KeyPath, unsigned char *pbDevcert, unsigned long *cbDevCert);
DTV_STATUS_T DDI_DRM_PLAYREADY_Set_DeviceCert (char *KeyPath, unsigned char *pbDevcert, unsigned long cbDevCert);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_BDeviceCert (char *KeyPath, unsigned char *pbBDevcert, unsigned long *cbBDevCert);
DTV_STATUS_T DDI_DRM_PLAYREADY_Set_BDeviceCert (char *KeyPath, unsigned char *pbBDevcert, unsigned long cbBDevCert);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_BGroupCert (char *KeyPath, unsigned char *pbBGroupCert, unsigned long *cbBGroupCert);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_ModelPrivateKey (char *KeyPath, unsigned char *pbModelPrivKey, unsigned long *cbModelPrivKey);
DTV_STATUS_T DDI_DRM_PLAYREADY_Get_GroupPrivateKey (char *KeyPath, unsigned char *pbGroupPrivKey, unsigned long *cbGroupPrivKey);

/* Adobe Access 2.0 DRM */
DTV_STATUS_T DDI_DRM_FLASHACCESS_AES_HwInit (int AES_MODE, unsigned char* pKey, unsigned char* IV, int INIT_TYPE, int padding);
DTV_STATUS_T DDI_DRM_FLASHACCESS_AES_HwUpdate (unsigned char *pOutData, int *pOutDataSize, unsigned char *pInData, int nInDataSize);
DTV_STATUS_T DDI_DRM_FLASHACCESS_AES_HwFinish (unsigned char *pOutData, int *pOutDataSize);
DTV_STATUS_T DDI_DRM_FLASHACCESS_Get_HwRandomData (int digitToGen, unsigned char *outData);
unsigned long long DDI_DRM_FLASHACCESS_Get_SecuredClock (void);
int DDI_DRM_FLASHACCESS_Read_SecuredFile (char* pFileName, unsigned char* pDecryptedData, int readSize);
int DDI_DRM_FLASHACCESS_Write_SecuredFile (char* pFileName, unsigned char* pDecryptedData, int writeSize);
long int DDI_DRM_FLASHACCESS_Get_SecuredFileLength (char *pFileName);

/* MarlinDRM*/
DTV_STATUS_T DDI_DRM_MARLIN_SetTrafficKey (const void* trafficKey);

#endif

