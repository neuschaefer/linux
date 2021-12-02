/*****************************************************************
|
|   Tomcrypt - pkcs#8 parsing missing from lib
|
|   $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/target/mt53xx_com/53xx_com_driver/dmx6896/drm/mtk_skb_driver/tomcrypt_asn1_missing.h#1 $
|   Original author:  Adam Miller
|
|   This software is provided to you pursuant to your agreement 
|   with Intertrust Technologies Corporation ("Intertrust"). 
|   This software may be used only in accordance with the terms 
|   of the agreement.
|      
|   Copyright (c) 2003-2011 by Intertrust. All rights reserved. 
|
****************************************************************/
#ifndef _TOMCRYPT_ASN1_MISSING_H_
#define _TOMCRYPT_ASN1_MISSING_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/
#include "tomcrypt.h"

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
int rsa_import_from_pkcs8(const unsigned char* in, 
                          unsigned long        inlen, 
                          rsa_key*             key);

#endif /* _TOMCRYPT_ASN1_MISSING_H_ */

