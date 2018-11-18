/*
 * linux/fs/fat/tz.c
 * based upon glibc-2.2.5 time/tzset.c
 *
 * Copyright 2003,2008,2009 Sony Corporation.
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
 */

#ifndef TEST_TZ
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/errno.h>
#include <linux/ctype.h>
#include "fat_tz.h"
#endif

#undef TZ_DEBUG
#ifdef TZ_DEBUG
#define tz_printk(x...)	printk(x)
#else
#define tz_printk(x...)	do{}while(0)
#endif

#define TZ_DEFAULT "GMT+00"
#define DST_SEP_CHAR ';'
/* #define DST_SEP_CHAR ',' */


struct dos_tm {
	int dt_year;	/* year     1980 <= dt_year <= 2038 */
	int dt_mon;	/* month       1 <= dt_mon  <=   12 */
	int dt_day;	/* day         1 <= dt_day  <=   31 */
	int dt_hour;	/* hour        0 <= dt_hour <=   23 */
	int dt_min;	/* minute      0 <= dt_min  <=   59 */
	int dt_sec;	/* second      0 <= dt_sec  <=   59 */
	int dt_wday;	/* week day    0 <= dt_wday <=    6 */
};

static void init_tz_part_date(struct tz_part *tp, unsigned short m,
			     unsigned short n, unsigned short d,
			     long dst_sec_offset)
{
	tp->m = m;
	tp->n = n;
	tp->d = d;
	tp->dst_sec_offset = dst_sec_offset;
}

#ifdef TZ_DEBUG
static int init_tz_part_ff(struct tz_part *tp)
{
	tp->name[0] = '-';
	tp->name[1] = '\0';
	tp->sec_offset = -1;
	tp->m = -1;
	tp->n = -1;
	tp->d = -1;
	tp->dst_sec_offset = -1;
	tp->change = -1;
	tp->computed_for = -1;
}

static int init_rule_ff(struct tz_rule *r)
{
	init_tz_part_ff(&r->p1);
	init_tz_part_ff(&r->p2);
}
#endif

static int parse_tzname(char *buf, char *name, int maxlen)
{
	int quote, p, n;

	tz_printk("parse_tzname: buf=\"%s\" maxlen=%d\n", buf, maxlen);
	quote = p = n = 0;

	if( buf[p] == '<' ) {
		quote = 1;
		p++;
	}

	while( buf[p] != '\0' && (quote || (!isdigit(buf[p]) &&
					    buf[p] != '-' &&
					    buf[p] != '+')) ) {
		if( quote ) {
			if( buf[p] == '>' ) {
				p++;
				quote = 0;
				break;
			}
		} else {
			if( buf[p] == '<' || buf[p] == '>' )
				return -EINVAL;
		}
		if( n == maxlen ) {
			p++;
			continue;
		}
		name[n++] = buf[p++];
	}
	if( quote )
		return -EINVAL;
	name[n] = '\0';
	return p;
}

static int parse_num(char *buf, long *num)
{
	int p, val;

	tz_printk("parse_num: buf=\"%s\"\n", buf);
	p = 0;
	val = 0;
	while( isdigit(buf[p]) ) {
		val = val * 10 + buf[p] - '0';
		p++;
	}
	if( p == 0 )
		return -EINVAL;
	*num = val;
	return p;
}

static int parse_tzoffset(char *buf, long *offset, int allow_leadsign)
{
	int sign, p, rval;
	long oa, num;

	tz_printk("parse_tzoffset: buf=\"%s\" allow_leadsign=%d\n", buf, allow_leadsign);
	p = 0;
	sign = 1;

	if( allow_leadsign ){
		if( buf[p] == '+' )
			p++;
		if( buf[p] == '-' ) {
			sign = -1;
			p++;
		}
	}

	rval = parse_num(buf+p, &num);
	if( rval <= 0 || rval > 2 )
		return -EINVAL;
	p += rval;
	oa = num * 60 * 60;

	if( buf[p] != ':' ) {
		*offset = oa * sign;
		return p;
	}
	p++;

	rval = parse_num(buf+p, &num);
	if( rval <= 0 || rval > 2 )
		return -EINVAL;
	p += rval;
	oa += num * 60;

	if( buf[p] != ':' ) {
		*offset = oa * sign;
		return p;
	}
	p++;

	rval = parse_num(buf+p, &num);
	if( rval <= 0 || rval > 2 )
		return -EINVAL;
	p += rval;
	oa += num;

	*offset = oa * sign;
	return p;
}

