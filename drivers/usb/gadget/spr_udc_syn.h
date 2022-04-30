/* 
 * linux/drivers/usb/gadget/spearhead_udc.h 
 * 
 *  Copyright (C) 2006   STMicroelectronics 
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License ,or 
 * ( at your option ) any later version. 
 * 
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not  , write to the Free Software 
 * Foundation ,Inc. ,59 Temple Place ,Suite 330 ,Boston ,MA  02111-1307 USA 
 * 
 *  Author - Rajeev Kumar <rajeev-dlh.kumar@st.com>
 */

/*******************************************************************
 *                                                                 *
 *                            CONSTANTS                            *
 *                                                                 *
 *******************************************************************/

#ifndef __SPR_UDC_SYN_H__
#define __SPR_UDC_SYN_H__

#define SYNUDC_NUM_ENDPOINTS     16

#define EP0_MAXPCK_SIZE         64

#define EP1_IN_MAXPCK_SIZE      512  
#define EP1_OUT_MAXPCK_SIZE     512 

#define EP2_IN_MAXPCK_SIZE      512  
#define EP2_OUT_MAXPCK_SIZE     512 

#define EP3_IN_MAXPCK_SIZE      512  
#define EP3_OUT_MAXPCK_SIZE     512 

#define EP4_IN_MAXPCK_SIZE      512  
#define EP4_OUT_MAXPCK_SIZE     512 

#define EP5_IN_MAXPCK_SIZE      512  
#define EP5_OUT_MAXPCK_SIZE     512 

#define EP6_IN_MAXPCK_SIZE      512  
#define EP6_OUT_MAXPCK_SIZE      1024

#define EP7_IN_MAXPCK_SIZE      1024  
#define EP7_OUT_MAXPCK_SIZE     512 

#define EP8_IN_MAXPCK_SIZE      512  
#define EP8_OUT_MAXPCK_SIZE     512 

#define EP9_IN_MAXPCK_SIZE      512  
#define EP9_OUT_MAXPCK_SIZE     512 

#define EP10_IN_MAXPCK_SIZE      512  
#define EP10_OUT_MAXPCK_SIZE     512 

#define EP11_IN_MAXPCK_SIZE      512  
#define EP11_OUT_MAXPCK_SIZE     512 

#define EP11_IN_MAXPCK_SIZE      512  
#define EP11_OUT_MAXPCK_SIZE     512 

#define EP12_IN_MAXPCK_SIZE      512  
#define EP12_OUT_MAXPCK_SIZE     512 

#define EP13_IN_MAXPCK_SIZE      512  
#define EP13_OUT_MAXPCK_SIZE     512 

#define EP14_IN_MAXPCK_SIZE      512  
#define EP14_OUT_MAXPCK_SIZE     512 

#define EP15_IN_MAXPCK_SIZE      512  
#define EP15_OUT_MAXPCK_SIZE     512 

#define EP0_FIFO_SIZE           64/4

#define EP1_IN_FIFO_SIZE        512/4
#define EP1_OUT_FIFO_SIZE       512/4  

#define EP2_IN_FIFO_SIZE       512/4  
#define EP2_OUT_FIFO_SIZE       512/4

#define EP3_IN_FIFO_SIZE       512/4  
#define EP3_OUT_FIFO_SIZE       512/4

#define EP4_IN_FIFO_SIZE       512/4  
#define EP4_OUT_FIFO_SIZE       512/4

#define EP5_IN_FIFO_SIZE       512/4  
#define EP5_OUT_FIFO_SIZE       512/4

#define EP6_IN_FIFO_SIZE       512/4  
#define EP6_OUT_FIFO_SIZE       1024/4

#define EP7_IN_FIFO_SIZE       2048/4  
#define EP7_OUT_FIFO_SIZE       512/4

#define EP8_IN_FIFO_SIZE       512/4  
#define EP8_OUT_FIFO_SIZE       512/4

#define EP9_IN_FIFO_SIZE       512/4  
#define EP9_OUT_FIFO_SIZE       512/4

#define EP10_IN_FIFO_SIZE       512/4  
#define EP10_OUT_FIFO_SIZE       512/4


