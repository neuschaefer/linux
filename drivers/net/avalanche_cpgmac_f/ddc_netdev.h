/*
 *
 * ddc_netdev.h
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

 
/** \file   ddc_netdev.h
    \brief  DDC Network Device Core header file

    This file mandates that all network device drivers based upon DDA/DDC based
    architecture use the type definations and API's along with signatures as is. 
    The device specific DDC header should include this file and use/follow the 
    prototypes given here.

 
    \author     PSP Architecture Team
    \version    1.0
 */ 
    
#ifndef _DDC_NETDEVICE_H_
#define _DDC_NETDEVICE_H_
    
#include "ddc.h"
    
/**
 * \defgroup DDCNetDevInteface DDC Network Device Interface
 * 
 * All DDC Network Devices use the following interface (along with DDC interface)
 */ 
    /*@{ */ 
    
/**
 * \defgroup DDCNetDevIoctl DDC Network Device Ioctl Codes
 * 
 * Refer to DDC Ioctl codes description. These are ioctl codes common
 * to all Network devices.
 */ 
    /*@{ */ 
#define DDC_NET_IOCTL_MIN               (DDC_IOCTL_MAX + 1)
    
#define DDC_NET_IOCTL_GET_NET_STATS     DDC_IOCTL(DDC_NET_IOCTL_MIN, 0)
    
#define DDC_NET_IOCTL_CLR_NET_STATS     DDC_IOCTL(DDC_NET_IOCTL_MIN, 1)
    
    /* ... More net-device Ioctls here */ 
    
#define DDC_NET_IOCTL_MAX               (DDC_IOCTL_MAX + 31)
    
    /*@} */ 
    
/**
 * \defgroup DDCNetDevErrorCodes DDC Network Device Error Codes
 * 
 * Refer to DDC Error codes description. The following are generic error 
 * codes for the DDC Net Device Class
 */ 
    /*@{ */ 
    
#define DDC_NETDEV_ERROR_MIN            (DDC_ERROR_MAX + 1)
    
    /* ... more generic DDC Net device class error codes to follow */ 
    
#define DDC_NETDEV_ERROR_MAX            (DDC_NETDEV_ERROR_MIN + 31)
    
    /*@} */ 
    
/**
 * \defgroup DDCNetPacketObjects  DDC Network Device Packet/Buffer Objects
 * 
 * All DDC Network Devices use the following network buffer/packet objects
 */ 
    /*@{ */ 
    
/**
 *  \brief Network Data Token 
 *
 *  Token associated with a network data buffer/fragment
 *  E.g.  Linux: skb, VxWorks: MBlk 
 */ 
typedef Ptr DDC_NetDataToken;

/**
 *  \brief Network Buffer Object 
 *
 *  Holds attributes of a buffer/fragment
 *  \note
 *  \n \b Send: Usually when the buffers are allocated by DDA, the Start of Packet token 
 *  will be the handle to the whole packet. This token/handle should be good enough 
 *  to free the packet or return to its pool. When the buffers are allocated by 
 *  DDC, typically token for each buffer needs to be indicated (TxComplete) rather
 *  than only the Start of Packet token. 
 *  \n \b Receive: For each buffer the token will be a handle to the buffer that can be 
 *  used by the allocater (DDA or DDC) of the buffer to free it or return to a pool.
 */ 
typedef struct  {
    DDC_NetDataToken bufToken;         /**< Buffer Token. */
    Char * dataPtr;                    /**< Pointer to data buffer */
    Int length;                        /**< Buffer Length (number of bytes) */
} DDC_NetBufObj;

/**
 *  \brief Network Packet Object 
 *
 *  Holds attributes of a network packet (NetBufObjs and packet size).
 */ 
