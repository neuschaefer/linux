/*
 * Copyright (C) <2008>, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

/***************************************************************************/

/*! \file ti_pp_path.h
    \brief Exports of PP Path counters

****************************************************************************/

#ifndef _TI_PP_PATH_H_
#define _TI_PP_PATH_H_

#ifndef _TI_PP_PATH_C
    #define EXTERN
#else
    #define EXTERN extern
#endif

/* To include debug code, enable this */
//#define PPP_DEBUG

/**************************************************************************/
/*      INCLUDES                                                          */
/**************************************************************************/

#include <asm-arm/arch-avalanche/generic/_tistdtypes.h>

/**************************************************************************/
/*      INTERFACE  Defines and Structs                                    */
/**************************************************************************/

/*! \var typedef enum PPP_pathDir_e
    \brief Defines the direction of a path
*/
typedef enum
{
    /* PPP_PATHDIR_NONE is for internal use only, not to be used by APIs */
    PPP_PATHDIR_NONE = 0,   /* Keep it ==0, so memest 0 will set to the default value of NONE */
    /* *** */
    PPP_PATHDIR_IN,
    PPP_PATHDIR_OUT
} PPP_pathDir_e;


/*! \var typedef struct PPP_1Counetr_t
    \brief Single counter with all counted element types
*/
typedef struct
{
    Uint64 octets;
    Uint64 packets;
} PPP_1Counetr_t;


/*! \var typedef struct PPP_unidirCounetrs_t
    \brief Uni-directional counters
*/
typedef struct
{
    PPP_1Counetr_t unicastCtr;
    PPP_1Counetr_t multicastCtr;
} PPP_unidirCounetrs_t;


/*! \var typedef struct PPP_counters_t
    \brief Path counters
*/
typedef struct
{
    PPP_unidirCounetrs_t inCtrs;
    PPP_unidirCounetrs_t outCtrs;
} PPP_counters_t;

/*! \var typedef struct TI_PPP_PACKET_INFO
    \brief Per packet info required for PP Path management

    Theoretiaclly, we need a linked list of all devices the packet travelled thru,
    but to save RT, we use a static array.
    Length 2 should suffice, but to be on the safe side we use 5
*/
typedef struct
{
    Uint32 num_devs;    /* Number of devices */
    int devices[5];     /* device index */
#ifdef PPP_DEBUG
    Uint32 num_clones;  /* Number of clonings this skb suffered */
    Uint32 num_copies;  /* Number of clonings this skb suffered */
    int clones[5];     /* device index */
    int copies[5];     /* device index */
#ifdef CONFIG_FTRACE
    unsigned long long markTime[5];
    unsigned long long allocTime;
#endif
#endif
} TI_PPP_PACKET_INFO; 



/**************************************************************************/
/*! \var PPP_1CTR(__ctr, __path, __session)
 **************************************************************************
 *  \brief Refernce a single counter
 *  \param[in] __ctr: Name of counter - PPP_counters_t *
 *  \param[in] __dir: Direction to count
 *  \param[in] __mcast: Multi/Unicast
 *  \return Counter variable PPP_uniCounetrs_t
 **************************************************************************/
#define PPP_1CTR(__ctr, __dir, __mcast) \
    ((__dir) == PPP_PATHDIR_IN) ? \
        ((__mcast) ? &((__ctr)->inCtrs.multicastCtr) : &((__ctr)->inCtrs.unicastCtr)) : \
        ((__mcast) ? &((__ctr)->outCtrs.multicastCtr) : &((__ctr)->outCtrs.unicastCtr)) 

#define PPP_ZERO_CTR(__ctr) \
    do { \
        memset((__ctr), 0, sizeof(*(__ctr))); \
    } while (0) 

#define PPP_ADD_1CTR_oct_pckt(__dest, __oct, __pckt) \
    do { \
       (__dest)->octets += (__oct); \
       (__dest)->packets += (__pckt); \
    } while (0)

#define PPP_ADD_1CTR(__dest, __add) \
    PPP_ADD_1CTR_oct_pckt((__dest), (__add)->octets, (__add)->packets) 
    
#define PPP_ADD_UNIDIRCTR(__dest, __add) \
    do { \
        PPP_ADD_1CTR(&((__dest)->unicastCtr), &((__add)->unicastCtr)); \
        PPP_ADD_1CTR(&((__dest)->multicastCtr), &((__add)->multicastCtr)); \
    } while (0)

