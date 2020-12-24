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
 *  MODULES     VKCSDriver (VKCSDrivers)\n
 *----------------------------------------------------------------------------\n
 *  @file   VKCSDriver.c
 *  @brief  Implement VKCSDriver Interface and VKCSDriver Buffering output/input
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
*   VKCSTimerCallBack()           - BMC response time out call back function
*   VKCSInit()            - Calculate and fill the check sum into
*                                       - the IPMB-form message.
*   VKCSExit()              - Decode IPMB-form message from byte
*                                       - stream to IPMI msg structure.
*   VKCSSoftwareBMCSetStatus()              - Encode IPMB-form message from IPMI
*                                       - msg structure to byte stream.
*   VKCSSoftwareBMCGetStatus()          - Send request message from BMC.
*   VKCSIpmiMessageReceiver()                    - Pass message to channel subsystem and
*                                         transmit.
*   VKCSIpmiMessageDeliver()                 - Trigger channel subsystem to receive
*                                         message data from channel driver.
*   VKCSIOCtrlHandler() - Notify channel the channel driver
*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include "VKCSDriver.h"

#define VKCS_BUFFER_SIZE 40
#define VKCS_VKCS_SEQUENSE_NUMBER_SIZE 1
#define VKCS_VKCS_LENGTH_SIZE 1
#define VKCS_IBF_MASK 0x02
#define VKCS_OBF_MASK 0x01
#define VKCS_SERVER_PRESENT 0x04
#define VKCS_BMC_RESPONSE_ERROR_MASK 0x04 

#define TIMEOUT_INTERVAL (HZ * 3) // 3 sec

//#define DBG_PRINTK printk
#define DBG_PRINTK(x)

int VKCSIOCtrlHandler(struct inode *psInode, struct file *psFlip, unsigned int uiCmd, unsigned long pulArg );

/******************************************************************************
*   STRUCT      :   VKCSBUFFER
******************************************************************************/
/**
 *  @brief   Input/output Buf Data
 *
 *****************************************************************************/
typedef struct vkcsbuffer
{
    unsigned char pucBuffer[VKCS_BUFFER_SIZE+VKCS_VKCS_SEQUENSE_NUMBER_SIZE+VKCS_VKCS_LENGTH_SIZE];
}VKCSBUFFER;

/***************************************************************************
 * External data declaration
 ***************************************************************************/
 
/***************************************************************************
 * Static data declaration
 ***************************************************************************/

static VKCSBUFFER *gsInputBuffer, *gsOutputBuffer;
static int giMajorNumber;
static int giVKCSStatusRegister;
static struct semaphore gsIPMI_Serialize;
static unsigned char gucVKCSSequenceNumber;
static wait_queue_head_t gsLibWaitQueue,gsSBMCWaitQueue;
/* soft bmc time out flag*/

struct file_operations vkcs_file_ops=
{
    .ioctl = VKCSIOCtrlHandler,
};
#define VKCS_COPY_SIZE (sizeof(VKCSBUFFER))

/******************************************************************************
*   FUNCTION        :   VKCSInit
******************************************************************************/
/**
 *  @brief      VKCS module init funciotn
 *
 *  @return     None
 *
 *  @dependency None
 *
 *  @limitation None
 *
 *  @warning    None
 *
 *  @note       init the varialbes of vkcs
 *
 *  @internal   Function Type: Global function\n
 *
 *****************************************************************************/
static int VKCSInit(void)
{
    int iI;

    /* register a majornumber for vkcs driver */
    giMajorNumber = register_chrdev(0, "vkcs",&vkcs_file_ops);
        

    /* show this message for vkcs install shell used */
    printk("mknod /dev/vkcs c %d 0\n", giMajorNumber);
   
     if(giMajorNumber < 0 )
    {
        return giMajorNumber;
    }
    
    /* init registers */
    giVKCSStatusRegister = 0x00;

    /* bmc waiting queue*/
    init_waitqueue_head(&gsSBMCWaitQueue);

    /* lib waiting  queue */
    init_waitqueue_head(&gsLibWaitQueue);
    
    /* init semaphore */
    sema_init(&gsIPMI_Serialize,1);
    gsInputBuffer = kmalloc(VKCS_COPY_SIZE, GFP_KERNEL);
    gsOutputBuffer = kmalloc(VKCS_COPY_SIZE, GFP_KERNEL);
    /* init the buffer value 0x01 0xFC in byte 1,2 that can help */
    /* software BMC/Lib to do error handling                          */   
    gsInputBuffer[0].pucBuffer[0] = 1;
    gsInputBuffer[0].pucBuffer[1] = 0xFC;

    gsOutputBuffer[0].pucBuffer[0] = 1;
    gsOutputBuffer[0].pucBuffer[1] = 0xFC;

    /**  init 0x00 input/out buffer from byte 3*/
   for (iI=2;iI<VKCS_BUFFER_SIZE;iI++)
   {
        gsOutputBuffer[0].pucBuffer[iI] = 0x00;
        gsInputBuffer[0].pucBuffer[iI] = 0x00;
   }



    /* init the sequence number*/
    gucVKCSSequenceNumber = 0;
    

     
    return 0;

}

