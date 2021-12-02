//-----------------------------------------------------------------------------
// $Header: 
// (C) Copyright 2001 NXP Semiconductors, All rights reserved
//
// This source code and any compilation or derivative thereof is the sole
// property of NXP Corporation and is provided pursuant to a Software
// License Agreement.  This code is the proprietary information of NXP
// Corporation and is confidential in nature.  Its use and dissemination by
// any party other than NXP Corporation is strictly limited by the
// confidential information provisions of the Agreement referenced above.
//-----------------------------------------------------------------------------
// FILE NAME:    tmbslTDA182I2.c
//
// %version: 25 %
//
// DESCRIPTION:  Function for the Hybrid silicon tuner TDA182I2
//
// DOCUMENT REF: 
//
// NOTES:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// Standard include files:
//-----------------------------------------------------------------------------
//

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

#include "tmddTDA182I2.h"

#ifdef TMBSL_TDA18272
 #include "tmbslTDA18272.h"
#else /* TMBSL_TDA18272 */
 #include "tmbslTDA18212.h"
#endif /* TMBSL_TDA18272 */

//-----------------------------------------------------------------------------
// Project include files:
//-----------------------------------------------------------------------------
//
#include "tmbslTDA182I2local.h"
#include "tmbslTDA182I2Instance.h"

