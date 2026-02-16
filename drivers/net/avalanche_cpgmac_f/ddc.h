/*
 *
 * ddc.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

 
/** \file   ddc.h
    \brief  Device Driver Core header file

    This file mandates that all drivers based upon DDA/DDC based architecture
    use the type definitions and API's along with signatures as is. The device 
    specific DDC header should include this file, use & follow the prototypes 
    given here.

 
    \author     PSP Architecture Team
    \version    1.0
 */ 
    
/**
    DDA-DDC interaction involves a particular call sequence. This mandates that
    the DDC layers implement certain functions to be called by the DDA layer.
    For control/notifications from DDC to DDA, it is required that the DDA implement
    a callback function to handle ioctl/async events/errors/reports from DDC.
 
    ---------------------------------------------------------------------------
    DDC Functions       DDA functions       (E.g Linux)         (E.g VxWorks)
    ---------------------------------------------------------------------------
    CreateInstance()    Drv Inst Create     module_init()       xxx_dev_load 
    DeleteInstance()    Drv Inst Delete     module_exit()       xxx_dev_unload
    Init()              Drv Init            xxx_dev_init()      (xxx_dev_load)
    Deinit()            Drv DeInit          xxx_dev_uninit()    (xxx_dev_unload)
    Open()              Drv Open            xxx_dev_open()      xxx_dev_start()
    Close()             Drv Close           xxx_dev_close()     xxx_dev_stop()
    Control()           Drv Ioctl           xxx_dev_doioctl()   xxx_dev_ioctl()
    ---------------------------------------------------------------------------
    
    ---------------------------------------------------------------------------
    DDA Functions
    ControlCb()         Callback (provided by DDA), invoked by DDC
    ---------------------------------------------------------------------------
    
 
    Call Sequence Diagram : (Items marked '*' are device specific)

    ---------------------------------------------------------------------------
    DDA (OS) (Function)                     DDC Function
    ---------------------------------------------------------------------------
    Driver Instance Create      --->        dev_CreateInstance()
    
    Driver Init (or config*)    --->        Init()

    *Driver Configuration       --->        Control() / *Config

    Driver Open                 --->        Open()

    Driver Ioctl                --->        Control()
    
    *Driver core operation      --->        *Device specific operations
    
    *ControlCb()                <---        Control/Notification to DDA
    
    Driver Close                --->        Close()
    
    Driver DeInit               --->        Deinit()
    
    Driver Instance Delete      --->        dev_DeleteInstance()
    ---------------------------------------------------------------------------
 */ 
    
#ifndef _DDC_H_
#define _DDC_H_
    
/**
 * \defgroup DDCInterface DDC Interface
 * 
 *  Device Driver Core Interface
 */ 
    /*@{ */ 
    
/**
 *  \brief Handle to DDA layer
 *
 *  Passed to the DDC layer during creation of DDC instance and stored by DDC.
 *  Is a parameter in all callback functions.
 *  DDA implementation casts this handle to the appropriate internal data structure.
 */ 
typedef Ptr DDA_Handle;

/**
 *  \brief Handle to DDC layer
 *
 *  Created as part of DDC CreateInstance and stored by DDA.
 *  Is a parameter in all DDC functions.
 *  DDC implementation casts this handle to the appropriate internal data structure.
 */ 
typedef Ptr DDC_Handle;

/**
 * \defgroup DDCIoctl DDC Ioctl Codes
 * 
 *  Ioctl's which are generic for all drivers. 
 *  Driver Core Ioctl's shall be of 16 bits minimum. The MSB 8 bits are reserved
 *  for DDC layer and LSB 8 bits are reserved for CSL layer. With this format, a 
 *  DDC layer Ioctl handler can call the CSL layer ioctl handler if the ioctl is
 *  not for the DDC layer, and DDC and CSL layer can have max 256 ioctl's each.
 *  The following ioctl codes are reserved:
 *  - Generic DDC Ioctl's: 0-31.
 *  - DDC class Ioctl's: 32-  63
 *  - Device specific Ioctl's: 64 - 255
 */ 
    /*@{ */ 
    
#define DDC_IOCTL_SHIFT                 8
    
#define DDC_IOCTL_MASK                  (0xFF00)
    
#define DDC_IOCTL_GET(ioctl)            ((ioctl) & DDC_IOCTL_MASK)
    
