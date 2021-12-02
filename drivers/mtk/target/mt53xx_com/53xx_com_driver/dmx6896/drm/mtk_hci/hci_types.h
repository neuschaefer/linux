/** @addtogroup hci hci
 * @ingroup marlin
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef HCI_TYPES_H
#define HCI_TYPES_H

typedef enum {
    L_KEY_FORMAT_AES,
    L_KEY_FORMAT_RSA,
    L_KEY_FORMAT_XML,    /* this is currently used to represent Starfish encrypted key */
    L_KEY_FORMAT_UNSPECIFIED
} hci_l_key_format_t;

typedef struct hci_l_key_ref_s {
    binstr_t           *key_data;            /* holds the raw key data if needed by the ref */
    hci_key_type_t      type;                /* type of key held by the reference */
    hci_l_key_format_t  format;              /* format of key held by the reference */
    bool_t              isAvailable;         /* is key_data available (i.e., after duplication) */
} hci_l_key_ref_t;

#endif
