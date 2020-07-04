/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef _LINUX_TTY_FLIP_H
#define _LINUX_TTY_FLIP_H
/*start: modify for Security red line*/
#include "atpconfig.h"
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
extern int g_TagConsole;
#endif
/*end: modify for Security red line*/

extern int tty_buffer_request_room(struct tty_struct *tty, size_t size);
extern int tty_insert_flip_string_flags(struct tty_struct *tty, const unsigned char *chars, const char *flags, size_t size);
extern int tty_insert_flip_string_fixed_flag(struct tty_struct *tty, const unsigned char *chars, char flag, size_t size);
extern int tty_prepare_flip_string(struct tty_struct *tty, unsigned char **chars, size_t size);
extern int tty_prepare_flip_string_flags(struct tty_struct *tty, unsigned char **chars, char **flags, size_t size);
void tty_schedule_flip(struct tty_struct *tty);
extern void tty_log_write(const char* name, const unsigned char *data, size_t len, int mode);
static inline int tty_insert_flip_char(struct tty_struct *tty,
					unsigned char ch, char flag)
{
	struct tty_buffer *tb = tty->buf.tail;
    /*start: modify for Security red line, when equipment is finished disable ttyS0 and ttyS1 */ 
#ifdef SUPPORT_ATP_SECURITY_REDLINE_CONSOLE
    if ((('S' == tty->name[3]) && (('0' == tty->name[4]) || ('1' == tty->name[4]))) && (0 == g_TagConsole))
    {
        flag = TTY_BREAK;
    }
#endif
    /*end: modify for Security red line, when equipment is finished disable ttyS0 and ttyS1 */
	tty_log_write(tty->name, &ch, 1, 0);
	if (tb && tb->used < tb->size) {
		tb->flag_buf_ptr[tb->used] = flag;
		tb->char_buf_ptr[tb->used++] = ch;
		return 1;
	}
	return tty_insert_flip_string_flags(tty, &ch, &flag, 1);
}

static inline int tty_insert_flip_string(struct tty_struct *tty, const unsigned char *chars, size_t size)
{
	return tty_insert_flip_string_fixed_flag(tty, chars, TTY_NORMAL, size);
}

#endif /* _LINUX_TTY_FLIP_H */
