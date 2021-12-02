/* Mmac PCI/Ethernet test chip driver for OS Linux
 *
 * Device-dependent (mostly system-independent) functions
 *
 * 1 Jun 1999 - G.Kazakov.
 */



//#define TR mmac_trace

#include <linux/mii.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include "mmac_dev.h"

/**********************************************************
 * PCI registers - BASE ADDRESS 0 offsets
 **********************************************************/
enum ConfigRegisters          /* Non-standart configuration registers, base address is BAR+ConfigRegistersBase */
{
  PciToSipBase      = 0x00,
  SipToPciMemBase   = 0x04,
  SipToPciIoBase    = 0x08,
  PciConfigComplete = 0x0C,
  PciSipControl     = 0x10,
  PciSipIntEnable   = 0x14,
  PciSipIntStatus   = 0x18,
  PciSipReadCount   = 0x24,
  Bar0Mask          = 0x2C,
  Bar1Mask          = 0x34,
};

enum GmacRegisters              /* GMAC registers, base address is BAR+GmacRegistersBase */
{
  GmacConfig         = 0x00,    /* config */
  GmacFrameFilter    = 0x04,    /* frame filter */
  GmacHashHigh       = 0x08,    /* multi-cast hash table high */
  GmacHashLow        = 0x0C,    /* multi-cast hash table low */
  GmacGmiiAddr       = 0x10,    /* GMII address */
  GmacGmiiData       = 0x14,    /* GMII data */
  GmacFlowControl    = 0x18,    /* Flow control */
  GmacVlan           = 0x1C,    /* VLAN tag */
  GmacAddr0High      = 0x40,    /* address0 high */
  GmacAddr0Low       = 0x44,    /* address0 low */
  GmacAddr1High      = 0x48,    /* address1 high */
  GmacAddr1Low       = 0x4C,    /* address1 low */
  GmacAddr2High      = 0x50,    /* address2 high */
  GmacAddr2Low       = 0x54,    /* address2 low */
  GmacAddr3High      = 0x58,    /* address3 high */
  GmacAddr3Low       = 0x5C,    /* address3 low */
  GmacAddr4High      = 0x60,    /* address4 high */
  GmacAddr4Low       = 0x64,    /* address4 low */
  GmacAddr5High      = 0x68,    /* address5 high */
  GmacAddr5Low       = 0x6C,    /* address5 low */
  GmacAddr6High      = 0x70,    /* address6 high */
  GmacAddr6Low       = 0x74,    /* address6 low */
  GmacAddr7High      = 0x78,    /* address7 high */
  GmacAddr7Low       = 0x7C,    /* address7 low */
  GmacAddr8High      = 0x80,    /* address8 high */
  GmacAddr8Low       = 0x84,    /* address8 low */
  GmacAddr9High      = 0x88,    /* address9 high */
  GmacAddr9Low       = 0x8C,    /* address9 low */
  GmacAddr10High     = 0x90,    /* address10 high */
  GmacAddr10Low      = 0x94,    /* address10 low */
  GmacAddr11High     = 0x98,    /* address11 high */
  GmacAddr11Low      = 0x9C,    /* address11 low */
  GmacAddr12High     = 0xA0,    /* address12 high */
  GmacAddr12Low      = 0xA4,    /* address12 low */
  GmacAddr13High     = 0xA8,    /* address13 high */
  GmacAddr13Low      = 0xAC,    /* address13 low */
  GmacAddr14High     = 0xB0,    /* address14 high */
  GmacAddr14Low      = 0xB4,    /* address14 low */
  GmacAddr15High     = 0xB8,    /* address15 high */
  GmacAddr15Low      = 0xBC,    /* address15 low */
};

enum DmaRegisters             /* DMA engine registers, base address is BAR+DmaRegistersBase */
{
  DmaBusMode        = 0x00,    /* CSR0 - Bus Mode */
  DmaTxPollDemand   = 0x04,    /* CSR1 - Transmit Poll Demand */
  DmaRxPollDemand   = 0x08,    /* CSR2 - Receive Poll Demand */
  DmaRxBaseAddr     = 0x0C,    /* CSR3 - Receive list base address */
  DmaTxBaseAddr     = 0x10,    /* CSR4 - Transmit list base address */
  DmaStatus         = 0x14,    /* CSR5 - Dma status */
  DmaControl        = 0x18,    /* CSR6 - Dma control */
  DmaInterrupt      = 0x1C,    /* CSR7 - Interrupt enable */
  DmaMissedFr       = 0x20,    /* CSR8 - Missed Frame Counter */
  DmaTxCurrAddr     = 0x50,    /* CSR20 - Current host transmit buffer address */
  DmaRxCurrAddr     = 0x54,    /* CSR21 - Current host receive buffer address */
};

enum MmcRegisters             /* DMA engine registers, base address is BAR+DmaRegistersBase */
{
  MmcCtrl             = 0x00,  
  MmcRxInt           = 0x04,    
  MmcTxInt            = 0x08,    
  MmcRxIntMask     = 0x0C, 
  MmcTxIntMask      = 0x10,
};


/**********************************************************
 * GMAC Network interface registers
 **********************************************************/

enum GmacConfigReg      /* GMAC Config register layout */
{                                            /* Bit description                      R/W   Reset value */
  GmacWatchdogDisable      = 0x00800000,     /* Disable watchdog timer               RW                */
  GmacWatchdogEnable       = 0,              /* Enable watchdog timer                          0       */

  GmacJabberDisable        = 0x00400000,     /* Disable jabber timer                 RW                */
  GmacJabberEnable         = 0,              /* Enable jabber timer                            0       */

  GmacFrameBurstEnable     = 0x00200000,     /* Enable frame bursting                RW                */
  GmacFrameBurstDisable    = 0,              /* Disable frame bursting                         0       */

  GmacJumboFrameEnable     = 0x00100000,     /* Enable jumbo frame                   RW                */
  GmacJumboFrameDisable    = 0,              /* Disable jumbo frame                            0       */

// CHANGE: Added on 07/28 SNPS  
  GmacInterFrameGap7       = 0x000E0000,     /* IFG Config7 - 40 bit times           RW        000     */
  GmacInterFrameGap6       = 0x000C0000,     /* IFG Config6 - 48 bit times                             */
  GmacInterFrameGap5       = 0x000A0000,     /* IFG Config5 - 56 bit times                             */
  GmacInterFrameGap4       = 0x00080000,     /* IFG Config4 - 64 bit times                             */
  GmacInterFrameGap3       = 0x00040000,     /* IFG Config3 - 72 bit times                             */
  GmacInterFrameGap2       = 0x00020000,     /* IFG Config2 - 80 bit times                             */
  GmacInterFrameGap1       = 0x00010000,     /* IFG Config1 - 88 bit times                             */
  GmacInterFrameGap0       = 000,            /* IFG Config0 - 96 bit times                             */

  GmacSelectMii            = 0x00008000,     /* Select MII mode                      RW                */
  GmacSelectGmii           = 0,              /* Select GMII mode                               0       */

//  CHANGE: Commented as Endian mode is not register configurable   
//  GmacBigEndian            = 0x00004000,     /* Big endian mode                    RW                */
//  GmacLittleEndian         = 0,              /* Little endian                                0       */

  GmacFESEnable            = 0x00004000,     /* 100Mbps                             RW                */
  GmacFESDisable           = 0,                    /* 10Mbps                                0       */

  GmacDisableRxOwn         = 0x00002000,     /* Disable receive own packets          RW                */
  GmacEnableRxOwn          = 0,              /* Enable receive own packets                     0       */

  GmacLoopbackOn           = 0x00001000,     /* Loopback mode                        RW                */
  GmacLoopbackOff          = 0,              /* Normal mode                                    0       */

  GmacFullDuplex           = 0x00000800,     /* Full duplex mode                     RW                */
  GmacHalfDuplex           = 0,              /* Half duplex mode                               0       */

  GmacIPCheckSumEnable     = 0x00000400,     /*HW IP check sum enable         RW                */
  GmacIPCheckSumDisable    = 0,              /* HW IP check sum disable                  0       */

