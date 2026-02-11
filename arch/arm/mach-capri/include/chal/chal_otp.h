/*****************************************************************************
*  Copyright 2001 - 2012 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    chal_otp.h
*
*  @brief   OTP driver interface header file.
*
*  @note
*
****************************************************************************/
#ifndef _CHAL_OTP_H
#define _CHAL_OTP_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <mach/rdb/brcm_rdb_otp.h>

#ifdef __KERNEL__
    #include <linux/kernel.h>
    #include <linux/delay.h>
    #include <linux/errno.h>
#else
    #include <mach/csp/delay.h>
#endif

#include <mach/otpHw_def.h>
#include <mach/otpHw_reg.h>

/* ---- Public Constants and Types --------------------------------------- */

/*
 * Handle for OTP controller operations.
 */
struct chal_otp_handle
{
   uint32_t bar;             /* Base address to use for register accesses */
   uint32_t otp_status_code; /* The latest OTP access status */
};
typedef struct chal_otp_handle CHAL_OTP_HANDLE_t;

/* ---- Private Constants and Types --------------------------------------- */

#define OTP_POLL_US_COUNT           5000000  /* Time-out duration in us */

#ifdef __KERNEL__
#define OTP_DEBUG_PRINT(fmt, args...) printk(KERN_INFO "OTP: " fmt, ## args)
#else
#define OTP_DEBUG_PRINT(fmt, args...)
#endif

/****************************************************************************/
/**
*  @brief   Enumerated type to define the various write/program modes.
*/
/****************************************************************************/
typedef enum
{
   otpHw_WRITE_MODE_RAW = 0,      /* Raw write to both words of each row                       */
   otpHw_WRITE_MODE_ECC,          /* Write ECC word (word 2) only, maintain data word (word 1) */
   otpHw_WRITE_MODE_ECC_AND_DATA, /* Write user-provided data and ECC, maintain fail bits      */
   otpHw_WRITE_MODE_DATA,         /* Write data word (word 1) only, maintain ECC word (word 2) */
} otpHw_write_t;

/* ---- Public Function Prototypes --------------------------------------- */

/*===========================================================================
* Functions for OTP operations.
* ===========================================================================*/

/**
*  Initialize OTP config structure
*
*  @param handle  Pointer to the OTP initialization structure
*  @param otpBar  Location of OTP base address register
*/
int chal_otp_init(CHAL_OTP_HANDLE_t* handle, uint32_t otpBar);

static inline void chal_otp_hw_init(CHAL_OTP_HANDLE_t* handle);
static inline void chal_otp_reset(CHAL_OTP_HANDLE_t* handle);
static inline int chal_otp_lockData(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t bitOffset);
static inline int chal_otp_programDataBit(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t bitOffset);
static inline int chal_otp_writeRawData(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t word1, uint32_t word2 );
static inline int chal_otp_writeDataEcc(CHAL_OTP_HANDLE_t* handle, uint32_t address,
   uint32_t val, uint32_t ecc );
static inline int chal_otp_calculateEcc(CHAL_OTP_HANDLE_t* handle, uint32_t address);
static inline int chal_otp_writeData(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t data);
static inline uint32_t chal_otp_readRawData( CHAL_OTP_HANDLE_t* handle, uint32_t address);
static inline uint32_t chal_otp_readRawDataUpper(CHAL_OTP_HANDLE_t* handle, uint32_t address);
static inline uint32_t chal_otp_readData(CHAL_OTP_HANDLE_t* handle, uint32_t address);

/* ---- Private Function Prototypes --------------------------------------- */

static inline uint32_t chal_otp_programEnter(CHAL_OTP_HANDLE_t* handle);
static inline void chal_otp_commandEnd(CHAL_OTP_HANDLE_t* handle);
static inline uint32_t chal_otp_accessModeGet(uint32_t address);
static inline uint32_t chal_otp_waitCmdComplete(CHAL_OTP_HANDLE_t* handle);
static inline uint32_t chal_otp_programExit(CHAL_OTP_HANDLE_t* handle);
static inline uint32_t chal_otp_write(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    otpHw_write_t mode, uint32_t word1, uint32_t word2);
static inline uint32_t chal_otp_read(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t raw, uint32_t* pData1, uint32_t* pData2);


/* ---- Public Function Implementation ------------------------------------ */

