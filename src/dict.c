/*
dict.c - Read the key in the dictionary

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

#include <stddef.h>
#include <string.h>
#include <errno.h>
#include "dict.h"

static size_t CmlDict_hash(char *p_key, size_t max)
{
    size_t digest = 0x43;
    size_t i = 0;
    for (; p_key[i] != 0; i++)
        digest += p_key[i] * 31;
    return digest % max;
}

static size_t CmlDict_findKey(struct CmlDict_Dict *p_dict, char *p_key)
{
    size_t i = CmlDict_hash(p_key, p_dict->size * 0.8);

    size_t j = i;
    for (; j < p_dict->size && (j - i) < CmlDict_MAX_SEARCH; j++) {
        size_t headerOffset = j * CmlDict_HEADER_SIZE;
        size_t keyRef = (p_dict->buff[headerOffset + 3] << 8) | p_dict->buff[headerOffset + 4];
        if ((p_dict->buff[headerOffset] & 0b1) && !strcmp(p_dict->buff + keyRef, p_key))
            return j;
    }

    errno = ENOENT;
    return -1;
}

int CmlDict_get(struct CmlDict_Dict *p_dict, char *p_key, struct CmlDict_Field *p_value)
{
    size_t i = CmlDict_findKey(p_dict, p_key);
    if (i == -1 && errno == ENOENT)
        return ENOENT;

    size_t headerOffset = i * CmlDict_HEADER_SIZE;
    size_t ref = (p_dict->buff[headerOffset + 1] << 8) | p_dict->buff[headerOffset + 2];
    if (p_dict->len <= ref)
        return errno = EINVAL;

    p_value->value = p_dict->buff + ref;
    p_value->flag = p_dict->buff[headerOffset];
    return 0;
}

__Cml_INLINE int CmlDict_has(struct CmlDict_Dict *p_dict, char *p_key)
{
    return CmlDict_findKey(p_dict, p_key) != -1 || errno != ENOENT;
}
