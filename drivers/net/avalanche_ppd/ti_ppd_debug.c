/*
 * ti_ppd_debug.c
 *  Description:
 *  Debug facility for Packet processor Driver.
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
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


#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/generic/ti_ppd.h>
#include "ppd_pvt.h"
#include "ppd_os.h"

#if defined PPD_ENABLE_LUT_DUMP
Int32 ppd_dump_eth_lut (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p, WSTR_W0(0, *ptr, "Dst LS"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "Src LS"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(2, *(ptr+2), "Src MS", "Dst MS"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(3, *(ptr+3), "VLAN VID", "Eth Type"));
    len += sprintf(dump_buff_p + len,
            WSTR_B3_0(4, *(ptr+4), "DSID/TAG", "VLAN Pri", "In PID","Pkt Key"));
    len += sprintf(dump_buff_p + len, WSTR_W0(5, *(ptr+5), "Enables"));
    return len;
}

Int32 ppd_dump_ipv4_lut (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p, WSTR_W0(0, *ptr, "Dst IP"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "Src IP"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(2, *(ptr+2), "Src Port", "Dst Port"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(3, *(ptr+3), "short1", "PPPoE Id"));
    len += sprintf(dump_buff_p + len,
            WSTR_B3_0(4, *(ptr+4), "TOS", "Protocol", "Frag", "Pkt Key"));
    len += sprintf(dump_buff_p + len, WSTR_W0(5, *(ptr+5), "Enables"));
    return len;
}

Int32 ppd_dump_ipv6_lut (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p, WSTR_W0(0, *ptr, "Dst IP"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "Src IP"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(2, *(ptr+2), "Src Port", "Dst Port"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(3, *(ptr+3), "Flwlbl LS", "PPPoE Id"));
    len += sprintf(dump_buff_p + len,
            WSTR_B3_0(4, *(ptr+4), "Trafic Cl", "Proto", "Flbl MS", "Pkt Key"));
    len += sprintf(dump_buff_p + len, WSTR_W0(5, *(ptr+5), "Enables"));
    return len;
}
#endif /* PPD_ENABLE_LUT_DUMP */

static Int32 ppd_dump_ipv6_rec (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len; len = sprintf(dump_buff_p, WSTR_W0(0, *ptr, "Src IP W3"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "Src IP W2"));
    len += sprintf(dump_buff_p + len, WSTR_W0(2, *(ptr+2), "Src IP W1"));
    len += sprintf(dump_buff_p + len, WSTR_W0(3, *(ptr+3), "Src IP W0"));
    len += sprintf(dump_buff_p + len, WSTR_W0(4, *(ptr+4), "Dst IP W3"));
    len += sprintf(dump_buff_p + len, WSTR_W0(5, *(ptr+5), "Dst IP W2"));
    len += sprintf(dump_buff_p + len, WSTR_W0(6, *(ptr+6), "Dst IP W1"));
    len += sprintf(dump_buff_p + len, WSTR_W0(7, *(ptr+7), "Dst IP W0"));
    return len;
}

static Int32 ppd_dump_l2_hdr (Uint32* ptr, Uint8* dump_buff_p, Int32 cnt)
{
    Int32 len = 0, i;

    for (i = 0; i < cnt/4; i++) {
        if (i%4 == 0) {
            if (i) len += sprintf(dump_buff_p + len, "\n");
            len += sprintf(dump_buff_p + len, "%08x: ", (unsigned int)&ptr[i]);
        }

        len += sprintf(dump_buff_p + len, "%08x ", ptr[i]);
    }

    for (i = i*4; i < cnt; i++) {
        if (i%16 == 0) {
            if (i) len += sprintf(dump_buff_p + len, "\n");
            len += sprintf(dump_buff_p + len, "%08x: ", (unsigned int)&ptr[i]);
        }

        len += sprintf(dump_buff_p + len, "%02x", *((Uint8*)ptr + i));
    }

    len += sprintf(dump_buff_p + len, "\n");
    return len;
}