/******************************************************************************
*   FUNCTION        :   VKCSExit
******************************************************************************/
/**
 *  @brief      vkcs module uninstall funciton
 *
 *  @return     None
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
static void VKCSExit (void)
{
    kfree(gsInputBuffer);
    kfree(gsOutputBuffer);
    unregister_chrdev(giMajorNumber, "vkcs");
}

/******************************************************************************
*   FUNCTION        :   VKCSSoftwareBMCSetStaus
******************************************************************************/
/**
 *  @brief      This function provide softbmc to set rigisters and transfer data
 *
 *  @return     VKCS_ERROR,VKCS_SUCCESS
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
int VKCSSoftwareBMCSetStaus(unsigned char *ulBuffer)
{
    unsigned long Ret;
    if( ulBuffer[(ulBuffer[0]+1)] != gucVKCSSequenceNumber)
    {
       return VKCS_SUCCESS;
    }

    Ret = copy_from_user((void *) gsOutputBuffer[0].pucBuffer,(void *) ulBuffer,VKCS_COPY_SIZE);

    giVKCSStatusRegister |= VKCS_OBF_MASK;

    wake_up_interruptible(&gsLibWaitQueue);

    return VKCS_SUCCESS;
    
}

/******************************************************************************
*   FUNCTION        :   VKCSSoftwareBMCGetStaus
******************************************************************************/
/**
 *  @brief      Software BMC get data API
 *
 *  @return     VKCS_ERROR,VKCS_SUCCESS
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
int VKCSSoftwareBMCGetStaus(unsigned char *ulBuffer)
{
    unsigned long Ret;
    giVKCSStatusRegister |= VKCS_SERVER_PRESENT;
    wait_event_interruptible(gsSBMCWaitQueue,((giVKCSStatusRegister & VKCS_IBF_MASK)==VKCS_IBF_MASK));
    giVKCSStatusRegister &= (~VKCS_SERVER_PRESENT);
    Ret = copy_to_user((void *)ulBuffer, (void *)gsInputBuffer[0].pucBuffer,VKCS_COPY_SIZE);
    

    giVKCSStatusRegister &= (~VKCS_IBF_MASK);

    return VKCS_SUCCESS ;
    
}
/******************************************************************************
*   FUNCTION        :   VKCSIpmiMessageReceiver
******************************************************************************/
/**
 *  @brief      Receive data from AESS library
 *
 *  @return     VKCS_SUCCESS,VKCS_ERROR
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
 
int VKCSIpmiMessageReceiver(unsigned char *ulBuffer)
{

    unsigned long Ret;
    Ret = copy_from_user(gsInputBuffer[0].pucBuffer,(unsigned char*)ulBuffer,VKCS_COPY_SIZE);

    gsInputBuffer[0].pucBuffer[(gsInputBuffer[0].pucBuffer[0]+1)]= ++gucVKCSSequenceNumber;
    giVKCSStatusRegister |= VKCS_IBF_MASK;
    wake_up_interruptible(&gsSBMCWaitQueue);

    return VKCS_SUCCESS;
}

/******************************************************************************
*   FUNCTION        :   VKCSIpmiMessageDeliver
******************************************************************************/
/**
 *  @brief      This API provides library to get message from vKCS outputbuffer
 *
 *  @return     VKCS_SUCCESS,VKCS_ERROR
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
int VKCSIpmiMessageDeliver(unsigned char *ulBuffer)
{
    unsigned char gucTimeOutFlag = 0;
    unsigned long Ret;
    /* tempery  response data buffer */
    

    if(0 == wait_event_interruptible_timeout(gsLibWaitQueue,\
       ((giVKCSStatusRegister & VKCS_OBF_MASK)==VKCS_OBF_MASK),\
       TIMEOUT_INTERVAL))
    {
        gucTimeOutFlag =1;
    }

    /* error handler */
    if( (1 == gucTimeOutFlag) 
    	|| ((giVKCSStatusRegister & VKCS_OBF_MASK) != VKCS_OBF_MASK)
      )
    {
    	int iTempbuffer[4]= {1, 0xFE, 0x09, 0x30};
        gucTimeOutFlag = 0;
        Ret = copy_to_user((unsigned char*)ulBuffer,iTempbuffer,sizeof(iTempbuffer));
        return VKCS_BUSY_TIMEOUT;
    }

    if (gsOutputBuffer[0].pucBuffer[(gsOutputBuffer[0].pucBuffer[0]+1)] != gucVKCSSequenceNumber)
    {
        DBG_PRINTK("gucVKCSSequenceNumber error\n");
        return VKCS_BUSY_TIMEOUT;
    }


    Ret = copy_to_user((unsigned char*)ulBuffer,gsOutputBuffer[0].pucBuffer,VKCS_COPY_SIZE);
    giVKCSStatusRegister &= (~VKCS_OBF_MASK);
    
    
    return VKCS_SUCCESS ;
    
}

