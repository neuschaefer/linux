/**
 * Copyright 2004,2005,2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Defines ICE Socket API
 * 
 * This interface is stylistically different from other ICE interfaces, but 
 * similar to BSD sockets.
 *
 * This API only needs to be implemented for products where Cabot software is
 * using the Ethernet port or modem.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */

#ifndef CABOT_ICE_ICE_SOCKET_H_INCLUDED
#define CABOT_ICE_ICE_SOCKET_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************************** */
/**
 * @name Initialisation
 */
/*@{*/

/**
 * @brief Initialises settings related to socket API 
 *
 * Initialise the required socket parameters.
 *
 * @return #frost_false if successful otherwise #frost_true
 */
frost_bool ICE_SocketInit(void);

/*@}*/


/* *********************************************************************** */
/**
 * @name Protocol numbers
 */
/*@{*/

/**
 * ICE_IPPROTO_IP is a dummy for IP.
 */
#define ICE_IPPROTO_IP         0

/**
 * ICE_IPPROTO_TCP represents tcp.
 */
#define ICE_IPPROTO_TCP        6

/**
 * ICE_IPPROTO_UDP represents udp.
 */
#define ICE_IPPROTO_UDP        17

/**
 * ICE_IPPROTO_RAW represents raw.
 *
 * @deprecated Not supported by any ICE implementation.  Do not use.
 *             Do not implement support for this.
 */
#define ICE_IPPROTO_RAW        255

/*@}*/

/* *********************************************************************** */
/**
 * @name Address structures
 */
/*@{*/

/**
 * The only supported address family.
 */
#define ICE_AF_INET            2

/**
 * ICE_INADDR_ANY represents all internet addresses.
 */
#define ICE_INADDR_ANY         0

/**
 * Macro to check if an address is multicast.
 */
#define ICE_IN_MULTICAST(x)    ((((frost_ulong)(x)) & 0xf0000000) == 0xe0000000)

/**
 * @brief Socket address in internet domain.
 */
typedef struct ICE_InAddr
{
    /**
     * 32 bits IP address, net byte order.
     */
    frost_uint32 s_addr;
} ICE_InAddr;

/**
 * @brief Socket address.
 */
typedef struct ICE_SockAddrIn
{
    /**
     * Address family.  Always #ICE_AF_INET.
     */
    frost_int16 sin_family;

    /**
     * 16 bits port number, net byte order.
     */
    frost_uint16 sin_port;

    /**
     * 32 bits IP address.
     */
    ICE_InAddr sin_addr;

    /**
     * This field is defined because conventionally sockaddr_in structure 
     * (#ICE_SockAddrIn) is casted to sockaddr structure (#ICE_SockAddr) 
     * in BSD socket applications and the sizes of these structures must match
     */
    char __pad[8];
} ICE_SockAddrIn;

/**
 * @brief Generic socket address.
 */
typedef struct ICE_SockAddr
{
    /**
     * Address family.
     */
    frost_uint16 sa_family;

    /**
     * Address value .
     */
    char sa_data[14];
} ICE_SockAddr;

/*@}*/

/* *********************************************************************** */
/**
 * @name Basic socket operations
 */
/*@{*/

/**
 * @brief Accepts a connection on a socket.
 * @param s Socket descriptor.
 * @param addr Pointer to the address of the peer. May be null.
 * @param addr_len Pointer to integer that contains the size of address 
 * structure. May be null.
 * @return A new socket descriptor for the accepted connection,
 * -1 if an error has occurred.
 */
frost_int32 ICE_SocketAccept(
    frost_int32 s,
    ICE_SockAddr * addr,
    frost_uint32 * addr_len);

