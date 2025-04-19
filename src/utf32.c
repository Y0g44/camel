/*
utf32.c - Encode and decode UTF-32

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

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include "def.h"
#include "utf32.h"
#include "utf.h"

inline size_t CmlUTF32_getOctetsLength(u_int8_t *p_buff, size_t len)
{
    return 4;
}

void CmlUTF32_LE_encode(u_int32_t code, u_int8_t *p_buff, size_t len)
{
    if (len < 4) {
        errno = EINVAL;
        return;
    }

    p_buff[0] = code & 0xFF;
    p_buff[1] = code >> 8 & 0xFF;
    p_buff[2] = code >> 16 & 0xFF;
    p_buff[3] = code >> 24;
}

u_int32_t CmlUTF32_LE_decode(u_int8_t *p_buff, size_t len)
{
    if (len < 4) {
        errno = EINVAL;
        return -1;
    }

    return (p_buff[3] << 24) | (p_buff[2] << 16) | (p_buff[1] << 8) | p_buff[0];
}

void CmlUTF32_BE_encode(u_int32_t code, u_int8_t *p_buff, size_t len)
{
    if (len < 4) {
        errno = EINVAL;
        return;
    }

    p_buff[0] = code >> 24;
    p_buff[1] = code >> 16 & 0xFF;
    p_buff[2] = code >> 8 & 0xFF;
    p_buff[3] = code & 0xFF;
}

u_int32_t CmlUTF32_BE_decode(u_int8_t *p_buff, size_t len)
{
    if (len < 4) {
        errno = EINVAL;
        return -1;
    }

    return (p_buff[0] << 24) | (p_buff[1] << 16) | (p_buff[2] << 8) | p_buff[3];
}

enum Cml_Endianness CmlUTF32_detectEndianness(u_int8_t *p_buff, size_t len)
{
    if (len < 4) {
        goto defaultEndian;
    }

    if (p_buff[0] == 0xFE && p_buff[1] == 0xFF && !(p_buff[2]) && !(p_buff[3])) {
        return Cml_BE_ENDIANNESS;
    } else if (!(p_buff[0]) && !(p_buff[1]) && p_buff[2] == 0xFF && p_buff[3] == 0xFE) {
        return Cml_LE_ENDIANNESS;
    } else {
        defaultEndian:
        #ifdef _WIN32
            return Cml_LE_ENDIANNESS;
        #else
            return Cml_BE_ENDIANNESS;
        #endif
    }
}

void CmlUTF32_new(struct CmlUTF_Buffer *p_utf, u_int8_t *p_buff, size_t offset, size_t len, enum Cml_Endianness endian)
{
    if (p_buff == NULL) {
        errno = EINVAL;
        return;
    }

    p_utf->buff = p_buff;
    p_utf->currIndex = 0;
    p_utf->offset = offset;
    p_utf->endian = endian == 0 ? CmlUTF32_detectEndianness(p_buff, len) : endian;
    p_utf->len = len;

    p_utf->codec = malloc(sizeof(struct CmlUTF_Codec));
    p_utf->codec->encodeBE = &CmlUTF32_BE_encode;
    p_utf->codec->encodeLE = &CmlUTF32_LE_encode;
    p_utf->codec->decodeLE = &CmlUTF32_LE_decode;
    p_utf->codec->decodeBE = &CmlUTF32_BE_decode;
    p_utf->codec->getOctetsLengthBE = &CmlUTF32_getOctetsLength;
    p_utf->codec->getOctetsLengthLE = &CmlUTF32_getOctetsLength;
}
