/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   WBAMR_LIB.H
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 *
 *******************************************************************************/

 
#ifndef WBAMR_LIB_H
#define WBAMR_LIB_H


/*
 * includes
 */
#include "lib_typedef.h"        /* ADI standard types */
#include "SCShared_Lib.h"       /* Shared speech codec stuff */


/*************************************************************************
 * Interface to 3GPP WBAMR Encoder                                       *
 *************************************************************************/


/*************************************************************************
 * Encoder memory summary
 *
 *    "state"  - 2696 bytes of 32-bit aligned memory
 *    "temp_0" - 8028 bytes of 32-bit aligned memory
 *
 *    Current as of the date of PVCS revision : Rev 1.19
 *    Most recent PVCS revision               : $Revision: #2 $
 *
 *    Please note, if the two revision numbers above do not match,
 *    then the memory numbers may be out of date.
 *
 */

#define WBAMR_ENC_STATIC_SIZE   (674+8)/*size in fract32 of Enc state*/
#if defined(AWB_OPT_VER) && (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
#define WBAMR_ENC_STACK_SIZE_0 (2759+128) /*size in fract32 of Enc primary scratch*/
#else
#define WBAMR_ENC_STACK_SIZE_0 2007 /*size in fract32 of Enc primary scratch*/
#endif
#define WBAMR_ENC_STACK_SIZE_1  194 /*size in fract32 of Enc secondary scr*/

/*************************************************************************
 * The following structures define the Input/Output parameters
 * to the WBAMR encoder.
 *
 */

struct WBAMR_IO_Enc_Struct
{
   int vad;
   void *temp_0;
            /* 
	     * NO INPUT/NO OUTPUT: Pointer to primary temporary 
	     * scratch area. To obtain low encoder cycle counts,
	     * temp_0 and temp_1 buffers must be located in
	     * alternate banks to allow dual memory access.
	     * See above for size and alignment requirements.
	     */

   void *temp_1;
            /* 
	     * NO INPUT/NO OUTPUT: Pointer to auxiliary temporary 
	     * scratch area.  See above for size and alignment
	     * requirements.
	     */

   fract16 *speech;
	    /* 
	     * INPUT: Input frame with 320 PCM audio samples. 
	     * fract16 array of length 320. 
	     */

   uint16 *parms; 
            /*
	     * OUTPUT: Encoded output bits, reordered according to 3GPP
	     * TS 26.201(speech codec performs the reordering!) and packed
	     * into unsigned 16-bit words.
	     *
	     * Array format: 
	     * The maximum number of encoded output bits from serial buffer
	     * are packed into 16 bits per word, and the remaining bits are 
	     * packed in the last word, right justified. The number of
	     * encoded output bits are rate dependent, which are:
	     *  * 477 bits for 23.85 kbps, 
	     *  * 461 bits for 23.05 kbps, 
	     *  * 397 bits for 19.85 kbps, 
	     *  * 365 bits for 18.25 kbps,
	     *  * 317 bits for 15.85 kbps,
	     *  * 285 bits for 14.25 kbps,
	     *  * 253 bits for 12.65 kbps,
	     *  * 177 bits for 8.85 kbps rates.
	     *  * 132 bits for 6.60 kbps rates.
	     * See also ETSI TS 126 190, V5.1.0 (2001-12): "AMR Wideband
	     * speech codec; Transcoding functions (3GPP TS 26.190 version
	     * 5.1.0 Release 5) Document for rate dependent parameter
	     * listing: Table1 (page 16) Tables 12a - 12i (pp. 43 - 48).
	     */

   uint8 dtx_mode;
   	    /*
	     * INPUT: binary DTX (discontinuous transmission) mode flag
	     *
	     *   0 : disable DTX
	     *   1 : enable DTX
	     */ 
	     
   int16 mode; 
             /*
	      * INPUT: Requested mode (bit-rate) in the range 0 through 8, 
	      * corresponding to rates 6.60 kbits/sec through 23.85 kbits/sec
	      * where 6.60 maps to mode=0, and 23.85 maps to mode=8:
	      * enum Mode {
	      *    MR660 = 0,
	      *    MR885,
	      *    MR1265,
	      *    MR1425,
	      *    MR1585,
	      *    MR1825,
	      *    MR1985,
	      *    MR2305,
	      *    MR2385,
	      *    MRDTX    (unused in encoder)
	      * }
	      */ 

   int16 usedMode;
             /*
	      * OUTPUT: Actual mode used to encode the frame in the 
	      * range 0-9 corresponding to the requested mode or 
	      * 9 corresponding to DTX  (Discontinuous Transmission) mode.
	      * In the latter mode of operation a SID frame is encoded.
	      */
   