static Int32 ppd_dump_ses_rec (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p,
            WSTR_B1_0_S0(0, *ptr, "In VPID", "Status", "Flags"));
    len += sprintf(dump_buff_p + len,
            WSTR_W0(1, *(ptr+1), "IPv6 Ptr"));
    len += sprintf(dump_buff_p + len,
            WSTR_S0_B1_0(2, *(ptr+2), "Reserved", "Next Col", "Prev Col"));
    len += sprintf(dump_buff_p + len, WSTR_W0(3, *(ptr+3), "EFR Ptr"));
    len += sprintf(dump_buff_p + len, WSTR_W0(4, *(ptr+4), "Timeout"));
    len += sprintf(dump_buff_p + len, WSTR_W0(5, *(ptr+5), "Ref Time"));
    return len;
}

static Int32 ppd_dump_ef_rec (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p, WSTR_W0(0, *ptr, "Next EFR"));
    len += sprintf(dump_buff_p + len,
            WSTR_B3_0(1, *(ptr+1), "Priority", "Out VPID","Framing","L2 Size"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(2, *(ptr+2), "Dst Tag", "Fwd Q"));
    len += sprintf(dump_buff_p + len,
            WSTR_S0_B1_0(3, *(ptr+3), "Reserved", "Egr Dev", "8023 Off"));
    len += sprintf(dump_buff_p + len, WSTR_W0(4, *(ptr+4), "TDox Ack"));
    len += sprintf(dump_buff_p + len, WSTR_W0(5, *(ptr+5), "L2 Ptr"));
    len += sprintf(dump_buff_p + len, WSTR_W0(6, *(ptr+6), "Mod Ptr"));
    len += sprintf(dump_buff_p + len, WSTR_W0(7, *(ptr+7), "ProtSpec"));
    return len;
}

static Int32 ppd_dump_mod_rec (Uint32* ptr, Uint8* dump_buff_p, Int32 buff_len)
{
    Int32 len;
    len = sprintf(dump_buff_p, WSTR_B1_0_S0(0, *ptr, "byte1", "TOS", "Flags"));
    len += sprintf(dump_buff_p + len, WSTR_W0(1, *(ptr+1), "IP Src"));
    len += sprintf(dump_buff_p + len, WSTR_W0(2, *(ptr+2), "IP Dst"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(3, *(ptr+3), "Src Port", "Dst Port"));
    len += sprintf(dump_buff_p + len,
            WSTR_S1_0(4, *(ptr+4), "L3 Chk", "L2 Chk"));
    return len;
}

Int32 ti_ppd_get_session_dump (char *buf, char **start, Uint32 offset,
                                int count, int *peof, void *data)
{
    Uint32 len = 0, cookie, blk_num;
    int egr_cnt = 0, l2_hdr_cnt = 0, mod_cnt = 0;
    ppd_egress_rec_t *egress, *next_egress;
    Uint32 mod_ptr = 0, l2_hdr_ptr;
    Uint8 session_handle = *(Uint8*)data;
    ppd_ses_info_blk_t *ses     = (ppd_ses_info_blk_t*)(gp_ses_blk
                                    + (TI_PP_ses_id_t)session_handle);

    if ((PPD_DUMP_BUFF_BLK_LEN > count) || !buf)
        return -1;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);
    if (!IS_SESSION_SLOT_FILLED(session_handle)) {
        PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);
        return -1;
    }
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    /* Get the block number to dump */
    blk_num = offset / PPD_DUMP_BUFF_BLK_LEN;

    egress = ses->w3_egress_ptr
                ? (ppd_egress_rec_t*) ppd_os_get_io_virt(ses->w3_egress_ptr)
                : 0;

    /* We will provide the session dump as a series of 'blocks'. Set 'start' as
     * block size so that subsequent call will move to the next block (if
     * applicable).
     */
    *(Uint32*)start = PPD_DUMP_BUFF_BLK_LEN;

    /* This logic won't work for the case when PPD_DUMP_BUFF_BLK_LEN > buf.
     * Return error in such case.
     */
    if (*start >= buf) return -1;

    /* There will be atleast one egress record present in a valid session.
     * So we are not checking for setting eof in block 0 (and 1, if LUT dump
     * enabled).
     */

    if (blk_num == 0) {
        len = snprintf (buf, count,
                "Session record dump for session id %d:\n",
                (Int32) session_handle);
        len += ppd_dump_ses_rec ((Uint32 *)ses, (buf+len), (count-len));
        return len;
    }

    /* Note that blk_num is decremented for Ipv6 address record and/or LUT dump
     * cases as applicable. This will ease out the calculations in egress loop.
     * This avoids use a #ifdefs or ifs and  extra variable(s). This is ok as
     * blk_num is not used for any other purpose.
     */

    if (ses->w1_ipv6_rec)
    {
        if (blk_num == 1)
        {
            len = snprintf (buf, count, "\nIPv6 Address record dump:\n");
            len += ppd_dump_ipv6_rec ((Uint32 *)ppd_os_get_io_virt(ses->w1_ipv6_rec),
                                        (buf+len), (count-len));
            return len;
        }
        else
            blk_num--;
    }