  GmacRetryDisable         = 0x00000200,     /* Disable retransmission               RW                */
  GmacRetryEnable          = 0,              /* Enable retransmission                          0       */

//  CHANGE: Commented as Pad / CRC strip is one single bit  
//  GmacPadStripEnable       = 0x00000100,     /* Pad stripping enable               RW                */
//  GmacPadStripDisable      = 0,              /* Pad stripping disable                        0       */

// CHANGE: 07/28 renamed GmacCrcStrip* GmacPadCrcStrip* 
  GmacPadCrcStripEnable    = 0x00000080,     /* Pad / Crc stripping enable           RW                */
  GmacPadCrcStripDisable   = 0,              /* Pad / Crc stripping disable                    0       */

  GmacBackoffLimit3        = 0x00000060,     /* Back-off limit                       RW                */
  GmacBackoffLimit2        = 0x00000040,     /*                                                        */
  GmacBackoffLimit1        = 0x00000020,     /*                                                        */
  GmacBackoffLimit0        = 00,             /*                                                00      */

  GmacDeferralCheckEnable  = 0x00000010,     /* Deferral check enable                RW                */
  GmacDeferralCheckDisable = 0,              /* Deferral check disable                         0       */

  GmacTxEnable             = 0x00000008,     /* Transmitter enable                   RW                */
  GmacTxDisable            = 0,              /* Transmitter disable                            0       */

  GmacRxEnable             = 0x00000004,     /* Receiver enable                      RW                */
  GmacRxDisable            = 0,              /* Receiver disable                               0       */

  GmacEnable             = 0x00000001,     /* Gmac enable                      RW                */
  GmacDisable            = 0,              /* Gmac disable                               0       */

};

enum GmacFrameFilterReg /* GMAC frame filter register layout */
{
  GmacFilterOff            = 0x80000000,     /* Receive all incoming packets         RW                */
  GmacFilterOn             = 0,              /* Receive filtered packets only                  0       */

// CHANGE: Added on 07/28 SNPS 
  GmacSrcAddrFilterEnable  = 0x00000200,     /* Source Address Filter enable         RW                */
  GmacSrcAddrFilterDisable = 0,              /*                                                0       */

// CHANGE: Added on 07/28 SNPS  
  GmacSrcInvAddrFilterEn   = 0x00000100,     /* Inverse Source Address Filter enable RW                */
  GmacSrcInvAddrFilterDis  = 0,              /*                                                0       */

// CHANGE: Changed the control frame config (07/28)  
  GmacPassControl3         = 0x000000C0,     /* Forwards control frames that pass AF RW                */
  GmacPassControl2         = 0x00000080,     /* Forwards all control frames                            */
  GmacPassControl1         = 0x00000040,     /* Does not pass control frames                           */
  GmacPassControl0         = 00,             /* Does not pass control frames                   00      */

  GmacBroadcastDisable     = 0x00000020,     /* Disable reception of broadcast frames RW               */
  GmacBroadcastEnable      = 0,              /* Enable broadcast frames                        0       */

  GmacMulticastFilterOff   = 0x00000010,     /* Pass all multicast packets           RW                */
  GmacMulticastFilterOn    = 0,              /* Pass filtered multicast packets                0       */

// CHANGE: Changed to Dest Addr Filter Inverse (07/28)  
  GmacDestAddrFilterInv    = 0x00000008,     /* Inverse filtering for DA             RW                */
  GmacDestAddrFilterNor    = 0,              /* Normal filtering for DA                        0       */

// CHANGE: Changed to Multicast Hash filter (07/28)  
  GmacMcastHashFilterOn    = 0x00000004,     /* perfom multicast hash filtering      RW                */
  GmacMcastHashFilterOff   = 0,              /* perfect filtering only                         0       */

// CHANGE: Changed to Unicast Hash filter (07/28)
  GmacUcastHashFilterOn    = 0x00000002,     /* Unicast Hash filtering only          RW                */
  GmacUcastHashFilterOff   = 0,              /* perfect filtering only                         0       */

  GmacPromiscuousModeOn    = 0x00000001,     /* Receive all valid packets            RW                */
  GmacPromiscuousModeOff   = 0,              /* Receive filtered packets only                  0       */
};

enum GmacGmiiAddrReg      /* GMII address register layout */
{
  GmiiDevMask    = 0x0000F800,     /* GMII device address */
  GmiiDevShift   = 11,

  GmiiRegMask    = 0x000007C0,     /* GMII register */
  GmiiRegShift   = 6,

// CHANGED: 3-bit config instead of older 2-bit (07/28)  
  GmiiAppClk5    = 0x00000014,     /* Application Clock Range 250-300 MHz */
  GmiiAppClk4    = 0x00000010,     /*                         150-250 MHz */
  GmiiAppClk3    = 0x0000000C,     /*                         35-60 MHz */
  GmiiAppClk2    = 0x00000008,     /*                         20-35 MHz */
  GmiiAppClk1    = 0x00000004,     /*                         100-150 MHz */
  GmiiAppClk0    = 00,             /*                         60-100 MHz */

  GmiiWrite      = 0x00000002,     /* Write to register */
  GmiiRead       = 0,              /* Read from register */

  GmiiBusy       = 0x00000001,     /* GMII interface is busy */
};

enum GmacGmiiDataReg      /* GMII address register layout */
{
  GmiiDataMask   = 0x0000FFFF,     /* GMII Data */
};

enum GmacFlowControlReg  /* GMAC flow control register layout */
{                                          /* Bit description                        R/W   Reset value */
  GmacPauseTimeMask        = 0xFFFF0000,   /* PAUSE TIME field in the control frame  RW      0000      */
  GmacPauseTimeShift       = 16,

// CHANGED: Added on (07/28)
  GmacPauseLowThresh3      = 0x00000030,   /* threshold for pause tmr 256 slot time  RW        00      */
  GmacPauseLowThresh2      = 0x00000020,   /*                         144 slot time                    */
  GmacPauseLowThresh1      = 0x00000010,   /*                         28  slot time                    */
  GmacPauseLowThresh0      = 00,           /*                         4   slot time                    */

  GmacUnicastPauseFrameOn  = 0x00000008,   /* Detect pause frame with unicast addr.  RW                */
  GmacUnicastPauseFrameOff = 0,            /* Detect only pause frame with multicast addr.     0       */

  GmacRxFlowControlEnable  = 0x00000004,   /* Enable Rx flow control                 RW                */
  GmacRxFlowControlDisable = 0,            /* Disable Rx flow control                          0       */

  GmacTxFlowControlEnable  = 0x00000002,   /* Enable Tx flow control                 RW                */
  GmacTxFlowControlDisable = 0,            /* Disable flow control                             0       */

  GmacSendPauseFrame       = 0x00000001,   /* send pause frame                       RW        0       */
};

/**********************************************************
 * DMA Engine registers
 **********************************************************/

enum DmaBusModeReg         /* DMA bus mode register */
{                                         /* Bit description                        R/W   Reset value */
// CHANGED: Commented as not applicable (07/28)
//  DmaBigEndianDesc        = 0x00100000,   /* Big endian data buffer descriptors   RW                */
//  DmaLittleEndianDesc     = 0,            /* Little endian data descriptors                 0       */

// CHANGED: Added on 07/28
  DmaFixedBurstEnable     = 0x00010000,   /* Fixed Burst SINGLE, INCR4, INCR8 or INCR16               */
  DmaFixedBurstDisable    = 0,            /*             SINGLE, INCR                         0       */

  DmaBurstLength32        = 0x00002000,   /* Dma burst length = 32                  RW                */
  DmaBurstLength16        = 0x00001000,   /* Dma burst length = 16                                    */
  DmaBurstLength8         = 0x00000800,   /* Dma burst length = 8                                     */
  DmaBurstLength4         = 0x00000400,   /* Dma burst length = 4                                     */
  DmaBurstLength2         = 0x00000200,   /* Dma burst length = 2                                     */
  DmaBurstLength1         = 0x00000100,   /* Dma burst length = 1                                     */
  DmaBurstLength0         = 0x00000000,   /* Dma burst length = 0                             0       */

// CHANGED: Commented as not applicable (07/28)
//  DmaBigEndianData        = 0x00000080,   /* Big endian data buffers              RW                */
//  DmaLittleEndianData     = 0,            /* Little endian data buffers                     0       */

