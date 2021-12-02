/** @addtogroup hci hci
 * @ingroup marlin
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010 Sony Corporation, All Rights Reserved.
 */

#ifndef HCI_OBFUSCATE_H
#define HCI_OBFUSCATE_H

#ifdef __cplusplus
extern "C" {
#endif

    /* Local function for scrambling the supplied key data */
retcode_t
hci_l_Scramble(binstr_t* io_key);

    /* Local function for unscrambling the supplied key data */
retcode_t
hci_l_Unscramble(binstr_t*			in_key,
                 binstr_t**			out_key);

    /* Free a scrambled key */
retcode_t
hci_l_FreeKey(binstr_t *io_key);

    /* Randomise the data in a buffer used to srcub key from memory 
     * when not needed anymore - safer than a free since this will 
     * leave the key in RAM 
     */
extern void
hci_l_DataScrub(u_int8_t         *io_buf,
                u_int32_t         in_buf_len);

#ifdef __cplusplus
}
#endif

#endif /* HCI_OBFUSCATE_H */
