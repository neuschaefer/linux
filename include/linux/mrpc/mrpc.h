/*
 *
 * mrpc.h
 * Description:
 * RPC over mailbox driver API
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#ifndef __HW_MRPC_H__
#define __HW_MRPC_H__

/* error codes */
enum {
    MRPC_SUCCESS = 0,           /* success */
    MRPC_ERR_INVALID_SIGNATURE, /* hw mailbox callback invalid mrpc signature */
    MRPC_ERR_INVALID_HEADER,    /* hw mailbox callback invalid mrpc header */
    MRPC_ERR_HWMBOX_NOT_READY,  /* hw mailbox driver not ready */
    MRPC_ERR_HWMBOX_ERROR,      /* hw mailbox error */
    MRPC_ERR_SERVER_NOT_READY,  /* remote server not registered */
    MRPC_ERR_INVALID_PROC,      /* invalid server proc  */
    MRPC_ERR_SERVER_ERROR,      /* remote server error */
    MRPC_ERR_INVALID_CLIENT,    /* invalid client */
    MRPC_ERR_INVALID_SERVER,    /* invalid server */
    MRPC_ERR_GARBAGE_ARGS,      /* server can't decode supplied arguments */
    MRPC_ERR_ARGLEN_MISMATCH,   /* server arglen not matches received arglen */
    MRPC_ERR_REPLEN_MISMATCH,   /* server replen not matches received replen */
    MRPC_ERR_MSG_TOO_BIG,       /* message too big */
    MRPC_ERR_UNKOWN             /* unknown error */
};

typedef int (*proc_t)(void *arg, ssize_t arg_len, void *rep, ssize_t rep_len);

/**
 * mrpc_procinfo structure, registered by the server to
 * the mrpc driver during initialization
 */
struct mrpc_procinfo {
    __u8 procid;        /* proc number */
    proc_t p_func;      /* proc function */
    ssize_t p_arglen;   /* argument hdr length (u32) */
    ssize_t p_replen;   /* reply hdr length (u32) */
    const char *p_name; /* name of procedure */
};

/* reserved IDs */
enum {
    MRPC_RESERVED_ID_PP_CONTROL = 0,
    MRPC_RESERVED_ID_PP_DEBUG,
    MRPC_RESERVED_ID_PP_FUNC_CALL, /* temporary until PP core and PP HAL are ready */
    MRPC_RESERVED_ID_NETEVENTS,
    MRPC_RESERVED_ID_NETHOOKS,
    MRPC_RESERVED_ID_MODPHY,
    MRPC_RESERVED_ID_HANDSHAKE,
    MRPC_RESERVED_ID_CONNTRACK,
    MRPC_RESERVED_ID_HILCMDS,
    MRPC_RESERVED_ID_CPPI,
    MRPC_RESERVED_ID_WIFI_PROXY_FUNC_CALL,
    MRPC_RESERVED_ID_DOCSIS_NETDEV,
    MRPC_RESERVED_ID_SGC,
    MRPC_RESERVED_ID_HILEVENTS,
    MRPC_RESERVED_ID_GPIO,
    MRPC_RESERVED_ID_REMOTE_CONFIG,
    MRPC_RESERVED_ID_LAST,
};

struct mrpc_client;
struct mrpc_server;

/************************************************/
/**                 API                         */
/************************************************/

/**
 * mrpc_client_register
 * client registration function
 *
 * @param id     client's unique ID
 * @param name   Client's name
 *
 * @return client handle for success, NULL otherwise
 */

struct mrpc_client *mrpc_client_register(unsigned int id, const char *name);
/**
 * mrpc_server_register
 * server's registration function
 *
 * @param id        server's unique ID
 * @param name      server's name
 * @param procs     server's procs array
 * @param num_procs number of procs
 *
 * @return server handle for success, NULL otherwise
 */
struct mrpc_server *mrpc_server_register(unsigned int id, const char *name,
                                         struct mrpc_procinfo *procs,
                                         u32 num_procs);
/**
 * mrpc_client_unregister
 * Client unregistration function
 *
 * @param client client handle
 *
 * @return 0 for success, error code otherwise
 */
int mrpc_client_unregister(struct mrpc_client *client);

/**
 * mrpc_server_unregister
 *
 * @param server server handle
 *
 * @return 0 for success, error code otherwise
 */

int mrpc_server_unregister(struct mrpc_server *server);

/**
 * mrpc_call
 * Blocking function call issued by a client to run a
 * remote procedure on the server.
 *
 * @param client  client handle
 * @param proc    remote procedure id
 * @param arg     remote procedure argument pointer (network byte order)
 * @param arg_len remote procedure argument length (bytes)
 * @param rep     remote procedure reply pointer
 * @param rep_len remote procedure reply length (bytes)
 * @param timeout milliseconds to wait for reply (0 for default)
 * @param errcode additional error code information
 *
 * @return 0 for success, mrpc error code otherwise
 */
int mrpc_call(struct mrpc_client *client, __u8 proc, void *arg,
              ssize_t arg_len, void *rep, ssize_t rep_len,
              unsigned long timeout, int *errcode);

#endif /* __HW_MRPC_H__ */