#define EP11_IN_FIFO_SIZE       512/4  
#define EP11_OUT_FIFO_SIZE       512/4

#define EP12_IN_FIFO_SIZE       512/4  
#define EP12_OUT_FIFO_SIZE       512/4

#define EP13_IN_FIFO_SIZE       512/4  
#define EP13_OUT_FIFO_SIZE       512/4

#define EP14_IN_FIFO_SIZE       512/4  
#define EP14_OUT_FIFO_SIZE       512/4

#define EP15_IN_FIFO_SIZE       512/4  
#define EP15_OUT_FIFO_SIZE       512/4

#define UDCAHB_EP0_MAXPCKSIZE_HS  64
#define UDCAHB_EP0_MAXPCKSIZE_FS  64
#define UDCAHB_EP1_MAXPCKSIZE_HS  512
#define UDCAHB_EP1_MAXPCKSIZE_FS  64
#define UDCAHB_EP2_MAXPCKSIZE_HS  512
#define UDCAHB_EP2_MAXPCKSIZE_FS  64

#define  CONTROL_ENDPT_MAX_SIZE_HS      64
#define  BULK_ENDPT_DMA_MAX_SIZE_HS     512
#define  BULK_ENDPT_SLAVE_MAX_SIZE_HS   512
#define  CONTROL_ENDPT_MAX_SIZE_FS      64
#define  BULK_ENDPT_DMA_MAX_SIZE_FS     64
#define  BULK_ENDPT_SLAVE_MAX_SIZE_FS   64


/*-------------------------------------------------------------------------*/

/* 
 * SYNOPSYS UDC MEMORY MAPPED REGISTERS
 *
 * Use ioread* / iowrite* to access the registers.
 */

struct spearhead_glob_regs {
	u32         dev_conf;
	u32         dev_control;
	u32         dev_status;
	u32         dev_int;
	u32         dev_int_mask;
	u32         endp_int;
	u32         endp_int_mask;
} __attribute__ ((aligned (4), packed));


/*
 * Device Configuration Register
 */

#define Dev_Conf_Msk                0x0000003f
#define Dev_Conf_Rst                0x00000000

                           /* Values ....*/
                           
#define Dev_Conf_HS_SPEED           0x00000000     /* RW */
#define Dev_Conf_LS_SPEED           0x00000002     /* RW */
#define Dev_Conf_FS_SPEED           0x00000003     /* RW */
#define Dev_Conf_REMWAKEUP          0x00000004     /* RW */
#define Dev_Conf_SELFPOW            0x00000008     /* RW */
#define Dev_Conf_SYNCFRAME          0x00000010     /* RW */
#define Dev_Conf_PHYINT_8           0x00000020     /* RW */                           
#define Dev_Conf_PHYINT_16          0x00000000     /* RW */
#define Dev_Conf_UTMI_BIDIR	    0x00000040     /* RW */
#define Dev_Conf_STATUS_STALL	    0x00000080     /* RW */
#define Dev_Conf_CSR_PRG			0x00020000


/*
 * Device Control Register
 */                          

                           /* Values ....*/

#define Dev_Cntl_RESUME             0x00000001     /* RW */
#define Dev_Cntl_TFFLUSH            0x00000002     /* RW */                         
#define Dev_Cntl_RxDMAEn            0x00000004     /* RW */                         
#define Dev_Cntl_TxDMAEn            0x00000008     /* RW */                         
#define Dev_Cntl_DescrUpd           0x00000010     /* RW */                         
#define Dev_Cntl_BigEnd             0x00000020     /* RW */                         
#define Dev_Cntl_BufFill            0x00000040     /* RW */                         
#define Dev_Cntl_TshldEn            0x00000080     /* RW */                         
#define Dev_Cntl_BurstEn            0x00000100     /* RW */                         
#define Dev_Cntl_DMAMode            0x00000200     /* RW */                         
#define Dev_Cntl_SoftDisconnect     0x00000400     /* RW */
#define Dev_Cntl_ScaleDown	    0x00000800     /* RW */
#define Dev_Cntl_BurstLenU          0x00010000     /* RW */                         
#define Dev_Cntl_BurstLenMsk        0x00ff0000     /* RW */                         
#define Dev_Cntl_TshldLenU          0x01000000     /* RW */                         
#define Dev_Cntl_TshldLenMsk        0xff000000     /* RW */     
#define Dev_Cntl_CSR_DONE			0x00002000

