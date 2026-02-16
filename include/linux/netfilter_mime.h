/* 
 * 
 *
 * Copyright (C) 2013, Intel Ltd.
 * All Rights Reserved.
 ** Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *
   You should have received a copy of the GNU General Public License 
   along with this program; if not, write to the Free Software 
   Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
   The full GNU General Public License is included in this distribution 
   in the file called LICENSE.GPL.

   Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052

/*
 * MIME functions for netfilter modules.  This file provides implementations
 * for basic MIME parsing.  MIME headers are used in many protocols, such as
 * HTTP, RTSP, SIP, etc.
 *
 * gcc will warn for defined but unused functions, so we only include the
 * functions requested.  The following macros are used:
 *   NF_NEED_MIME_NEXTLINE      nf_mime_nextline()
 */
#ifndef _NETFILTER_MIME_H
#define _NETFILTER_MIME_H

/* Only include these functions for kernel code. */
#ifdef __KERNEL__

#include <linux/ctype.h>

/*
 * Given a buffer and length, advance to the next line and mark the current
 * line.  If the current line is empty, *plinelen will be set to zero.  If
 * not, it will be set to the actual line length (including CRLF).
 *
 * 'line' in this context means logical line (includes LWS continuations).
 * Returns 1 on success, 0 on failure.
 */
#ifdef NF_NEED_MIME_NEXTLINE
static int
nf_mime_nextline(char* p, uint len, uint* poff, uint* plineoff, uint* plinelen)
{
    uint    off = *poff;
    uint    physlen = 0;
    int     is_first_line = 1;

    if (off >= len)
    {
        return 0;
    }

    do
    {
        while (p[off] != '\n')
        {
            if (len-off <= 1)
            {
                return 0;
            }

            physlen++;
            off++;
        }

        /* if we saw a crlf, physlen needs adjusted */
        if (physlen > 0 && p[off] == '\n' && p[off-1] == '\r')
        {
            physlen--;
        }

        /* advance past the newline */
        off++;

        /* check for an empty line */
        if (physlen == 0)
        {
            break;
        }

        /* check for colon on the first physical line */
        if (is_first_line)
        {
            is_first_line = 0;
            if (memchr(p+(*poff), ':', physlen) == NULL)
            {
                return 0;
            }
        }
    }
    while (p[off] == ' ' || p[off] == '\t');

    *plineoff = *poff;
    *plinelen = (physlen == 0) ? 0 : (off - *poff);
    *poff = off;

    return 1;
}
#endif /* NF_NEED_MIME_NEXTLINE */

#endif /* __KERNEL__ */

#endif /* _NETFILTER_MIME_H */

