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

#ifndef HCI_PERS_H
#define HCI_PERS_H

#define HCI_PERSONALITY_VERSION 1

typedef struct pers_box1 {
    u_int16_t   offset;
    u_int16_t   len[1];
} pers_box1_t; 

typedef struct pers_box2 {
    u_int16_t   offset;
    u_int16_t   len[2];
    u_int8_t    reserved[2];
} pers_box2_t; 

typedef struct pers_box3 {
    u_int16_t   offset;
    u_int16_t   len[3];
} pers_box3_t; 

typedef struct pers_box4 {
    u_int16_t   offset;
    u_int16_t   len[4];
    u_int8_t    reserved[2];
} pers_box4_t; 

typedef struct nemo_priv {
    u_int8_t    version;
    u_int8_t    reserved[3];
    pers_box2_t  pkcs8;      /* 0:pkcs#8 for sig, 1:pkcs#8 for enc */
} nemo_priv_t;

typedef struct oct_priv {
    u_int8_t    version;
    u_int8_t    reserved[3];

    pers_box2_t  scuba;      /* 0:uid, 1:subject */
    pers_box2_t  seckey;     /* 0:uid, 1:keydata */
    pers_box3_t  privkey;    /* 0:pairid, 1:uid, 2:keydata */
    pers_box2_t  torpedo;    /* 0:uid, 1:subject */
    pers_box4_t  starfish;   /* 0:uid, 1:source, 2: algorithm 3: keydata*/
} oct_priv_t;

typedef enum {
    L_PERS_TYPE_OPPR_SEC,
    L_PERS_TYPE_OPPR_PRV,
    L_PERS_TYPE_STARFISH
} hci_l_pers_type_t;

/* -------------------------------------------------------------------------- */
/**
 * @brief Parse the nemo personality and return the private keys.
 *
 * @param[in]  in_bs       Binary string containing the nemo personality.
 * @param[out] out_sigkey  Nemo signing key in PKCS#8 format.
 * @param[out] out_enckey  Nemo encoding key in PKCS#8 format.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_ILLEGALPERSONALITY
 */
/* -------------------------------------------------------------------------- */
retcode_t
hci_l_ParseNEMOPrivateKeys(
    binstr_t    *in_bs,
    binstr_t   **out_sigkey,
    binstr_t   **out_enckey);

/* -------------------------------------------------------------------------- */
/**
 * @brief Parse the octopus personality and return the private keys.
 *
 * @param[in]  in_bs           Binary string containing the octopus personality.
 * @param[in]  in_type         Type of private key to return.
 * @param[out] out_keyid       Octopus key id.
 * @param[out] out_keydata     Octopus key data.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_ILLEGALPERSONALITY
 */
/* -------------------------------------------------------------------------- */
retcode_t
hci_l_ParseOctopusPrivatePersonality(
    binstr_t        *in_bs,
    hci_l_pers_type_t in_type,
    binstr_t       **out_keyid,
    binstr_t       **out_keydata); 

/* -------------------------------------------------------------------------- */
/**
 * @brief Load the Nemo/Octopus private personality into the HCI.
 *
 * @param[in] in_type     Personality type
 * @param[in] in_len      Length of the buffer.
 * @param[in] in_buf      Buffer which contains the personality data.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ssmem_SetObject
 */
/* -------------------------------------------------------------------------- */
retcode_t 
hci_l_PersonalityKeyLoad(hci_pers_key_type_t in_type,
                         np_size_t in_len,
                         u_int8_t *in_buf);

/* -------------------------------------------------------------------------- */
/**
 * @brief Retrieve the Nemo/Octopus private personality from the HCI.
 *
 * @param[in]  in_type      Personality type
 * @param[out] out_object   Object

 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_NG
 */
/* -------------------------------------------------------------------------- */
retcode_t 
hci_l_PersonalityKeyRetrieve(hci_pers_key_type_t in_type,
                             binstr_t          **out_object);


/* -------------------------------------------------------------------------- */
/**
 * @brief Check whether the Nemo/Octopus private personality is present.
 *
 * @param[in] in_type     Personality type
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_HCI_NOPERSONALITYREGISTERED
 */
/* -------------------------------------------------------------------------- */
retcode_t
hci_l_PersonalityKeyCheck (hci_pers_key_type_t in_type);

#endif
