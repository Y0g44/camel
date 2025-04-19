/*
dict.h - Read the key in the dictionary

Copyright (C) 2025 Yoga

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see
<https://www.gnu.org/licenses/>.
*/

#ifndef __DICT_H
#define __DICT_H

#include <stddef.h>
#include "def.h"

#define CmlDict_DICT_SIZE 1024
#define CmlDict_MAX_SEARCH 512
#define CmlDict_HEADER_SIZE 5

extern unsigned char *___Dict_bin;

struct CmlDict_Dict {
    char *buff;
    size_t len;
    size_t size;
};

struct CmlDict_Field {
    char *value;
    unsigned char flag;
};

int CmlDict_get(struct CmlDict_Dict *p_dict, char *p_key, struct CmlDict_Field *p_value);
int CmlDict_has(struct CmlDict_Dict *p_dict, char *p_key);

#endif