  DmaDescriptorSkip16     = 0x00000040,   /* number of dwords to skip               RW                */
  DmaDescriptorSkip8      = 0x00000020,   /* between two unchained descriptors                        */
  DmaDescriptorSkip4      = 0x00000010,   /*                                                          */
  DmaDescriptorSkip2      = 0x00000008,   /*                                                          */
  DmaDescriptorSkip1      = 0x00000004,   /*                                                          */
  DmaDescriptorSkip0      = 0,            /*                                                  0       */

  DmaResetOn              = 0x00000001,   /* Reset DMA engine                       RW                */
  DmaResetOff             = 0,            /*                                                  0       */
};

enum DmaStatusReg         /* DMA Status register */
{                                         /* Bit description                        R/W   Reset value */
// CHANGED: Added on 07/28
  DmaLineIntfIntr         = 0x04000000,   /* Line interface interrupt               R         0       */
// CHANGED: Added on 07/28
  DmaErrorBit2            = 0x02000000,   /* err. 0-data buffer, 1-desc. access     R         0       */
  DmaErrorBit1            = 0x01000000,   /* err. 0-write trnsf, 1-read transfr     R         0       */
  DmaErrorBit0            = 0x00800000,   /* err. 0-Rx DMA, 1-Tx DMA                R         0       */

  DmaTxState              = 0x00700000,   /* Transmit process state                 R         000     */
  DmaTxStopped            = 0x00000000,   /* Stopped                                                  */
  DmaTxFetching           = 0x00100000,   /* Running - fetching the descriptor                        */
  DmaTxWaiting            = 0x00200000,   /* Running - waiting for end of transmission                */
  DmaTxReading            = 0x00300000,   /* Running - reading the data from memory                   */
  DmaTxSuspended          = 0x00600000,   /* Suspended                                                */
  DmaTxClosing            = 0x00700000,   /* Running - closing descriptor                             */

  DmaRxState              = 0x000E0000,   /* Receive process state                  R         000     */
  DmaRxStopped            = 0x00000000,   /* Stopped                                                  */
  DmaRxFetching           = 0x00020000,   /* Running - fetching the descriptor                        */
// CHANGED: Commented as not applicable (07/28)
//  DmaRxChecking           = 0x00040000,   /* Running - checking for end of packet                   */
  DmaRxWaiting            = 0x00060000,   /* Running - waiting for packet                             */
  DmaRxSuspended          = 0x00080000,   /* Suspended                                                */
  DmaRxClosing            = 0x000A0000,   /* Running - closing descriptor                             */
// CHANGED: Commented as not applicable (07/28)
//  DmaRxFlushing           = 0x000C0000,   /* Running - flushing the current frame                   */
  DmaRxQueuing            = 0x000E0000,   /* Running - queuing the recieve frame into host memory     */

  DmaIntNormal            = 0x00010000,   /* Normal interrupt summary               RW        0       */
  DmaIntAbnormal          = 0x00008000,   /* Abnormal interrupt summary             RW        0       */

  DmaIntBusError          = 0x00002000,   /* Fatal bus error (Abnormal)             RW        0       */
  DmaIntRxWdogTO          = 0x00000200,   /* Receive Watchdog Timeout (Abnormal)    RW        0       */
  DmaIntRxStopped         = 0x00000100,   /* Receive process stopped (Abnormal)     RW        0       */
  DmaIntRxNoBuffer        = 0x00000080,   /* Receive buffer unavailable (Abnormal)  RW        0       */
  DmaIntRxCompleted       = 0x00000040,   /* Completion of frame reception (Normal) RW        0       */
  DmaIntTxUnderflow       = 0x00000020,   /* Transmit underflow (Abnormal)          RW        0       */
// CHANGED: Added on 07/28
  DmaIntRcvOverflow       = 0x00000010,   /* Receive Buffer overflow interrupt      RW        0       */
  DmaIntTxJabberTO        = 0x00000008,   /* Transmit Jabber Timeout (Abnormal)     RW        0       */
  DmaIntTxNoBuffer        = 0x00000004,   /* Transmit buffer unavailable (Normal)   RW        0       */
  DmaIntTxStopped         = 0x00000002,   /* Transmit process stopped (Abnormal)    RW        0       */
  DmaIntTxCompleted       = 0x00000001,   /* Transmit completed (Normal)            RW        0       */
};

enum DmaControlReg        /* DMA control register */
{                                         /* Bit description                        R/W   Reset value */
  DmaTxEnable             = 0x00200000,   /*                                        RW        0       */
  DmaTxStart              = 0x00002000,   /* Start/Stop transmission                RW        0       */
// CHANGED: Added on 07/28
  DmaFwdErrorFrames       = 0x00000080,   /* Forward error frames                   RW        0       */
  DmaFwdUnderSzFrames     = 0x00000040,   /* Forward undersize frames               RW        0       */
//  DmaTxSecondFrame        = 0x00000004,   /* Operate on second frame                RW        0       */
  DmaRxStart              = 0x00000002,   /* Start/Stop reception                   RW        0       */
};

enum  DmaInterruptReg     /* DMA interrupt enable register */
{                                         /* Bit description                        R/W   Reset value */
  DmaIeNormal            = DmaIntNormal     ,   /* Normal interrupt enable                 RW        0       */
  DmaIeAbnormal          = DmaIntAbnormal   ,   /* Abnormal interrupt enable               RW        0       */

  DmaIeBusError          = DmaIntBusError   ,   /* Fatal bus error enable                  RW        0       */
  DmaIeRxWdogTO          = DmaIntRxWdogTO   ,   /* Receive Watchdog Timeout enable         RW        0       */
  DmaIeRxStopped         = DmaIntRxStopped  ,   /* Receive process stopped enable          RW        0       */
  DmaIeRxNoBuffer        = DmaIntRxNoBuffer ,   /* Receive buffer unavailable enable       RW        0       */
  DmaIeRxCompleted       = DmaIntRxCompleted,   /* Completion of frame reception enable    RW        0       */
  DmaIeTxUnderflow       = DmaIntTxUnderflow,   /* Transmit underflow enable               RW        0       */
// CHANGED: Added on 07/28
  DmaIeRxOverflow        = DmaIntRcvOverflow,   /* Receive Buffer overflow interrupt       RW        0       */
  DmaIeTxJabberTO        = DmaIntTxJabberTO ,   /* Transmit Jabber Timeout enable          RW        0       */
  DmaIeTxNoBuffer        = DmaIntTxNoBuffer ,   /* Transmit buffer unavailable enable      RW        0       */
  DmaIeTxStopped         = DmaIntTxStopped  ,   /* Transmit process stopped enable         RW        0       */
  DmaIeTxCompleted       = DmaIntTxCompleted,   /* Transmit completed enable               RW        0       */
};

/**********************************************************
 * DMA Engine descriptors
 **********************************************************/

enum DmaDescriptorStatus    /* status word of DMA descriptor */
{
  DescOwnByDma          = 0x80000000,   /* Descriptor is owned by DMA engine  */
// CHANGED: Added on 07/29
  DescDAFilterFail      = 0x40000000,   /* Rx - DA Filter Fail for the received frame        E  */
  DescFrameLengthMask   = 0x3FFF0000,   /* Receive descriptor frame length */
  DescFrameLengthShift  = 16,

  DescError             = 0x00008000,   /* Error summary bit  - OR of the following bits:    v  */