   int16 tx_type;
             /*
	      * OUTPUT: Type of frame transmitted, according to:
              * enum TXFrameType {
	      *    TX_SPEECH_GOOD = 0,
              *    TX_SID_FIRST,
              *    TX_SID_UPDATE,
              *    TX_NO_DATA,
	      *    TX_SPEECH_DEGRADED,
	      *    TX_SPEECH_BAD,
	      *    TX_SID_BAD,
	      *    TX_ONSET
	      * }
	      */

   uint8 nsync_mode;
             /*
	      * INPUT: binary NSYNC mode flag
	      *
	      *   0 : disable NSYNC mode
	      *   1 : enable NSYNC mode
	      *
	      * At handover, TX/RX DTX handler synchronization may be
	      * initiated by enabling nsync_mode for the duration of the
	      * 12 NSYNC frames.
	      *
	      * During the NSYNC frames immediately following handover, the
	      * encoder will always follow the end of a speech burst with a
	      * hangover period.  This hangover will last for the duration of
	      * the NSYNC frames, or for the normal number (7) of hangover
	      * frames, whichever is greater.  Also, the encoder will generate
	      * TX_SID_UPDATE frames in place of all TX_NO_DATA frames.
	      *
	      */

             /*
	      * OUTPUT: binary VAD (voice activity detection) flag
	      *
	      *   0 : no voice activity detected
	      *   1 : voice activity detected
	      *
	      * Note that the vad output is undefined if dtx_mode input is 0.
	      */

   void ( *vad_callback ) ( uint8 vad, int16 usedMode );
             /*
	      * INPUT: function pointer used to report VAD flag early
	      *
	      *   NULL    : don't report vad early
	      *   address : report VAD and usedMode early
	      *
	      * If early knowledge of the VAD flag is required (before
	      * the encoder has completed,) the caller may provide a pointer
	      * to a function to report it early.  When the computations of
	      * vad and usedMode are complete, the encoder will call the
	      * provided function with vad and usedMode as parameters.
	      *
	      * Note that the vad_callback function will only be executed
	      * on frames where dtx_mode is enabled.
	      */	      
 };

typedef struct WBAMR_Static_Mem_Enc_Struct *WBAMR_Handle_Enc;


/*************************************************************************
 * Entry point to the WBAMR Encoder
 *
 */

extern int WBAMR_AIL_Enc
( 
   /*
    * RETURNS:
    *   SC_RETURN_SUCCESS      : successful
    *   SC_RETURN_MALLOC_ERROR : memory allocation error
    */

   struct WBAMR_IO_Enc_Struct *Ptr0, 
	     /*
	      * INPUT/OUTPUT: Pointer to encoder IO structure.
              */

   void *Ptr1
	     /*
	      * INPUT/OUTPUT: Pointer to encoder state 
	      * variable structure.  To obtain low encoder
	      * cycle counts, this buffer must be located in
	      * an alternate memory bank from temp_0 to allow
	      * dual memory access.  See above for size
	      * and alignment requirements.
	      */
);                   
 
             
/*************************************************************************
 * Initialization for the WBAMR encoder
 *
 */

extern void *WBAMR_AIL_Static_Mem_Enc_Allocate
(
   /*
    * RETURNS: Pointer to encoder state variable structure
    */

   void *Ptr
             /*
	      * INPUT/OUTPUT: Pointer to encoder state variable structure.
	      * If not NULL, initialize the state variables and return.
	      * See above for size and aligment requirements.
	      *
	      * INPUT: Pointer to encoder state variable structure.  If
	      * the parameter is NULL, allocate the memory, initialize
	      * the state variables, and return.  Note that this capability
	      * is dependent on the code having been built to support
	      * dynamic memory allocation.
	      */
);

extern void WBAMR_AIL_Static_Mem_Enc_DeAllocate
(
   void *Ptr
             /*
	      * INPUT: Pointer to encoder state variable structure.
	      * Function deallocates the memory if the code was built to
	      * support dynamic memory allocation.
	      */
);


/*************************************************************************
 * Interface to 3GPP WBAMR Decoder                                       *
 *************************************************************************/


/*************************************************************************
 * Decoder memory summary
 *
 *    "state"  - 1532 bytes of 32-bit aligned memory
 *    "temp_0" - 3416 bytes of 32-bit aligned memory
 *
 *    Current as of the date of PVCS revision : Rev 1.19
 *    Most recent PVCS revision               : $Revision: #2 $
 *
 *    Please note, if the two revision numbers above do not match,
 *    then the memory numbers may be out of date.
 */