#define PPP_ADD_CTRS(__dest, __add) \
    do { \
        PPP_ADD_UNIDIRCTR(&((__dest)->inCtrs), &((__add)->inCtrs)); \
        PPP_ADD_UNIDIRCTR(&((__dest)->outCtrs),&((__add)->outCtrs)); \
    } while (0)

#define PPP_DISPLAY_FORMAT_1CTR \
            "Bytes %llu, Packets %llu"

#define PPP_DISPLAY_ELEMENTS_1CTR(__ctr) \
            (__ctr)->octets, (__ctr)->packets 

#define PPP_DISPLAY_FORMAT_UNIDIRCTR(__indent) \
            "\n" __indent "Unicast [" PPP_DISPLAY_FORMAT_1CTR "]" \
            "\n" __indent "Multicast [" PPP_DISPLAY_FORMAT_1CTR "]"

#define PPP_DISPLAY_ELEMENTS_UNIDIRCTR(__ctr) \
            PPP_DISPLAY_ELEMENTS_1CTR(&((__ctr)->unicastCtr)), \
            PPP_DISPLAY_ELEMENTS_1CTR(&((__ctr)->multicastCtr))

#define PPP_DISPLAY_FORMAT_CTRS(__indent) \
            "\n" __indent "In" PPP_DISPLAY_FORMAT_UNIDIRCTR(__indent "\t") \
            "\n" __indent "Out" PPP_DISPLAY_FORMAT_UNIDIRCTR(__indent "\t")

#define PPP_DISPLAY_ELEMENTS_CTRS(__ctr) \
            PPP_DISPLAY_ELEMENTS_UNIDIRCTR(&((__ctr)->inCtrs)), \
            PPP_DISPLAY_ELEMENTS_UNIDIRCTR(&((__ctr)->outCtrs))

/**************************************************************************/
/*      EXTERN definition block                                           */
/**************************************************************************/

/**************************************************************************/
/*      INTERFACE VARIABLES (prefix with EXTERN)                          */
/**************************************************************************/

/**************************************************************************/
/*      INTERFACE FUNCTIONS Prototypes:                                   */
/**************************************************************************/

#if defined(__KERNEL__)
/**************************************************************************/
/*! \fn int PPP_Init(void)
 **************************************************************************
 *  \brief Init the PPP module
 *  \return 0 (OK) / 1 (NOK)
 **************************************************************************/
int PPP_Init(void);

/**************************************************************************/
/*! \fn int PPP_AddPath(char *rxFrom, char *txTo, char *viaVirtualDev, PPP_pathDir_e pathDir)
 **************************************************************************
 *  \brief Add a path
 *  \param[in] rxFrom - input device 
 *  \param[in] txTo - output device 
 *  \param[in] viaVirtualDev - virtual device through which the traffic flows
 *  \param[in] pathDir - does the count belong to the IN (to the box) or OUT (of the box)
 *  \return 0 (OK) / err (NOK)
 **************************************************************************/
int PPP_AddPath(char *rxFrom, char *txTo, char *viaVirtualDev, PPP_pathDir_e pathDir);

/**************************************************************************/
/*! \fn int PPP_DelPath(char *rxFrom, char *txTo, char *viaVirtualDev)
 **************************************************************************
 *  \brief Delete a path
 *  \param[in] rxFrom - device used as IN
 *  \param[in] txTo - device used as OUT
 *  \param[in] viaVirtualDev - virtual device through which the traffic flows
 *  \return 0 (OK) / 1 (NOK)
 **************************************************************************/
int PPP_DelPath(char *rxFrom, char *txTo, char *viaVirtualDev);

/**************************************************************************/
/*! \fn int PPP_ReadCounters(char *viaVirtualDev, PPP_counters_t *pppCtrs)
 **************************************************************************
 *  \brief Read a path's counters
 *  \param[in] viaVirtualDev - virtual device through which the traffic flows
 *  \param[out] pppCtrs - Counters of PP traffic going through the virtual device
 *              in/out are provided according to the paths requested when
 *              adding the paths.
 *  \return 0 (OK) / 1 (NOK)
 **************************************************************************/
int PPP_ReadCounters(char *viaVirtualDev, PPP_counters_t *pppCtrs);

/**************************************************************************/
/*! \fn static int PPP_MarkPacket(struct sk_buff *skb, int dev)
 **************************************************************************
 *  \brief Mark (in the skb) that the packet flowed via the device
 *  \param[in] skb - skb that started the session
 *  \param[in] dev - the device index through which the packet is flowing
 *  \return 0 (OK) / !=1 (NOK)
 **************************************************************************/
int PPP_MarkPacket(struct sk_buff *skb, int dev);

#endif /*KERNEL*/
#endif