  DescRxTruncated       = 0x00004000,   /* Rx - no more descriptors for receive frame        E  */
// CHANGED: Added on 07/29
  DescSAFilterFail      = 0x00002000,   /* Rx - SA Filter Fail for the received frame        E  */
/* added by reyaz */
  DescRxLengthError	= 0x00001000,   /* Rx - frame size not matching with length field    E  */
  DescRxDamaged         = 0x00000800,   /* Rx - frame was damaged due to buffer overflow     E  */
// CHANGED: Added on 07/29
  DescRxVLANTag         = 0x00000400,   /* Rx - received frame is a VLAN frame               I  */
  DescRxFirst           = 0x00000200,   /* Rx - first descriptor of the frame                I  */
  DescRxLast            = 0x00000100,   /* Rx - last descriptor of the frame                 I  */
  DescRxLongFrame       = 0x00000080,   /* Rx - frame is longer than 1518 bytes              E  */
  DescRxCollision       = 0x00000040,   /* Rx - late collision occurred during reception     E  */
  DescRxFrameEther      = 0x00000020,   /* Rx - Frame type - Ethernet, otherwise 802.3          */
  DescRxWatchdog        = 0x00000010,   /* Rx - watchdog timer expired during reception      E  */
  DescRxMiiError        = 0x00000008,   /* Rx - error reported by MII interface              E  */
  DescRxDribbling       = 0x00000004,   /* Rx - frame contains noninteger multiple of 8 bits    */
  DescRxCrc             = 0x00000002,   /* Rx - CRC error                                    E  */

  DescTxTimeout         = 0x00004000,   /* Tx - Transmit jabber timeout                      E  */
// CHANGED: Added on 07/29
  DescTxFrameFlushed    = 0x00002000,   /* Tx - DMA/MTL flushed the frame due to SW flush    I  */
  DescTxLostCarrier     = 0x00000800,   /* Tx - carrier lost during tramsmission             E  */
  DescTxNoCarrier       = 0x00000400,   /* Tx - no carrier signal from the tranceiver        E  */
  DescTxLateCollision   = 0x00000200,   /* Tx - transmission aborted due to collision        E  */
  DescTxExcCollisions   = 0x00000100,   /* Tx - transmission aborted after 16 collisions     E  */
  DescTxVLANFrame       = 0x00000080,   /* Tx - VLAN-type frame                                 */
  DescTxCollMask        = 0x00000078,   /* Tx - Collision count                                 */
  DescTxCollShift       = 3,
  DescTxExcDeferral     = 0x00000004,   /* Tx - excessive deferral                           E  */
  DescTxUnderflow       = 0x00000002,   /* Tx - late data arrival from the memory            E  */
  DescTxDeferred        = 0x00000001,   /* Tx - frame transmision deferred                      */
};

enum DmaDescriptorLength    /* length word of DMA descriptor */
{
  DescTxIntEnable       = 0x80000000,   /* Tx - interrupt on completion                         */
  DescTxLast            = 0x40000000,   /* Tx - Last segment of the frame                       */
  DescTxFirst           = 0x20000000,   /* Tx - First segment of the frame                      */
  DescTxDisableCrc      = 0x04000000,   /* Tx - Add CRC disabled (first segment only)           */

  DescEndOfRing         = 0x02000000,   /* End of descriptors ring                              */
  DescChain             = 0x01000000,   /* Second buffer address is chain address               */
  DescTxDisablePadd	= 0x00800000,   /* disable padding, added by - reyaz */

  DescSize2Mask         = 0x003FF800,   /* Buffer 2 size                                        */
  DescSize2Shift        = 11,
  DescSize1Mask         = 0x000007FF,   /* Buffer 1 size                                        */
  DescSize1Shift        = 0,
};

/**********************************************************
 * Initial register values
 **********************************************************/

enum InitialRegisters
{
//  GmacConfigInitFdx1000       /* Full-duplex mode with perfect filter on */
  GmacConfigInitFdx100       /* Full-duplex mode with perfect filter on */  	
                          = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable | GmacJumboFrameDisable
// CHANGED: Removed Endian configuration, added single bit config for PAD/CRC strip,   			  
//                          | GmacSelectGmii     | GmacEnableRxOwn          | GmacLoopbackOff
                          | GmacSelectMii
                          | GmacFESEnable
//temp loopback 
                          | GmacEnableRxOwn          | GmacLoopbackOff
//loopback off                          | GmacEnableRxOwn          | GmacLoopbackOn
                          | GmacFullDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable
//                          | GmacIPCheckSumEnable
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable
                          | GmacEnable
                          ,
  GmacConfigInitFdx10       /* Full-duplex mode with perfect filter on */
                          = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable  | GmacJumboFrameDisable
// CHANGED: Removed Endian configuration, added single bit config for PAD/CRC strip,   			  
//                          | GmacSelectMii      | GmacEnableRxOwn          | GmacLoopbackOff
                          | GmacSelectMii
                          | GmacFESDisable
//temp loopback   
                          | GmacEnableRxOwn          | GmacLoopbackOff
//loopback off                          | GmacEnableRxOwn          | GmacLoopbackOn
                          | GmacFullDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable
                          | GmacEnable
                          ,
  GmacFrameFilterInitFdx  /* Full-duplex mode */
// CHANGED: Pass control config, dest addr filter normal, added source address filter, multicast & unicast 
// Hash filter. 
                          = GmacFilterOn          | GmacPassControl0   | GmacBroadcastEnable |  GmacSrcAddrFilterDisable
/*                        = GmacFilterOff         | GmacPassControlOff | GmacBroadcastEnable */
                          | GmacMulticastFilterOn | GmacDestAddrFilterNor | GmacMcastHashFilterOff
                          | GmacPromiscuousModeOff | GmacUcastHashFilterOff,

  GmacFlowControlInitFdx  /* Full-duplex mode */
                          = GmacUnicastPauseFrameOff | GmacRxFlowControlEnable | GmacTxFlowControlEnable,

  GmacGmiiAddrInitFdx     /* Full-duplex mode */
                          = GmiiAppClk2,

//  GmacConfigInitHdx1000       /* Half-duplex mode with perfect filter on */
  GmacConfigInitHdx100       /* Half-duplex mode with perfect filter on */
                          = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable  | GmacJumboFrameDisable
// CHANGED: Removed Endian configuration, added single bit config for PAD/CRC strip,   			  
                        /*| GmacSelectMii      | GmacLittleEndian         | GmacDisableRxOwn      | GmacLoopbackOff*/
                          | GmacSelectMii
                          | GmacFESEnable
//                          | GmacSelectGmii     | GmacDisableRxOwn         | GmacLoopbackOff
                                                      | GmacDisableRxOwn         | GmacLoopbackOff
                          | GmacHalfDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable   
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable
                          | GmacEnable
                          ,

  GmacConfigInitHdx10       /* Half-duplex mode with perfect filter on */
                          = GmacWatchdogEnable | GmacJabberEnable         | GmacFrameBurstEnable  | GmacJumboFrameDisable
// CHANGED: Removed Endian configuration, added single bit config for PAD/CRC strip,   			  
                          | GmacSelectMii
                          | GmacFESDisable
//                          | GmacSelectMii      | GmacDisableRxOwn         | GmacLoopbackOff
                                                      | GmacDisableRxOwn         | GmacLoopbackOff
                          | GmacHalfDuplex     | GmacRetryEnable          | GmacPadCrcStripDisable 
                          | GmacBackoffLimit0  | GmacDeferralCheckDisable | GmacTxEnable          | GmacRxEnable
                          | GmacEnable
                          ,

  GmacFrameFilterInitHdx  /* Half-duplex mode */
                          = GmacFilterOn          | GmacPassControl0        | GmacBroadcastEnable | GmacSrcAddrFilterDisable
                          | GmacMulticastFilterOn | GmacDestAddrFilterNor   | GmacMcastHashFilterOff
                          | GmacUcastHashFilterOff| GmacPromiscuousModeOff,

  GmacFlowControlInitHdx  /* Half-duplex mode */
                          = GmacUnicastPauseFrameOff | GmacRxFlowControlDisable | GmacTxFlowControlDisable,

  GmacGmiiAddrInitHdx     /* Half-duplex mode */
                          = GmiiAppClk2,