#ifdef PPD_ENABLE_LUT_DUMP
    if (blk_num == 1) { //TODO LUT dumps
        Uint32* lut_l2_base = &g_lut_dump_buff[session_handle][0];
        Uint32* lut_l3_base =
            &g_lut_dump_buff[session_handle][(PPD_LUT_DUMP_WORDS)/2];

        if (LUT_DATA_TYPE(lut_l2_base) == TI_PP_LUT_DATA_L2_ETH)
        {
            len += snprintf (buf, count, "\nL2 (Ethernet) LUT dump:\n");
            len += ppd_dump_eth_lut (lut_l2_base, (buf+len), (count-len));
        }
        else
            len = snprintf (buf, count, "\nUnsupported type(%d) for L2 LUT\n",
                            LUT_DATA_TYPE(lut_l2_base));

        if (LUT_DATA_TYPE(lut_l3_base) == TI_PP_LUT_DATA_L3_IPV4)
        {
            len += snprintf ((buf+len), count, "\nL3 (IPv4) LUT dump:\n");
            len += ppd_dump_ipv4_lut (lut_l3_base, (buf+len), (count-len));
        }
        else if (LUT_DATA_TYPE(lut_l3_base) == TI_PP_LUT_DATA_L3_IPV6)
        {
            len += snprintf ((buf+len), count, "\nL3 (IPv6) LUT dump:\n");
            len += ppd_dump_ipv6_lut (lut_l3_base, (buf+len), (count-len));
        }
        else
            len += snprintf ((buf+len), count,
                    "\nUnsupported type(%d) for L3 LUT\n",
                    LUT_DATA_TYPE(lut_l3_base));
    }

    blk_num--;
#endif

    while (egress) {
        next_egress = egress->w0_next_egr_ptr
                        ? (ppd_egress_rec_t*) ppd_os_get_io_virt(egress->w0_next_egr_ptr)
                        : 0;
        l2_hdr_ptr  = egress->w5_l2_hdr_ptr;
        mod_ptr     = egress->w6_mod_rec_ptr;
        egr_cnt++;
        if (blk_num == (egr_cnt + l2_hdr_cnt + mod_cnt)) {
            len += snprintf ((buf+len), (count-len),
                    "\nEgress record(%d) dump:\n", egr_cnt);
            len += ppd_dump_ef_rec ((Uint32 *)egress,
                    (buf+len), (count-len));
            if (!next_egress && !l2_hdr_ptr && !mod_ptr)
                *peof = 1;
            return len;
        }
        if (l2_hdr_ptr) {
            l2_hdr_cnt++;
            if (blk_num == (egr_cnt + l2_hdr_cnt + mod_cnt)) {
                len += snprintf ((buf+len), (count-len), "\nL2 Header dump:\n");
                len += ppd_dump_l2_hdr ((Uint32 *)PAL_osMemPhy2Virt(l2_hdr_ptr),
                                        (buf+len), egress->w1.frags & 0xff);
                if (!next_egress && !mod_ptr)
                    *peof = 1;
                return len;
            }
        }
        if (mod_ptr) {
            mod_cnt++;
            if (blk_num == (egr_cnt + l2_hdr_cnt + mod_cnt)) {
                len += snprintf ((buf+len), (count-len),
                        "\nMod record(%d) dump:\n", egr_cnt);
                len += ppd_dump_mod_rec ((Uint32 *)ppd_os_get_io_virt(mod_ptr),
                        (buf+len), (count-len));
                if (!next_egress)
                    *peof = 1;
                return len;
            }
        }
        egress = next_egress;
    }

    return len;
}