/******************************************************************************
*   FUNCTION        :   VKCSIOCtrlHandler
******************************************************************************/
/**
 *  @brief      Dspatch request message to corresponding API
 *
 *  @return     VKCS_SUCCESS
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
int VKCSIOCtrlHandler(struct inode *psInode, struct file *psFlip, unsigned int uiCmd, unsigned long pulArg )
{   
	int iReturnCode = VKCS_ERROR;
    switch(uiCmd)
    {
        case VKCS_WRITE_MESSAGE:

            if(0 == down_interruptible(&gsIPMI_Serialize)){
                giVKCSStatusRegister &= (~VKCS_OBF_MASK);
            	iReturnCode = VKCSIpmiMessageReceiver((unsigned char *)pulArg);
            	up(&gsIPMI_Serialize);
            }

        break;
        
        case VKCS_READ_MESSAGE:
            if(0 == down_interruptible(&gsIPMI_Serialize)){
            	iReturnCode = VKCSIpmiMessageDeliver((unsigned char *)pulArg);
            	up(&gsIPMI_Serialize);
        	}

        break;
        
        case VKCS_GET_REGISTERS:

            DBG_PRINTK("bmc waiting msg\n");
            iReturnCode = VKCSSoftwareBMCGetStaus((unsigned char *)pulArg);
            DBG_PRINTK("bmc got msg\n");

        break;
        
        case VKCS_SET_REGISTERS:
            DBG_PRINTK("bmc send msg\n");
            iReturnCode = VKCSSoftwareBMCSetStaus((unsigned char *)pulArg);
            DBG_PRINTK("bmc sent msg\n");
        break;
        
        case VKCS_WRITE_READ_MESSAGE:
        if((giVKCSStatusRegister & VKCS_SERVER_PRESENT)==VKCS_SERVER_PRESENT)
	        if(0 == down_interruptible(&gsIPMI_Serialize) )
			{
	            DBG_PRINTK("ap send msg\n");
                giVKCSStatusRegister &= (~VKCS_OBF_MASK);
	            iReturnCode = VKCSIpmiMessageReceiver((unsigned char *)pulArg);
	            iReturnCode = VKCSIpmiMessageDeliver((unsigned char *)pulArg);
	            DBG_PRINTK("ap got msg \n");
	            up(&gsIPMI_Serialize);
			}
			else
				iReturnCode = VKCS_ERROR;
		else
			iReturnCode = VKCS_BMC_NOT_EXISTENCE;
        break;
    }
    
    return (iReturnCode);
}

module_init(VKCSInit);
module_exit(VKCSExit);
MODULE_DESCRIPTION("Virtual KCS Driver");
MODULE_AUTHOR("Avocent@avocent.com");
MODULE_LICENSE("GPL");

/* add Module info function */