typedef struct  {
    DDC_NetDataToken pktToken;         /**< OS Data Token /may hold Tx/Rx chan id */
    DDC_NetBufObj * bufList;           /**< Array of Network Buffer objects */
    Int numBufs;                       /**< Number of Network Buffer objects */
    Int pktLength;                     /**< Packet Length (number of bytes) */
} DDC_NetPktObj;

    /*@} */ 
    
/**
 *  \brief Net Channel State
 *
 *  State of the channel (initialized, about to be closed, closed etc
 */ 
typedef enum  { DDC_NET_CH_DIR_TX = 0, /**< Transmit only */ 
        DDC_NET_CH_DIR_RX, /**< Receive only */ 
        DDC_NET_CH_DIR_BIDIRECTIONAL, /**< Bidirectonaly - TX/RX  */ 
        DDC_NET_CH_DIR_UNDEFINED /**< Not defined */  
        /* ... more generic channel states */ 
} DDC_NetChDir;

/**
 *  \brief Net Channel State
 *
 *  State of the channel (initialized, about to be closed, closed etc
 */ 
typedef enum  { DDC_NET_CH_UNINITIALIZED = 0, /**< Uninitialized state */ 
        DDC_NET_CH_INITIALIZED, /**< Initialization complete, resources allocated */ 
        DDC_NET_CH_OPENED, /**< Channel in open state */ 
        DDC_NET_CH_CLOSE_IN_PROGRESS, /**< Channel close/teardown in progress */ 
        DDC_NET_CH_CLOSED
        /**< Channel is closed, resources deallocated - require this ? */
        
        /* ... more generic channel states */ 
} DDC_NetChState;

/**
 *  \brief Channel Info - common to TX and RX
 *
 *  Optional - used only if underlying device supports some form of channel concept
 *  Used to pass channel config info from DDA to DDC for CPMAC channels
 */ 
typedef struct  {
    Int channel;                       /**< Channel number */
    Int direction;                     /**< Channel direction - 0=Input, 1=Output, 2=Bidirectional */
    DDC_NetChState state;              /**< Channel State */
    
        /* ... more generic channel params */ 
} DDC_NetChInfo;

/**
 *  \brief DDC Network Device Init Configuration
 *
 *  Configuration information provided to DDC layer during initialization.
 *  DDA gets the config information from the OS/PAL layer and passes the relevant
 *  config to the DDC during initialization. The config info can come from various
 *  sources - static compiled in info, boot time (ENV, Flash) info etc.
 */ 
typedef struct  {
    DDC_InitConfig ddcInitConfig;      /**< DDC 'inherited' init config parameters */
    Int numTxChannels;                 /**< Number of supported TX channels */
    Int numRxChannels;                 /**< Number of supported TX channels */
    
        /* ... more config params for NET DDC here */ 
} DDC_NetInitConfig;

    /* A DDC network driver will implement these functions and pass the pointers to DDA layer */ 
    
/** \name NetDev DDC Functions
 *  NetDev Class DDC Functions
 * @{
 */ 
    