  DmaBusModeInit          /* Little-endian mode */
// CHANGED: Removed Endian configuration   			  
//                          = DmaFixedBurstEnable |   DmaBurstLength8   | DmaDescriptorSkip2       | DmaResetOff,
                          = DmaFixedBurstEnable |   DmaBurstLength8   | DmaResetOff,                          

//  DmaControlInit1000      /* 1000 Mb/s mode */
//                          = DmaTxEnable       | DmaTxSecondFrame ,

  DmaControlInit100       /* 100 Mb/s mode */
                          = DmaTxEnable,

  DmaControlInit10        /* 10 Mb/s mode */
                          = DmaTxEnable,

                          /* Interrupt groups */
  DmaIntErrorMask         = DmaIntBusError,           /* Error */
  DmaIntRxAbnMask         = DmaIntRxNoBuffer,         /* receiver abnormal interrupt */
  DmaIntRxNormMask        = DmaIntRxCompleted,        /* receiver normal interrupt   */
  DmaIntRxStoppedMask     = DmaIntRxStopped,          /* receiver stopped */
  DmaIntTxAbnMask         = DmaIntTxUnderflow,        /* transmitter abnormal interrupt */
  DmaIntTxNormMask        = DmaIntTxCompleted,        /* transmitter normal interrupt */
  DmaIntTxStoppedMask     = DmaIntTxStopped,          /* receiver stopped */

#if 0
  DmaIntEnable            = DmaIeNormal     | DmaIeAbnormal    | DmaIntErrorMask
                          | DmaIntRxAbnMask | DmaIntRxNormMask | DmaIntRxStoppedMask
                          | DmaIntTxAbnMask | DmaIntTxNormMask | DmaIntTxStoppedMask,
#else
  DmaIntEnable            = DmaIeNormal     | DmaIeAbnormal    | DmaIntErrorMask
                          | DmaIntRxAbnMask | DmaIntRxNormMask  
                          | DmaIntTxAbnMask | DmaIntTxNormMask,
#endif
  DmaIntDisable           = 0,
};


/**********************************************************
 * device cfg related functions
 **********************************************************/
void hwcfgsetup(void)
{
    u32 data;
    //pinmux setup
#if defined (CONFIG_ARCH_MT5391)
    /******* MT5391 ********/
    //MII & gpio125
    data = __raw_readl(CKGEN_VIRT + 0x404);
    data &= ~0xfff00000;
#ifdef MT539X_LAN8700
    data |= 0x11100000;                                 //use TX_ER
#else    
    data |= 0x12100000;                                 //[26:24]->2 GPIO125(ONDA8)
#endif
    __raw_writel(data, CKGEN_VIRT + 0x404);  //0x12100000 [30:28], [26:24], [22:20]
                                                               
    data = __raw_readl(CKGEN_VIRT + 0x408);
    data &= ~0x0000FFFF;
    data |= 0x1111;
     __raw_writel(data, CKGEN_VIRT + 0x408); //0x00001111 [14:12], [10:8], [6:4], [2:0]

    //ckgen setup, all register own by MII CKCFG
    __raw_writel(0x3200, CKGEN_VIRT + 0x264);
    
#elif defined (CONFIG_ARCH_MT5363)
    /******* MT5363 ********/
    //MII pinmux
#if defined (MT539X_LAN8700)
    /* when SMSC LAN87000 is used, TX_ER[19:18] is used (func1)*/
    __raw_writel(0x00155555, CKGEN_VIRT + 0x414);
#elif defined (ETH_RMII_PHY)
    /* RMII phy is used */
    __raw_writel(0x00104001, CKGEN_VIRT + 0x414);
#else 
    /* default use RTL PHY */
    /* TX_ER[19:18] pin is set to GPIO (func0) used for RST*/
    __raw_writel(0x00115555, CKGEN_VIRT + 0x414);
#endif    

#ifdef ETH_RMII_PHY
    /* RMII */
    //ckgen setup -rmii
    data = __raw_readl(CKGEN_VIRT + 0x288);
    data &= ~0x7;
    data |= 0x2;
    __raw_writel(data, CKGEN_VIRT + 0x288);
    //ckgen setup - miitx
    data = __raw_readl(CKGEN_VIRT + 0x28c);
    data &= ~0x7;
    data |= 0x2;
    __raw_writel(data, CKGEN_VIRT + 0x28c);
    //ckgen setup - miirx
    data = __raw_readl(CKGEN_VIRT + 0x290);
    data &= ~0x7;
    data |= 0x2;
    __raw_writel(data, CKGEN_VIRT + 0x290);
    
#else
    /* MII */
    //ckgen setup - miitx
    data = __raw_readl(CKGEN_VIRT + 0x28c);
    data &= ~0x7;
    data |= 0x4;
    __raw_writel(data, CKGEN_VIRT + 0x28c);
    //ckgen setup - miirx
    data = __raw_readl(CKGEN_VIRT + 0x290);
    data &= ~0x7;
    data |= 0x5;
    __raw_writel(data, CKGEN_VIRT + 0x290);
#endif
    
#endif /* CONFIG_ARCH_MT53xx */

}

void hw_reset(void)
{
    volatile u32 data;

#ifdef MT539X_LAN8700   //use gpio18
    //reset output ++
    data = __raw_readl(CKGEN_VIRT + 0x520);
    data &= ~0x00040000;
    data |= 0x00040000;                                 //[29]->1 GPIO125 out (ONDA8)
    __raw_writel(data, CKGEN_VIRT + 0x520);  //0x12100000 [30:28], [26:24], [22:20]
    //reset output --
    
    //reset low 
    data = __raw_readl(CKGEN_VIRT + 0x500);
    data &= ~0x00040000;                              //[29]->0 GPIO125 low (ONDA8)
    __raw_writel(data, CKGEN_VIRT + 0x500); 

    //wait 
    /* wait for 10ms */
    msleep(10);
                                                                 
    //reset high
    //release HW reset pin first
    data = __raw_readl(CKGEN_VIRT + 0x500);
    data |= 0x00040000;                                 //[29]->0 GPIO125 high (ONDA8)
    __raw_writel(data, CKGEN_VIRT + 0x500); 

#else /* RTL */

    #if defined (CONFIG_ARCH_MT5391)
        /******** MT5391 *********/
        //reset output ++
        data = __raw_readl(CKGEN_VIRT + 0x52C);
        data &= ~0x20000000;
        data |= 0x20000000;                                 //[29]->1 GPIO125 out (ONDA8)
        __raw_writel(data, CKGEN_VIRT + 0x52C);  //0x12100000 [30:28], [26:24], [22:20]
        //reset output --
        
        //reset low 
        data = __raw_readl(CKGEN_VIRT + 0x50C);
        data &= ~0x20000000;                              //[29]->0 GPIO125 low (ONDA8)
        __raw_writel(data, CKGEN_VIRT + 0x50C); 

        //wait 
        /* wait for 10ms */
        msleep(10);
                                                                     
        //reset high
        //release HW reset pin first
        data = __raw_readl(CKGEN_VIRT + 0x50C);
        data |= 0x20000000;                                 //[29]->0 GPIO125 high (ONDA8)
        __raw_writel(data, CKGEN_VIRT + 0x50C); 
    
    #elif defined (CONFIG_ARCH_MT5363)
        /******** MT5363 *********/
        data = __raw_readl(CKGEN_VIRT + 0x534); //EN: 0xd534
        data |= 0x00004000; //bit14
        __raw_writel(data, CKGEN_VIRT + 0x534);

        //reset low --
        data = __raw_readl(CKGEN_VIRT + 0x530); //GPIOOUT: 0xd530
        data &= ~0x00004000; //bit14
        __raw_writel(data, CKGEN_VIRT + 0x530);
        
        /* wait for 10ms */
        msleep(10);

        //reset high ++
        data = __raw_readl(CKGEN_VIRT + 0x530); //GPIOOUT: 0xd530
        data |= 0x00004000; //bit14
        __raw_writel(data, CKGEN_VIRT + 0x530);
        
    #endif /* CONFIG_ARCH_MT53xx */

    
#endif /* MT539X_LAN8700 */
}

/**********************************************************
 * Common functions
 **********************************************************/