/**
 * @brief Gets the number of bytes available for the given socket for reading and 
 * stores the result into the value pointed to by the second argument.
 * @param s Socket descriptor.
 * @param result Pointer to a variable to store the result. Cannot be null.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketAvailable(
    frost_int32 s, 
    frost_uint32 * result);

/**
 * @brief Binds a local address to a socket.
 * @param s Socket descriptor.
 * @param addr Pointer to address to bind. Cannot be null.
 * @param addr_len Size of address structure.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketBind(
    frost_int32 s,
    const ICE_SockAddr * addr,
    frost_uint32 addr_len);

/**
 * @brief Closes a socket.
 * @param s Socket descriptor.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketCloseSocket(frost_int32 s);

/**
 * @brief Initiates a connection on a socket.
 * @param s Socket descriptor.
 * @param addr Pointer to address to connect to. Cannot be null.
 * @param addr_len Size of address structure.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketConnect(
    frost_int32 s,
    const ICE_SockAddr * addr,
    frost_uint32 addr_len);

/**
 * @brief Gets socket name.
 * @param s Socket descriptor.
 * @param addr Pointer to local address of the socket. Cannot be null.
 * @param addr_len Pointer to size of address structure. Cannot be null.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketGetSockName(
    frost_int32 s, 
    ICE_SockAddr * addr, 
    frost_uint32 * addr_len);

/**
 * @brief Listens for connections on a socket.
 * @param s Socket descriptor.
 * @param count Maximum number of pending connections.  If this value exceeds
 *              the maximum supported by the TCP/IP stack then it is clipped
 *              to the maximum.  (Implementation note: Typically TCP/IP stacks
 *              define SOMAXCONN to the maximum value).  Cabot code does
 *              deliberately pass 0xFFFFFFFF here when we want the maximum.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketListen(
    frost_int32 s,
    frost_uint32 count);

/**
 * ICE_MSG_PEEK is peek at incoming message.
 * This is a value for the <code>flags</code> parameter to ICE_SocketRecv()
 * and ICE_SocketRecvFrom().
 */
#define ICE_MSG_PEEK           0x2

/**
 * @brief Receives data from a connected socket.
 *
 * Note that this should never cause a SIGPIPE signal to be raised,
 * as FROST/ICE do not support signals.  On some platforms, the ICE
 * implementation might need to always add the MSG_NOSIGNAL flag
 * to this call.  On other platforms, the ICE implementation might
 * need to set the socket option SO_NOSIGPIPE before calling the
 * underlying recv() function.
 *
 * @param s Socket descriptor.
 * @param buf Pointer to destination buffer. Cannot be null.
 * @param buf_len Maximum number of bytes to read.
 * @param flags Flags specifying how the call is made. Can be 0 or
 *              #ICE_MSG_PEEK.  #ICE_MSG_PEEK means the data is not removed
 *              from the input buffer.
 * @return The number of read bytes if the call is successful,
 *         -1 if an error has occurred,
 *         0 if the connection has been closed.
 */
frost_int32 ICE_SocketRecv(
    frost_int32 s,
    void * buf, 
    frost_uint32 buf_len, 
    frost_int32 flags);

/**
 * @brief Receives a datagram and stores the source address.
 * @param s Socket descriptor.
 * @param buf Pointer to destination buffer. Cannot be null.
 * @param buf_len Maximum number of bytes to read.
 * @param flags Flags specifying how the call is made. Can be 0 or
 *              #ICE_MSG_PEEK. #ICE_MSG_PEEK means The data is not removed
 *              from the input buffer.
 * @param addr Pointer to source address. May be null.
 * @param addr_len Pointer to size of address structure. May be null.
 * @return The number of read bytes if the call is successful,
 *         -1 if an error has occurred,
 *         0 if the connection has been closed.
 */
frost_int32 ICE_SocketRecvFrom(
    frost_int32 s, 
    void * buf, 
    frost_uint32 buf_len, 
    frost_int32 flags, 
    ICE_SockAddr * addr, 
    frost_uint32 * addr_len);

/**
 * ICE_MSG_DONTROUTE.  Send without using routing tables.
 * This is a value for the <code>flags</code> parameter to ICE_SocketSend()
 * and ICE_SocketSendTo().
 */