//-----------------------------------------------------------------------------
// Types and defines:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// Global data:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// Exported functions:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslTDA18211Init:
//
// DESCRIPTION: create an instance of a TDA182I2 Tuner
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TM_OK
//  
// NOTES:
//-----------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2Init
(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if (psSrvFunc == Null)
    {
        err = TDA182I2_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        //----------------------
        // initialize the Object
        //----------------------
        // pObj initialization
        err = TDA182I2GetInstance(tUnit, &pObj);
		if (err != TM_OK)
			{
                 mcSHOW_DBG_MSG(("TDA182I2GetInstance: need allcate!\n"));
		   }
		else
		  {
                      
	     mcSHOW_DBG_MSG(("TDA182I2GetInstance:success !\n"));

		  }
				
    }

    /* check driver state */
    if (err == TM_OK || err == TDA182I2_ERR_NOT_INITIALIZED)
    {
        if (pObj != Null && pObj->init == True)
        {
            err = TDA182I2_ERR_NOT_INITIALIZED;
        }
        else 
        {
            /* initialize the Object */
            if (pObj == Null)
            {
                err = TDA182I2AllocInstance(tUnit, &pObj);
                if (err != TM_OK || pObj == Null)
                {
                    err = TDA182I2_ERR_NOT_INITIALIZED;        
                }
            }

            if (err == TM_OK)
            {
                // initialize the Object by default values
                pObj->sRWFunc = psSrvFunc->sIo;
                pObj->sTime = psSrvFunc->sTime;
                pObj->sDebug = psSrvFunc->sDebug;

                if(  psSrvFunc->sMutex.Init != Null
                    && psSrvFunc->sMutex.DeInit != Null
                    && psSrvFunc->sMutex.Acquire != Null
                    && psSrvFunc->sMutex.Release != Null)
                {
                    pObj->sMutex = psSrvFunc->sMutex;

                    err = pObj->sMutex.Init(&pObj->pMutex);
                }

                pObj->init = True;
                err = TM_OK;

                err = tmddTDA182I2Init(tUnit, psSrvFunc);
				if (err != TM_OK)
					{
                       mcSHOW_USER_MSG(("tmddTDA182I2Init fail !\n"));
				    }
				else
					{
                      
					  mcSHOW_USER_MSG(("tmddTDA182I2Init success !\n"));

				    }
				
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Init(0x%08X) failed.", pObj->tUnit));
            }
        }
    }
	mcSHOW_USER_MSG(("tmbslTDA182I2Init success !\n"));

    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2DeInit:
//
// DESCRIPTION: destroy an instance of a TDA182I2 Tuner
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-----------------------------------------------------------------------------
//
tmErrorCode_t 
tmbslTDA182I2DeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2DeInit(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2DeInit(0x%08X) failed.", pObj->tUnit));

        (void)TDA182I2MutexRelease(pObj);

        if(pObj->sMutex.DeInit != Null)
        {
            if(pObj->pMutex != Null)
            {
                err = pObj->sMutex.DeInit(pObj->pMutex);
            }

            pObj->sMutex.Init = Null;
            pObj->sMutex.DeInit = Null;
            pObj->sMutex.Acquire = Null;
            pObj->sMutex.Release = Null;

            pObj->pMutex = Null;
        }
    }

    err = TDA182I2DeAllocInstance(tUnit);

    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2GetSWVersion:
//
// DESCRIPTION: Return the version of this device
//
// RETURN:      TM_OK
//
// NOTES:       Values defined in the tmTDA182I2local.h file
//-----------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2GetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
)
{
    tmErrorCode_t   err = TDA182I2_ERR_NOT_INITIALIZED;
    
    err = tmddTDA182I2GetSWVersion(pSWVersion);
    
    return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2CheckHWVersion:
//
// DESCRIPTION: Check HW version
//
// RETURN:      TM_OK if no error
//
// NOTES:       Values defined in the tmTDA182I2local.h file
//-----------------------------------------------------------------------------
tmErrorCode_t
tmbslTDA182I2CheckHWVersion
(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TDA182I2_ERR_NOT_INITIALIZED;
    UInt16              uIdentity = 0;
    UInt8               majorRevision = 0;

    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2GetIdentity(tUnit, &uIdentity);

        if(err == TM_OK)
        {
            if(uIdentity == 18272 || uIdentity == 18212)
            {
                /* TDA18272/12 found. Check Major Revision*/
                err = tmddTDA182I2GetMajorRevision(tUnit, &majorRevision);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetMajorRevision(0x%08X) failed.", tUnit));

                if(err == TM_OK && majorRevision != 1)
                {
                    /* Only TDA18272/12 ES2 are supported */
                    err = TDA182I2_ERR_BAD_VERSION;
                }
            }
            else
            {
                err = TDA182I2_ERR_BAD_VERSION;
            }
        }

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2SetPowerState:
//
// DESCRIPTION: Set the power state of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:       
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2SetPowerState
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmTDA182I2PowerState_t  powerState  /* I: Power state of this device */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
	 if (err != TM_OK)
	 	{
            mcSHOW_DBG_MSG(("TDA182I2GetInstance failed.\n"));	
	    }
     else
     	{
            mcSHOW_DBG_MSG(("TDA182I2GetInstance success.\n"));	
	    }
    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // Call tmddTDA182I2SetPowerState
        err = tmddTDA182I2SetPowerState(tUnit,(tmddTDA182I2PowerState_t) powerState);
		 if (err != TM_OK)
	 	{
           mcSHOW_DBG_MSG(("tmddTDA182I2SetPowerState failed.\n"));	
	    }
		 else
		 {
            mcSHOW_DBG_MSG(("tmddTDA182I2SetPowerState success.\n"));	

		  }

        if(err == TM_OK)
        {
            // set power state
            pObj->curPowerState = powerState;
        }

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2GetPowerState:
//
// DESCRIPTION: Get the power state of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2GetPowerState
(
    tmUnitSelect_t          tUnit,          /* I: Unit number */
    tmTDA182I2PowerState_t  *pPowerState    /* O: Power state of this device */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if(pPowerState == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        // pObj initialization
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // get power state
        *pPowerState = pObj->curPowerState;

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2SetStandardMode:
//
// DESCRIPTION: Set the standard mode of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:       
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2SetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number */
    tmTDA182I2StandardMode_t    StandardMode    /* I: Standard mode of this device */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // store standard mode 
        pObj->StandardMode = StandardMode;

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2GetStandardMode:
//
// DESCRIPTION: Get the standard mode of this device.
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2GetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number */
    tmTDA182I2StandardMode_t    *pStandardMode  /* O: Standard mode of this device */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if(pStandardMode == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        // pObj initialization
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        /* Get standard mode */
        *pStandardMode = pObj->StandardMode;

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2SetRf:
//
// DESCRIPTION: Calculate i2c I2CMap & write in TDA182I2
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TDA182I2_ERR_BAD_PARAMETER
//              TMBSL_ERR_IIC_ERR
//              TM_OK
//
// NOTES:       
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2SetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32          uRF     /* I: RF frequency in hertz */
)
{    
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;
    Bool                bIRQWait = True;
    /* ---------- ES2 PATCH BEGIN ---------- */
    UInt8               majorRevision = 0;
    UInt8               minorRevision = 0;
    /* ---------- ES2 PATCH END ---------- */
    UInt8   ratioL, ratioH;  /* patch Spurious */
    UInt32  DeltaL, DeltaH; /* patch Spurious */

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2GetIRQWait(tUnit, &bIRQWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetIRQWait(0x%08X) failed.", tUnit));

        pObj->uRF = uRF;

        if(err == TM_OK)
        {
            /* Set LPF */
            err = tmddTDA182I2SetLP_FC(tUnit, pObj->Std_Array[pObj->StandardMode].LPF);
			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetLP_FC failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetLP_FC success!\n"));
			    }
			
           
        }
 
        if(err == TM_OK)
        {
            /* Set LPF Offset */
            err = tmddTDA182I2SetLP_FC_Offset(tUnit, pObj->Std_Array[pObj->StandardMode].LPF_Offset);
			
			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetLP_FC_Offset failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetLP_FC_Offset success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set IF Gain */
            err = tmddTDA182I2SetIF_Level(tUnit, pObj->Std_Array[pObj->StandardMode].IF_Gain);

			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_Level failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_Level success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set IF Notch */
            err = tmddTDA182I2SetIF_ATSC_Notch(tUnit, pObj->Std_Array[pObj->StandardMode].IF_Notch);
			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_ATSC_Notch failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_ATSC_Notch success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Enable/disable HPF */
            if ( pObj->Std_Array[pObj->StandardMode].IF_HPF == tmTDA182I2_IF_HPF_Disabled )
            {
                err = tmddTDA182I2SetHi_Pass(tUnit, 0);
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetHi_Pass failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetHi_Pass success!\n"));
			    }
               
            }
            else
            {
                err = tmddTDA182I2SetHi_Pass(tUnit, 1); 
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetHi_Pass failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetHi_Passs success!\n"));
			    }
               
              

                if(err == TM_OK)
                {
                    /* Set IF HPF */
                    err = tmddTDA182I2SetIF_HP_Fc(tUnit, (UInt8)(pObj->Std_Array[pObj->StandardMode].IF_HPF - 1));
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_HP_Fc failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_HP_Fc success!\n"));
			    }
                    
                }
            }
        }

        if(err == TM_OK)
        {
            /* Set DC Notch */
            err = tmddTDA182I2SetIF_Notch(tUnit, pObj->Std_Array[pObj->StandardMode].DC_Notch);
			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_Notch failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_Notch success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set AGC1 LNA Top */
            err = tmddTDA182I2SetAGC1_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC1_LNA_TOP);
			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC1_TOP failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC1_TOP success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set AGC2 RF Top */
            err = tmddTDA182I2SetAGC2_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC2_RF_Attenuator_TOP);
			if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC2_TOP failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC2_TOP success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set AGC3 RF AGC Top */
            if (pObj->uRF < tmTDA182I2_AGC3_RF_AGC_TOP_FREQ_LIM)
            {
                err = tmddTDA182I2SetRFAGC_Top(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_RF_AGC_TOP_Low_band);
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetRFAGC_Top failed!\n"));
				}
			else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetRFAGC_Top success!\n"));
			    }
           
              
            }
            else
            {
                err = tmddTDA182I2SetRFAGC_Top(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_RF_AGC_TOP_High_band);
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetRFAGC_Top failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetRFAGC_Top success!\n"));
			    }
           
            
            }
        }

        if(err == TM_OK)
        {
            /* Set AGC4 IR Mixer Top */
            err = tmddTDA182I2SetIR_Mixer_Top(tUnit, pObj->Std_Array[pObj->StandardMode].AGC4_IR_Mixer_TOP);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetIR_Mixer_Top failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetIR_Mixer_Top success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set AGC5 IF AGC Top */
            err = tmddTDA182I2SetAGC5_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC5_IF_AGC_TOP);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC5_TOP failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC5_TOP success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set AGC3 Adapt */
            err = tmddTDA182I2SetPD_RFAGC_Adapt(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_Adapt);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetPD_RFAGC_Adapt failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetPD_RFAGC_Adapt success!\n"));
			    }
            
        }
    
        if(err == TM_OK)
        {
            /* Set AGC3 Adapt TOP */
            err = tmddTDA182I2SetRFAGC_Adapt_TOP(tUnit, pObj->Std_Array[pObj->StandardMode].AGC3_Adapt_TOP);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetRFAGC_Adapt_TOP failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetRFAGC_Adapt_TOP success!\n"));
			    }
          
        }

        if(err == TM_OK)
        {
            /* Set AGC5 Atten 3dB */
            err = tmddTDA182I2SetRF_Atten_3dB(tUnit, pObj->Std_Array[pObj->StandardMode].AGC5_Atten_3dB);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetRF_Atten_3dB failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetRF_Atten_3dB success!\n"));
			    }
            
        }

        if(err == TM_OK)
        {
            /* Set AGC5 Detector HPF */
            err = tmddTDA182I2SetAGC5_Ana(tUnit, pObj->Std_Array[pObj->StandardMode].AGC5_Detector_HPF);
			  if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC5_Ana failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetAGC5_Ana success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set AGCK Mode */
            err = tmddTDA182I2SetAGCK_Mode(tUnit, pObj->Std_Array[pObj->StandardMode].GSK&0x03);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetAGCK_Mode failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetAGCK_Mode success!\n"));
			    }
            

            err = tmddTDA182I2SetAGCK_Step(tUnit, (pObj->Std_Array[pObj->StandardMode].GSK&0x0C)>>2);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetAGCK_Step failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetAGCK_Step success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set H3H5 VHF Filter 6 */
            err = tmddTDA182I2SetPSM_StoB(tUnit, pObj->Std_Array[pObj->StandardMode].H3H5_VHF_Filter6);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetPSM_StoB failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetPSM_StoB success!\n"));
			    }
           
        }

        if(err == TM_OK)
        {
            /* Set IF */
            err = tmddTDA182I2SetIF_Freq(tUnit, pObj->Std_Array[pObj->StandardMode].IF - pObj->Std_Array[pObj->StandardMode].CF_Offset);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_Freq failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetIF_Freq success!\n"));
			    }
          
        }

        /* ---------- ES2 PATCH BEGIN ---------- */
        if(err == TM_OK)
        {
            err = tmddTDA182I2GetMajorRevision(tUnit, &majorRevision);
			  if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2GetMajorRevision failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2GetMajorRevision success!\n"));
			    }
      

            err = tmddTDA182I2GetMinorRevision(tUnit, &minorRevision);
			  if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2GetMinorRevision failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2GetMinorRevision success!\n"));
			    }
           
        }

        if(err == TM_OK && majorRevision == 1 && minorRevision == 0)
        {
            /* Set LPF Gain */
            err = tmddTDA182I2SetLPF_Gain_Mode(tUnit, pObj->Std_Array[pObj->StandardMode].LPF_Gain);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetLPF_Gain_Mode failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetLPF_Gain_Mode success!\n"));
			    }
          
        }
        /* ---------- ES2 PATCH END ---------- */

        if(err == TM_OK)
        {
            /* Set RF */
            err = tmddTDA182I2SetRF_Freq(tUnit, uRF + pObj->Std_Array[pObj->StandardMode].CF_Offset);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetRF_Freq failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetRF_Freq success!\n"));
			    }
           
            
            /* PATCH ES2 begin : in case of timeout WAIT IRQ */
            if(err == ddTDA182I2_ERR_NOT_READY  && bIRQWait && majorRevision == 1 && minorRevision == 0)
            {
                err = tmddTDA182I2Reset(tUnit);
			   if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2Reset failed!\n"));
				}
			   else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2Reset success!\n"));
			    }
              

                err = tmddTDA182I2SetRF_Freq(tUnit, uRF + pObj->Std_Array[pObj->StandardMode].CF_Offset);
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetRF_Freq failed!\n"));
				}
			    else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetRF_Freq success!\n"));
			    }
			
            }
            /* PATCH ES2 end */
        }
        if(err == TM_OK)
        {
            /*  patch Spurious begin */
            ratioL = (UInt8)(uRF / 16000000);
            ratioH = (UInt8)(uRF / 16000000) + 1;
            DeltaL = (uRF - (ratioL * 16000000));
            DeltaH = ((ratioH * 16000000) - uRF);

            if (uRF < 72000000 )
            {
                /* Set sigma delta clock*/
                err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 1);
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				}
			    else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			    }
                                   
            }
            else if (uRF < 104000000 )
            {
                /* Set 16 Mhz Xtal clock */
                err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 0);
			    if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				}
			    else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			    }
               
            }
            else if (uRF <= 120000000 )
            {
                /* Set sigma delta clock*/
                err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 1);
				if (err != TM_OK)
				{
			      mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				}
			    else
				{
                  mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			    }
                                 
            }
            else  /* RF above 120 MHz */
            {
                if  (DeltaL <= DeltaH )  
                {
                    if (ratioL & 0x000001 )  /* ratioL odd */
                    {
                        /* Set 16 Mhz Xtal clock */
                        err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 0);
						if (err != TM_OK)
				        {
			              mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				        }
			            else
				        {
                          mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			            }
                       
                    }
                    else /* ratioL even */
                    {
                        /* Set sigma delta clock*/
                        err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 1);
						if (err != TM_OK)
				        {
			              mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				        }
			            else
				        {
                          mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			            }
                       
                    }
                    
                }
                else  /* (DeltaL > DeltaH ) */
                {
                    if (ratioL & 0x000001 )  /*(ratioL odd)*/
                    {
                        /* Set sigma delta clock*/
                        err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 1);
						if (err != TM_OK)
				        {
			              mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				        }
			            else
				        {
                          mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			            }
                      
                    }
                    else
                    {
                        /* Set 16 Mhz Xtal clock */
                        err = tmddTDA182I2SetDigital_Clock_Mode(tUnit, 0);
						if (err != TM_OK)
				        {
			              mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode failed!\n"));
				        }
			            else
				        {
                          mcSHOW_DBG_MSG(("tmddTDA182I2SetDigital_Clock_Mode success!\n"));
			            }
                        
                    }
                }
            }
        }
        /*  patch Spurious end */
        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}


