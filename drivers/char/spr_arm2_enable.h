#ifndef _SPEAR_ARM2_ENABLE_
#define _SPEAR_ARM2_ENABLE_

#define MEM				64  					/* total RAM available in MB   		*/

#define THREADX_TEXT_OFFSET		0x8000					/* Threadx text section offset 		*/
#define THREADX_TEXT			0x02108000				/* Threadx text section 	   		*/

#define THREADX_VEC			0xC0000000				/* Threadx vector to be placed here */
#define THREADX_VEC_SIZE		0x4c					/* Threadx vector size				*/


#define SPEAR_EN_ARM2_IOC_BASE		'A'
#define SPEAR_HALT_ARM2			_IO(SPEAR_EN_ARM2_IOC_BASE, 0)
#define SPEAR_START_ARM2		_IO(SPEAR_EN_ARM2_IOC_BASE, 1)
#define SPEAR_REBOOT_ARM2		_IO(SPEAR_EN_ARM2_IOC_BASE, 2)


#endif
