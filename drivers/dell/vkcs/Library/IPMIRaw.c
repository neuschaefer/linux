/** 
 * $Id$
 *
 *<center>
 *               Avocent Corporation. Proprietary Information.
 * \n<em>
 *      This software is supplied under the terms of a license agreement or
 *      nondisclosure agreement with Avocent Corporation, or its subsidiary, and
 *      may not be copied, disseminated, or distributed except in accordance
 *      with the terms of that agreement.
 *
 *      2001 Gateway Place, Suite 520W, San Jose, California, 95110 U.S.A.
 *\n
 *                  US phone: 408.436.6333
 *
 *        Copyright &copy; 2001-2006 Avocent Corporation.
 *</em> </center>
 *----------------------------------------------------------------------------\n
 *  MODULES     IPMIRaw(AESSLib)\n
 *----------------------------------------------------------------------------\n
 *  @file   AESSLib.c
 *  @brief  Implement AESSLib Interface and AESSLib Buffering output
 *          statemachine APIs
 *
 *  @internal
 *  $RCSfile$
 *  $Revision$
 *  $Date$
 *  $Author$
 *  $Source$
 *  $Name$
 *----------------------------------------------------------------------------*/
/******************************************************************************
* Content
* ----------------
*   AESSOpenNode()           - OpenVKCSDriverNode
*   AESSCloseNode()           - CloseVKCSDriverNode
*   AESSMethodVKCS()              - Send IPMI message use VKCS method
*   AESSIPMIMessage()              - Send/Get IPMI message API
*****************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

/* debug message open/close flag */
#define VKCS_DEBUG_CLOSE

#define VKCS_BUFFER_SIZE 40
#define VKCS_LENGTH_SIZE 1
#define VKCS_WRITE_MESSAGE 0x00
#define VKCS_READ_MESSAGE   0x01
#define VKCS_WRITE_READ_MESSAGE 0x05



#include "IPMIRaw.h"

/***************************************************************************
 * Static data declaration
 ***************************************************************************/

/* the device node of vkcs*/
static char * S_cNodeName="/dev/vkcs";

/* filde ID of vkcs*/
static int S_iFd;