#define ICE_MSG_DONTROUTE      0x4

/**
 * @brief Sends data to a connected socket.
 *
 * Note that this should never cause a SIGPIPE signal to be raised,
 * as FROST/ICE do not support signals.  On some platforms, the ICE
 * implementation might need to always add the MSG_NOSIGNAL flag
 * to this call.  On other platforms, the ICE implementation might
 * need to set the socket option SO_NOSIGPIPE before calling the
 * underlying send() function.
 *
 * @param s Socket descriptor.
 * @param buf Pointer to source buffer. Cannot be null.
 * @param buf_len Number of bytes to send.
 * @param flags Flags specifying how the call is made. Flags can be 0,
 *    or #ICE_MSG_DONTROUTE.  #ICE_MSG_DONTROUTE means that the data should
 *    be sent without using routing tables.
 * @return The number of bytes sent if the call is successful,
 *         -1 if an error has occurred.
 */
frost_int32 ICE_SocketSend(
    frost_int32 s, 
    const void * buf, 
    frost_uint32 buf_len, 
    frost_int32 flags);

/**
 * @brief Sends a datagram to the destination address.
 * @param s Socket descriptor.
 * @param buf Pointer to source buffer. Cannot be null.
 * @param buf_len Number of bytes to send.
 * @param flags Flags specifying how the call is made. Flags can be 0,
 *    or #ICE_MSG_DONTROUTE.  #ICE_MSG_DONTROUTE means that the data should
 *    be sent without using routing tables.
 * @param addr Pointer to destination address. May be null.
 * @param addr_len Size of address structure.
 * @return The number of bytes sent if the call is successful,
 *         -1 if an error has occurred.
 */
frost_int32 ICE_SocketSendTo(
    frost_int32 s, 
    const void * buf, 
    frost_uint32 buf_len, 
    frost_int32 flags, 
    ICE_SockAddr * addr, 
    frost_uint32 addr_len);

/**
 * @brief Shut down socked reading.
 * Parameter to ICE_SocketShutdown().
 */
#define ICE_SHUT_RD   0

/**
 * @brief Shut down socked writing.
 * Parameter to ICE_SocketShutdown().
 */
#define ICE_SHUT_WR   1

/**
 * @brief Shut down socked reading and writing.
 * Parameter to ICE_SocketShutdown().
 */
#define ICE_SHUT_RDWR 2

/**
 * @brief Shuts down part of a connection.
 * @param s Socket descriptor.
 * @param how How to shutdown.
 * The how parameter can take one of the following values:
 * #ICE_SHUT_RD - socket won't allow further data reception.
 * #ICE_SHUT_WR - socket won't allow further data transmission.
 * #ICE_SHUT_RDWR - socket won't allow further data reception or transmission.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketShutdown(
    frost_int32 s, 
    frost_int32 how);

/**
 * ICE_SOCK_DGRAM means datagram socket.
 * This is a socket type for the <code>type</code> parameter to
 * ICE_SocketSocket().
 */
#define ICE_SOCK_DGRAM         2

/**
 * ICE_SOCK_STREAM means stream socket.
 * This is a socket type for the <code>type</code> parameter to
 * ICE_SocketSocket().
 */
#define ICE_SOCK_STREAM        1

/**
 * ICE_SOCK_RAW means raw socket.
 * This is a socket type for the <code>type</code> parameter to
 * ICE_SocketSocket().
 * @deprecated Not supported by any ICE implementation.  Do not use.
 *             Do not implement support for this.
 */
#define ICE_SOCK_RAW           3

/**
 * @brief Creates a new socket.
 * @param domain Protocol domain.  Always pass #ICE_AF_INET.
 * @param type Type of socket to create.
 * Available types are:
 * #ICE_SOCK_STREAM Provides reliable, connection-based byte streams, uses TCP.
 * #ICE_SOCK_DGRAM Supports datagrams, connectionless unreliable messages, uses UDP.
 * #ICE_SOCK_RAW Supports datagrams, connectionless unreliable messages, uses 
 * a protocol specified by the protocol argument.
 * @param protocol Protocol number. Must be #ICE_IPPROTO_TCP.
 * @return A new socket descriptor if the call is successful, 
 * -1 if an error has occurred.
 */