/**
*  @brief   Lock OTP data
*
*  This function Locks programmed data using the LOCK command
*
*  @param address  Address (row) of the OTP memory
*  @param bitOffset  Offset of row bit to lock
*
*  @return 0 : On Success
*          1 : On Failure
*/
static inline int chal_otp_lockData(CHAL_OTP_HANDLE_t* handle, uint32_t address, uint32_t bitOffset )
{
   uint32_t control = 0;
   uint32_t ones_pat;
   uint32_t status;
   uint32_t temp;

   /* Check call parameters */
	if (address > otpHw_REPAIR_END_ROW || bitOffset >= otpHw_BITS_PER_ROW)
	{
   	  handle->otp_status_code = otpHW_REG_STATUS_OVERLAY_BAD_CALL_PARAMS;
      return 1;
	}

   /* Enter programming mode */
   status = chal_otp_programEnter(handle);
   if ( status != 0 )
   {
      handle->otp_status_code = status;

      /* Clean up and return the OTP hardware to the proper state */
      chal_otp_commandEnd(handle);
      return 1;
   }

   /* Set the program row address and bit offset */
   otpHw_REG_BIT_ADDR_SET(handle->bar, address, bitOffset);

   /* Get access mode, set OTP command to write bit, and SW ECC generation */
   control = chal_otp_accessModeGet(address);
   otpHw_REG_OTP_COMMAND_LOCK_SET(control);
   otpHw_REG_OTP_ECC_BY_SW_SET(control);

   /* Execute, wait, and complete command */
   otpHw_REG_OPT_CONTROL_SET(handle->bar, control);
   status = chal_otp_waitCmdComplete(handle);
   chal_otp_commandEnd(handle);

   /* Generate pattern for status checking: WRP_FDONE = 1,
    *                                       PROGOK = 1,
    *                                       CMD_DONE = 1
    */
   ones_pat = otpHW_REG_STATUS_BITMSK_WRP_FDONE |
	      otpHW_REG_STATUS_BITMSK_PROGOK    |
	      otpHW_REG_STATUS_BITMSK_CMD_DONE;

   /* Check for all bits that are expected to be 1s */
   if (status == ones_pat)
   {
      status = 0;
   }

   /* Take OTP out of the programming state - this may cause an error */
   temp = chal_otp_programExit(handle);

   /* If program status is successful, then return status for program mode termination */
   if (status == 0)
   {
      if (temp != (otpHW_REG_STATUS_BITMSK_WRP_FDONE |
		   otpHW_REG_STATUS_BITMSK_CMD_DONE))
      {
	 status = temp;
      }
   }

   handle->otp_status_code = status;
   if (!status)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/**
*  @brief   Write a specific OTP bit
*
*  This function programs OTP memory with the location of the data bit
*
*  @param address  Address (row) of the OTP memory
*  @param bitOffset  Offset of bit within row
*  @return 0 : On Success, 1 : On Failure
*/
static inline int chal_otp_programDataBit(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t bitOffset)
{
   uint32_t control = 0;
   uint32_t status;
   uint32_t ones_pat;
   uint32_t temp;

   /* Check call parameters */
	if (address > otpHw_REPAIR_END_ROW || bitOffset >= otpHw_BITS_PER_ROW)
	{
     	handle->otp_status_code = otpHW_REG_STATUS_OVERLAY_BAD_CALL_PARAMS;
		return 1;
	}

   /* Enter programming mode */
   chal_otp_programEnter(handle);

   /* Set the program row address and bit offset */
   otpHw_REG_BIT_ADDR_SET(handle->bar, address, bitOffset);

   /* Get access mode, set OTP command to write bit, and SW ECC generation */
   control = chal_otp_accessModeGet(address);
   otpHw_REG_OTP_COMMAND_WRITE_BIT_SET(control);

   /* Execute, wait, & cleanup command */
   otpHw_REG_OPT_CONTROL_SET(handle->bar, control);
   status = chal_otp_waitCmdComplete(handle);
   chal_otp_commandEnd(handle);

   /* Take OTP out of the programming state -- this can cause an error */
   temp = chal_otp_programExit(handle);

   /* Generate pattern for status checking: WRP_FDONE = 1,
    *                                       CMD_DONE = 1,
    *                                       PROGOK
    */
   ones_pat = otpHW_REG_STATUS_BITMSK_WRP_FDONE |
	      otpHW_REG_STATUS_BITMSK_CMD_DONE  |
	      otpHW_REG_STATUS_BITMSK_PROGOK;

   /* Check for all bits that are expected to be 1s */
   if (status == ones_pat)
   {
      status = 0;
   }

   /* If program status is successful, then return status for program mode termination */
   if (status == 0)
   {
      if (temp != (otpHW_REG_STATUS_BITMSK_WRP_FDONE | otpHW_REG_STATUS_BITMSK_CMD_DONE))
      {
	 status = temp;
      }
   }

   handle->otp_status_code = status;
   if (!status)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/**
*  @brief   Write raw OTP data
*
*  This function writes into OTP memory with the given 41 bits of data
*
*  @param address  Address (row) of the OTP memory
*  @param word1  Lower 32 bits of data to write
*  @param word2  Upper 9 bits of data to write
*  @return 0 on success, 1 on failure
*/
static inline int chal_otp_writeRawData(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t word1, uint32_t word2 )
{
   handle->otp_status_code = chal_otp_write(handle, address, otpHw_WRITE_MODE_RAW,
       word1, word2);
   if (!handle->otp_status_code)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/**
*  @brief   Write OTP data with ECC
*
*  This function writes into OTP memory, where 7 bit ECC is supplied along with data
*  @param address  Address (row) of the OTP memory
*  @param data  Value to be written
*  @param ecc  ECC to be written

*  @return 0 on success, 1 : On failure
*/
static inline int chal_otp_writeDataEcc(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t data, uint32_t ecc )
{
   handle->otp_status_code = chal_otp_write(handle, address,
       otpHw_WRITE_MODE_ECC_AND_DATA, data, ecc);
   if (!handle->otp_status_code)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/**
*  @brief   Perform ECC calculation in hardware
*
*  This function calculates ECC in hardware and writes to the OTP memory
*
*  @param address  Address (row) of the OTP memory
*  @return 0 on success, 1 on failure
*
*  @note   This function should be called after finalizing the data write into
*          the OTP memory
*/
static inline int chal_otp_calculateEcc(CHAL_OTP_HANDLE_t* handle, uint32_t address)
{
   handle->otp_status_code = chal_otp_write(handle, address, otpHw_WRITE_MODE_ECC, 0, 0);
   if (!handle->otp_status_code)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/**
*  @brief   Write OTP data
*
*  This function writes into OTP memory. No ECC computation is done by HW, until
*  chal_otp_calculateEcc() is executed in turn.
*
*  @param address  Address (row) of the OTP memory
*  @param data  Value to be written
*  @return 0 on success, 1 on failure
*/
static inline int chal_otp_writeData(CHAL_OTP_HANDLE_t* handle, uint32_t address, uint32_t data)
{
   handle->otp_status_code = chal_otp_write(handle, address, otpHw_WRITE_MODE_DATA, data, 0);
   if (!handle->otp_status_code)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/**
*  @brief   Read Raw OTP data
*
*  This function reads data from OTP memory without doing error correction
*
*  @param address  Address (row) of the OTP memory
*  @return  Data from OTP memory completely ignoring ECC field
*/
static inline uint32_t chal_otp_readRawData(CHAL_OTP_HANDLE_t* handle, uint32_t address)
{
   uint32_t lsword;
   uint32_t msword;

   /* Execute a raw read */
   handle->otp_status_code = chal_otp_read(handle, address, 1, &lsword, &msword);

   return lsword;
}

/**
*  @brief   Read Raw OTP data (Upper/ECC word)
*
*  This function reads data from OTP memory without doing error correction
*
*  @return  Data upper word or ECC field from OTP memory
*/
static inline uint32_t chal_otp_readRawDataUpper(CHAL_OTP_HANDLE_t* handle, uint32_t address)
{
   uint32_t lsword;
   uint32_t msword;

   /* Execute a raw read */
   handle->otp_status_code = chal_otp_read(handle, address, 1, &lsword, &msword);

   return msword;
}

/**
*  @brief   Read OTP data
*
*  This function reads error corrected data (if needed) from OTP memory
*
*  @param address  Address (row) of the OTP memory
*  @return  Error corrected (if needed) from OTP memory
*
*  @note    This function should only be called after writing ECC
*/
static inline uint32_t chal_otp_readData(CHAL_OTP_HANDLE_t* handle, uint32_t address)
{
   uint32_t lsword;
   uint32_t msword;

   /* Execute a read with ECC correction, if necessary */
   handle->otp_status_code = chal_otp_read(handle, address, 0, &lsword, &msword);

   return lsword;
}

/**
*  @brief   Initialize/setup the OTP block
*
*  This function prepares the OTP block for read and write accesses.
*
*  @return  None - handle->otp_status_code is updated
*
*  @note    This function must be called once before any access to the OTP
*/
static inline void chal_otp_hw_init(CHAL_OTP_HANDLE_t* handle)
{
   uint32_t config;
   uint32_t control;
   uint32_t status;
   uint32_t timeout_us_count;

   OTP_DEBUG_PRINT("hardware driver initializing\n");

   /* Wait for hardware init done status */
   timeout_us_count = OTP_POLL_US_COUNT;
   while (timeout_us_count != 0)
   {
      config = otpHW_REG_CONFIG_GET(handle->bar);
      if (otpHw_REG_CONFIG_OTP_HW_INIT_DONE(config) != 0)
      {
	 break;
      }
      else
      {
	 udelay(1);
	 timeout_us_count--;
      }
   }

   if (timeout_us_count == 0)
   {
      /* Timed out */
      handle->otp_status_code = otpHW_REG_STATUS_OVERLAY_WAIT_TIMEOUT;
      return;
   }

   /* Enable OTP and CPU mode */
   config = otpHW_REG_CONFIG_GET(handle->bar);
   otpHw_REG_CONFIG_OTP_CPU_MODE_ENABLE(config);
   otpHw_REG_CONFIG_OTP_ENABLE(config);
   otpHW_REG_CONFIG_SET(handle->bar, config);

   /* Wait for hardware init done status */
   timeout_us_count = OTP_POLL_US_COUNT;
   while (timeout_us_count != 0)
   {
      config = otpHW_REG_CONFIG_GET(handle->bar);
      if (otpHw_REG_CONFIG_OTP_HW_INIT_DONE(config) != 0)
      {
	 break;
      }
      else
      {
	 udelay(1);
	 timeout_us_count--;
      }
   }

   if (timeout_us_count == 0)
   {
      /* Timed out */
      handle->otp_status_code = otpHW_REG_STATUS_OVERLAY_WAIT_TIMEOUT;
      return ;
   }

   /* Set up init command */
   otpHw_REG_WORD_ADDR_SET(handle->bar, 0);
   control = 0;
   otpHw_REG_OTP_COMMAND_INIT_SET(control);

   /* Execute, wait, and clean up after command completion */
   otpHw_REG_OPT_CONTROL_SET(handle->bar, control);
   status = chal_otp_waitCmdComplete(handle);
   chal_otp_commandEnd(handle);

   /* Check status -- WRP_FDONE = 1, CMD_DONE = 1, all others = 0 */
   if (status == (otpHW_REG_STATUS_BITMSK_WRP_FDONE | otpHW_REG_STATUS_BITMSK_CMD_DONE))
   {
      status = 0;
   }

   handle->otp_status_code = status;
}

/**
 * Resets internal state machines of OTP by setting the RST bit of the OTP
 * configuration register and then clearing it.
 */
static inline void chal_otp_reset(CHAL_OTP_HANDLE_t* handle)
{
   uint32_t configVal;

   configVal = otpHW_REG_CONFIG_GET(handle->bar);
   otpHw_REG_CONFIG_OTP_RST(configVal);
   otpHW_REG_CONFIG_SET(handle->bar, configVal);

   configVal = otpHW_REG_CONFIG_GET(handle->bar);
   otpHw_REG_CONFIG_OTP_RST_CLEAR(configVal);
   otpHW_REG_CONFIG_SET(handle->bar, configVal);
}

/**
*  @brief   Retrieve the most recent status code
*
*  This function returns the most recent OTP API access status code
*
*  @return status code (0 = success)
*/
static inline uint32_t chal_otp_getStatusCode(CHAL_OTP_HANDLE_t* handle)
{
   return handle->otp_status_code;
}


/* ==== Private Functions ================================================= */

/**
*  @brief   Get the access mode
*
*  @return  The access mode field based on the address given
*/
static inline uint32_t chal_otp_accessModeGet(uint32_t address)
{
   uint32_t control = 0;

   if (address <= otpHw_MANU_END_ROW)
   {
       otpHw_REG_OTP_ACC_MANU_AREA_SET(control);
   }
   else if (address <= otpHw_RESERVED_END_ROW)
   {
       otpHw_REG_OTP_ACC_CONFIG_AREA_SET(control);
   }
   else if (address >= otpHw_REPAIR_START_ROW && address <= otpHw_REPAIR_END_ROW)
   {
       otpHw_REG_OTP_ACC_RAM_REPAIR_AREA_SET(control);
   }

   return control;
}

/**
*  @brief   Check and wait until previously executed command is done.
*
*  This function polls the OTP status register to check for a command completion.
*
*  @return  The content of the status register, less the ignored bits,
*           when CMD_DONE is asserted
*/
static inline uint32_t chal_otp_waitCmdComplete(CHAL_OTP_HANDLE_t* handle)
{
   uint32_t status;
   uint32_t timeout_us_count = OTP_POLL_US_COUNT;

   /* Poll status register until CMD_DONE is set */
   while (timeout_us_count != 0)
   {
      /* Read the OTP status register */
      status = otpHw_REG_STATUS_GET(handle->bar);
      if ((status & otpHW_REG_STATUS_BITMSK_CMD_DONE) != 0)
      {
	 break;
      }
      else
      {
	 udelay(1);
	 timeout_us_count--;
      }
   }

   /* Filter out ignored bits */
   if ((status & otpHW_REG_STATUS_BITMSK_CMD_DONE) != 0)
   {
      status &= ~(otpHW_REG_STATUS_BITMSK_REFOK          |
		  otpHW_REG_STATUS_BITMSK_WRP_BUSY       |
		  otpHW_REG_STATUS_BITMSK_WRP_DATA_READY |
		  otpHW_REG_STATUS_BITMSK_WRP_BIT_DOUT);
   }
   else
   {
      /* Timed out waiting for completion! */
      status = otpHW_REG_STATUS_OVERLAY_WAIT_TIMEOUT;
   }

   return status;
}

/**
*  @brief   Put the OTP state-machine back into the IDLE state after a
*           command completion.
*
*  This function clears the CMD_DONE bit, if it's set.
*
*  @return  N/A
*/
static inline void chal_otp_commandEnd(CHAL_OTP_HANDLE_t* handle)
{
   uint32_t status = otpHw_REG_STATUS_GET(handle->bar);

   /* Clear CMD_DONE, ECC_COR, and/or ECC_DET, if necessary */
   if (status & (otpHW_REG_STATUS_BITMSK_CMD_DONE |
		 otpHW_REG_STATUS_BITMSK_ECC_COR  |
		 otpHW_REG_STATUS_BITMSK_ECC_DET))
   {
      otpHw_REG_STATUS_SET(handle->bar, status);
   }
}

/**
*  @brief   Assist function to help put the OTP into the programming mode.
*
*  This function writes the individual program sequence control words.
*
*  @return  0      : success
*           others : errors
*/
static inline uint32_t otpHw_programSequenceSet(CHAL_OTP_HANDLE_t* handle, uint32_t bitSel)
{
   uint32_t control = 0;
   uint32_t status;

   /* Set address */
   otpHw_REG_BIT_ADDR_SET(handle->bar, 0, bitSel);
   otpHw_REG_OTP_COMMAND_PROG_ENABLE_SET(control);

   /* Execute command */
   otpHw_REG_OPT_CONTROL_SET(handle->bar, control);

   /* Wait for read completion */
   status = chal_otp_waitCmdComplete(handle);

   chal_otp_commandEnd(handle);

   /* Check for expected completion */
   if (status == (otpHW_REG_STATUS_BITMSK_CMD_DONE | otpHW_REG_STATUS_BITMSK_WRP_FDONE))
   {
      status = 0;
   }

   return status;
}

/**
*  @brief   Put the OTP into the programming mode.
*
*  This function writes the program sequence control words.
*
*  @return  0      : success
*           others : errors
*/
static inline uint32_t chal_otp_programEnter(CHAL_OTP_HANDLE_t* handle)
{
   uint32_t status;

   /* Write the 0xF, 0x4, 0x8, and 0xD sequence */
   do
   {
      status = otpHw_programSequenceSet(handle, 0xF);
      if ( status != 0 )
      {
	 break;
      }

      status = otpHw_programSequenceSet(handle, 0x4);
      if ( status != 0 )
      {
	 break;
      }

      status = otpHw_programSequenceSet(handle, 0x8);
      if ( status != 0 )
      {
	 break;
      }

      status = otpHw_programSequenceSet(handle, 0xD);

      /* Expecting PROGOK to be set at this point */
      if (status == (otpHW_REG_STATUS_BITMSK_CMD_DONE  |
		     otpHW_REG_STATUS_BITMSK_WRP_FDONE |
		     otpHW_REG_STATUS_BITMSK_PROGOK))
      {
	 status = 0;
      }

   } while (0);

   return status;
}

/**
*  @brief   Exit the OTP into the programming mode.
*
*  This function takes the OTP out of the programming mode.
*
*  @return  0      : success
*           others : errors
*/
static inline uint32_t chal_otp_programExit(CHAL_OTP_HANDLE_t* handle)
{
   uint32_t control = 0;
   uint32_t status;

   /* Set program disable command */
   otpHw_REG_OTP_COMMAND_PROG_DISABLE_SET(control);

   /* Execute command */
   otpHw_REG_OPT_CONTROL_SET(handle->bar, control);

   /* Wait for read completion */
   status = chal_otp_waitCmdComplete(handle);

   chal_otp_commandEnd(handle);
   return status;
}

/**
*  @brief   Generic OTP read
*
*  This function performs a generic read of the OTP data
*
*  @return 0      : On Success
*          others : On Failure
*/
static inline uint32_t chal_otp_read(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    uint32_t raw, uint32_t* pData1, uint32_t* pData2)
{
   uint32_t control = 0;
   uint32_t status;
   uint32_t ones_pat;

   /* Check call parameters */
   if (!handle || !pData1 || !pData2 || (address > otpHw_REPAIR_END_ROW))
   {
      return otpHW_REG_STATUS_OVERLAY_BAD_CALL_PARAMS;
   }

   /* Set the row address */
   otpHw_REG_WORD_ADDR_SET(handle->bar, address);

   /* Set access mode to raw, OTP command to read, and enable error correction */
   otpHw_REG_OTP_ACC_RAW_SET(control);
   otpHw_REG_OTP_COMMAND_READ_SET(control);
   if ( !raw )
   {
      /* Enable ECC correction/check */
      otpHw_REG_OTP_ECC_CHECK_ENABLE_SET(control);
   }

   /* Execute command & wait for completion */
   otpHw_REG_OPT_CONTROL_SET(handle->bar, control);
   status = chal_otp_waitCmdComplete(handle);

   /* Copy the data words */
   *pData1 = otpHw_REG_RDATA_0_GET(handle->bar);
   *pData2 = otpHw_REG_RDATA_1_GET(handle->bar);

   /* Clean up and return the OTP hardware to the proper state */
   chal_otp_commandEnd(handle);

   /* Generate pattern for status checking: WRP_FDONE = 1,
    *                                       CMD_DONE = 1
    */
   ones_pat = otpHW_REG_STATUS_BITMSK_WRP_FDONE |
	      otpHW_REG_STATUS_BITMSK_CMD_DONE;

   /* Check for all bits that are expected to be 1s */
   if ((status & ones_pat) == ones_pat)
   {
      /* Check for all bits that are expected to be 0s */
      if ((status & ~ones_pat) == 0)
      {
	 status = 0;
      }
      /* Otherwise, can tolerate ECC_COR = 1 */
      else if ((status & ~ones_pat) == otpHW_REG_STATUS_BITMSK_ECC_COR)
      {
	 status = 0;
      }
   }

   return status;
}

/**
*  @brief   Generic OTP write
*
*  This function writes OTP data based on the selected write mode
*
*  @return 0      : On Success
*          others : On Failure
*/
static inline uint32_t chal_otp_write(CHAL_OTP_HANDLE_t* handle, uint32_t address,
    otpHw_write_t mode, uint32_t word1, uint32_t word2)
{
   uint32_t control = 0;
   uint32_t cur_data[2];
   uint32_t status;
   uint32_t ones_pat;
   uint32_t temp;

   /* Check call parameters */
   if (address > otpHw_REPAIR_END_ROW)
   {
      return otpHW_REG_STATUS_OVERLAY_BAD_CALL_PARAMS;
   }
   else if (mode > otpHw_WRITE_MODE_DATA)
   {
      return otpHW_REG_STATUS_OVERLAY_BAD_WRITE_MODE;
   }
   /* Raw read existing data from OTP */
   else if (mode != otpHw_WRITE_MODE_RAW)
   {
      status = chal_otp_read(handle, address, 1, &cur_data[0], &cur_data[1]);
      if (status != 0)
      {
	 return status;
      }
   }

   /* Enter programming mode */
   status = chal_otp_programEnter(handle);
   if ( status != 0 )
   {
      /* Clean up and return the OTP hardware to the proper state */
      chal_otp_commandEnd(handle);
   }
   else
   {
      /* Set the program row address, data word, & ECC word */
      otpHw_REG_WORD_ADDR_SET(handle->bar, address);

      /* Get the access mode */
      control = chal_otp_accessModeGet(address);

      /* Set the write data, depending on the write mode selected */
      switch ( mode )
      {
	 case otpHw_WRITE_MODE_RAW:
	      /* User provided word 1 bits */
	      otpHw_REG_WDATA_0_SET(handle->bar, word1);
	      /* User provided word 2 fail & ECC bits     */
	      otpHw_REG_WDATA_1_SET(handle->bar, word2);
	      break;

	 case otpHw_WRITE_MODE_ECC:
	      /* Keep existing word 1 bits */
	      otpHw_REG_WDATA_0_SET(handle->bar, cur_data[0]);
	      /* Keep existing fail bits; HW writes ECC   */
	      otpHw_REG_WDATA_1_SET(handle->bar,
		  cur_data[1] & OTP_WDATA_1_OTP_WDATA_FAIL_MASK);
		  /* HW to generate ECC */
		  otpHw_REG_OTP_ECC_BY_HW_SET(control);
	      break;

	 case otpHw_WRITE_MODE_ECC_AND_DATA:
	      /* User provided word 1 bits */
	      otpHw_REG_WDATA_0_SET(handle->bar, word1);
	      otpHw_REG_WDATA_1_SET(handle->bar,
		/* Keep existing fail bits */
		(cur_data[1] & OTP_WDATA_1_OTP_WDATA_FAIL_MASK) |
		/* User provided word 2 ECC bits */
		(word2 & OTP_WDATA_1_OTP_WDATA_ECC_MASK));
	      break;

	 case otpHw_WRITE_MODE_DATA:
	 default:
	      /* User provided word 1 bits */
	      otpHw_REG_WDATA_0_SET(handle->bar, word1);
	      /* Keep existing word 2 fail & ECC bits  */
	      otpHw_REG_WDATA_1_SET(handle->bar, cur_data[1]);
	      break;
      }

      /* Set OTP command to write word */
      otpHw_REG_OTP_COMMAND_WRITE_WORD_SET(control);

      /* Execute command */
      otpHw_REG_OPT_CONTROL_SET(handle->bar, control);

      /* Wait for write completion */
      status = chal_otp_waitCmdComplete(handle);

      /* End the write command */
      chal_otp_commandEnd(handle);

      /* Generate pattern for status checking: WRP_FDONE = 1,
       *                                       CMD_DONE = 1,
       *                                       PROGOK
       */
      ones_pat = otpHW_REG_STATUS_BITMSK_WRP_FDONE |
		 otpHW_REG_STATUS_BITMSK_CMD_DONE  |
		 otpHW_REG_STATUS_BITMSK_PROGOK;

      /* Check for all bits that are expected to be 1s */
      if (status == ones_pat)
      {
	 status = 0;
      }

      /* Take OTP out of the programming state - this may cause an error */
      temp = chal_otp_programExit(handle);

      /* If program status is successful, then return status for program mode termination */
      if (status == 0)
      {
	 if (temp != (otpHW_REG_STATUS_BITMSK_WRP_FDONE | otpHW_REG_STATUS_BITMSK_CMD_DONE))
	 {
	    status = temp;
	 }
      }
   }

   return status;
}

#endif