/**
 *  \brief Network Device Send (Single Packet) Function 
 *
 *  This function supports single packet/multiple fragments transmit 
 *  functionality. It transmits a single packet buffers passed as parameters 
 *  to the function. DDC implementation layer may queue up the buffers (using 
 *  hardware mechanisms) or implement synchronous transmit mechanism depending 
 *  upon the hardware of the device. After successful completion of the call, 
 *  it is expected that the packet will be transmitted on the network. When 
 *  the packet transmit mechanism is "asynchronous" in nature, the packet 
 *  completion callback is needed to indicate transmit completion to the upper 
 *  layer. The implementation details of the function are left to the specifics 
 *  of the hardware device.
 *
 *  \param  hDDC [IN]           DDC Handle
 *  \param  pkt [IN]            Network Packet (contains buffers and packet attributes)
 *  \param  channel [IN]        Channel number to be used to send packets
 *  \param  sendArgs [IN/OUT]   Optional Arguments
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_NetSend) (
                                  DDC_Handle hDDC, 
                                  DDC_NetPktObj * pkt, 
                                  Int channel, Ptr sendArgs);

/**
 *  \brief Network Device Send (Multiple Packets) Function 
 *
 *  This function supports multiple packet/multiple fragments transmit 
 *  functionality. DDC implementation layer may queue up the packets/fragments 
 *  (using hardware mechanisms) or implement synchronous transmit mechanism 
 *  depending upon the hardware of the device. After successful completion of 
 *  the call, it is expected that the packets will be transmitted on the 
 *  network. When the packet transmit mechanism is "asynchronous" in nature, 
 *  the packet completion callback is needed to indicate transmit completion 
 *  to the upper layer. The packets are transmitted sequencially (i.e in order).
 *  The caller should compare the returned successfully transmitted packets vs.
 *  packets given to transmit as a check to know which packets were not 
 *  transmitted. The implementation details of the function are left to the 
 *  specifics of the hardware device.
 *  The caller should check returned "pktsSent" with "numPkts" to find out which
 *  packets were not sent.Either Send() or SendPkts() needs to be implemented.
 *
 *  \param  hDDC [IN]           DDC Handle
 *  \param  netPktList [IN]     Array of Network Packet Objects
 *  \param  numPkts [IN]        Total size of fragments
 *  \param  channel [IN]        Channel to be used to sent packets
 *  \param  sendArgs [IN/OUT]   Optional Arguments
 *  \param  pktsSent [OUT]      Packets sent successfully
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_NetSendMultiple) (
                                          DDC_Handle hDDC, 
                                          DDC_NetPktObj * netPktList, 
                                          Int numPkts, 
                                          Int channel, 
                                          Ptr sendArgs, Int * pktsSent);

/**
 *  \brief Network Device Poll Receive (Single/Multiple Packet) Function 
 *  
 *  This function is optional to implement and is provided to support the DDC 
 *  to poll RX packets from the DDC layer. Poll mode may not mean, "without 
 *  interrupt support". It just means that DDA invokes the receive function 
 *  rather than using the receive callback. This function receives a given 
 *  number of packets (as specified in the API) in the caller supplied data 
 *  structures. The caller is responsible to allocate data structure memory 
 *  to receive the packets/fragments. 
 *  After the successful completion of the call, the number of packets received 
 *  is returned back. The implementation details of the function are left to the 
 *  specifics of the hardware device.
 *
 *  \note Optional implementation - Either the polled interface or callback
 *  can be used for receive packet processing.
 *  \sa DDA_NetRxCb, DDA_NetRxMultipleCb
 * 
 *  \param  hDDC [IN]           DDC Handle
 *  \param  netPktList [OUT]    Placeholder for array of Network Packet objects
 *  \param  numPkts [IN]        Number of packets to receive
 *  \param  rxArgs  IN/OUT]     Channel no, timeout can be specified as arguments
 *  \param  retNumPkts [OUT]    Number of packets actually received
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_NetPollRx) (
                                    DDC_Handle hDDC, 
                                    DDC_NetPktObj ** netPktList, 
                                    Int numPkts, 
                                    Ptr rxArgs, Int * retNumPkts);

/**
 *  \brief Network Device RX (Buffer) Return Function 
 *  
 *  This function is OPTIONAL and required only when DDC allocates/manages 
 *  receive buffers depending upon implementation of the DDC layer. It is used 
 *  to indicate back the packets from DDA to DDC layer. The token provided in 
 *  this function is the same as that was passed to the DDA layer when the 
 *  incoming received packet was passed to it.
 *  \note Optional implementation - Either the polled interface or callback 
 *  can be used for receive packet processing. 
 *
 *  \sa DDA_NetAllocRxBufCb, DDA_NetFreeRxBufCb
 *  \param  hDDC [IN]           DDC Handle
 *  \param  netDataTokens [IN]  Array of buffer/packet associated token
 *  \param  numTokens [IN]      Number of tokens
 *  \param  rxRetArgs [IN/OUT]  Arguments - channel no may be indicated here
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_NetRxReturn) (
                                      DDC_Handle hDDC, 
                                      DDC_NetDataToken netDataTokens, 
                                      Int numTokens, Ptr rxRetArgs);

/** 
 *  \brief Net ISR (to be called by the DDA)
 *
 *  DDA registers its ISR with the OS and internally calls the DDC interrupt service
 *  routine. Typically Net ISR returns the status of interrupt (TX, RX, Error etc). DDA
 *  ISR can choose to process the respective interrupts in ISR context (e.g Error interrupt)
 *  or just trigger the thread processing packets. This flexible mechanism allows packet
 *  processing priority to be controlled in its own thread to balance the overall system.
 *
 *  \param  hDDC        DDC Handle
 *  \param  isrArgs     Arguments of any, else NULL
 *  \return Interrupt (pending) status (TX, RX, Error etc). or PAL Error code
 */ 