frost_int32 ICE_SocketSocket(
    frost_int32 domain, 
    frost_int32 type, 
    frost_int32 protocol);

/*@}*/

/* *********************************************************************** */
/**
 * @name Socket options
 * ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(), and parameters.
 */
/*@{*/

/**
 * ICE_SOL_SOCKET is option for socket level.
 *
 * For use as the <code>level</code> parameter to ICE_SocketSetSockOpt()
 * and ICE_SocketGetSockOpt(),
 */
#define ICE_SOL_SOCKET         0xffff

/**
 * ICE_SOL_TCP TCP level.
 *
 * For use as the <code>level</code> parameter to ICE_SocketSetSockOpt()
 * and ICE_SocketGetSockOpt(),
 */
#define ICE_SOL_TCP            ICE_IPPROTO_TCP

/**
 * ICE_SOL_TCP UDP level.
 *
 * For use as the <code>level</code> parameter to ICE_SocketSetSockOpt()
 * and ICE_SocketGetSockOpt(),
 */
#define ICE_SOL_UDP            ICE_IPPROTO_UDP

/**
 * ICE_SOL_TCP IP level.
 *
 * For use as the <code>level</code> parameter to ICE_SocketSetSockOpt()
 * and ICE_SocketGetSockOpt(),
 */
#define ICE_SOL_IP             ICE_IPPROTO_IP


/**
 * linger on close if data present.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_SOCKET.  Parameter is an #ICE_Linger structure.
 */
#define ICE_SO_LINGER          0x0080

/**
 * ICE_SO_BROADCAST is socket option that permits sending of broadcast msgs.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_SOCKET.  Parameter is an <code>int</code> that may
 * be 0 for false or 1 for true.
 */
#define ICE_SO_BROADCAST       0x0020

/**
 * ICE_SO_REUSEADDR means allowing local address reuse.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_SOCKET.  Parameter is an <code>int</code> that may
 * be 0 for false or 1 for true.
 */
#define ICE_SO_REUSEADDR       0x0004

/**
 * ICE_SO_SNDBUF is socket option for send buffer size.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_SOCKET.  Parameter is a nonnegative <code>int</code>
 * that specifies the buffer size in bytes.
 */
#define ICE_SO_SNDBUF          0x1001

/**
 * ICE_SO_RCVBUF is socket option for receive buffer size.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_SOCKET.  Parameter is a nonnegative <code>int</code>
 * that specifies the buffer size in bytes.
 */
#define ICE_SO_RCVBUF          0x1002

/**
 * ICE_IP_MULTICAST_TTL is socket option to set/get IP multicast
 * time to live.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_IP.  Parameter is a nonnegative <code>int</code>.
 */
#define ICE_IP_MULTICAST_TTL   3

/**
 * ICE_IP_MULTICAST_IF is socket option to set/get IP multicast interface.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_IP.  Parameter is an #ICE_InAddr.
 */
#define ICE_IP_MULTICAST_IF    2

/**
 * ICE_IP_ADD_MEMBERSHIP is socket option to add  an IP group membership.
 *
 * Socket option used in ICE_SocketSetSockOpt() with level #ICE_SOL_IP.
 * Parameter is an #ICE_IpMreq.
 *
 * It is an error to pass this value to ICE_SocketGetSockOpt().
 */
#define ICE_IP_ADD_MEMBERSHIP  5

/**
 * ICE_IP_DROP_MEMBERSHIP is socket option to drop an IP group membership.
 *
 * Socket option used in ICE_SocketSetSockOpt() with level #ICE_SOL_IP.
 * Parameter is an #ICE_IpMreq.
 *
 * It is an error to pass this value to ICE_SocketGetSockOpt().
 */
