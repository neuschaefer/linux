/*
 *  linux/include/asm/arch/irqs.h
 *
 *  Copyright (C) 2003 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <asm/arch/platform.h>

/*
 *  IRQ interrupts definitions are the same the INT definitions
 *  held within platform.h
 */

#define IRQ_VIC_START	  	        		0
#define IRQ_INTRCOMM_SW_IRQ					(IRQ_VIC_START + INT_INTRCOMM_SW_IRQ	)
#define IRQ_INTRCOMM_CPU_1			        (IRQ_VIC_START + INT_INTRCOMM_CPU_1		)
#define IRQ_INTRCOMM_CPU_2			        (IRQ_VIC_START + INT_INTRCOMM_CPU_2		)
#define IRQ_INTRCOMM_RAS2A11_1		        (IRQ_VIC_START + INT_INTRCOMM_RAS2A11_1	)
#define IRQ_INTRCOMM_RAS2A11_2		        (IRQ_VIC_START + INT_INTRCOMM_RAS2A11_2	)
#define IRQ_INTRCOMM_RAS2A12_1		        (IRQ_VIC_START + INT_INTRCOMM_RAS2A12_1	)
#define IRQ_INTRCOMM_RAS2A12_2		        (IRQ_VIC_START + INT_INTRCOMM_RAS2A12_2	)
#define IRQ_GEN_RAS_0				        (IRQ_VIC_START + INT_GEN_RAS_0		)
#define IRQ_GEN_RAS_1			    	    (IRQ_VIC_START + INT_GEN_RAS_1		)
#define IRQ_GEN_RAS_2			        	(IRQ_VIC_START + INT_GEN_RAS_2		)
#define IRQ_GEN_RAS_3				        (IRQ_VIC_START + INT_GEN_RAS_3		)
#define IRQ_GEN_RAS_4			    	    (IRQ_VIC_START + INT_GEN_RAS_4		)
#define IRQ_GEN_RAS_5			        	(IRQ_VIC_START + INT_GEN_RAS_5		)
#define IRQ_GEN_RAS_6			        	(IRQ_VIC_START + INT_GEN_RAS_6		)
#define IRQ_GEN_RAS_7			        	(IRQ_VIC_START + INT_GEN_RAS_7		)
#define IRQ_GEN_RAS_8			        	(IRQ_VIC_START + INT_GEN_RAS_8		)
#define IRQ_GPT0					        (IRQ_VIC_START + INT_GPT0			)
#define IRQ_GPT1					        (IRQ_VIC_START + INT_GPT1			)
#define IRQ_LOCAL_GPIO				        (IRQ_VIC_START + INT_LOCAL_GPIO		)
#define IRQ_PLL_UNLOCK				    	(IRQ_VIC_START + INT_PLL_UNLOCK		)
#define IRQ_JPEG			        		(IRQ_VIC_START + INT_JPEG			)
#define IRQ_FSMC			        		(IRQ_VIC_START + INT_FSMC			)
#define IRQ_IRDA			        		(IRQ_VIC_START + INT_IRDA			)
#define IRQ_RESERVED		        		(IRQ_VIC_START + INT_RESERVED		)
#define IRQ_UART_0		        			(IRQ_VIC_START + INT_UART_0			)
#define IRQ_UART_1		        			(IRQ_VIC_START + INT_UART_1			)
#define IRQ_SSP_1			        		(IRQ_VIC_START + INT_SSP_1			)
#define IRQ_SSP_2							(IRQ_VIC_START + INT_SSP_2			)
#define IRQ_I2C			        			(IRQ_VIC_START + INT_I2C			)
#define IRQ_GEN_RAS_9			       	 	(IRQ_VIC_START + INT_GEN_RAS_9		)
#define IRQ_GEN_RAS_10			       	 	(IRQ_VIC_START + INT_GEN_RAS_10		)
#define IRQ_GEN_RAS_11			       	 	(IRQ_VIC_START + INT_GEN_RAS_11		)