/*
 * Device Status register
 */                            

                           /* Values ....*/
                           
#define Dev_Stat_CFG                0x0000000f     /* RO */
#define Dev_Stat_INTF               0x000000f0     /* RO */
#define Dev_Stat_ALT                0x00000f00     /* RO */
#define Dev_Stat_SUSP               0x00001000     /* RO */
#define Dev_Stat_ENUM               0x00006000     /* RO */
#define Dev_Stat_ENUM_SPEED_HS      0x00000000     /* RO */  
#define Dev_Stat_ENUM_SPEED_FS      0x00002000     /* RO */
#define Dev_Stat_ENUM_SPEED_LS      0x00004000     /* RO */
#define Dev_Stat_RXFIFO_EMPTY	    0x00008000     /* RO */
#define Dev_Stat_PHY_Err	    0x00010000     /* RO */
#define Dev_Stat_TS                 0xf0000000     /* RO */                           
                           
/*
 * Device Interrupt Register
 */                           
#define Dev_Int_Msk                 0x0000007f                          
#define Dev_Int_Rst                 0x00000000
                         
                           /* Values ....*/
                           
#define Dev_Int_SETCFG              0x00000001     /* RW */ 
#define Dev_Int_SETINTF             0x00000002     /* RW */
#define Dev_Int_INACTIVE            0x00000004     /* RW */
#define Dev_Int_USBRESET            0x00000008     /* RW */
#define Dev_Int_SUSPUSB             0x00000010     /* RW */
#define Dev_Int_SOF                 0x00000020     /* RW */
#define Dev_Int_ENUM                0x00000040     /* RW */                          
    
/*
 * Device Interrupt Mask Register
 */                           
                           
#define Dev_Int_Mask                (VUINTP(USB_Base + 0x410))    /* RW */ 
#define Dev_Int_Mask_Msk            0x0000007f
#define Dev_Int_Mask_Rst            0x00000000 

/*
 * Endpoint Interrupt Mask Register
 */                            
                           
                           /* Values ....*/
                           
#define Endp_Int_INEPMsk            0x0000ffff     /* RW */
#define Endp_Int_OUTEPMsk           0xffff0000     /* RW */

#define Endp0_Int_CtrlINMsk         0x00000001
#define Endp1_Int_BulkINMsk         0x00000002
#define Endp2_Int_BulkINMsk         0x00000004
#define Endp0_Int_CtrlOUTMsk        0x00010000
#define Endp1_Int_BulkOUTMsk        0x00020000
#define Endp2_Int_BulkOUTMsk        0x00040000

#define Endp0_IN_Int      0x00000001 
#define Endp0_OUT_Int     0x00010000

struct spearhead_epin_regs {
	u32         control;
	u32         status;
	u32         bufsize;
	u32         max_pack_size;
	u32         reserved1;
	u32         desc_ptr;
	u32         reserved2;
	u32         write_confirm;
} __attribute__ ((aligned (4), packed));

struct spearhead_epout_regs {
	u32         control;
	u32         status;
	u32         frame_num;
	u32         bufsize;
	u32         setup_ptr;
	u32         desc_ptr;
	u32         reserved1;
	u32         read_confirm;
} __attribute__ ((aligned (4), packed));

/*
 * Endpoint Control Register
 */

#define  Endp_Cntl_Msk_In          0x0000003b
#define  Endp_Cntl_Msk_Out         0x00000035
#define  Endp_Cntl_Rst             0x00000000
#define  Endp_Cntl_Msk_r           0xffffffff

                           /* Values ....*/

#define  Endp_Cntl_STALL           0x00000001      /* RW */
#define  Endp_Cntl_FLUSH           0x00000002      /* RW */        
#define  Endp_Cntl_SNOOP           0x00000004      /* RW */
#define  Endp_Cntl_POLL            0x00000008      /* RW */
#define  Endp_Cntl_Control         0x00000000      /* RW */
#define  Endp_Cntl_Iso             0x00000010      /* RW */
#define  Endp_Cntl_Bulk            0x00000020      /* RW */
#define  Endp_Cntl_Int             0x00000030      /* RW */
#define  Endp_Cntl_NAK             0x00000040      /* RW */
#define  Endp_Cntl_SNAK 	   0x00000080	   /* RW */
#define  Endp_Cntl_CNAK 	   0x00000100	   /* RW */
#define  Endp_Cntl_RRDY 	   0x00000200	   /* RW */