int MmacInit( Mmac *Dev, u32 ConfigBase, u32 MacBase, u32 DmaBase, u32 PhyAddr )
{
  memset( Dev, 0, sizeof(*Dev) );

  Dev->configBase = ConfigBase;
  Dev->macBase = MacBase;
  Dev->dmaBase = DmaBase;
  Dev->phyAddr = PhyAddr;
  Dev->mmcBase = MMC_BASE;

  return 0;
}

/**********************************************************
 * MAC module functions
 **********************************************************/
void  MmacMacReset(Mmac *mmac)
{
//  MmacClearMacReg(mmac, GmacConfig, GmacTxEnable |GmacRxEnable);
  MmacClearMacReg(mmac, GmacConfig, GmacTxEnable |GmacRxEnable | GmacFullDuplex | GmacFESEnable);  
  if( mmac->full_duplex)
  {
    if(mmac->speed_100)
    {
      MmacSetMacReg( mmac, GmacConfig, GmacConfigInitFdx100 ); /* set init values of config registers */
    }
    else
    {
      MmacSetMacReg( mmac, GmacConfig, GmacConfigInitFdx10 ); /* set init values of config registers with MII port */
    }

    MmacWriteMacReg( mmac, GmacFrameFilter, GmacFrameFilterInitFdx );
    MmacWriteMacReg( mmac, GmacFlowControl, GmacFlowControlInitFdx );
    MmacWriteMacReg( mmac, GmacGmiiAddr, GmacGmiiAddrInitFdx );
  }
  else
  {
    if(mmac->speed_100)
    {
      MmacSetMacReg( mmac, GmacConfig, GmacConfigInitHdx100 ); /* set init values of config registers */
    }
    else
    {
      MmacSetMacReg( mmac, GmacConfig, GmacConfigInitHdx10 ); /* set init values of config registers with MII port */
    }

    MmacWriteMacReg( mmac, GmacFrameFilter, GmacFrameFilterInitHdx );
    MmacWriteMacReg( mmac, GmacFlowControl, GmacFlowControlInitHdx );
    MmacWriteMacReg( mmac, GmacGmiiAddr, GmacGmiiAddrInitHdx );
  }

//  MmacSetMacReg(mmac, GmacConfig, GmacTxEnable |GmacRxEnable);
}

static int Hash( u8 addr[6] )   /* returns hash bit number for given MAC address */
{
  int i;
  u32 crc = 0xFFFFFFFF;
  u32 poly = 0xEDB88320;

  for( i=0; i<6; i++ )
  {
    int bit;
    u8 data = addr[i];
    for( bit=0; bit<8; bit++ )
    {
      int p = (crc^data) & 1;
      crc >>= 1;
      if( p != 0 ) crc ^= poly;
      data >>= 1;
    }
  }

  return (crc>>26) & 0x3F;      /* return upper 6 bits */
}

int MmacMacInit( Mmac *Dev, u8 Addr[6], u8 Broadcast[6] )
{
  u32 data;


//+++
//---



  data = (Addr[5]<<8) | Addr[4];                            /* set our MAC address */
  MmacWriteMacReg( Dev, GmacAddr0High, data );
  data = (Addr[3]<<24) | (Addr[2]<<16) | (Addr[1]<<8) | Addr[0];
  MmacWriteMacReg( Dev, GmacAddr0Low, data );

  if(0)
  {
    int n;

    MmacWriteMacReg( Dev, GmacHashHigh, 0 );                /* clear hash table */
    MmacWriteMacReg( Dev, GmacHashLow, 0 );

    n = Hash( Broadcast );                                    /* set the broadcast address */
    if( n>=32 ) MmacSetMacReg( Dev, GmacHashHigh, 1<<(n-32) );
    else MmacSetMacReg( Dev, GmacHashLow, 1<<n );
  }
  MmacSetMacReg( Dev, GmacConfig, GmacEnable ); /* enable mac before set registers */      
  MmacClearMacReg(Dev, GmacConfig, GmacTxEnable |GmacRxEnable | GmacFullDuplex | GmacFESEnable);  

  if( Dev->full_duplex)
  {
/* Added by Lokesh */
    if(Dev->speed_100)
    {
      MmacSetMacReg( Dev, GmacConfig, GmacConfigInitFdx100 ); /* set init values of config registers */      
    }
    else
    {
      MmacSetMacReg( Dev, GmacConfig, GmacConfigInitFdx10 ); /* set init values of config registers with MII port */
    }

    MmacWriteMacReg( Dev, GmacFrameFilter, GmacFrameFilterInitFdx );
    MmacWriteMacReg( Dev, GmacFlowControl, GmacFlowControlInitFdx );
    MmacWriteMacReg( Dev, GmacGmiiAddr, GmacGmiiAddrInitFdx );
  }
  else
  {
/* Modified by Lokesh */
    if(Dev->speed_100)
    {
      MmacSetMacReg( Dev, GmacConfig, GmacConfigInitHdx100 ); /* set init values of config registers */
    }
    else
    {
      MmacSetMacReg( Dev, GmacConfig, GmacConfigInitHdx10 ); /* set init values of config registers with MII port */
    }

    MmacWriteMacReg( Dev, GmacFrameFilter, GmacFrameFilterInitHdx );
    MmacWriteMacReg( Dev, GmacFlowControl, GmacFlowControlInitHdx );
    MmacWriteMacReg( Dev, GmacGmiiAddr, GmacGmiiAddrInitHdx );
/* To set PHY register to enable CRS on Transmit */
//    MmacWriteMacReg( Dev, GmacGmiiAddr, GmiiBusy | 0x00000408);
//    PHYreg = MmacReadMacReg(Dev, GmacGmiiData);
//    MmacWriteMacReg( Dev, GmacGmiiData, PHYreg | 0x00000800);
//    MmacWriteMacReg( Dev, GmacGmiiAddr, GmiiBusy | 0x0000040a);
  }

  return 0;
}

void MmacPassAllPacket( Mmac *Dev)
{
  MmacSetMacReg( Dev, GmacFrameFilter, GmacPromiscuousModeOn );
}


void MmacPassAllMulticast( Mmac *Dev)
{
  MmacClearMacReg( Dev, GmacFrameFilter, GmacPromiscuousModeOn );
  MmacSetMacReg( Dev, GmacFrameFilter, GmacMulticastFilterOff );
}

void MmacPassMatchPacket(struct net_device *dev, Mmac *mmac )
{
    int i;
    u32 filter;
    struct dev_mc_list *cur_addr;
    u32 data;
    u8* Addr;

    //clear all filter 
    for (i = 0; i < ETHER_MAX_MULTICAST_NUM; i++) 
    {
      filter = MmacReadMacReg(mmac, GmacAddr1High + (i*8));
      if(filter & (0x01<<31))
      {
        MmacWriteMacReg(mmac, GmacAddr1High + (i*8), 0xFFFF);
      }
      else
      {
    //    TR(KERN_NOTICE "%s: Filter final clear on %d\n", dev->name, i);
        break;
      }
    }

    if (dev->mc_count)                         // pass multicast frames which match DA  filter, need to set filter here
    {
      TR(KERN_NOTICE "%s: MC count %d\n", dev->name, dev->mc_count);
//      struct dev_mc_list *cur_addr;
//      u32 data;
//      u8* Addr;
      cur_addr = dev->mc_list;
	for (i = 0; i < dev->mc_count; i++, cur_addr = cur_addr->next) 
	{
	  /* do we have a pointer here? */
	  if (!cur_addr) 	break;
	  /* make sure this is a multicast address -shouldn't this be a given if we have it here ? */
	  if (!(*cur_addr->dmi_addr & 1))	 continue;

         Addr = cur_addr->dmi_addr;
         data = (Addr[5]<<8) | Addr[4] | (1<<31);                          
         MmacWriteMacReg( mmac, GmacAddr1High + (i*8), data );
         data = (Addr[3]<<24) | (Addr[2]<<16) | (Addr[1]<<8) | Addr[0];
         MmacWriteMacReg( mmac, GmacAddr1Low + (i*8), data );
	}
    }

    MmacClearMacReg( mmac, GmacFrameFilter, GmacPromiscuousModeOn );
    MmacClearMacReg( mmac, GmacFrameFilter, GmacMulticastFilterOff );
}