#define DDC_IOCTL(ioctl, offset)        ( ((ioctl) + (offset)) << DDC_IOCTL_SHIFT)
    
#define DDC_IOCTL_MIN                   0
    
/** \def DDC_IOCTL_GET_NAME_STRING Get DDC Name String */ 
#define DDC_IOCTL_GET_NAME_STRING       DDC_IOCTL(DDC_IOCTL_MIN, 0)
    
/** \def DDC_IOCTL_GET_VERSION_ID Get Version ID */ 
#define DDC_IOCTL_GET_VERSION_ID        DDC_IOCTL(DDC_IOCTL_MIN, 1)
    
/** \def DDC_IOCTL_GET_VERSION_STRING Get DDC Version String */ 
#define DDC_IOCTL_GET_VERSION_STRING    DDC_IOCTL(DDC_IOCTL_MIN, 2)
    
/** \def DDC_IOCTL_GET_DRV_STATE Get DDC State */ 
#define DDC_IOCTL_GET_DRV_STATE         DDC_IOCTL(DDC_IOCTL_MIN, 3)
    
/** \def DDC_IOCTL_SET_DRV_POWER_DOWN Set DDC in Power Down State */ 
#define DDC_IOCTL_SET_DRV_POWER_DOWN    DDC_IOCTL(DDC_IOCTL_MIN, 4)
    
/** \def DDC_IOCTL_SET_DRV_POWER_UP Set DDC in Power Up Sate */ 
#define DDC_IOCTL_SET_DRV_POWER_UP      DDC_IOCTL(DDC_IOCTL_MIN, 5)
    
    /* ... More DDC Generic Ioctl's to follow */ 
    
#define DDC_IOCTL_MAX                   31
    
    /*@} */ 
    
/**
 * \defgroup DDCErrorCodes DDC Error Codes
 * 
 * For DDC implementations, the error code is divided into two parts - error value
 * and DDC specific details. This allows an error code to provide more information
 * on where it was generated and the error itself. 
 * \n
 * |<----------------32----------------->|
 * \n
 * |1(A)| 3(B) |  4(C) |    8(D)   |   8(E)  |   8(F)  |
 * - A - MSB - Set if Error / 0 if Success
 * - B - Error level - 0=Informational, 1=Warning, 2=Minor, 3=Major, 4=Critical
 * - C - PSP Architecture Component - 0=Reserved, 1=CSL, 2=Driver, 3=PAL, 4=SRV etc
 * - D - Device specific - eg Instance Id of DDC
 * - E,F - Error number - based upon implementation. 
 *
 * For DDC/DDC Class based architecture, generic error codes have 32  
 * reserved values each. These have to be used by the DDC implementation 
 * in its code when returning errors. 
 * Note: For DDC/DDC Class based architecture, following error codes are reserved:
 * - Generic PAL error codes: 0 - 31
 * - DDC generic codes: 32 - 63
 * - DDC Class error codes: 64- 95
 * - Device specific error codes: 96 - 255
 */ 
    /*@{ */ 
    
    /* Generic DDC Error (includes the PAL generic 32 errors (0-31) */ 
#define DDC_ERROR                       (0x200001F)
    /* Generic DDC (Major) Error (includes the PAL generic 32 errors (0-31) */ 
#define DDC_MAJOR_ERROR                 (0xB200001F)
    
    /* Start of DDC (generic) error codes */ 
#define DDC_ERROR_MIN                   (DDC_ERROR + 1)
    
    /* ... more generic DDC error codes to follow */ 
    
    /* End of DDC generic error codes */ 
#define DDC_ERROR_MAX                   (DDC_ERROR_MIN + 31)
    
    /*@} */ 
    
/** 
 *  \brief  DDC State
 *
 *  DDC state is maintained/updated by every DDC implementation
 */ 
typedef enum  { DDC_CREATED, DDC_INITIALIZED, DDC_OPENED, DDC_CLOSED,
    DDC_DEINITIALIZED, DDC_POWERED_DOWN, 
        /* ... more generic DDC states here */ 
} DDC_DriverState;

/** 
 *  \brief  DDC Init config params
 *
 *  Every DDC implementation MUST include this data structure as part of
 *  its DDC initial configuration information data structure.
 * 
 */ 
typedef struct  {
    Uint32 instId;                 /**< Instance id/number */
    
        /* ... more config params for DDC here */ 
} DDC_InitConfig;