#define ICE_IP_DROP_MEMBERSHIP 6

/**
 * ICE_TCP_NODELAY is tcp option to set or disable the Nagle algorithm.
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_TCP.  Parameter is an <code>int</code> that may
 * be 0 for false or 1 for true.
 */
#define ICE_TCP_NODELAY        0x0001

/**
 * IP option to include IP header in messages (SOCK_RAW sockets).
 *
 * Socket option used in ICE_SocketSetSockOpt() and ICE_SocketGetSockOpt(),
 * with level #ICE_SOL_IP.  Parameter is an <code>int</code> that may
 * be 0 for false or 1 for true.
 */
#define ICE_IP_HDRINCL         2

/**
 * @brief Used by #ICE_IP_ADD_MEMBERSHIP/#ICE_IP_DROP_MEMBERSHIP socket
 * options.
 */
typedef struct ICE_IpMreq
{
    /**
     * Multicast group address.
     */
    ICE_InAddr imr_multiaddr;

    /**
     * Local interface address.
     */
    ICE_InAddr imr_interface;
} ICE_IpMreq;

/**
 * @brief Used by #ICE_SO_LINGER socket option.
 */
typedef struct ICE_Linger
{
    /**
     * Option on/off.
     */
    frost_uint16 l_onoff;

    /**
     * Linger time in seconds.
     */
    frost_uint16 l_linger;
} ICE_Linger;

/**
 * @brief Gets option on a socket.
 * @param s Socket descriptor.
 * @param level Level of option.
 * The level parameter specify at which level the option operates:
 * #ICE_SOL_SOCKET Socket level.
 * #ICE_SOL_TCP TCP level
 * #ICE_SOL_UDP UDP level
 * #ICE_SOL_IP IP level
 * @param opt_name Option name.
 * For the socket level the options presented below are available. When not 
 * specified the option value has the type int.
 * #ICE_SO_REUSEADDR The socket may be bound to an address already used locally.
 * #ICE_SO_LINGER Blocks the process when a close is performed until all
 * data has been delivered to the peer. The argument is a
 * linger structure containing the linger flag and the linger
 * interval, specifying the timeout value for the closing call.
 * The members of this structure are:
 * l_onoff linger flags.
 * l_linger linger time in seconds.
 * #ICE_SO_SNDBUF Size of output buffer.
 * #ICE_SO_RCVBUF Size of input buffer.
 * For the TCP level the following options are available:
 * #ICE_TCP_NODELAY If set, disable the Nagle algorithm.
 * For the IP level the following options are available:
 * #ICE_IP_HDRINCL Include IP header in messages (SOCK_RAW sockets).
 * #ICE_IP_MULTICAST_IF Default interface for outgoing multicasts and broadcasts.
 * This option takes a sockaddr_in structure as argument to select the outgoing
 * network interface.
 * #ICE_IP_ADD_MEMBERSHIP Adds a new multicast group membership to an interface.
 * The argument is an #ICE_IpMreq structure. The structure #ICE_IpMreq 
 * contains at least the following members:
 * imr_multiaddr IP multicast address of the group.
 * imr_interface IP address of the interface.
 * #ICE_IP_DROP_MEMBERSHIP Removes a multicast group membership previously
 * added to an interface. The argument is an #ICE_IpMreq structure
 * @param opt_val Pointer to the buffer in which the value for the requested 
 * option is to be returned. Cannot be null.
 * @param opt_len Pointer to size of option value. Cannot be null.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketGetSockOpt(
    frost_int32 s, 
    frost_int32 level, 
    frost_int32 opt_name, 
    void * opt_val, 
    frost_uint32 * opt_len);

/**
 * @brief Sets option on a socket.
 * @param s Socket descriptor.
 * @param level Level of option.
 * @param opt_name Option name. See ICE_SocketGetSockOpt() for the options.
 * @param opt_val Pointer to option value. Cannot be null.
 * @param opt_len Size of option value.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketSetSockOpt(
    frost_int32 s, 
    frost_int32 level, 
    frost_int32 opt_name, 
    const void * opt_val, 
    frost_uint32 opt_len);

/*@}*/