static int parse_dstdate(char *buf, struct tz_part *tp)
{
	int p, rval;
	long num;

	tz_printk("parse_dstdate: buf=\"%s\"\n", buf);
	p = 0;

	if( buf[p++] != 'M' )
		return -EINVAL;

	rval = parse_num(buf+p, &num);
	if( rval < 0 )
		return rval;
	p += rval;
	if( num < 1 || num > 12 )
		return -EINVAL;
	tp->m = num;

	if( buf[p++] != '.' )
		return -EINVAL;

	rval = parse_num(buf+p, &num);
	if( rval < 0 )
		return rval;
	p += rval;
	if( num < 1 || num > 5 )
		return -EINVAL;
	tp->n = num;

	if( buf[p++] != '.' )
		return -EINVAL;

	rval = parse_num(buf+p, &num);
	if( rval < 0 )
		return rval;
	p += rval;
	if( num < 0 || num > 6 )
		return -EINVAL;
	tp->d = num;

	if( buf[p] != '/' ) {
		tp->dst_sec_offset = 2 * 60 * 60;
		return p;
	}
	p++;

	rval = parse_tzoffset(buf+p, &num, 0);
	if( rval < 0 )
		return rval;
	p += rval;

	tp->dst_sec_offset = num;
	return p;
}

static int parse_tz(char *tz, struct tz_rule *rule)
{
	int rval;

	tz_printk("parse_tz: tz=\"%s\"\n", tz);

#ifdef TZ_DEBUG
	init_rule_ff(rule);
#endif
	rule->p1.computed_for = rule->p2.computed_for = -1;
	rule->p1.change = rule->p2.change = 0;

	if( tz == NULL )
		tz = TZ_DEFAULT;
	if( *tz == ':' )
		tz++;

	rval = parse_tzname(tz, rule->p1.name, _TZNAME_MAX);
	if( rval < 0 )
		return rval;
	tz += rval;

	rval = parse_tzoffset(tz, &rule->p1.sec_offset, 1);
	if( rval < 0 )
		return rval;
	tz += rval;

	rule->p1.sec_offset *= -1;

	if( *tz == '\0' ) {
		rule->p2.sec_offset = rule->p1.sec_offset;
		rule->p2.name[0] = '\0';
		init_tz_part_date(&rule->p1,4,1,0,2*60*60);
		init_tz_part_date(&rule->p2,10,5,0,2*60*60);
		return 0;
	}

	rval = parse_tzname(tz, rule->p2.name, _TZNAME_MAX);
	if( rval < 0 )
		return rval;
	tz += rval;

	rval = parse_tzoffset(tz, &rule->p2.sec_offset, 1);
	if( rval < 0 )
		return rval;
	tz += rval;

	rule->p2.sec_offset *= -1;

	if( *tz++ != DST_SEP_CHAR )
		return -EINVAL;

	rval = parse_dstdate(tz, &rule->p1);
	if( rval < 0 )
		return rval;
	tz += rval;

	if( *tz++ != DST_SEP_CHAR )
		return -EINVAL;

	rval = parse_dstdate(tz, &rule->p2);
	if( rval < 0 )
		return rval;
	tz += rval;

	if( *tz != '\0' )
		return -EINVAL;
	return 0;
}

/* compute_change()
   based on glibc-2.2.5:time/tzset.c:compute_change() */
#define SECSPERDAY	((long) 24 * 60 * 60)
#define SECSPERHOUR	((long) 60 * 60)
#ifndef __isleap
/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
# define __isleap(year)	\
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#endif
static const unsigned short int __mon_yday[2][13] = {
	/* Normal years.  */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years.  */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};