/** 
 *  \brief  DDC Object
 *
 *  Every DDC implementation MUST include this data structure as part of
 *  its DDC object data structure.
 */ 
typedef struct  {
    Uint32 versionId;              /**< Version Id */
    Uint32 instId;                 /**< Instance id/number */
    DDC_DriverState state;         /**< DDC State */
    DDA_Handle hDDA;               /**< DDA Handle */
    
        /* ... more generic DDC parameters here */ 
} DDC_Obj;

/** \name DDC Functions
 *  DDC Functions
 * @{
 */ 
    
/**
 *  \brief DDC Delete Instance
 *
 *  Deletes the device DDC instance.
 *  Frees memory allocated by CreateInstance. 
 *
 *  \param  hDDC        DDC Handle
 *  \param  param       Optional argument if any
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_DeleteInstance) (DDC_Handle hDDC, Ptr param);

/**
 *  \brief Initialize instance of the device
 *
 *  After the instance is "created", DDA calls the initialization function 
 *  on the DDC to initialize the instance of the device. Typically, software 
 *  bookkeeping functions are performed in this call. Memory for device 
 *  instance specific data structures may be allocated and initialized. 
 *  Configuration information may also be passed in the call and initialization 
 *  based upon this information is done.
 *  Depending upon the type of hardware and its needs, this call may "probe" 
 *  the hardware (for existence) or enumerate its capabilities. The device may 
 *  be further configured using the DDC_Control() API before being opened. 
 *  Typically, the device ISR is "installed" by the upper layer driver software 
 *  after completion of this call as part of initialization (but not enabled). 
 *  On successful completion of this call, the device is ready to be "opened". 
 *  The implementation details of the function are left to the specifics of 
 *  the hardware device.
 *
 *  \param  hDDC [IN]       DDC Handle
 *  \param  param [IN/OUT]  Device specific argument
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_Init) (DDC_Handle hDDC, Ptr param);

/**
 *  \brief De-initialize instance of the device
 *
 *  This function performs exactly the opposite of the initialization function 
 *  namely de-allocation of resources and internal data structures and 
 *  optionally programming the hardware and preparing for shutdown. Typically, 
 *  the device ISR is "uninstalled" by the upper layer driver software after 
 *  the completion of this call. On successful completion of this call, the init
 *  / open sequence has to be repeated to use the device again. The 
 *  implementation details of the function are left to the specifics of the 
 *  hardware device.Free the memory allocated during initialization
 *
 *  \param  hDDC [IN]       DDC Handle
 *  \param  param [IN/OUT]  Device specific argument
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_DeInit) (DDC_Handle hDDC, Ptr param);

/**
 *  \brief Open Instance of the device
 *
 *  This function prepares the device hardware for data transfers and 
 *  usage by the upper layer driver software. As applicable to the device, 
 *  channels already configured can be enabled here for operation. 
 *  It is expected that the upper layer driver software will install the ISR 
 *  during initialization and enable it only after the completion of this 
 *  call. This is necessary since the device needs to be in a ready state before
 *  the device interrupt is enabled. The specifics of the hardware will further 
 *  dictate the ISR installation/enabling/disabling.
 *  On successful completion of this call, the device is ready for data IO. 
 *  The implementation details of the function are left to the specifics of 
 *  the hardware device.
 *
 *  \param  hDDC [IN]       DDC Handle
 *  \param  param [IN/OUT]  Device specific argument
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_Open) (DDC_Handle hDDC, Ptr param);

/**
 *  \brief Close Instance of the device
 *
 *  The Close function closes the device for data transfers and usage by the 
 *  upper layer driver software. The hardware is programmed to stop/abort data 
 *  transfer (depending upon the type of device and its specifics) and the 
 *  device ISR is "disabled" by the upper layer driver software after the 
 *  completion of this call. After the successful completion of this call, the 
 *  device cannot perform any data IO. The implementation details of the 
 *  function are left to the specifics of the hardware device. Program the 
 *  hardware as applicable to close the device for data transfer.
 *
 *  \param  hDDC [IN]       DDC Handle
 *  \param  param [IN/OUT]  Device specific argument
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_Close) (DDC_Handle hDDC, Ptr param);

/**
 *  \brief Control operations (ioctl)
 *
 * This API provides the capability for control operations to be performed on 
 * the device driver core. Typically the "ioctl" style set/get functionality 
 * is used to configure/program the parameters in DDC or get status or read DDC 
 * parameters. This mechanism can also be used by the upper layer driver 
 * software to configure a parameter in the device driver core or change a 
 * parameter during run time. The implementation details and capabilities of the 
 * function are left to the specifics of the hardware device. 
 * Typically, this function will have the option to get/set all the configuration 
 * parameters via a structure (apart from capability to get/set one parameter.  
 * This helps in getting/setting all the configuration parameters all at once 
 * if required.
 * 
 *  \param  hDDC [IN]       DDC Handle
 *  \param  cmd [IN]        Operation to be performed, typically an enum gets passed
 *  \param  cmdArg [IN/OUT] Provides additonal information related to the operation
 *  \param  param [IN/OUT]  Device/Cmd specific argument
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_Control) (
                                  DDC_Handle hDDC, 
                                  Int cmd, Ptr cmdArg, Ptr param);

/**
 *  \brief DDC Function Table
 *
 *  Every DDC implementation MUST include this data structure as part of
 *  its interface (function table).
 */ 