/* VIC 2 */                                 
#define IRQ_APPL_TIMER_1_1				    (IRQ_VIC_START + 32 + INT_APPL_TIMER_1_1		  )
#define IRQ_APPL_TIMER_1_2				    (IRQ_VIC_START + 32 + INT_APPL_TIMER_1_2		  )
#define IRQ_APPL_TIMER_2_1                  (IRQ_VIC_START + 32 + INT_APPL_TIMER_2_1          )
#define IRQ_APPL_TIMER_2_2                  (IRQ_VIC_START + 32 + INT_APPL_TIMER_2_2          )
#define IRQ_APPL_GPIO                       (IRQ_VIC_START + 32 + INT_APPL_GPIO               )
#define IRQ_APPL_SSP                        (IRQ_VIC_START + 32 +  INT_APPL_SSP               )
#define IRQ_APPL_ADC                        (IRQ_VIC_START + 32 +  INT_APPL_ADC               )
#define IRQ_APPL_RESERVED                   (IRQ_VIC_START + 32 +  INT_APPL_RESERVED          )
#define IRQ_AHB_EXP_MASTER                  (IRQ_VIC_START + 32 +  INT_AHB_EXP_MASTER         )
#define IRQ_DDR_CONTROLLER                  (IRQ_VIC_START + 32 +  INT_DDR_CONTROLLER         )
#define IRQ_BASIC_DMA	                    (IRQ_VIC_START + 32 +  INT_BASIC_DMA              )
#define IRQ_BASIC_SMI                       (IRQ_VIC_START + 32 +  INT_BASIC_SMI              )
#define IRQ_BASIC_CLCD                      (IRQ_VIC_START + 32 +  INT_BASIC_CLCD             )
#define IRQ_EXP_AHB_1                       (IRQ_VIC_START + 32 +  INT_EXP_AHB_1              )
#define IRQ_EXP_AHB_2                       (IRQ_VIC_START + 32 +  INT_EXP_AHB_2              )
#define IRQ_BASIC_TIMER_3_1                 (IRQ_VIC_START + 32 +  INT_BASIC_TIMER_3_1        )
#define IRQ_BASIC_TIMER_3_2                 (IRQ_VIC_START + 32 +  INT_BASIC_TIMER_3_2        )
#define IRQ_BASIC_RTC                       (IRQ_VIC_START + 32 +  INT_BASIC_RTC              )
#define IRQ_BASIC_GPIO                      (IRQ_VIC_START + 32 +  INT_BASIC_GPIO             )
#define IRQ_BASIC_WDT              	    (IRQ_VIC_START + 32 +  INT_BASIC_WDT             )
#define IRQ_BASIC_RESERVED                  (IRQ_VIC_START + 32 +  INT_BASIC_RESERVED         )
#define IRQ_AHB_EXP_SLAVE                   (IRQ_VIC_START + 32 +  INT_AHB_EXP_SLAVE          )
#define IRQ_GMAC_1               			(IRQ_VIC_START + 32 +  INT_GMAC_1      			  )
#define IRQ_GMAC_2               			(IRQ_VIC_START + 32 +  INT_GMAC_2      			  )
#define IRQ_USB_DEV              			(IRQ_VIC_START + 32 +  INT_USB_DEV     			  )
#define IRQ_USB_H_OHCI_0	        		(IRQ_VIC_START + 32 +  INT_USB_H_OHCI_0			  )
#define IRQ_USB_H_EHCI_0         			(IRQ_VIC_START + 32 +  INT_USB_H_EHCI_0			  )
#define IRQ_USB_H_OHCI_1          			(IRQ_VIC_START + 32 +  INT_USB_H_OHCI_1			  )
#define	IRQ_USB_H_EHCI_1         			(IRQ_VIC_START + 32 +  INT_USB_H_EHCI_1			  )
#define	IRQ_EXP_AHB_3                       (IRQ_VIC_START + 32 +  INT_EXP_AHB_3              )
#define	IRQ_EXP_AHB_4                       (IRQ_VIC_START + 32 +  INT_EXP_AHB_4              )

#define IRQ_VIC_END							(IRQ_VIC_START + 63)

#define NR_IRQS								64 		//no of irqs

#define FIQ_START 						IRQ_VIC_START
#define FIQ_INTRCOMM_SW_IRQ				(FIQ_START + INT_INTRCOMM_SW_IRQ	)
#define FIQ_INTRCOMM_CPU_1			    (FIQ_START + INT_INTRCOMM_CPU_1		)
#define FIQ_INTRCOMM_CPU_2			    (FIQ_START + INT_INTRCOMM_CPU_2		)
#define FIQ_INTRCOMM_RAS2A11_1		    (FIQ_START + INT_INTRCOMM_RAS2A11_1	)
#define FIQ_INTRCOMM_RAS2A11_2		    (FIQ_START + INT_INTRCOMM_RAS2A11_2	)
#define FIQ_INTRCOMM_RAS2A12_1		    (FIQ_START + INT_INTRCOMM_RAS2A12_1	)                        
#define FIQ_INTRCOMM_RAS2A12_2		    (FIQ_START + INT_INTRCOMM_RAS2A12_2	)                                           
#define FIQ_GEN_RAS_0				    (FIQ_START + INT_GEN_RAS_0		)
#define FIQ_GEN_RAS_1			    	(FIQ_START + INT_GEN_RAS_1		)
#define FIQ_GEN_RAS_2			        (FIQ_START + INT_GEN_RAS_2		)
#define FIQ_GEN_RAS_3				    (FIQ_START + INT_GEN_RAS_3		)
#define FIQ_GEN_RAS_4			    	(FIQ_START + INT_GEN_RAS_4		)
#define FIQ_GEN_RAS_5			        (FIQ_START + INT_GEN_RAS_5		)
#define FIQ_GEN_RAS_6			        (FIQ_START + INT_GEN_RAS_6		)
#define FIQ_GEN_RAS_7			        (FIQ_START + INT_GEN_RAS_7		)
#define FIQ_GEN_RAS_8			        (FIQ_START + INT_GEN_RAS_8		)
#define FIQ_GPT0					    (FIQ_START + INT_GPT0			)
#define FIQ_GPT1					    (FIQ_START + INT_GPT1			)
#define FIQ_LOCAL_GPIO				    (FIQ_START + INT_LOCAL_GPIO		)
#define FIQ_PLL_UNLOCK				    (FIQ_START + INT_PLL_UNLOCK		)
#define FIQ_JPEG			        	(FIQ_START + INT_JPEG			)
#define FIQ_FSMC			        	(FIQ_START + INT_FSMC			)
#define FIQ_IRDA			        	(FIQ_START + INT_IRDA			)
#define FIQ_RESERVED		        	(FIQ_START + INT_RESERVED		)
#define FIQ_UART_0		        		(FIQ_START + INT_UART_0			)
#define FIQ_UART_1		        		(FIQ_START + INT_UART_1			)
#define FIQ_SSP_1			        	(FIQ_START + INT_SSP_1			)
#define FIQ_SSP_2						(FIQ_START + INT_SSP_2			)
#define FIQ_I2C			        		(FIQ_START + INT_I2C			)
#define FIQ_GEN_RAS_9			       	(FIQ_START + INT_GEN_RAS_9		)
#define FIQ_GEN_RAS_10			       	(FIQ_START + INT_GEN_RAS_10		)
#define FIQ_GEN_RAS_11			       	(FIQ_START + INT_GEN_RAS_11		)
                                                                                            