typedef Int(*DDC_NetIsr) (DDC_Handle hDDC, Ptr isrArgs);

/** 
 *  \brief Net device Channel Open/Setup function (to be called by the DDA)
 *
 *  Optional to implement - only if underlying device supports any channel 
 *  concept. Initialises the data structure and allocates resources. Typically 
 *  done after DDC Init and before DDC Open ??. May be called after Open()
 *  depending upon implementation and support for dynamic channels. Depending 
 *  upon device, may touch the hardware.
 *
 *  \param  hDDC        DDC Handle
 *  \param  chInfo      Channel configuration
 *  \param  chSetupArgs Arguments if any, else NUL
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_NetChOpen) (
                                    DDC_Handle * hDDC, 
                                    DDC_NetChInfo * chInfo, 
                                    Ptr chOpenArgs);

/**
 *  \brief CPMAC Channel Close/Teardown function
 *
 *  Invoked by DDA or called during DDC Close. Optional to implement - only 
 *  if underlying device supports any channel concept Close/Teardown the channel 
 *  and release resources. Typically done before the close is called or as part 
 *  of Close.
 *
 *  \param  hDDC        DDC Handle
 *  \param  channel     Channel number
 *  \param  direction   Channel direction
 *  \param  chCloseArgs Blocking/NonBlocking etc 
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDC_NetChClose) (
                                     DDC_Handle hDDC, 
                                     Int channel, 
                                     Int direction, Ptr chCloseArgs);

/**
 *  \brief  Network device Function Table

 *  Every DDC implementation MUST include this data structure as part of
 *  its interface (function table).
 */ 
typedef struct  {
    DDC_FuncTable ddcFuncTable;                /**< Reference to the DDC layer */
    DDC_NetSend ddcNetSend;                    /**< Send one packet on the network */
    DDC_NetSendMultiple ddcNetSendMultiple;    /**< Send multiple pkts on the network */
    DDC_NetPollRx ddcNetPollReceive;           /**< Poll packets from the DDC */
    DDC_NetRxReturn ddcNetRxReturn;            /**< Return packet buffers back to DDC */
    DDC_NetIsr ddcNetIsr;                      /**< Interrupt handler */
    DDC_NetChOpen ddcNetChOpen;                /**< Network Channel Open */
    DDC_NetChClose ddcNetChClose;              /**< Network Channel Close */
} DDC_NetFuncTable;
typedef DDC_NetFuncTable *DDC_NetFuncTableHandle;

    /*@} */ 
    
/** \name NetDev DDA Callback Functions
 *  NetDev Class DDA Callback Functions
 * @{
 */ 
    
    /* A DDA network driver will implement these functions and pass the pointers to DDC layer */ 
    