//-------------------------------------------------------------------------------------
// FUNCTION:    tmbslTDA182I2GetRf:
//
// DESCRIPTION: Get the frequency programmed in the tuner
//
// RETURN:      TMBSL_ERR_TUNER_BAD_UNIT_NUMBER
//              TDA182I2_ERR_NOT_INITIALIZED
//              TM_OK
//
// NOTES:       The value returned is the one stored in the Object
//-------------------------------------------------------------------------------------
//
tmErrorCode_t
tmbslTDA182I2GetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puRF    /* O: RF frequency in hertz */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    if(puRF == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        /* Get RF */
        *puRF = pObj->uRF/* - pObj->Std_Array[pObj->StandardMode].CF_Offset*/;

        (void)TDA182I2MutexRelease(pObj);
    }
    return err;
}

/*============================================================================*/
/* tmbslTDA182I2Reset                                                         */
/*============================================================================*/
tmErrorCode_t
tmbslTDA182I2Reset
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    Bool                bIRQWait = True;
    /* ---------- ES2 PATCH BEGIN ---------- */
    UInt8               majorRevision = 0;
    UInt8               minorRevision = 0;
    /* ---------- ES2 PATCH END ---------- */

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2GetIRQWait(tUnit, &bIRQWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetIRQWait(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2Init(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2Init(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2GetMajorRevision(tUnit, &majorRevision);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetMajorRevision(0x%08X) failed.", tUnit));

        err = tmddTDA182I2GetMinorRevision(tUnit, &minorRevision);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetMinorRevision(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        // initialize tuner
        err =  tmddTDA182I2Reset(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Reset(0x%08X) failed.", tUnit));


        /* PATCH ES2 begin : in case of timeout WAIT IRQ */
        if(err == ddTDA182I2_ERR_NOT_READY && bIRQWait && majorRevision == 1 && minorRevision == 0)
        {
            err = tmddTDA182I2Reset(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Reset(0x%08X) failed.", tUnit));
        }
        /* PATCH ES2 end */

        /****** Begin settings V3.0.1 *********/
        
#ifdef TMBSL_TDA18272
        /* LT_Enable  N     0*/
        if (err == TM_OK)
        {
            err = tmddTDA182I2SetLT_Enable(tUnit, 0 );
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2SetLT_Enable(0x%08X, 0) failed.", tUnit));
        }
        /* PSM AGC1   -5,9 mA     1*/
        if (err == TM_OK)
        {
            err = tmddTDA182I2SetPSM_AGC1(tUnit, 1 );
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2SetPSM_AGC1(0x%08X, 1) failed.", tUnit));
        }

        /* AGC1 gain range  [-12, +15dB]      0*/
        if (err == TM_OK)
        {
            err = tmddTDA182I2SetAGC1_6_15dB(tUnit, 0 );
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2SetAGC1_6_15dB(0x%08X, 0) failed.", tUnit));
        }
        /****** End settings V3.0.1 *********/
#endif


    }
    (void)TDA182I2MutexRelease(pObj);

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetIF                                                         */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetIF
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(puIF == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;
    
    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *puIF = pObj->Std_Array[pObj->StandardMode].IF - pObj->Std_Array[pObj->StandardMode].CF_Offset;

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetCF_Offset                                                  */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetCF_Offset(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32*         puOffset    /* O: Center frequency offset in hertz */
    )
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(puOffset == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;
    
    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *puOffset = pObj->Std_Array[pObj->StandardMode].CF_Offset;

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetLockStatus                                                 */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetLockStatus
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uValue, uValueLO;

    if(pLockStatus == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }
    if(err == TM_OK)
    {
        err =  tmddTDA182I2GetLO_Lock(tUnit, &uValueLO);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetLO_Lock(0x%08X) failed.", tUnit));
    }
    if(err == TM_OK)
    {
        err =  tmddTDA182I2GetIRQ_status(tUnit, &uValue);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetIRQ_status(0x%08X) failed.", tUnit));

        uValue = uValue & uValueLO;
    }
    if(err == TM_OK)
    {
        *pLockStatus =  (uValue)? tmbslFrontEndStateLocked : tmbslFrontEndStateNotLocked;
    }
    else
    {
        *pLockStatus = tmbslFrontEndStateUnknown;  
    }

    (void)TDA182I2MutexRelease(pObj);

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetPowerLevel                                                 */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetPowerLevel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32*         pPowerLevel /* O: Power Level in dBµV */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pPowerLevel == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pPowerLevel = 0;

        err = tmddTDA182I2GetPower_Level(tUnit, (UInt8 *)pPowerLevel);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetPower_Level(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }
    return err;
}

/*============================================================================*/
/* tmbslTDA182I2SetIRQWait                                                  */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2SetIRQWait
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    Bool            bWait   /* I: Determine if we need to wait IRQ in driver functions */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2SetIRQWait(tUnit, bWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2SetIRQWait(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }
    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetIRQWait                                                  */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetIRQWait
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    Bool*           pbWait  /* O: Determine if we need to wait IRQ in driver functions */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pbWait == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2GetIRQWait(tUnit, pbWait);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetIRQWait(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetIRQ                                                        */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetIRQ
(
    tmUnitSelect_t  tUnit  /* I: Unit number */,
    Bool*           pbIRQ  /* O: IRQ triggered */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pbIRQ == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pbIRQ = 0;

        err = tmddTDA182I2GetIRQ_status(tUnit, (UInt8 *)pbIRQ);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetIRQ_status(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2WaitIRQ                                                       */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2WaitIRQ
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          timeOut,    /* I: timeOut for IRQ wait */
    UInt32          waitStep,   /* I: wait step */
    UInt8           irqStatus   /* I: IRQs to wait */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2WaitIRQ(tUnit, timeOut, waitStep, irqStatus);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2WaitIRQ(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2GetXtalCal_End                                                */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2GetXtalCal_End
(
    tmUnitSelect_t  tUnit  /* I: Unit number */,
    Bool*           pbXtalCal_End  /* O: XtalCal_End triggered */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(pbXtalCal_End == Null)
        err = TDA182I2_ERR_BAD_PARAMETER;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        *pbXtalCal_End = 0;

        err = tmddTDA182I2GetMSM_XtalCal_End(tUnit, (UInt8 *)pbXtalCal_End);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2GetMSM_XtalCal_End(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2WaitXtalCal_End                                               */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2WaitXtalCal_End
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          timeOut,    /* I: timeOut for IRQ wait */
    UInt32          waitStep   /* I: wait step */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2WaitXtalCal_End(tUnit, timeOut, waitStep);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2WaitXtalCal_End(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2SoftReset                                                     */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2SoftReset
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        err = tmddTDA182I2SoftReset(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2SoftReset(0x%08X) failed.", tUnit));

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2CheckRFFilterRobustness                                       */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2CheckRFFilterRobustness 
(
 tmUnitSelect_t                         tUnit,      /* I: Unit number */
 ptmTDA182I2RFFilterRating              rating      /* O: RF Filter rating */
 )
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    if(err == TM_OK)
    {
        err = TDA182I2GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        UInt8  rfcal_log_0 = 0;
        UInt8  rfcal_log_2 = 0;
        UInt8  rfcal_log_3 = 0;
        UInt8  rfcal_log_5 = 0;
        UInt8  rfcal_log_6 = 0;
        UInt8  rfcal_log_8 = 0;
        UInt8  rfcal_log_9 = 0;
        UInt8  rfcal_log_11 = 0;

        double   VHFLow_0 = 0;
        double   VHFLow_1 = 0;
        double   VHFHigh_0 = 0;
        double   VHFHigh_1 = 0;
        double   UHFLow_0 = 0;
        double   UHFLow_1 = 0;
        double   UHFHigh_0 = 0;
        double   UHFHigh_1 = 0;

        err = tmddTDA182I2Getrfcal_log_0(tUnit, &rfcal_log_0);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_0(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_2(tUnit, &rfcal_log_2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_2(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_3(tUnit, &rfcal_log_3);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_3(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_5(tUnit, &rfcal_log_5);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_5(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_6(tUnit, &rfcal_log_6);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_6(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_8(tUnit, &rfcal_log_8);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_8(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_9(tUnit, &rfcal_log_9);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_9(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        {
            err = tmddTDA182I2Getrfcal_log_11(tUnit, &rfcal_log_11);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA182I2Getrfcal_log_11(0x%08X) failed.", tUnit));
        }

        if(err == TM_OK)
        { 
            // VHFLow_0
            VHFLow_0 = 100 * (45 - 39.8225 * (1 + (0.31 * (rfcal_log_0 < 64 ? rfcal_log_0 : rfcal_log_0 - 128)) / 1.0 / 100.0)) / 45.0;
            rating->VHFLow_0_Margin = 0.0024 * VHFLow_0 * VHFLow_0 * VHFLow_0 - 0.101 * VHFLow_0 * VHFLow_0 + 1.629 * VHFLow_0 + 1.8266;
            if (rating->VHFLow_0_Margin >= 0) 
            {
                rating->VHFLow_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->VHFLow_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // VHFLow_1
            VHFLow_1 = 100 * (152.1828 * (1 + (1.53 * (rfcal_log_2 < 64 ? rfcal_log_2 : rfcal_log_2 - 128)) / 1.0 / 100.0) - (144.896 - 6)) / (144.896 - 6); 
            rating->VHFLow_1_Margin = 0.0024 * VHFLow_1 * VHFLow_1 * VHFLow_1 - 0.101 * VHFLow_1 * VHFLow_1 + 1.629 * VHFLow_1 + 1.8266;
            if (rating->VHFLow_1_Margin >= 0)
            {
                rating->VHFLow_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->VHFLow_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // VHFHigh_0    
            VHFHigh_0 = 100 * ((144.896 + 6) - 135.4063 * (1 + (0.27 * (rfcal_log_3 < 64 ? rfcal_log_3 : rfcal_log_3 - 128)) / 1.0 / 100.0)) / (144.896 + 6);
            rating->VHFHigh_0_Margin = 0.0024 * VHFHigh_0 * VHFHigh_0 * VHFHigh_0 - 0.101 * VHFHigh_0 * VHFHigh_0 + 1.629 * VHFHigh_0 + 1.8266;
            if (rating->VHFHigh_0_Margin >= 0)
            {
                rating->VHFHigh_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->VHFHigh_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // VHFHigh_1
            VHFHigh_1 = 100 * (383.1455 * (1 + (0.91 * (rfcal_log_5 < 64 ? rfcal_log_5 : rfcal_log_5 - 128)) / 1.0 / 100.0) - (367.104 - 8)) / (367.104 - 8);
            rating->VHFHigh_1_Margin = 0.0024 * VHFHigh_1 * VHFHigh_1 * VHFHigh_1 - 0.101 * VHFHigh_1 * VHFHigh_1 + 1.629 * VHFHigh_1 + 1.8266;
            if (rating->VHFHigh_1_Margin >= 0)
            {
                rating->VHFHigh_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->VHFHigh_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // UHFLow_0
            UHFLow_0 = 100 * ((367.104 + 8) - 342.6224 * (1 + (0.21 * (rfcal_log_6 < 64 ? rfcal_log_6 : rfcal_log_6 - 128)) / 1.0 / 100.0)) / (367.104 + 8);
            rating->UHFLow_0_Margin = 0.0024 * UHFLow_0 * UHFLow_0 * UHFLow_0 - 0.101 * UHFLow_0 * UHFLow_0 + 1.629 * UHFLow_0 + 1.8266;
            if (rating->UHFLow_0_Margin >= 0)
            {
                rating->UHFLow_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->UHFLow_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // UHFLow_1
            UHFLow_1 = 100 * (662.5595 * (1 + (0.33 * (rfcal_log_8 < 64 ? rfcal_log_8 : rfcal_log_8 - 128)) / 1.0 / 100.0) - (624.128 - 2)) / (624.128 - 2);
            rating->UHFLow_1_Margin = 0.0024 * UHFLow_1 * UHFLow_1 * UHFLow_1 - 0.101 * UHFLow_1 * UHFLow_1 + 1.629 * UHFLow_1 + 1.8266;
            if (rating->UHFLow_1_Margin >= 0)
            {
                rating->UHFLow_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->UHFLow_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // UHFHigh_0
            UHFHigh_0 = 100 * ((624.128 + 2) - 508.2747 * (1 + (0.23 * (rfcal_log_9 < 64 ? rfcal_log_9 : rfcal_log_9 - 128)) / 1.0 / 100.0)) / (624.128 + 2);
            rating->UHFHigh_0_Margin = 0.0024 * UHFHigh_0 * UHFHigh_0 * UHFHigh_0 - 0.101 * UHFHigh_0 * UHFHigh_0 + 1.629 * UHFHigh_0 + 1.8266;
            if (rating->UHFHigh_0_Margin >= 0)
            {
                rating->UHFHigh_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->UHFHigh_0_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }

            // UHFHigh_1
            UHFHigh_1 = 100 * (947.8913 * (1 + (0.3 * (rfcal_log_11 < 64 ? rfcal_log_11 : rfcal_log_11 - 128)) / 1.0 / 100.0) - (866 - 14)) / (866 - 14);
            rating->UHFHigh_1_Margin = 0.0024 * UHFHigh_1 * UHFHigh_1 * UHFHigh_1 - 0.101 * UHFHigh_1 * UHFHigh_1 + 1.629 * UHFHigh_1 + 1.8266;            
            if (rating->UHFHigh_1_Margin >= 0)
            {
                rating->UHFHigh_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_High;
            }
            else
            {
                rating->UHFHigh_1_RFFilterRobustness =  tmTDA182I2RFFilterRobustness_Low;
            }
        }

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2Write                                                         */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2Write
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          uIndex,     /* I: Start index to write */
    UInt32          WriteLen,   /* I: Number of bytes to write */
    UInt8*          pData       /* I: Data to write */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // Call tmddTDA182I2Write

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2Read                                                          */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2Read
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          uIndex,     /* I: Start index to read */
    UInt32          ReadLen,    /* I: Number of bytes to read */
    UInt8*          pData       /* I: Data to read */
)
{
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        // Call tmddTDA182I2Read

        (void)TDA182I2MutexRelease(pObj);
    }

    return err;
}

//-----------------------------------------------------------------------------
// Internal functions:
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// FUNCTION:    TDA182I2Init:
//
// DESCRIPTION: initialization of the Tuner
//
// RETURN:      always True
//
// NOTES:       
//-----------------------------------------------------------------------------
//
static tmErrorCode_t
TDA182I2Init
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{     
    ptmTDA182I2Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    //------------------------------
    // test input parameters
    //------------------------------
    // pObj initialization
    err = TDA182I2GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA182I2GetInstance(0x%08X) failed.", tUnit));

    if(err == TM_OK)
    {
        //err = tmddTDA182I2SetIRQWait(tUnit, True);

        //if(pObj->bIRQWait)
        //{
        //    err = TDA182I2WaitIRQ(pObj);
        //}
    }

    return err;
}

//-------------------------------------------------------------------------------------
// FUNCTION:    TDA182I2Wait
//
// DESCRIPTION: This function waits for requested time
//
// RETURN:      True or False
//
// NOTES:       
//-------------------------------------------------------------------------------------
//

/*static tmErrorCode_t 
TDA182I2Wait
(
    ptmTDA182I2Object_t pObj,   // I: Driver object 
    UInt32              Time   //  I: Time to wait for 
)
{
    tmErrorCode_t   err  = TM_OK;

    // wait Time ms
    err = POBJ_SRVFUNC_STIME.Wait(pObj->tUnit, Time);

    // Return value
    return err;
}*/

/*============================================================================*/
/* TDA182I2MutexAcquire                                                       */
/*============================================================================*/
extern tmErrorCode_t
TDA182I2MutexAcquire
(
 ptmTDA182I2Object_t    pObj,
 UInt32                 timeOut
 )
{
    tmErrorCode_t       err = TM_OK;

    if(pObj->sMutex.Acquire != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Acquire(pObj->pMutex, timeOut);
    }

    return err;
}

/*============================================================================*/
/* TDA182I2MutexRelease                                                       */
/*============================================================================*/
extern tmErrorCode_t
TDA182I2MutexRelease
(
 ptmTDA182I2Object_t    pObj
 )
{
    tmErrorCode_t       err = TM_OK;

    if(pObj->sMutex.Release != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Release(pObj->pMutex);
    }

    return err;
}

/*============================================================================*/
/* tmbslTDA182I2WriteTuner_Register                                                    */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2WriteTuner_Register
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uSubAddress,    /* I: sub address */
    UInt8           uValue  /* I: Item value */
)
{
	ptmTDA182I2Object_t pObj = Null;
		tmErrorCode_t		err  = TM_OK;
	
		// pObj initialization
		err = TDA182I2GetInstance(tUnit, &pObj);
		if (err != TM_OK)
			{
              mcSHOW_DBG_MSG(( "TDA182I2GetInstance(0x%08X) failed.",tUnit));
		    }
	
		if(err == TM_OK)
		{
			err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
		}
	
		if(err == TM_OK)
		{
			err = tmddTDA182I2WriteTuner_Register(tUnit, uSubAddress, uValue);
			if (err != TM_OK)
			{
              mcSHOW_DBG_MSG(( " tmbslTDA182I2WriteTuner_Register(0x%08X) failed.", uSubAddress));
		    }
	        
			(void)TDA182I2MutexRelease(pObj);
		}
	
		return err;


}

/*============================================================================*/
/* tmbslTDA182I2ReadTuner_Register                                                   */
/*============================================================================*/

tmErrorCode_t
tmbslTDA182I2ReadTuner_Register
(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt8           uSubAddress,    /* I: sub address */
    UInt8*           pValue  /* I: Item value */
)
{

   ptmTDA182I2Object_t pObj = Null;
   tmErrorCode_t	   err	= TM_OK;

   // pObj initialization
   err = TDA182I2GetInstance(tUnit, &pObj);
   if (err != TM_OK)
	   {
           mcSHOW_DBG_MSG(( "TDA182I2GetInstance(0x%08X) failed.",tUnit));
	   }
   else
       {
           mcSHOW_DBG_MSG(( "TDA182I2GetInstance(0x%08X) success.",tUnit));
       }

   if(err == TM_OK)
   {
	   err = TDA182I2MutexAcquire(pObj, TDA182I2_MUTEX_TIMEOUT);
   }

   if(err == TM_OK)
   {
	   err = tmddTDA182I2ReadTuner_Register(tUnit, uSubAddress, pValue);
			if (err != TM_OK)
			{
              mcSHOW_DBG_MSG(( " tmddTDA182I2ReadTuner_Register(0x%08X) failed.", uSubAddress));
		    }

	   (void)TDA182I2MutexRelease(pObj);
   }

   return err;


}