/* *********************************************************************** */
/**
 * @name Socket ioctl
 */
/*@{*/

/**
 * ICE_FIONBIO Enables/disables non-blocking mode.
 *
 * Used as a <code>cmd</code> parameter to ICE_SocketIoctl().
 * The <code>arg</code> parameter should be a pointer to an int that
 * is 1 to enable non-blocking mode or 0 to disable it.
 */
#define ICE_FIONBIO 10

/**
 * @brief Manipulates a socket descriptor.
 * @param s Socket descriptor
 * @param cmd Command name
 * The supported commands are one the following:
 * #ICE_FIONBIO Enables/disables non-blocking mode. Parameter is an int.
 * @param arg Pointer to command argument. May be null.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketIoctl(
    frost_int32 s,
    frost_int32 cmd,
    void * arg);

/*@}*/

/* *********************************************************************** */
/**
 * @name select() on sockets
 */
/*@{*/

/**
 * @brief Checks for activity on sockets.
 *
 * It will set values of read_fds, write_fds, except_fds to -1 for unchanged 
 * descriptors.
 *
 * @param read_fds File descriptor to test for reading. If read_fds_count is
 *                 nonzero, then this cannot be NULL.
 * @param read_fds_count number of fds for read.
 * @param write_fds File descriptor to test for writing. If write_fds_count is
 *                 nonzero, then this cannot be NULL.
 * @param write_fds_count number of fds for write.
 * @param except_fds File descriptor to test for exceptional condition. If
 *                 except_fds_count is nonzero, then this cannot be NULL.
 * @param except_fds_count number of fds for exception.
 * @param timeout Maximum delay to wait for, in milliseconds. Negative
 *                 value means infinite wait. Zero means return immediately.
 * @return The number of ready descriptors if the call is successful,
 *         negative value if an error has occurred,
 *         0 when the timeout has expired.
 */
frost_int32 ICE_SocketSelect(
        frost_int32 * read_fds,
        frost_int32 read_fds_count,
        frost_int32 * write_fds,
        frost_int32 write_fds_count,
        frost_int32 * except_fds,
        frost_int32 except_fds_count,
        frost_int32 timeout);

/**
 * @brief Used to timeout blocking operations.
 *
 * Returns immediately if an accept, recv, or recvfrom operation would succeed
 * without blocking. Otherwise, blocks up to timeout in milliseconds.
 * @param s Socket descriptor.
 * @param timeout Interval in milliseconds.
 * @return 1 if something's ready, 0 if it timed out, -1 on error, and -2 on 
 * interruption.
 */
frost_int32 ICE_SocketTimeout(
    frost_int32 s, 
    frost_int32 timeout);

/**
 * ICE_SocketPoll() should check if, or has detected that, the socket
 * has data that is readable without blocking.
 */
#define ICE_POLLIN 1

/**
 * ICE_SocketPoll() should check if, or has detected that, the socket
 * has free buffers so it can be written to without blocking.
 */
#define ICE_POLLOUT 2

/**
 * ICE_SocketPoll() should check if, or has detected that, the socket
 * has an error condition.
 */
#define ICE_POLLERR 4

/**
 * Structure used by ICE_SocketPoll().
 */
typedef struct ICE_SocketPollFd {
    frost_int32 fd;        /**< the socket descriptor being polled */
    frost_int16 events;    /**< the input event flags */
    frost_int16 revents;   /**< the output event flags */
} ICE_SocketPollFd;

