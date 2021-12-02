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

#ifndef X_NET_SOCKET_H
#define X_NET_SOCKET_H

#include "u_net_socket.h"

#ifndef _USE_LINUX
#ifdef MT8520
#include "u_time.h"
#else
#ifndef _USE_LINUX
#include "time.h"
#else
#include <time.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /*  __cplusplus  */

/*-----------------------------------------------------------------------------
 * Name: socket
 *
 * Description: Create a new unnamed socket within a specified communication 
 *              domain
 *
 * Inputs:      AF_INET     : internet protocol version 4
 *              AF_INET6    : internet protocol version 6
 * Outputs:     NA
 * Returns:     -1 is returned if an error occurs; 
 *              otherwise the return value is a descriptor referencing the socket
 ----------------------------------------------------------------------------*/
int socket  (int domain, int type, int protocol);

/*-----------------------------------------------------------------------------
 * Name: connect
 *
 * Description: The connect function is used by a TCP client to establish 
 *              a connect with a TCP server.
 *
 * Inputs:      sockfd      :   socket descriptor
 *              servaddr    :   server address
 *              addrlen     :   sizeof servaddr
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int connect (int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

/*-----------------------------------------------------------------------------
 * Name: bind
 *
 * Description: The bind function assigns a local protocol address to a socket
 *
 * Inputs:      sockfd      :   socket descriptor
 *              myaddr      :   myaddr address
 *              addrlen     :   sizeof myaddr
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int bind    (int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);

/*-----------------------------------------------------------------------------
 * Name: listen
 *
 * Description: The listen function prepare a socket to accept incoming connections
 *
 * Inputs:      sockfd      :   socket descriptor
 *              backlog     :   maximum number of connections
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int listen(int sockfd, int backlog);

/*-----------------------------------------------------------------------------
 * Name: accept
 *
 * Description: The accept function is called by a TCP server to return 
 *              the next completed connection 
 *
 * Inputs:      sockfd      :   socket descriptor
 *              cliaddr     :   client address
 *              addrlen     :   size of address
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

/*-----------------------------------------------------------------------------
 * Name: recv
 *
 * Description: recv data from client (TCP)
 *
 * Inputs:      sockfd      :   socket descriptor
 *              buff        :   buffer to save received data
 *              nbytes      :   buffer size
 *              flags       :   
 *                              MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_DONTWAIT
 *                              MSG_WAITALL  
 * Outputs:     NA              
 * Returns:     the bytes of data has received
 *             -1 on error      
 ----------------------------------------------------------------------------*/
ssize_t recv(int sockfd, void *buff, size_t nbytes, int flags);

/*-----------------------------------------------------------------------------
 * Name: send
 *
 * Description: send data to client (TCP)
 * Inputs:      sockfd      :   socket descriptor
 *              buff        :   the data to be send
 *              nbytes      :   data size
 *              flags       :   
 *                              MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_DONTWAIT
 *                              MSG_WAITALL  
 *
 * Outputs:     NA
 * Returns:     the bytes of data has send
 *             -1 on error
 ----------------------------------------------------------------------------*/
ssize_t send(int sockfd, const void *buff, size_t nbytes, int flags);

/*-----------------------------------------------------------------------------
 * Name: recvfrom
 *
 * Description: receive data to client (UDP)
 * Inputs:      sockfd      :   socket descriptor
 *              buff        :   the data to be send
 *              nbytes      :   data size
 *              flags       :   
 *                              MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_DONTWAIT
 *                              MSG_WAITALL  
 *              from        :   the address which data come from
 *              addrlen     :   the size of from   
 *
 * Outputs:     NA
 * Returns:     the bytes of data has send
 *             -1 on error
 ----------------------------------------------------------------------------*/
ssize_t recvfrom(int sockfd, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);

/*-----------------------------------------------------------------------------
 * Name: sendto
 *
 * Description: send data to client (UDP)
 * Inputs:      sockfd      :   socket descriptor
 *              buff        :   the data to be send
 *              nbytes      :   data size
 *              flags       :   
 *                              MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_DONTWAIT
 *                              MSG_WAITALL  
 *              to          :   the address to send data out
 *              addrlen     :   the size of to 
 *
 * Outputs:     NA
 * Returns:     the bytes of data has send
 *             -1 on error
 ----------------------------------------------------------------------------*/
ssize_t sendto(int sockfd, const void *buff, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen);

