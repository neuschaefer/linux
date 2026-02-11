#if !defined(VC_LOOPAUDIO_H)
#define VC_LOOPAUDIO_H

#include <linux/ioctl.h>
#include <linux/broadcom/vc_vchi_audioserv_defs.h>

#define VC_LOOPAUDIO_IOCTL  0xF9

#define VC_IOCTL_LOOPAUDIO_SETFLAGS      _IOW(VC_LOOPAUDIO_IOCTL, 1, uint32_t)
#define VC_IOCTL_LOOPAUDIO_GETFLAGS      _IOR(VC_LOOPAUDIO_IOCTL, 2, uint32_t)

#endif /* VC_LOOPAUDIO_H */

