/*
 * based upon glibc-2.2.5 time/tzset.c
 *
 * Copyright 2003,2008 Sony Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#ifndef __FAT_TZ_H__
#define __FAT_TZ_H__

#define _TZNAME_MAX 6

struct tz_part {
	char name[_TZNAME_MAX+1];
	long sec_offset;
	unsigned short m;	/* Month  1 <= m <= 12 */
	unsigned short n;	/* week   1 <= n <= 5  */
	unsigned short d;	/* day    0 <= d <= 6  */
	long dst_sec_offset;
	time_t change;
	int computed_for;
};

struct tz_rule {
	struct tz_part p1;
	struct tz_part p2;
};
#endif /* !__FAT_TZ_H__ */