/**
 *  \brief Rx availiable Callback 
 *  
 *  It is implemented by DDA layer, called by DDC during packet processing 
 *  Notifies the DDA about the packet arrival. This function is OPTIONAL and
 *  implemented only if a notification of received packets is required. 
 *
 *  \param  hDDA       [IN]           DDA Handle
 *  \param  count      [IN]           number of packets available to be pulled
 *  \param  chanNum    [IN]           channel Id
 *  \return PAL_SUCCESS or PAL Error code
 *  
 */ 
typedef PAL_Result(*DDA_NetRxNotifyCb) (DDA_Handle hDDA, /**[In]< DDA Handle */ 
                                        Int count, /**[In]< Number of available Packets */ 
                                        Int chanNum);
                                                /**[In]< Channel number */

/**
 *  \brief Allocate RX (Network) Buffer Callback
 *
 *  This callback function is implemented by DDA layer and invoked by DDC layer. 
 *  DDC requests DDA layer to provide buffers for RX packets. The buffer pointer 
 *  is returned on success, or NULL on error. 
 *  DDA provides a data token associated with the buffer that is returned for 
 *  book-keeping and DDC passes this token back to DDA the buffer is filled in 
 *  and handed over to DDA during receive.
 *  \note Optionally DDC may choose to implement its own buffer pool and 
 *  allocate buffers internally. In that case these functions are optional, 
 *  but DDC should implement the DDC_NetRxReturn call so that DDA can return 
 *  the buffers to DDC. 
 *  \note "allocArgs" param can be used to enhance the functionality of the function
 *  For example, memory base and range can be specified from which the buffer 
 *  will be allocated, or DDA specific token holder can be passed in the call 
 *  which will help in freeing the buffer.  All this functionality is device
 *  implementation specific and the parameter is a void pointer
 *
 *  \sa DDA_NetFreeRxBufCb, DDC_NetRxReturn
 *  \param  hDDA [IN]           DDA Handle
 *  \param  bufSize [IN]        Size of buffer requested
 *  \param  dataToken [OUT]     Place holder for data token associated with buffer
 *  \param  allocArgs [IN/OUT]  Arguments - channel no may be indicated here
 *  \return Returns the allocated buffer pointer
 */ 
typedef Ptr(*DDA_NetAllocRxBufCb) (
                                   DDA_Handle hDDA, 
                                   Int bufSize, 
                                   DDC_NetDataToken * dataToken, 
                                   Ptr allocArgs);

/**
 *  \brief Free RX (Network) Buffer Callback
 *
 *  This callback function is implemented by DDA layer and invoked by DDC layer. 
 *  This function frees the RX buffer previously allocated using AllocRxBufCb(). 
 *  The data token received during allocation is passed back to DDA for freeing 
 *  the buffer. Typically this function is used by the DDC layer during "closing" 
 *  when the receive buffers are freed. 
 *  \note Optionally DDC may choose to implement its own buffer pool and allocate 
 *  buffers internally. In that case these functions are optional, but DDC should 
 *  implement the DDC_NetRxReturn call so that DDA can return the buffers to DDC. 
 *
 *  \sa DDA_NetAllocRxBufCb, DDC_NetRxReturn
 *  \param  hDDA [IN]           DDA Handle
 *  \param  buffer IN]          Size of buffer requested
 *  \param  dataToken [IN]      Data token associated with buffer
 *  \param  freeArgs [IN/OUT]   Arguments - channel no may be indicated here
 *  \return PAL_SUCCESS or PAL Error code
 *
 */ 
typedef PAL_Result(*DDA_NetFreeRxBufCb) (
                                         DDA_Handle hDDA, 
                                         Ptr buffer, 
                                         DDC_NetDataToken dataToken, 
                                         Ptr freeArgs);

