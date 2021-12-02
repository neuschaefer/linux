/*##############################################################################
##									 VPS.H									  ##
##                     ---------------------------------                      ##
##					VPS (Video Programming System) Insertion				  ##
##                                                                            ##
##                   Copyright ¨Ï 2004. LG Electronics Inc.                   ##
##						 Edited by David (Dong-Il) Lee						  ##
##							 E-mail : dilee@lge.com							  ##
##                                                                            ##
##############################################################################*/
/**
 * @file	vps.h
 * @brief	VPS (Video Programming System) Insertion
 * This file defines the LG VBI VPS API.
 */

#ifndef _vps_h_
#define _vps_h_
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @enum LGVBI_PIL_SVC_type
 *
 * It specifies the service code which defines several reserved values for
 * receiver control in certain conditions.
 *
 * <table>
 * <tr><th><b>  Enumerations        </b> <th><b> Values </b>						<th><b> Meaning </b>
 * <tr><td><tt> LGVBI_PIL_SVC_TC   </tt> <td> 0x07fff: 0000 0111 1111 1111 1111 (2)	<td> switch to timer-control
 * <tr><td><tt> LGVBI_PIL_SVC_RIT  </tt> <td> 0x07fbf: 0000 0111 1111 1011 1111 (2)	<td> terminate recording
 * <tr><td><tt> LGVBI_PIL_SVC_INT  </tt> <td> 0x07f7f: 0000 0111 1111 0111 1111 (2)	<td> interrupt recording
 * <tr><td><tt> LGVBI_PIL_SVC_CONT </tt> <td> 0x07f3f: 0000 0111 1111 0011 1111 (2)	<td> continue recording
 * <tr><td><tt> LGVBI_PIL_SVC_NSPV </tt> <td> 0xfffff: 1111 1111 1111 1111 1111 (2)	<td> refer PTY
 * </table>
 */
typedef enum _LGVBI_PIL_SVC_type {
	LGVBI_PIL_SVC_TC	= 0x00007fff, // 0000 0111 1111 1111 1111 (2)
	LGVBI_PIL_SVC_RIT	= 0x00007fbf, // 0000 0111 1111 1011 1111 (2)
	LGVBI_PIL_SVC_INT	= 0x00007f7f, // 0000 0111 1111 0111 1111 (2)
	LGVBI_PIL_SVC_CONT	= 0x00007f3f, // 0000 0111 1111 0011 1111 (2)
	LGVBI_PIL_SVC_NSPV	= 0x000fffff  // 1111 1111 1111 1111 1111 (2)
} LGVBI_PIL_SVC_type;

/* the order of following definitions are the order of priority */
#define	MSK_PIL_EMPTY	0x01000000
#define	MSK_PIL_ERR		0x07000000
#define	MSK_PIL_INVALID	0x80000000
#define	MSK_PIL_SVC		0x00070000
#define	MSK_PIL_WARN	0x0000000f

/**
 * @def LGVBI_INVALID_PIL
 *
 * LGVBI_INVALID_PIL means that the value of PDC_descriptor is invalid currently
 * because proper EIT or TOT has not been received by the receiver in the case
 * of booting time or channel changing.
 *
 * Although the PIL value in PDC_descriptor is not valid in receiver's point of
 * view, any value must be sent to VCR. If receiver send TC (Timer-control)
 * service code in this case, the delayed event can not be recorded at proper
 * time because VCR records something using its own free-running clock and then
 * the VCR does not stop until the end time passed or the PIL to be recorded
 * delivered. This abnormal state can easily be made by booting up TV or
 * changing channel, so receiver should treat the VPS output with consider.
 *
 * LGE concluded that iDTV shall generate INT (Interruption) code in the case
 * of invalid PIL value to avoid claims from inappropriate recording by
 * consumers.
 * (2005.05.10, by dilee, freestar, jskim)
 */
#define	LGVBI_INVALID_PIL	(MSK_PIL_INVALID | LGVBI_PIL_SVC_INT) // 0x80007f7f
/**
 * @def LGVBI_NO_PIL
 *
 * LGVBI_NO_PIL means that the PDC_descriptor field is not encoded in EIT.
 * This problem is caused by broadcaster. Receiver is not responsible for
 * the correct value of VPS in this case, but it is need to avoid confusion of
 * user from no result after one's effort for recording an event.
 *
 * LGE concluded that iDTV shall generate TC (Timer-control) code when there is
 * no PDC_descriptor encoded in EIT so that consumers can record some events
 * using recording timers of VCR and LGE iDTV when consumers turn VPS field on
 * in recording timers and there is no PDC_descriptor in the channel.
 * (2005.05.10, by dilee, freestar, jskim)
 */
#define	LGVBI_NO_PIL		(MSK_PIL_EMPTY   | LGVBI_PIL_SVC_TC)  // 0x01007fff
/*
 * VPS inteface data format / PIL data format
 * |0|1|2|3|4|5|6|7| |0|1|2|3|4|5|6|7| |0|1|2|3|4|5|6|7| |0|1|2|3|4|5|6|7|
 * |1|0 0 0 0 0 0 0|  0 0 0 0|0 0 0 0   0 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1|
 * --> LGVBI_INVALID_PIL (0x80007fff): invalid + TC (Timer-control code)
 * |i|  error flag |  x x x x|   day:5   |month:4|   hour:5  |  minute:6 |
 * --> LGE (i: invalid flag)
 * |x x x x x x x x   x x|  day:5  | month:4 |  hour:5 |    minute:6 |x x|
 * --> STM/VPS
 */



unsigned int LGVBI_vps_get(void);
unsigned int LGVBI_vps_test(unsigned int vps_data);
unsigned int LGVBI_vps_put(unsigned int vps_data);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif	/* !_vps_h_ */