/*-----------------------------------------------------------------------------
 * Name: close
 *
 * Description: terminate connection and release socket
 *
 * Inputs:      sockfd      :   socket descriptor
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int close(int sockfd);

/*-----------------------------------------------------------------------------
 * Name: getsockname
 *
 * Description: get local protocol address
 *
 * Inputs:      sockfd      :   socket descriptor
 *              localaddr   :   localaddr address
 *              addrlen     :   size of localaddr
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);

/*-----------------------------------------------------------------------------
 * Name: getpeername
 *
 * Description: get peer protocol address
 *
 * Inputs:      sockfd      :   socket descriptor
 *              peername    :   peername address
 *              addrlen     :   size of peername
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int getpeername(int sockfd, struct sockaddr *peername, socklen_t *addrlen);

/*-----------------------------------------------------------------------------
 * Name: getsockopt
 *
 * Description: set socket option
 *
 * Inputs:      sockfd      :   socket descriptor
 *              level       :   option level 
 *              optname     :   option nanme
 *              optval      :   buffer to store return value    
 *              optlen      :   return value size
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);

/*-----------------------------------------------------------------------------
 * Name: setsockopt
 *
 * Description: get socket option
 *
 * Inputs:      sockfd      :   socket descriptor
 *              level       :   option level 
 *              optname     :   option nanme
 *              optval      :   data to set    
 *              optlen      :   data size
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

/*-----------------------------------------------------------------------------
 * Name: select
 *
 * Description: get socket option
 *
 * Inputs:      maxfdp1      :   socket descriptor
 *              readset      :   option level 
 *              writeset     :   option nanme
 *              exceptset    :   data to set    
 *              timeout      :   data size
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout);

/*-----------------------------------------------------------------------------
 * Name: poll
 *
 * Description: 
 *     poll is a variation on the theme of select.  
 *     It specifies an array of nfds structures of type
 *               struct pollfd {
 *                      int fd;           // file descriptor 
 *                      short events;     // requested events
 *                      short revents;    // returned events 
 *              };
 *      and a timeout in milliseconds. A negative value means infinite timeout.
 *      The  field  fd  contains a file descriptor for an open file.  The field
 *      events is an input parameter,  a  bitmask  specifying  the  events  the
 *      application  is  interested in.  The field revents is an output parame-
 *      ter, filled by the kernel  with  the  events  that  actually  occurred,
 *      either of the type requested, or of one of the types POLLERR or POLLHUP
 *      or POLLNVAL.  (These three bits are meaningless in  the  events  field,
 *      and  will be set in the revents field whenever the corresponding condi-
 *      tion is true.)  If none of the events  requested  (and  no  error)  has
 *      occurred  for any of the file descriptors, the kernel waits for timeout
 *      milliseconds for one of these events to occur. 
 *          #define POLLIN      0x0001    // There is data to read 
 *          #define POLLPRI     0x0002    // Not support
 *          #define POLLOUT     0x0004    // Writing now will not block 
 *          #define POLLERR     0x0008    // Error condition 
 *          #define POLLHUP     0x0010    // Hung up 
 *          #define POLLNVAL    0x0020    // Invalid request: fd not open 
 *
 * Inputs:      ufds       : polling fs set
 *              nfds       : number of ufds
 *              timeout    : polling timer
 * Outputs:     NA
 *
 * Returns:     
 *     On success, a positive number is returned, where the number returned is
 *     the  number  of structures which have non-zero revents fields (in other
 *     words, those descriptors with events or errors reported).  A value of 0
 *     indicates  that  the  call  timed out and no file descriptors have been
 *     selected. 
 *
 *     AT_SOCK_EINVAL, invalidate socket
 ----------------------------------------------------------------------------*/
int poll(struct pollfd *ufds, unsigned int nfds, int timeout);

/*-----------------------------------------------------------------------------
 * Name: shutdown
 *
 * Description: terminate connection in one or both direction
 *
 * Inputs:      sockfd       :   socket descriptor
 *              howto        :   shutdown method (SHUT_RD, SHUT_WR, SHUT_RDWR)
 * Outputs:     NA
 * Returns:     0 if OK
 *             -1 on error
 ----------------------------------------------------------------------------*/
int shutdown(int sockfd, int howto);

/**
 * gethostbyname
 * 
 * resolve host name to ip address
 * 
 * @param hostname, user specify name
 * 
 * @return struct hostent*
 *        nonnull pointer if OK
 *        NULL on error
 */
struct hostent *gethostbyname(const char *hostname);

/**
 * 
 * @param addr
 * @param len
 * @param family
 * 
 * @return struct hostent*
 */
struct hostent *gethostbyaddr(const char *addr, size_t len, int family);

/**
 * This function return the name of the current host
 * @parm name, the space to save hostname
 * @parm namlen, the space size
 * 
 * @return int
 * 0: ok
 * -1: error
 */
int gethostname(char *name, size_t namlen);

/* utility API 
 * inet_aton
 * inet_ntoa
*/
/**
 * inet_aton
 * 
 * convert address string to 32bits network order byte value
 * @param strptr, input
 * @param addrptr, result
 * 
 * @return int
 *  1: ok
 *  0: failed
 */
int inet_aton(const char *strptr, struct in_addr *addrptr);

/**
 * inet_ntoa
 * 
 * convert a 32bits network byte address into dot string format
 * user must release memory after this return is unused.
 * 
 * @param inaddr
 * 
 * @return char*
 *     NULL, faile
 *     others, ok
 */
char *inet_ntoa(struct in_addr inaddr);
in_addr_t inet_addr(const char *strptr);
int inet_pton(int family, const char *strptr, void *addrptr);
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);


/**
 * x_net_socket_nonblocking
 * 
 * configure socket as nonblocking mode
 * 
 * @param sock, socket to config
 * 
 * @return int
 * 0: ok
 * -1: failed
 */
int x_net_socket_nonblocking(int sock);

/**
 * x_net_socket_blocking
 * 
 * configure socket as blocking mode
 * 
 * @param sock, socket to config
 * 
 * @return int
 * 0: ok
 * -1: failed
 */
int x_net_socket_blocking(int sock);

int fcntl(int fd, int cmd, long arg);

/**
 * x_net_get_socket_mode
 * 
 * query current socket mode
 * 
 * @param sock
 * 
 * @return int
 * 0: blocking
 * 1: nonblocking
 */
int x_net_get_socket_mode(int sock);
void x_net_free_hostent(struct hostent *ptr);


/* only FIONREAD */
int x_net_ioctl(int i4_sockfd, int i4_cmd, void *pv_data);

#ifdef __cplusplus
}
#endif  /*  __cplusplus  */

#endif /* _USE_LINUX */

#endif