/*
 * Endpoint Status Register
 */
                           
#define  Endp_Status_Msk           0x003fffff
#define  Endp_Status_Rst           0x00000000
#define  Endp_Status_Msk_r         0xff8007cf
   
                           /* Values ....*/
                           
#define  Endp_Status_PIDMsk        0x0000000f      /* RO */
#define  Endp_Status_OUTMsk        0x00000030      /* RO */
#define  Endp_Status_OUT_none      0x00000000
#define  Endp_Status_OUT_Data      0x00000010
#define  Endp_Status_OUT_Setup     0x00000020
#define  Endp_Status_IN            0x00000040      /* RW */
#define  Endp_Status_BUFFNAV       0x00000080      /* RW */   
#define  Endp_Status_FATERR        0x00000100      /* RW */
#define  Endp_Status_HOSTBUSERR    0x00000200      /* RW */  
#define  Endp_Status_TDC           0x00000400      /* RW */
#define  Endp_Status_RXPKTMsk      0x003ff800      /* RO */

/*
 * Enpoint maximum Packet Size Register
 */                           

#define  Endp_Max_Pack_Size_Msk    0x0000ffff
#define  Endp_Max_Pack_Size_Rst    0x00000000
#define  Endp_Max_Pack_Size_Msk_r  0xffffffff
#define  Endp_BufSize_OUT_Msk	   0xffff0000
#define  Endp_BufSize_OUT_Base     0x00010000

struct spearhead_plug_regs {
	u32        status;
	u32        pending;
} __attribute__ ((aligned (4), packed));

#define Plug_Status_en          (unsigned int)0x1
#define Plug_Status_attached    (unsigned int)0x2
#define Plug_Status_phy_reset   (unsigned int)0x4
#define Plug_Status_phy_mode    (unsigned int)0x8

#define Plug_intpend            (unsigned int)0x1

struct spearhead_bulkd {
	__le32        status;
	__le32        reserved;
	__le32        bufp;
	__le32        nextd;
	/* The following fields are used by the driver only */
	struct list_head desc_list;
	dma_addr_t       dma_addr;
} __attribute__ ((aligned (16), packed));

/* 
 * Bulk Descriptor Status Field 
 */
#define  DmaUsb_HostRdy            0x00000000
#define  DmaUsb_DmaBsy             0x40000000
#define  DmaUsb_DmaDone            0x80000000
#define  DmaUsb_HostBsy            0xC0000000
#define  DmaUsb_BS_Mask            0xC0000000
#define  DmaUsb_Success            0x00000000
#define  DmaUsb_DescrErr           0x10000000
#define  DmaUsb_BufErr             0x30000000
#define  DmaUsb_RXSts_Mask         0x30000000
#define  DmaUsb_LastDescr          0x08000000
#define  DmaUsb_Len_Mask           0x0000FFFF

struct spearhead_setupd {
	__le32        status;
	__le32        reserved;
	__le32        data1;
	__le32        data2;
	/* The following fields are used by the driver only */
} __attribute__ ((aligned (16), packed));


struct spearhead_ep {
	struct usb_ep                            ep;
        struct spearhead_dev                       *dev;
	struct spearhead_epin_regs                  __iomem *in_regs;
	struct spearhead_epout_regs                 __iomem *out_regs;
	const struct usb_endpoint_descriptor	*desc;
	struct spearhead_bulkd                     *curr_chain_in;
        struct spearhead_bulkd                     *curr_chain_out;
	struct list_head                         queue;
	
	/*
	 * These pointer are the virtual equivalents of those in
	 * the UDC registers.
	 */
	struct spearhead_bulkd                     *desc_in_ptr; 
	struct spearhead_bulkd                     *desc_out_ptr;
	struct spearhead_bulkd                     *setup_ptr; 
	u16                                      fifo_size;
	u8                                       addr;
	u8                                       attrib;
	unsigned                                 stopped:1;
	unsigned                                 config_req:1;
};