u16 MmacMiiRead( Mmac *Dev, u8 Reg )
{
  u32 addr;
  u16 data;

  addr = ((Dev->phyAddr << GmiiDevShift) & GmiiDevMask) | ((Reg << GmiiRegShift) & GmiiRegMask);
  MmacWriteMacReg( Dev, GmacGmiiAddr, (addr | GmiiAppClk4 | GmiiBusy));

  do{} while( (MmacReadMacReg( Dev, GmacGmiiAddr ) & GmiiBusy) == GmiiBusy );

  data = MmacReadMacReg( Dev, GmacGmiiData ) & 0xFFFF;
  return data;
}

void MmacMiiWrite( Mmac *Dev, u8 Reg, u16 Data )
{
  u32 addr;

  MmacWriteMacReg( Dev, GmacGmiiData, Data );

  addr = ((Dev->phyAddr << GmiiDevShift) & GmiiDevMask) | ((Reg << GmiiRegShift) & GmiiRegMask) | GmiiWrite;
  MmacWriteMacReg( Dev, GmacGmiiAddr, (addr | GmiiAppClk4 | GmiiBusy));

  do{} while( (MmacReadMacReg( Dev, GmacGmiiAddr ) & GmiiBusy) == GmiiBusy );
}

/**********************************************************
 * DMA engine functions
 **********************************************************/

static void DescInit( DmaDesc *Desc, int EndOfRing )
{
  Desc->status = 0;
  Desc->length = EndOfRing ? DescEndOfRing : 0;
  Desc->length |= DescChain;
  Desc->buffer1 = 0;
//  Desc->buffer2 = 0;
  Desc->data1 = 0;
  Desc->data2 = 0;
}

static int DescLast( DmaDesc *Desc )
{
  return (Desc->length & DescEndOfRing) != 0;
}

static int DescEmpty( DmaDesc *Desc )
{
//  return (Desc->length & ~ (DescEndOfRing|DescChain)) == 0;
  return (Desc->length & DescSize1Mask) == 0;  
}

static int DescDma( DmaDesc *Desc )
{
  return (Desc->status & DescOwnByDma) != 0;   /* Owned by DMA */
}
#if 1
void DescTake( DmaDesc *Desc )   /* Take ownership */
{
  if( DescDma(Desc) )               /* if descriptor is owned by DMA engine */
  {
    Desc->status &= ~DescOwnByDma;    /* clear DMA own bit */
    Desc->status |= DescError;        /* Set error bit to mark this descriptor bad */
  }
}
#endif
void MmacMMCInit( Mmac *Dev)
{
    MmacWriteMmcReg( Dev, MmcRxIntMask, 0xffffffff );         // mask all Rx MMC IRQ 
    MmacWriteMmcReg( Dev, MmcTxIntMask, 0xffffffff );       // mask all Tx MMC IRQ 
}
int MmacDmaInit( Mmac *Dev, void *Buffer, u32 Size )
{
  int n = Size/sizeof(DmaDesc);     /* number of descriptors that can fit into the buffer passed */
  int i;

  Dev->txCount = n/2;               /* use 1/2 for Tx descriptors and 1/2 for Rx descriptors */
  Dev->rxCount = n - Dev->txCount;

  Dev->txFreeCount = Dev->txCount;
  Dev->tx = Buffer;
  Dev->rx = Dev->tx + Dev->txCount;

  for( i=0; i<Dev->txCount; i++ ) DescInit( Dev->tx + i, i==Dev->txCount-1 );
  for( i=0; i<Dev->rxCount; i++ ) DescInit( Dev->rx + i, i==Dev->rxCount-1 );

  Dev->txNext = 0;
  Dev->txBusy = 0;
  Dev->rxNext = 0;
  Dev->rxBusy = 0;

  MmacWriteDmaReg( Dev, DmaBusMode, DmaResetOn );       /* Reset DMA engine */
  udelay(2);      /* Delay 2 usec (=50 PCI cycles on 25 Mhz) */
  MmacWriteDmaReg( Dev, DmaBusMode, DmaBusModeInit );   /* Set init register values */
  MmacWriteDmaReg( Dev, DmaRxBaseAddr, virt_to_phys(Dev->rx) );
  MmacWriteDmaReg( Dev, DmaTxBaseAddr, virt_to_phys(Dev->tx) );

  for( i=0; i<Dev->txCount; i++ ) 
  {
  	MmacDmaTxSet(Dev,0,0,0);
  }
  if( Dev->speed_100 ) 
  {
    MmacWriteDmaReg( Dev, DmaControl, DmaControlInit100 );
  }
  else 
  {
    MmacWriteDmaReg( Dev, DmaControl, DmaControlInit10 );
  }
  MmacWriteDmaReg( Dev, DmaInterrupt, DmaIntDisable );

  return 0;
}

int MmacDmaRxSet( Mmac *Dev, u32 Buffer1, u32 Length1, u32 Data1 )
{
  int desc = Dev->rxNext;
  DmaDesc *rx = Dev->rx + desc;
  DmaDesc *prxNext;
  if( !DescEmpty(rx) ) return -1;       /* descriptor is not empty - cannot set */
#if 0
  rx->length &= DescEndOfRing;      /* clear everything */
  rx->length |= ((Length1 << DescSize1Shift) & DescSize1Mask);
  rx->buffer1 = Buffer1;
  rx->data1   = Data1;
  rx->status  = DescOwnByDma;
  
  Dev->rxNext = DescLast(rx) ? 0 : desc + 1;
#else
  rx->length &= DescEndOfRing;      /* clear everything */
  rx->length |= ((Length1 << DescSize1Shift) & DescSize1Mask);
  rx->length |= DescChain;  
  rx->buffer1 = Buffer1;
  rx->data1   = Data1;
  prxNext = Dev->rx + (DescLast(rx) ? 0 : desc + 1);
  rx->buffer2 = virt_to_phys(prxNext);
  rx->status  = DescOwnByDma;
  
  Dev->rxNext = DescLast(rx) ? 0 : desc + 1;
#endif


  return desc;
}

int MmacDmaRxGet( Mmac *Dev, u32 *Status, u32 *Buffer1, u32 *Length1, u32 *Data1 )
{
  int desc = Dev->rxBusy;
  DmaDesc *rx = Dev->rx + desc;

//  MmacDmaDescSync(Dev, rx, sizeof(DmaDesc), DMA_FROM_DEVICE);  //invalidate cache from physical memory
  if( DescDma(rx) ) return -1;          /* descriptor is owned by DMA - cannot get */
  if( DescEmpty(rx) ) return -1;        /* descriptor is empty - cannot get */

  if( Status != 0 ) *Status = rx->status;
  if( Length1 != 0 ) *Length1 = (rx->length & DescSize1Mask) >> DescSize1Shift;
  if( Buffer1 != 0 ) *Buffer1 = rx->buffer1;
  if( Data1 != 0 ) *Data1 = rx->data1;

  DescInit( rx, DescLast(rx) );
  Dev->rxBusy = DescLast(rx) ? 0 : desc + 1;
  MmacDmaDescSync(Dev, rx, sizeof(DmaDesc), DMA_TO_DEVICE);  //flush cache to physical memory

  return desc;
}

void   RxDescInit(struct net_device *dev, Private *pr, Mmac *mmac )
{
  int r = 0;

  do                          /* prepare recieve descriptors */
  {
    struct sk_buff *skb = alloc_skb( dev->mtu + ETHERNET_PACKET_EXTRA, GFP_ATOMIC );

    if( skb == NULL )
    {
      printk( KERN_ERR "Mmac::open(%s) - no memory for sk_buff\n", dev->name );
      break;
    }
    /* do not skb_reserve( skb, 2 ) because rx buffer must be 4-bytes aligned */
    r = MmacDmaRxSet( mmac, virt_to_phys(skb->tail), skb_tailroom(skb), (u32)skb );
    TR( KERN_DEBUG "Mmac::open(%s) - set rx descriptor %d for skb %p\n", dev->name, r, skb );
    if( r < 0 ) 
    {
      dev_kfree_skb(skb);
    }
  } while( r >= 0 );
  
  MmacDmaDescSync(mmac, mmac->rx, mmac->rxCount*sizeof(DmaDesc), DMA_TO_DEVICE);  //flush cache to physical memory
}