#define WBAMR_DEC_STATIC_SIZE  383 /*size in fract32 of Dec state */
#if defined(AWB_OPT_VER) && (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
#define WBAMR_DEC_STACK_SIZE_0 879 /*size in fract32 of Dec primary scratch*/
#else
#define WBAMR_DEC_STACK_SIZE_0 854 /*size in fract32 of Dec primary scratch*/
#endif
#define WBAMR_DEC_STACK_SIZE_1  32 /*size in fract32 of Dec secondary scratch*/


/*************************************************************************
 * The following structures define the Input/Output parameters
 * to the WBAMR decoder.
 *
 */

struct WBAMR_IO_Dec_Struct
{
   int32 mode; // move to the front, and change to int32
   void *temp_0;
	     /*
	      * NO INPUT/NO OUTPUT: Pointer to primary temporary scratch
	      * area.  See above for size and alignment requirements.
              */

   void *temp_1;
            /* 
	     * NO INPUT/NO OUTPUT: Pointer to auxiliary temporary 
	     * scratch area.  See above for size and alignment
	     * requirements.
	     */
   
   uint16 *parms;     
             /*
	      * INPUT: Encoded input bits, reordered according to 3GPP
	      * TS 26.201(speech codec performs the reordering!) and packed
	      * into unsigned 16-bit words.  The bits are packed 16 bits
	      * per word, and the remaining bits are packed in the last
	      * word, right justified.  The number of encoded output
	      * bits are rate dependent.
	      */

   fract16 *speech;   
	     /*
	      * OUTPUT: Synthesized speech, 320 PCM speech output  samples.
	      */
   
	int16 prev_mode;
             /*
	      * INPUT: Received mode (bit-rate) in the range 0 through 8, 
	      * corresponding to rates 6.60 kbits/sec through 23.85 kbits/sec
	      * where 6.60 maps to mode=0, and 23.85 maps to mode=8. 
	      * 9 corresponds to DTX (Discontinuous Transmission) mode:
	      * enum Mode {
	      *    MR660 = 0,
	      *    MR885,
	      *    MR1265,
	      *    MR1425,
	      *    MR1585,
	      *    MR1825,
	      *    MR1985,
	      *    MR2305,
	      *    MR2385,
	      *    MRDTX    (unused in encoder)
	      * }
	      */ 
    
   int16 rx_type;
             /*
	      * INPUT: Type of frame received, according to:
              * enum RXFrameType {
	      *    RX_SPEECH_GOOD = 0,
              *    RX_SPEECH_PROBABLY_DEGRADED,
              *    RX_SPEECH_LOST,
              *    RX_SPEECH_BAD,
              *    RX_SID_FIRST,
              *    RX_SID_UPDATE,
              *    RX_SID_BAD,
              *    RX_NO_DATA
	      * }
	      */
}; 

typedef struct WBAMR_Static_Mem_Dec_Struct *WBAMR_Handle_Dec;
     

/*************************************************************************
 * Entry point to the WBAMR Decoder
 *
 */

extern int WBAMR_AIL_Dec 
( 
   /*
    * RETURNS:
    *   SC_RETURN_SUCCESS      : successful
    *   SC_RETURN_MALLOC_ERROR : memory allocation error
    */

   struct WBAMR_IO_Dec_Struct *Ptr0, 
	     /*
	      * INPUT/OUTPUT: Pointer to decoder IO structure.
	      */

   void *Ptr1
             /*
	      * INPUT/OUTPUT: Pointer to decoder state 
	      * variable structure.  See above for size and
	      * alignment requirements.
	      */
);
        	   
        	   
/*************************************************************************
 * Initialization for the WBAMR decoder
 *
 */

extern void *WBAMR_AIL_Static_Mem_Dec_Allocate
(
   /*
    * RETURNS: Pointer to decoder state variable structure.
    */

   void *Ptr
             /*
	      * INPUT/OUTPUT: Pointer to decoder state variable structure.
	      * If not NULL, initialize the state variables and return.
	      * See above for size and alignment requirements.
	      *
	      * INPUT: Pointer to encoder state variable structure.  If
	      * the parameter is NULL, allocate the memory, initialize
	      * the state variables, and return.  Note that this capability
	      * is dependent on the code having been built to support
	      * dynamic memory allocation.
	      */
);

extern void WBAMR_AIL_Static_Mem_Dec_DeAllocate
(
   void *Ptr
             /*
	      * INPUT: Pointer to decoder state variable structure.
	      * Function deallocates the memory if the code was built to
	      * support dynamic memory allocation.
	      */
);


#endif    /* WBAMR_LIB_H */