/**
 * @brief Checks for activity on sockets.
 *
 * This function reads the <code>fd</code> and <code>events</code> fields in
 * the passed ICE_SocketPollFd structures, and sets the <code>revents</code>
 * fields.  Note that this function must guarantee to always write to all the
 * <code>revents</code> fields, overwriting whatever was there before.
 *
 * Valid values in the <code>events</code> field are #ICE_POLLIN,
 * #ICE_POLLOUT, and #ICE_POLLERR.  Some platforms may choose to behave as
 * though ICE_POLLERR is always set in the <code>events</code> field; this
 * is acceptable (but not required).
 *
 * Note that this function can be implemented on any platform that has
 * either select() or poll() support.  I.e. it is possible to implement it
 * to internally call select() or poll(), it is the ICE implementer's
 * decision about which one to use.
 *
 * @param fds Array of data.  Then this cannot be NULL.
 * @param nfds Number of entries in array.  Must be nonzero.
 * @param timeout_millis Maximum delay to wait for, in milliseconds. Negative
 *                 value means infinite wait. Zero means return immediately.
 * @return The number of ready descriptors if the call is successful,
 *         negative value if an error has occurred,
 *         0 when the timeout has expired.
 */
frost_int32 ICE_SocketPoll(ICE_SocketPollFd fds[], frost_uint32 nfds,
                           frost_int32 timeout_millis);

/*@}*/

/* *********************************************************************** */
/**
 * @name DNS queries
 */
/*@{*/

/**
 * @brief Host information.
 */
typedef struct ICE_HostEnt
{
    /**
     * Host name.
     */
    char *h_name;

    /** 
     * List of aliases.
     */
    char **h_aliases;

    /**
     * Type of address (always #ICE_AF_INET).
     */
    frost_int16 h_addrtype;

    /**
     * Length of address.
     */
    frost_int16 h_length;

    /**
     * List of addresses.
     */
    char **h_addr_list;

    /**
     * This define is made because it is conventionally used in BSD socket 
     * applications
     */
#define h_addr h_addr_list[0]
} ICE_HostEnt;

/**
 * @brief Gets host entry from address.
 * @param addr Pointer to host address. Cannot be null.
 * @param len Length of address.
 * @param type Type of address (#ICE_AF_INET).
 * @param result Pointer to resulting host entry. Cannot be null.
 * @param buf Pointer to buffer used to store host addresses. Cannot be null.
 * @param buf_len Size of the buffer.
 * @param err Pointer to integer filled with error code. Cannot be null.
 * @return A pointer to the #ICE_HostEnt structure,
 * null if the host was not found.
 */
ICE_HostEnt * ICE_SocketGetHostByAddr(
    const char * addr,
    frost_uint32 len, 
    frost_int32 type,
    ICE_HostEnt * result, 
    char * buf, 
    frost_uint32 buf_len, 
    frost_int32 * err);

/**
 * @brief Gets host entry from name.
 * @param name Pointer to host name. Must be null terminated. Cannot be null.
 * @param result Pointer to resulting host entry. Cannot be null.
 * @param buf Pointer to buffer used to store host addresses. Cannot be null.
 * @param buf_len Size of buffer.
 * @param err Pointer to integer filled with error code. Cannot be null.
 * @return A pointer to the #ICE_HostEnt structure,
 * null if the host was not found.
 */
ICE_HostEnt * ICE_SocketGetHostByName(
    const char * name, 
    ICE_HostEnt * result, 
    char * buf, 
    frost_uint32 buf_len, 
    frost_int32 * err);

/*@}*/

/* *********************************************************************** */
/**
 * @name Text representation of IP addresses
 */
/*@{*/

/**
 * @brief Converts an ASCII string representing an internet address in standard
 * dot notation into a 32-bits internet address in network-byte order.
 * @param str Pointer to the string containing the address in standard dot
 * notation. Must be null terminated. Cannot be null.
 * @return The internet address if the call is successful, and
 * -1 cast to a frost_uint32 if an error occurs.
 */
frost_uint32 ICE_SocketInetAddr(const char * str);

/**
 * @brief Converts an ASCII string representing an internet address in standard
 * dot notation into a 32-bits internet address in network-byte order.
 * @param str Pointer to the string containing the address in standard dot
 * notation. Must be null terminated. Cannot be null.
 * @param addr Pointer to the resulting address. Cannot be null.
 * @return 1 if the string is valid, 0 if the string is invalid.
 */