void MmacDmaDescSync( Mmac *Dev, void *vaddr, size_t size, int direction )
{
  //consistent_sync(vaddr, size, direction);
   dma_map_single(NULL, vaddr, size, direction);  
}
int MmacDmaTxSet( Mmac *Dev, u32 Buffer1, u32 Length1, u32 Data1 )
{
  int desc = Dev->txNext;
  DmaDesc *tx = Dev->tx + desc;
  DmaDesc *ptxNext;

  if( !DescEmpty(tx) ) return -1; /* descriptor is not empty - cannot set */

  tx->length &= DescEndOfRing;      /* clear everything */
  tx->length |= ((Length1 << DescSize1Shift) & DescSize1Mask) | DescTxFirst | DescTxLast | DescTxIntEnable;
  tx->length |= DescChain;  


if( Buffer1 != 0 ) tx->buffer1 = Buffer1;
if( Data1 != 0 )   tx->data1   = Data1;
  ptxNext = Dev->tx + (DescLast(tx) ? 0 : desc + 1);
  tx->buffer2 = virt_to_phys(ptxNext);
if( Buffer1 != 0 )  tx->status  = DescOwnByDma;

  Dev->txNext = DescLast(tx) ? 0 : desc + 1;

/*
  printk("<1> \n Desc length = %x \n", tx->length);
  printk("<1> \n Buffer1 = 0x%x", tx->buffer1);
  printk("<1> \n Data1   = 0x%x", tx->data1);
  printk("<1> \n Status  = 0x%x", tx->status);
  printk("<1> \n Next Desc Num  = 0x%d",Dev->txNext);
  printk("<1> \n CurrBufferAddr = 0x%x\n",MmacReadDmaReg(Dev,DmaTxCurrAddr));
*/

  return desc;
}

int MmacDmaTxGet( Mmac *Dev, u32 *Status, u32 *Buffer1, u32 *Length1, u32 *Data1 )
{
  int desc = Dev->txBusy;
  DmaDesc *tx = Dev->tx + desc;

//  MmacDmaDescSync(Dev, tx, sizeof(DmaDesc), DMA_FROM_DEVICE);  //invalidate cache from physical memory


  if( DescDma(tx) ) return -1;          /* descriptor is owned by DMA - cannot get */
  if( DescEmpty(tx) ) return -1;        /* descriptor is empty - cannot get */

  if( Status != 0 ) *Status = tx->status;
  if( Buffer1 != 0 ) *Buffer1 = tx->buffer1;
  if( Length1 != 0 ) *Length1 = (tx->length & DescSize1Mask) >> DescSize1Shift;
  if( Data1 != 0 ) *Data1 = tx->data1;

  DescInit( tx, DescLast(tx) );
  Dev->txBusy = DescLast(tx) ? 0 : desc + 1;
  MmacDmaDescSync(Dev, tx, sizeof(DmaDesc), DMA_TO_DEVICE);  //flush cache to physical memory

  return desc;
}

void MmacDmaRxStart( Mmac *Dev )
{
  MmacSetDmaReg( Dev, DmaControl, DmaRxStart );
}

void MmacDmaRxStop( Mmac *Dev )
{
//  int i;
  MmacClearDmaReg( Dev, DmaControl, DmaRxStart );
//  for( i=0; i<Dev->rxCount; i++ ) DescTake( Dev->rx + i );
}

void MmacDmaTxStart( Mmac *Dev )
{
  MmacSetDmaReg( Dev, DmaControl, DmaTxStart );
}

void MmacDmaTxStop( Mmac *Dev )
{
//  int i;
  MmacClearDmaReg( Dev, DmaControl, DmaTxStart );
//  for( i=0; i<Dev->txCount; i++ ) DescTake( Dev->tx + i );
}

void MmacDmaRxResume( Mmac *Dev )
{
  MmacWriteDmaReg( Dev, DmaRxPollDemand, 0 );
}

void MmacDmaTxResume( Mmac *Dev )
{
  MmacWriteDmaReg( Dev, DmaTxPollDemand, 0 );
}

int MmacDmaRxValid( u32 Status )
{
  return ( (Status & DescError) == 0 )      /* no errors, whole frame is in the buffer */
      && ( (Status & DescRxFirst) != 0 )
      && ( (Status & DescRxLast) != 0 );
}

// Returns 1 when the Dribbling status is set
int MmacDmaRxStatusDriblling( u32 Status )
{
  return ( (Status & DescRxDribbling) != 0 );
}

/* Returns 1 when the Crc Error is set
int MmacDmaRxCrc ( u32 Status )
{
  return ( (Status & DescRxCrc) != 0 );
}
*/

u32 MmacDmaRxLength( u32 Status )
{
  return (Status & DescFrameLengthMask) >> DescFrameLengthShift;
}

int MmacDmaRxCollisions(  u32 Status)
{
  if( Status & (DescRxDamaged | DescRxCollision) ) return 1;
  return 0;
}

int MmacDmaRxCrc(  u32 Status)
{
  if( Status & DescRxCrc ) return 1;
  return 0;
}
/* added by reyaz */
int MmacDmaRxFrame(  u32 Status)
{
  if( Status & DescRxDribbling ) return 1;
  return 0;
}

int MmacDmaRxLongFrame(  u32 Status)
{
  if( Status & DescRxLongFrame ) return 1;
  return 0;
}
int MmacDmaRxLengthError(  u32 Status)
{
  if( Status & DescRxLengthError ) return 1;
  return 0;
}
/* Test the status word if the descriptor is valid */
int MmacDmaTxValid(  u32 Status)
{
  return ( (Status & DescError) == 0 );
}

int MmacDmaTxCollisions(  u32 Status)
{
  return (Status & DescTxCollMask) >> DescTxCollShift;
}

int MmacDmaTxAborted(  u32 Status)
{
  if( Status & (DescTxLateCollision | DescTxExcCollisions )) return 1;
  return 0;
}

int MmacDmaTxCarrier(  u32 Status)
{
  if( Status & (DescTxLostCarrier | DescTxNoCarrier )) return 1;
  return 0;
}


/**********************************************************
 * DMA engine interrupt handling functions
 **********************************************************/

void MmacDmaIntEnable( Mmac *Dev )
{
  MmacWriteDmaReg( Dev, DmaInterrupt, DmaIntEnable );    /* enable interrupts */
}

void MmacDmaIntDisable( Mmac *Dev )
{
  MmacWriteDmaReg( Dev, DmaInterrupt, DmaIntDisable );    /* disable interrupts */
}

void MmacDmaIntClear( Mmac *Dev )
{
  MmacWriteDmaReg( Dev, DmaStatus, MmacReadDmaReg( Dev, DmaStatus) );    /* clear all interrupt requests */
}

u32 MmacDmaIntType( Mmac *Dev )
{
  u32 status = MmacReadDmaReg( Dev, DmaStatus );
  u32 type = 0;

  if( status & DmaIntErrorMask )      type |= MmacDmaError;
  if( status & DmaIntRxNormMask )     type |= MmacDmaRxNormal;
  if( status & DmaIntRxAbnMask )      type |= MmacDmaRxAbnormal;
  if( status & DmaIntRxStoppedMask )  type |= MmacDmaRxStopped;
  if( status & DmaIntTxNormMask )     type |= MmacDmaTxNormal;
  if( status & DmaIntTxAbnMask )      type |= MmacDmaTxAbnormal;
  if( status & DmaIntTxStoppedMask )  type |= MmacDmaTxStopped;

  MmacWriteDmaReg( Dev, DmaStatus, status );     /* clear all interrupt requests */

  return type;
}