static int
compute_change(struct tz_part *tp, int year)
{
	time_t t;
	unsigned int i;
	int d, m1, yy0, yy1, yy2, dow;
	const unsigned short int *myday;

	if (year != -1 && tp->computed_for == year)
		return 1;

	myday =	&__mon_yday[__isleap(year)][tp->m];

	if (year > 1970)
		t = ((year - 1970) * 365
		     +
		     /* Compute the number of leapdays between 1970 and YEAR
			(exclusive).  There is a leapday every 4th year ...  */
		     + ((year - 1) / 4 - 1970 / 4)
		     /* ... except every 100th year ... */
		     - ((year - 1) / 100 - 1970 / 100)
		     /* ... but still every 400th year.  */
		     + ((year - 1) / 400 - 1970 / 400)) * SECSPERDAY;
	else
		t = 0;

	/* First add SECSPERDAY for each day in months before M.  */
	t += myday[-1] * SECSPERDAY;

	/* Use Zeller's Congruence to get day-of-week of first day of month. */
	m1 = (tp->m + 9) % 12 + 1;
	yy0 = (tp->m <= 2) ? (year - 1) : year;
	yy1 = yy0 / 100;
	yy2 = yy0 % 100;
	dow = ((26 * m1 - 2) / 10 + 1 + yy2 + yy2 / 4 + yy1 / 4 - 2 * yy1) % 7;
	if (dow < 0)
		dow += 7;

	/* DOW is the day-of-week of the first day of the month.  Get the
	   day-of-month (zero-origin) of the first DOW day of the month.  */
	d = tp->d - dow;
	if (d < 0)
		d += 7;
	for (i = 1; i < tp->n; ++i) {
		if (d + 7 >= (int) myday[0] - myday[-1])
			break;
		d += 7;
	}

	/* D is the day-of-month (zero-origin) of the day we want.  */
	t += d * SECSPERDAY;

	/* T is now the Epoch-relative time of 0:00:00 GMT on the day we want.
	   Just add the time of day and local offset from GMT, and we're done.  */

	tp->change = t - tp->sec_offset + tp->dst_sec_offset;
	tp->computed_for = year;
	return 1;
}

static int
get_offtime_year(time_t t, long int offset)
{
	long int days, rem, y;

	days = t / SECSPERDAY;
	rem = t % SECSPERDAY;
	rem += offset;
	while (rem < 0) {
		rem += SECSPERDAY;
		--days;
	}
	while (rem >= SECSPERDAY) {
		rem -= SECSPERDAY;
		++days;
	}

	y = 1970;

#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

	while (days < 0 || days >= (__isleap (y) ? 366 : 365)) {
		/* Guess a corrected year, assuming 365 days per year.  */
		long int yg = y + days / 365 - (days % 365 < 0);

		/* Adjust DAYS and Y to match the guessed year.  */
		days -= ((yg - y) * 365
			 + LEAPS_THRU_END_OF (yg - 1)
			 - LEAPS_THRU_END_OF (y - 1));
		y = yg;
	}
	return y;
}