enum ep0_state { 
	EP0_CTRL_IDLE           = 0,
	EP0_DATA_IN_STAGE       = 1,    // The DATA IN  stage is expected
	EP0_DATA_OUT_STAGE      = 2,    // The DATA OUT stage is expected
	EP0_STATUS_IN_STAGE     = 3,    // The STATUS IN  stage is expected
	EP0_STATUS_OUT_STAGE    = 4     // The STATUS OUT stage is expected
};

struct spearhead_request {
        struct usb_request                      req;
        struct list_head                        queue;
};

struct spearhead_dev {
	struct usb_gadget                       gadget;
	struct usb_gadget_driver               *driver;
	struct device                          *dev;
	spinlock_t                              lock;
	enum ep0_state                          ep0state;
	struct dma_pool                        *desc_pool;

	unsigned                                got_irq : 1;
	unsigned                                int_cmd : 1;

	u8              		        __iomem *csr_base;
	struct spearhead_epin_regs		        __iomem *epin_base;
	struct spearhead_epout_regs	        __iomem *epout_base;
	struct spearhead_glob_regs		        __iomem *glob_base;
	struct spearhead_plug_regs		        __iomem *plug_base;
	struct spearhead_fifo_regs		        __iomem *fifo_base;
	struct spearhead_ep                        ep[SYNUDC_NUM_ENDPOINTS];
};

#define is_ep_in(ep) (((ep)->addr) & USB_DIR_IN)

/* Debug facilities */

#define create_proc_files() \
	create_proc_read_entry(proc_node_name, 0, NULL, spearhead_proc_read, dev); \
	create_proc_read_entry(proc_node_regs_name, 0, NULL, spearhead_proc_regs_read, dev); \
	create_proc_read_entry(proc_node_endp_name, 0, NULL, spearhead_proc_endp_read, dev); \
	create_proc_read_entry(proc_node_out_chain_name, 0, NULL, spearhead_proc_out_chain, dev)
#define remove_proc_files() \
	remove_proc_entry(proc_node_name, NULL);	\
	remove_proc_entry(proc_node_regs_name, NULL);   \
	remove_proc_entry(proc_node_endp_name, NULL);   \
	remove_proc_entry(proc_node_out_chain_name, NULL)

#ifdef DEBUG
#define DBG(type,fmt,args...)			    \
	do {                                        \
		if (spearhead_debug_flags & (type)) {  \
			printk(KERN_INFO "spearhead_udc: " fmt , ## args);	\
		}                                   \
	}                                           \
	while (0)
#else
#define DBG(type,fmt,args...) \
	do { } while (0)
#endif /* DEBUG */

/* Modify this variable in order to trace different parts of the driver */
#define DBG_GENERIC      0x1 
#define DBG_FUNC_ENTRY   0x2 
#define DBG_FUNC_EXIT    0x4 
#define DBG_INTS         0x8 
#define DBG_PACKETS      0x10
#define DBG_ADDRESS      0x20
#define DBG_ENDPOINT     0x40
#define DBG_REQUESTS     0x80
#define DBG_FLOW         0x100
#define DBG_QUEUES       0x200
#define DBG_REGISTERS    0x400
#define DBG_EP0STATE     0x800


u32 spearhead_debug_flags =  0;

/* u32 spearhead_debug_flags = 0; */

/* Shortcuts */
#define DBG_ENTRY DBG(DBG_FUNC_ENTRY, "Enter %s\n", __FUNCTION__)
#define DBG_EXIT  DBG(DBG_FUNC_EXIT, "Exit %s\n", __FUNCTION__)
#define DBG_PASS DBG(DBG_FLOW, "Passed from %s %d\n", __FILE__, __LINE__);

#define SYNUDC_INFO(fmt,args...) printk(KERN_INFO "%s: " fmt, __FUNCTION__, ## args)
#define SYNUDC_ERR(fmt,args...) printk(KERN_ERR "#################### ERROR ####################\n%s: " fmt, __FUNCTION__, ## args)


#endif /* end of file */
