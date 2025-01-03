/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _SKEYPROVIDER_H
#define _SKEYPROVIDER_H

#define KEY_FOUND_SUCCESS 0
#define MANDATORY_KEY_NOT_FOUND 1
#define NORMAL_KEY_NOT_FOUND -1

extern int get_encrypted_key(unsigned char* keyname, unsigned char *key, int keylen);
extern int get_derived_key(char *key, int keylen, char* dst);
#endif