typedef struct  {
    DDC_Init ddcInit;              /**< DDC Instance Initialization */
    DDC_DeInit ddcDeinit;          /**< DDC Instance Cleanup/De-init */
    DDC_Open ddcOpen;              /**< Opens DDC Instance */
    DDC_Close ddcClose;            /**< Closes DDC instance */
    DDC_Control ddcControl;        /**< DDC IoControl operations */
    DDC_DeleteInstance ddcDelInst; /**< DDC Delete Instance */
} DDC_FuncTable;
typedef DDC_FuncTable *DDC_FuncTableHandle;

    /*@} */ 
    
/** \name DDA Callback Functions
 *  DDC Callback Functions
 * @{
 */ 
    
    /* A DDA driver will implement these functions & pass pointers to DDC layer */ 
    
/**
 *  \brief DDA Control (or notification) callback function 
 *
 *  This callback function is implemented by DDA layer and invoked by the DDC. 
 *  It can be used by the DDC to report asynchronous events, errors, messages 
 *  etc to the DDA layer. It can also be used to get/set parameters on DDA. 
 *  The nature of operations/usage is device specific and is left to the 
 *  specifics of the hardware and driver implementation.
 *
 *  \param  hDDC [IN]       DDC Handle
 *  \param  cmd [IN]        Operation to be performed, typically an enum gets passed
 *  \param  cmdArg [IN/OUT] Provides additonal information related to the operation
 *  \param  param [IN/OUT]  Device/Cmd specific argument
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDA_ControlCb) (
                                    DDA_Handle hDDA, 
                                    Int cmd, Ptr cmdArg, Ptr param);

/**
 *  \brief DDA Callback Function Table
 *
 *  Every DDA MUST include this data structure as part of
 *  its callback interface (function table).
 */ 
typedef struct  {
    DDA_ControlCb ddaControlCb;        /**< DDA Control Callback */
} DDA_FuncTable;
typedef DDA_FuncTable *DDA_FuncTableHandle;

/**
 *  \brief DDC Create Instance 
 *
 *  Allocates memory for device DDC (instance) object, and provides a handle to the DDC.
 *  The handle returned in "hDDC" will be passed in every DDC function called by DDA.
 *  Stores DDA Handle in DDC internal structure and is passed in every DDA callback.
 *  Exchanges function pointers with DDA layer. Typically, the DDC implementation 
 *  maintains only one copy of the DDC Function table structure and passes the handle to it 
 *  for every instance. DDA stores the DDC Interface structure handle in its instance 
 *  object for later use.
 *
 *  \note CreateInstance() should not touch the hardware of the device.
 *
 *  \param  instId      Instance Id
 *  \param  hDDA        DDA Handle provided by DDA layer
 *  \param  hDDACbIf    DDA Callback Function Table handle
 *  \param  hDDC        Placeholder for DDC Handle
 *  \param  hDDCIf      Placeholder for DDC Function Table Handle
 *  \param  param       Optional argument if any
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_CreateInstance) (
                                         Int instId, 
                                         DDA_Handle hDDA, 
                                         DDA_FuncTableHandle hDDACbIf, 
                                         DDC_Handle * hDDC, 
                                         DDC_FuncTableHandle * hDDCIf, 
                                         Ptr param);

    /*@} */ 
    
    /*@} */ 
    
#endif  /* _DDC_H_ */
    