/* VIC 2 */                             
#define FIQ_APPL_TIMER_1_1				(FIQ_START + 32 + INT_APPL_TIMER_1_1		  )
#define FIQ_APPL_TIMER_1_2				(FIQ_START + 32 + INT_APPL_TIMER_1_2		  )
#define FIQ_APPL_TIMER_2_1              (FIQ_START + 32 + INT_APPL_TIMER_2_1          )
#define FIQ_APPL_TIMER_2_2              (FIQ_START + 32 + INT_APPL_TIMER_2_2          )
#define FIQ_APPL_GPIO                   (FIQ_START + 32 + INT_APPL_GPIO               )
#define FIQ_APPL_SSP                    (FIQ_START + 32 +  INT_APPL_SSP               )
#define FIQ_APPL_ADC                    (FIQ_START + 32 +  INT_APPL_ADC               )
#define FIQ_APPL_RESERVED               (FIQ_START + 32 +  INT_APPL_RESERVED          )
#define FIQ_AHB_EXP_MASTER              (FIQ_START + 32 +  INT_AHB_EXP_MASTER         )
#define FIQ_DDR_CONTROLLER              (FIQ_START + 32 +  INT_DDR_CONTROLLER         )
#define FIQ_BASIC_DMA                	(FIQ_START + 32 +  INT_BASIC_DMA              )
#define FIQ_BASIC_SMI                   (FIQ_START + 32 +  INT_BASIC_SMI              )
#define FIQ_BASIC_CLCD                  (FIQ_START + 32 +  INT_BASIC_CLCD             )
#define FIQ_EXP_AHB_1                   (FIQ_START + 32 +  INT_EXP_AHB_1              )
#define FIQ_EXP_AHB_2                   (FIQ_START + 32 +  INT_EXP_AHB_2              )
#define FIQ_BASIC_TIMER_3_1             (FIQ_START + 32 +  INT_BASIC_TIMER_3_1        )
#define FIQ_BASIC_TIMER_3_2             (FIQ_START + 32 +  INT_BASIC_TIMER_3_2        )
#define FIQ_BASIC_RTC                   (FIQ_START + 32 +  INT_BASIC_RTC              )
#define FIQ_BASIC_GPIO                  (FIQ_START + 32 +  INT_BASIC_GPIO             )
#define FIQ_BASIC_WDT              	    (FIQ_START + 32 +  INT_BASIC_WDT             )
#define FIQ_BASIC_RESERVED              (FIQ_START + 32 +  INT_BASIC_RESERVED         )
#define FIQ_AHB_EXP_SLAVE               (FIQ_START + 32 +  INT_AHB_EXP_SLAVE          )
#define FIQ_GMAC_1               		(FIQ_START + 32 +  INT_GMAC_1      			  )
#define FIQ_GMAC_2               		(FIQ_START + 32 +  INT_GMAC_2      			  )
#define FIQ_USB_DEV              		(FIQ_START + 32 +  INT_USB_DEV     			  )
#define FIQ_USB_H_OHCI_0	        	(FIQ_START + 32 +  INT_USB_H_OHCI_0			  )
#define FIQ_USB_H_EHCI_0         		(FIQ_START + 32 +  INT_USB_H_EHCI_0			  )
#define FIQ_USB_H_OHCI_1          		(FIQ_START + 32 +  INT_USB_H_OHCI_1			  )
#define	FIQ_USB_H_EHCI_1         		(FIQ_START + 32 +  INT_USB_H_EHCI_1			  )
#define	FIQ_EXP_AHB_3                   (FIQ_START + 32 +  INT_EXP_AHB_3              )
#define	FIQ_EXP_AHB_4                   (FIQ_START + 32 +  INT_EXP_AHB_4              )
                                                                                        
#define FIQ_END						    (FIQ_START + 63)



/* End of file - irqs.h */                  
                                    