/**
 *  \brief DDA Receive (Single Packet) Callback
 *
 *  This callback function is implemented by DDA layer and invoked by DDC 
 *  layer. This function passes a single received packet to the DDA layer.
 *  The data token associated with each fragment is passed to the DDA layer. 
 *  This is the same token that was received with each buffer/fragment during 
 *  buffer allocation using AllocRxBufCb().
 *  \note Optional implementation - Either the polled interface or callback
 *  can be used for receive packet processing.
 *  \sa DDC_NetPollRx
 *
 *  \param  hDDA [IN]       DDA Handle
 *  \param  pkt [IN]        Network packet (contains buffers and packet attributes)
 *  \param  rxArgs [IN/OUT] Arguments - channel no may be indicated here
 *  \param  param [IN/OUT]  Arguments - Extra arguments can be passed here
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDA_NetRxCb) (
                                  DDA_Handle hDDC, 
                                  DDC_NetPktObj * pkt, 
                                  Ptr rxArgs, Ptr param);

/**
 *  \brief DDA Receive (Multiple Packets) Callback
 *
 *  This callback function is implemented by DDA layer and invoked by DDC layer. 
 *  This function passes multiple received packet to the DDA layer. The data 
 *  token associated with each fragment in each packet is passed to the DDA 
 *  layer. This is the same token that was received with each buffer/fragment 
 *  during buffer allocation using AllocRxBufCb().
 *  \note Optional implementation - Either the polled interface or callback
 *  can be used for receive packet processing.
 *  \sa DDC_NetPollRx
 *
 *  \param  hDDA [IN]       DDA Handle
 *  \param  netPktList [IN] Array of Network Packet objects
 *  \param  numPkts [IN]    Number of packets
 *  \param  rxArgs [IN/OUT] Arguments - channel no may be indicated here
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDA_NetRxMultipleCb) (
                                          DDA_Handle hDDC, 
                                          DDC_NetPktObj * netPktList, 
                                          Int numPkts, Ptr rxArgs);

/**
 *  \brief DDA Transmit Complete Callback 
 *
 *  This callback function is implemented by DDA layer and invoked by 
 *  DDC layer. When transmit functionality is asynchronous in nature, 
 *  a callback is needed to indicate the transmit completion of a packet 
 *  to the DDA layer. This function indicates multiple packet transmit 
 *  complete event to DDA.
 *  The data token associated with each packet is passed to the DDA to 
 *  indicate packet transmit completion event.
 *
 *  \note Optionally when bridging is done at DDC-DDC level, the token 
 *  may be associated with each buffer and will be indicated by this call.
 *
 *  \param  hDDC [IN]           DDC Handle
 *  \param  netDataTokens [IN]  Array of tokens
 *  \param  numTokens [IN]      Number of tokens
 *  \param  args [IN/OUT]       Arguments - Channel number may be indicated here
 *  \return PAL_SUCCESS or PAL Error code
 */ 
typedef PAL_Result(*DDA_NetTxCompleteCb) (
                                          DDA_Handle hDDA, 
                                          DDC_NetDataToken netDataTokens, 
                                          Int numTokens, Ptr args);

/** 
 *  DDA Provided Callback Function structure 
 */ 
typedef struct  {
    DDA_FuncTable ddaFuncTable;                       /**< Reference to the DDC layer */
    DDA_NetRxNotifyCb ddaNetRxNotify;                 /**< Indication of Rx available to be pulled */
    DDA_NetAllocRxBufCb ddaNetAllocRxBufCb;           /**< Allocate Network Buffer from DDA-OS dependent pool */
    DDA_NetFreeRxBufCb ddaNetFreeRxBufCb;             /**< Free the buffer previously allocated from DDA-OS dependent pool */
    DDA_NetRxCb ddaNetrxCb;                           /**< Receive  single packet */
    DDA_NetRxMultipleCb ddaNetrxMultipleCb;           /**< Receive multiple packets */
    DDA_NetTxCompleteCb ddaNettxCompleteCb;           /**< Transmit completion callback */
} DDA_NetFuncTable;
typedef DDA_NetFuncTable *DDA_NetFuncTableHandle;

    /*@} */ 
    
    /*@} */ 
    
#endif  /* _DDC_NETDEVICE_H_ */
    
