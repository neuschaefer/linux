
#ifndef _SWITCHCTL_IOCTL_H_
#define _SWITCHCTL_IOCTL_H_

/* Includes */
/* -------- */
#include <linux/ioctl.h>


#define SWITCHCTL_MAGIC_NUMBER      0xC3
#define SWITCHCTL_MAJOR             SWITCHCTL_MAGIC_NUMBER

typedef struct
{
    unsigned char phyad;
    unsigned char regad;
    unsigned char pagtype;
    unsigned char npage;
    unsigned short value;
} switchreg_t;



/* IOCTL values */
/* ------------ */
#define SWITCHCTL_IOC_READREG                       _IO(SWITCHCTL_MAGIC_NUMBER, 0)
#define SWITCHCTL_IOC_WRITEREG                      _IO(SWITCHCTL_MAGIC_NUMBER, 1)




#endif

