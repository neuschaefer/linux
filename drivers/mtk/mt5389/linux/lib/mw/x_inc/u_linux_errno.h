/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_linux_errno.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description: This file define the error code used for wlan module.
 */
/*----------------------------------------------------------------------------*/

//#ifndef _ASM_GENERIC_ERRNO_H
#define _ASM_GENERIC_ERRNO_H    /**< _ASM_GENERIC_ERRNO_H       */
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_INET_WIFI
 *  @{
 */
/*----------------------------------------------------------------------------*/


//#include <errno-base.h>
#define	EPERM		 1	/**< Operation not permitted */
#define	ENOENT		 2	/**< No such file or directory */
#define	ESRCH		 3	/**< No such process */
#define	EINTR		 4	/**< Interrupted system call */
#define	EIO		     5	/**< I/O error */
#define	ENXIO		 6	/**< No such device or address */
#define	E2BIG		 7	/**< Argument list too long */
#define	ENOEXEC		 8	/**< Exec format error */
#define	EBADF		 9	/**< Bad file number */
#define	ECHILD		10	/**< No child processes */
#define	EAGAIN		11	/**< Try again */
#define	ENOMEM		12	/**< Out of memory */
#define	EACCES		13	/**< Permission denied */
#define	EFAULT		14	/**< Bad address */
#define	ENOTBLK		15	/**< Block device required */
#define	EBUSY		16	/**< Device or resource busy */
#define	EEXIST		17	/**< File exists */
#define	EXDEV		18	/**< Cross-device link */
#define	ENODEV		19	/**< No such device */
#define	ENOTDIR		20	/**< Not a directory */
#define	EISDIR		21	/**< Is a directory */
#define	EINVAL		22	/**< Invalid argument */
#define	ENFILE		23	/**< File table overflow */
#define	EMFILE		24	/**< Too many open files */
#define	ENOTTY		25	/**< Not a typewriter */
#define	ETXTBSY		26	/**< Text file busy */
#define	EFBIG		27	/**< File too large */
#define	ENOSPC		28	/**< No space left on device */
#define	ESPIPE		29	/**< Illegal seek */
#define	EROFS		30	/**< Read-only file system */
#define	EMLINK		31	/**< Too many links */
#define	EPIPE		32	/**< Broken pipe */
#define	EDOM		33	/**< Math argument out of domain of func */
#define	ERANGE		34	/**< Math result not representable */


#define	EDEADLK		35	/**< Resource deadlock would occur */
#define	ENAMETOOLONG	36	/**< File name too long */
#define	ENOLCK		37	/**< No record locks available */
#define	ENOSYS		38	/**< Function not implemented */
#define	ENOTEMPTY	39	/**< Directory not empty */
#define	ELOOP		40	/**< Too many symbolic links encountered */
#define	EWOULDBLOCK	EAGAIN	/**< Operation would block */
#define	ENOMSG		42	/**< No message of desired type */
#define	EIDRM		43	/**< Identifier removed */
#define	ECHRNG		44	/**< Channel number out of range */
#define	EL2NSYNC	45	/**< Level 2 not synchronized */
#define	EL3HLT		46	/**< Level 3 halted */
#define	EL3RST		47	/**< Level 3 reset */
#define	ELNRNG		48	/**< Link number out of range */
#define	EUNATCH		49	/**< Protocol driver not attached */
#define	ENOCSI		50	/**< No CSI structure available */
#define	EL2HLT		51	/**< Level 2 halted */
#define	EBADE		52	/**< Invalid exchange */
#define	EBADR		53	/**< Invalid request descriptor */
#define	EXFULL		54	/**< Exchange full */
#define	ENOANO		55	/**< No anode */
#define	EBADRQC		56	/**< Invalid request code */
#define	EBADSLT		57	/**< Invalid slot */

#define	EDEADLOCK	EDEADLK    /**< Resource deadlock would occur       */

#define	EBFONT		59	/**< Bad font file format */
#define	ENOSTR		60	/**< Device not a stream */
#define	ENODATA		61	/**< No data available */
#define	ETIME		62	/**< Timer expired */
#define	ENOSR		63	/**< Out of streams resources */
#define	ENONET		64	/**< Machine is not on the network */
#define	ENOPKG		65	/**< Package not installed */
#define	EREMOTE		66	/**< Object is remote */
#define	ENOLINK		67	/**< Link has been severed */
#define	EADV		68	/**< Advertise error */
#define	ESRMNT		69	/**< Srmount error */
#define	ECOMM		70	/**< Communication error on send */
#define	EPROTO		71	/**< Protocol error */
#define	EMULTIHOP	72	/**< Multihop attempted */
#define	EDOTDOT		73	/**< RFS specific error */
#define	EBADMSG		74	/**< Not a data message */
#define	EOVERFLOW	75	/**< Value too large for defined data type */
#define	ENOTUNIQ	76	/**< Name not unique on network */
#define	EBADFD		77	/**< File descriptor in bad state */
#define	EREMCHG		78	/**< Remote address changed */
#define	ELIBACC		79	/**< Can not access a needed shared library */
#define	ELIBBAD		80	/**< Accessing a corrupted shared library */
#define	ELIBSCN		81	/**< .lib section in a.out corrupted */
#define	ELIBMAX		82	/**< Attempting to link in too many shared libraries */
#define	ELIBEXEC	83	/**< Cannot exec a shared library directly */
#define	EILSEQ		84	/**< Illegal byte sequence */
#define	ERESTART	85	/**< Interrupted system call should be restarted */
#define	ESTRPIPE	86	/**< Streams pipe error */
#define	EUSERS		87	/**< Too many users */
#define	ENOTSOCK	88	/**< Socket operation on non-socket */
#define	EDESTADDRREQ	89	/**< Destination address required */
#define	EMSGSIZE	90	/**< Message too long */
#define	EPROTOTYPE	91	/**< Protocol wrong type for socket */
#define	ENOPROTOOPT	92	/**< Protocol not available */
#define	EPROTONOSUPPORT	93	/**< Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/**< Socket type not supported */
#define	EOPNOTSUPP	95	/**< Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/**< Protocol family not supported */
#define	EAFNOSUPPORT	97	/**< Address family not supported by protocol */
#define	EADDRINUSE	98	/**< Address already in use */
#define	EADDRNOTAVAIL	99	/**< Cannot assign requested address */
#define	ENETDOWN	100	/**< Network is down */
#define	ENETUNREACH	101	/**< Network is unreachable */
#define	ENETRESET	102	/**< Network dropped connection because of reset */
#define	ECONNABORTED	103	/**< Software caused connection abort */
#define	ECONNRESET	104	/**< Connection reset by peer */
#define	ENOBUFS		105	/**< No buffer space available */
#define	EISCONN		106	/**< Transport endpoint is already connected */
#define	ENOTCONN	107	/**< Transport endpoint is not connected */
#define	ESHUTDOWN	108	/**< Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	109	/**< Too many references: cannot splice */
#define	ETIMEDOUT	110	/**< Connection timed out */
#define	ECONNREFUSED	111	/**< Connection refused */
#define	EHOSTDOWN	112	/**< Host is down */
#define	EHOSTUNREACH	113	/**< No route to host */
#define	EALREADY	114	/**< Operation already in progress */
#define	EINPROGRESS	115	/**< Operation now in progress */
#define	ESTALE		116	/**< Stale NFS file handle */
#define	EUCLEAN		117	/**< Structure needs cleaning */
#define	ENOTNAM		118	/**< Not a XENIX named type file */
#define	ENAVAIL		119	/**< No XENIX semaphores available */
#define	EISNAM		120	/**< Is a named type file */
#define	EREMOTEIO	121	/**< Remote I/O error */
#define	EDQUOT		122	/**< Quota exceeded */

#define	ENOMEDIUM	123	/**< No medium found */
#define	EMEDIUMTYPE	124	/**< Wrong medium type */
#define	ECANCELED	125	/**< Operation Canceled */
#define	ENOKEY		126	/**< Required key not available */
#define	EKEYEXPIRED	127	/**< Key has expired */
#define	EKEYREVOKED	128	/**< Key has been revoked */
#define	EKEYREJECTED	129	/**< Key was rejected by service */

/* for robust mutexes */
#define	EOWNERDEAD	130	/**< Owner died */
#define	ENOTRECOVERABLE	131	/**< State not recoverable */

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/

//#endif