static void
get_offtime(time_t t, long int offset, struct dos_tm *dt)
{
	long int days, rem, y;
	const unsigned short int *ip;

	days = t / SECSPERDAY;
	rem = t % SECSPERDAY;
	rem += offset;
	while (rem < 0) {
		rem += SECSPERDAY;
		--days;
	}
	while (rem >= SECSPERDAY) {
		rem -= SECSPERDAY;
		++days;
	}
	dt->dt_hour = rem / SECSPERHOUR;
	rem %= SECSPERHOUR;
	dt->dt_min = rem / 60;
	dt->dt_sec = rem % 60;
	/* January 1, 1970 was a Thursday.  */
	dt->dt_wday = (4 + days) % 7;
	if (dt->dt_wday < 0)
		dt->dt_wday += 7;

	y = 1970;

#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

	while (days < 0 || days >= (__isleap (y) ? 366 : 365)) {
		/* Guess a corrected year, assuming 365 days per year.  */
		long int yg = y + days / 365 - (days % 365 < 0);

		/* Adjust DAYS and Y to match the guessed year.  */
		days -= ((yg - y) * 365
			 + LEAPS_THRU_END_OF (yg - 1)
			 - LEAPS_THRU_END_OF (y - 1));
		y = yg;
	}
	dt->dt_year = y;
	ip = __mon_yday[__isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	dt->dt_mon = y+1;
	dt->dt_day = days + 1;
}

static time_t
mktimet(struct dos_tm *dt)
{
	time_t t;
	int y, leap_days, days;

	leap_days = 0;
	for( y=1972; y < dt->dt_year; y+= 4 )
		if( y % 100 != 0 || y % 400 == 0 )
			leap_days++;
	y = dt->dt_year - 1970;
	days = 365 * y + leap_days
		+ __mon_yday[__isleap(dt->dt_year)][dt->dt_mon-1]
		+ (dt->dt_day - 1);
	t = SECSPERDAY * days + dt->dt_hour * SECSPERHOUR
		+ dt->dt_min * 60 + dt->dt_sec;
	return t;
}

static int
is_dst(struct tz_rule *rule, time_t t)
{
	int year;

	if( rule->p1.sec_offset == rule->p2.sec_offset )
		return 0;

	year = get_offtime_year(t, 0);
	compute_change(&rule->p1, year);
	compute_change(&rule->p2, year);
	if( rule->p1.change < rule->p2.change ) {
		return rule->p1.change <= t && t < rule->p2.change;
	} else {
		return rule->p1.change <= t || t < rule->p2.change;
	}
}

static void
systime2localtime(struct tz_rule *rule, time_t t, struct dos_tm *dt)
{
	if( is_dst(rule, t) )
		get_offtime(t, rule->p2.sec_offset, dt);
	else
		get_offtime(t, rule->p1.sec_offset, dt);
	if( dt->dt_year < 1980 ) {
		dt->dt_year = 1980;
		dt->dt_mon = 1;
		dt->dt_day = 1;
		dt->dt_hour = 0;
		dt->dt_min = 0;
		dt->dt_sec = 0;
		dt->dt_wday = 2;
	}
}

static time_t
localtime2systime(struct tz_rule *rule, struct dos_tm *dt)
{
	time_t t;

	if( dt->dt_year > 2038 )
		return 0x7fffffff;

	t = mktimet(dt);
	if( rule->p1.sec_offset == rule->p2.sec_offset ) {
		t -= rule->p1.sec_offset;
	} else {
		if( is_dst(rule, t - rule->p1.sec_offset) ||
		    is_dst(rule, t - rule->p2.sec_offset) )
			t -= rule->p2.sec_offset;
		else
			t -= rule->p1.sec_offset;
	}
	if( t >= 0 && t <= 0x7fffffff )
		return t;
	else
		return 0x7fffffff;
}

#ifndef TEST_TZ
int fat_tz_set(struct tz_rule *rule, char *timezone)
{
	extern struct timezone sys_tz;
	int rval;

	if( !timezone ) {
		rule->p1.sec_offset = rule->p2.sec_offset
			= -sys_tz.tz_minuteswest*60;
		rval = 0;
	} else {
		rval = parse_tz(timezone, rule);
	}
	tz_printk("std=\"%s\" offset=%d(%s)  dst=\"%s\" dst_offset=%d(%s)\n",
		       rule->p1.name, rule->p1.sec_offset,
		       "-", rule->p1.name,
		       rule->p2.sec_offset,
		       "-");
	tz_printk(" dst begin=%d.%d.%d %s end=%d.%d.%d %s\n",
		       rule->p1.m,
		       rule->p1.n,
		       rule->p1.d,
		       "-",
		       rule->p2.m,
		       rule->p2.n,
		       rule->p2.d,
		       "-");
	return rval;
}

void fat_tz_fat2unix(struct tz_rule *rule, u16 time, u16 date,
		     time_t *unix_date)
{
	struct dos_tm dt;

	dt.dt_year = 1980 + ((date >> 9) & 0x7f);
	dt.dt_mon = (date >> 5) & 0x0f;
	dt.dt_day = date & 0x1f;
	dt.dt_hour = (time >> 11) & 0x1f;
	dt.dt_min = (time >> 5) & 0x3f;
	dt.dt_sec = (time & 0x1f) << 1;

	if (!dt.dt_mon)
		dt.dt_mon = 1;
	if (!dt.dt_day)
		dt.dt_day = 1;
	if (31 < dt.dt_day)
		dt.dt_day = 31;
	if (23 < dt.dt_hour)
		dt.dt_hour = 23;
	if (59 < dt.dt_min)
		dt.dt_min = 59;
	if (58 < dt.dt_sec)
		dt.dt_sec = 58; /* NOTE: sec is 2-second unit */

	*unix_date = localtime2systime(rule, &dt);
}

void fat_tz_unix2fat(struct tz_rule *rule, time_t unix_date, __le16 *time,
		     __le16 *date)
{
	struct dos_tm dt;

	systime2localtime(rule, unix_date, &dt);
	*time = cpu_to_le16((dt.dt_sec >> 1) + (dt.dt_min << 5) +
			    (dt.dt_hour << 11));
	*date = cpu_to_le16(dt.dt_day + (dt.dt_mon << 5) +
			    ((dt.dt_year - 1980) << 9));
}
#endif /* ! TEST_TZ */