frost_uint32 ICE_SocketInetAton(
    const char * str, 
    ICE_InAddr * addr
);

/**
 * @brief Converts a 32-bits internet address into a string in standard dot 
 * notation.
 * @param addr The internet address.
 * @return A string containing the internet address in standard dot notation.
 */
char * ICE_SocketInetNtoa(ICE_InAddr addr);

/*@}*/

/* *********************************************************************** */
/**
 * @name Local address
 */
/*@{*/

/**
 * @brief Gets the local host name.
 * @param name Pointer to a buffer to be filled with host name. Cannot be null.
 * @param name_len Size of host name buffer.
 * @return 0 if the call is successful, -1 if an error has occurred.
 */
frost_int32 ICE_SocketGetHostName(
    char * name,
    frost_uint32 name_len);

/**
 * @brief Gets the local IP address.
 *
 * This returns the externally-visible IP address, it must never return the
 * loopback address.
 *
 * @param out recieves the local IP address.
 * @return frost_true on success, frost_false on error.
 */
frost_bool ICE_SocketGetLocalAddress(ICE_InAddr *out);

/*@}*/

/* *********************************************************************** */
/**
 * @name Protocol names
 */
/*@{*/

/**
 * @brief Protocol entry.
 */
typedef struct ICE_ProtoEnt
{
    /**
     * Official name of protocol.
     */
    char *p_name;

    /**
     * Alias list (not implemented).
     */
    char **p_aliases;

    /**
     * Protocol number.
     */
    frost_int16 p_proto;
} ICE_ProtoEnt;

/**
 * @brief Gets protocol information.
 * @param name Pointer to the protocol name. Must be null terminated.
 * Cannot be null.
 * @return A pointer to a #ICE_ProtoEnt structure,
 * null if the protocol was not found.
 */
ICE_ProtoEnt * ICE_SocketGetProtoByName(const char * name);

/*@}*/

/* *********************************************************************** */
/**
 * @name Network byte order conversion
 */
/*@{*/

/**
 * @brief Converts host byte order value 32-bits (Long) to network byte order.
 * @param value The host byte order value.
 * @return The converted value in network byte order.
 */
frost_uint32 ICE_SocketHtonl(frost_uint32 value);

/**
 * @brief Converts host byte order value 16-bits (Short) to network byte order.
 * @param value The host byte order value.
 * @return The converted value in network byte order.
 */
frost_uint16 ICE_SocketHtons(frost_uint16 value);

/**
 * @brief Converts network byte order value to host byte order value 32-bits (Long).
 * @param value The network byte order value.
 * @return The converted value in host byte order.
 */
frost_uint32 ICE_SocketNtohl(frost_uint32 value);

/**
 * @brief Converts network byte order value to host byte order value 16-bits (Short).
 * @param value The network byte order value.
 * @return The converted value in host byte order.
 */
frost_uint16 ICE_SocketNtohs(frost_uint16 value);

/*@}*/

/**
 * Operation completed successfuly.
 */
#define ICE_SOCKET_SUCCESS 0 
 
/**
 * Operation failed due to a general error.
 */
#define ICE_SOCKET_FAIL -1 

/**
 * Operation failed because it would block if it continued. Indicates that an 
 * operation on a non-blocking socket cannot be completed immediately.
 */
#define ICE_SOCKET_WOULD_BLOCK -2

/**
 * Operation timed out.
 */
#define ICE_SOCKET_TIMEOUT -3

/**
 * @brief Returns the error status for the last operation that failed.
 *
 * @note This functions is not necessary, but it will make porting and debugging 
 * simpler
 *
 * @return The last error code.
 */
frost_int32 ICE_SocketGetLastError(void);

frost_long ICE_SocketGetMacAddress ( char *addr);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_SOCKET_H_INCLUDED*/