/******************************************************************************
*   FUNCTION        :   AESSOpenNode
******************************************************************************/
/**
 *  @brief      Open the Node of vKCS
 *
 *  @return     AESS_DRIVER_SUCCESS,AESS_DRIVER_OPEN_ERROR
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
int AESSOpenNode(int *iFd)
{
    #ifdef VKCS_DEBUG
    printf("LIB:OpenNode iFD:%d\n",*iFd);
    #endif
    
    S_iFd = open(S_cNodeName, O_RDWR, 0);	
    if (S_iFd < 0) 
    {		
            return AESS_DRIVER_OPEN_ERROR;	
    }
    *iFd = S_iFd;
    return AESS_DRIVER_SUCCESS;	
	
}

/******************************************************************************
*   FUNCTION        :   AESSCloseNode
******************************************************************************/
/**
 *  @brief      This funciton is using to close the VKCS node
 *
 *  @return     AESS_DRIVER_SUCCESS
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
int AESSCloseNode(int *iFd)
{
    close(*iFd);
    return AESS_DRIVER_SUCCESS;
}

/******************************************************************************
*   FUNCTION        :   ChannelCheckMsgChecksum
******************************************************************************/
/**
 *  @brief      Verify the checksum of the IPMB-form message
 *
 *  @return     AESS_DRIVER_BUSY, AESS_DRIVER_SUCCESS
 *                    ,AESS_BMC_RESPONSE_TIMEOUT
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
int AESSMethodVKCS(int *iFd,unsigned char * pucBuffer)
{
    int iReturnCode = AESS_DRIVER_SUCCESS;
       
    iReturnCode = ioctl(*iFd, VKCS_WRITE_READ_MESSAGE, (unsigned int *)pucBuffer);
    
    if(0!=iReturnCode)
    {
    	return AESS_DRIVER_BUSY;
    }

    if((pucBuffer[0]==1)&&(pucBuffer[1]==0xff))
    {
        return AESS_BMC_RESPONSE_TIMEOUT;
    }

    if((pucBuffer[0]==1)&&(pucBuffer[1]==0xfe))
    {
        return AESS_DRIVER_READ_BUSY;
    }
     
    return iReturnCode;
    	
}

/******************************************************************************
*   FUNCTION        :   AESSIPMIMessage
******************************************************************************/
/**
 *  @brief      Handle IPMI message transmission
 *
 *  @return     AESS_DRIVER_SUCCESS,AESS_LENGTH_ERROR,AESS_POINTER_ERROR
 *                  AESS_METHOD_ERROR
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       None
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
int AESSIPMIMessage(int * iFd,int iMethod,int *iCommandLength,unsigned char * pucCommandBuffer,int* iResponseLength,unsigned char * pucResponseBuffer)
{
    unsigned char ucBuffer[VKCS_BUFFER_SIZE + VKCS_LENGTH_SIZE];
    int iReturnCode = AESS_DRIVER_SUCCESS;
    int iCounter;
    
    /** init ucBuffer */
    for(iCounter = 0;iCounter < VKCS_BUFFER_SIZE+VKCS_LENGTH_SIZE;iCounter++)
    {
    	ucBuffer[iCounter] = 0;
    }
    
    /** check command size */
     if((*iCommandLength > VKCS_BUFFER_SIZE)||(*iCommandLength < 2))
    {
    	return AESS_LENGTH_ERROR;
    }
    
    /** check null pointer */
    if((NULL==pucCommandBuffer)||(NULL==pucResponseBuffer))
    {
    	return 	AESS_POINTER_ERROR;
    }
    
    /** copy command to local buffer , the 1st byte of buffer is the length of command */
    ucBuffer[0] = *iCommandLength;

    #ifdef VKCS_DEBUG
    printf("LIB:Data Length :%d\n",*iCommandLength);
    #endif
    
    for(iCounter = 0;iCounter < *iCommandLength;iCounter++)
    {
        /* shift 1 byte , because the first byte is keeped the length value **/
        ucBuffer[iCounter+1] = pucCommandBuffer[iCounter];
    }
    
    
    switch(iMethod)
    {
    	case AESS_METHOD_VKCS:
    	      iReturnCode = AESSMethodVKCS(iFd,ucBuffer);

             #ifdef VKCS_DEBUG
    	      printf("LIB:enter AESSMethodVKCS iFd:%d\n",*iFd);
             #endif
             
    	      if(AESS_DRIVER_SUCCESS!=iReturnCode)
    	      {
    	      	    return iReturnCode;
    	      }
    	      break;
              
    	case AESS_METHOD_IPC:
    	default:
    	
    	iReturnCode = AESS_METHOD_ERROR;
    	
    	break;
    }
    
    /** copy response to destination buffer */
    *iResponseLength = ucBuffer[0];
         
    for(iCounter = 0;iCounter < *iResponseLength ; iCounter++)
    {
    	pucResponseBuffer[iCounter] = ucBuffer[iCounter+1];
    }

    #ifdef VKCS_DEBUG
    printf("LIB:return Data length : %d\n",*iResponseLength);
    #endif
    
    return iReturnCode;
	
}




int send_ipmi_cmd(unsigned char *send_data,int *send_data_length,unsigned char *res_buffer,int *res_data_length)
{ 
  
    int i,iReturnCode;
    int mask1=63;
    int mask2=3;
    int temp;        
    int new_send_data_length;
    unsigned char new_send_data[VKCS_BUFFER_SIZE + VKCS_LENGTH_SIZE];
    int iFd;
            
    temp = ((send_data[1] & mask1)<<2)|(send_data[2] & mask2);
    new_send_data[0] = temp;
    new_send_data_length =  *send_data_length - 2;
        
    if (new_send_data_length < 2)
    {
        #ifdef FIGO_DEBUG
        printf("SSH: new data length %d\n",new_send_data_length);
        #endif
         return STATUS_FAIL;
    }
    
    for(i=1;i< new_send_data_length ;i++)
    {
        new_send_data[i] = send_data[i+2];
    }
    AESSOpenNode(&iFd);
    iReturnCode =AESSIPMIMessage(&iFd,AESS_METHOD_VKCS,&new_send_data_length,new_send_data,res_data_length,res_buffer);  
    AESSCloseNode(&iFd);
    if(iReturnCode!=AESS_DRIVER_SUCCESS)
    {
        printf("SSHD:Error Code : %d\n",iReturnCode);
        return STATUS_FAIL;
    }   
     
    return STATUS_OK;
}

/* End of code */
