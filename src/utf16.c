/*
utf16.c - Encode and decode UTF-16

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
#include <errno.h>
#include <stdlib.h>
#include "def.h"
#include "utf.h"
#include "utf16.h"

static __Cml_INLINE void CmlUTF16_encode32bits(CmlUTF_Code code, unsigned short int *p_w1, unsigned short int *p_w2)
{
    CmlUTF_Code u = code - 0x10000;
    *p_w1 = (u >> 10) + 0xD800;
    *p_w2 = (u & 0x3FF) + 0xDC00;
}

static __Cml_INLINE CmlUTF_Code CmlUTF16_decode32bits(unsigned short int w1, unsigned short int w2)
{
    return (((w1 - 0xD800) << 10) | (w2 - 0xDC00)) + 0x10000;
}

__Cml_INLINE size_t CmlUTF16_getOctetsLengthBE(unsigned char *p_buff, size_t len)
{
    return len >= 4 && (p_buff[0] & 0xFC) == 0xD8 && (p_buff[2] & 0xFC) == 0xDC
        ? 4
        : 2;
}

__Cml_INLINE size_t CmlUTF16_getOctetsLengthLE(unsigned char *p_buff, size_t len)
{
    return len >= 4 && (p_buff[1] & 0xFC) == 0xD8 && (p_buff[3] & 0xFC) == 0xDC
        ? 4
        : 2;
}

void CmlUTF16_encodeBE(CmlUTF_Code code, unsigned char *p_buff, size_t len)
{
    if (code <= 0xFFFF) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        p_buff[0] = code >> 8;
        p_buff[1] = code & 0xFF;
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        unsigned short int w1, w2;
        CmlUTF16_encode32bits(code, &w1, &w2);
        p_buff[0] = w1 >> 8;
        p_buff[1] = w1 & 0xFF;
        p_buff[2] = w2 >> 8;
        p_buff[3] = w2 & 0xFF;
    }

    return;
    bufferTooSmallErr:
    errno = EINVAL;
}

CmlUTF_Code CmlUTF16_decodeBE(unsigned char *p_buff, size_t len)
{
    if (CmlUTF16_getOctetsLengthBE(p_buff, len) == 2) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        return (p_buff[0] << 8) | p_buff[1];
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        return CmlUTF16_decode32bits((p_buff[0] << 8) | p_buff[1], (p_buff[2] << 8) | p_buff[3]);
    }

    bufferTooSmallErr:
    errno = EINVAL;
    return -1;
}

void CmlUTF16_encodeLE(CmlUTF_Code code, unsigned char *p_buff, size_t len)
{
    if (code <= 0xFFFF) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        p_buff[0] = code & 0xFF;
        p_buff[1] = code >> 8;
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        unsigned short int w1, w2;
        CmlUTF16_encode32bits(code, &w1, &w2);
        p_buff[0] = w1 & 0xFF;
        p_buff[1] = w1 >> 8;
        p_buff[2] = w2 & 0xFF;
        p_buff[3] = w2 >> 8;
    }

    return;
    bufferTooSmallErr:
    errno = EINVAL;
}

CmlUTF_Code CmlUTF16_decodeLE(unsigned char *p_buff, size_t len)
{
    if (CmlUTF16_getOctetsLengthLE(p_buff, len) == 2) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        return (p_buff[1] << 8) | p_buff[0];
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        return CmlUTF16_decode32bits((p_buff[1] << 8) | p_buff[0], (p_buff[3] << 8) | p_buff[2]);
    }

    bufferTooSmallErr:
    errno = EINVAL;
    return -1;
}

enum Cml_Endianness CmlUTF16_detectEndianness(unsigned char *buff, size_t len)
{
    if (len < 2) {
        goto defaultEndian;
    }

    if (buff[0] == 0xFE && buff[1] == 0xFF) {
        return Cml_BE;
    } else if (buff[0] == 0xFF && buff[1] == 0xFE) {
        return Cml_LE;
    } else {
        defaultEndian:
        #ifdef _WIN32
            return Cml_LE;
        #else
            return Cml_BE;
        #endif
    }

}

void CmlUTF16_new(struct CmlUTF_Buffer *p_utf, unsigned char *p_buff, size_t offset, size_t len, enum Cml_Endianness endian)
{
    if (p_buff == NULL) {
        errno = EINVAL;
        return;
    }

    p_utf->buff = p_buff;
    p_utf->currIndex = 0;
    p_utf->offset = offset;
    p_utf->endian = endian == 0 ? CmlUTF16_detectEndianness(p_buff, len) : endian;
    p_utf->len = len;

    p_utf->codec = malloc(sizeof(struct CmlUTF_Codec));
    p_utf->codec->encodeLE = &CmlUTF16_encodeLE;
    p_utf->codec->encodeBE = &CmlUTF16_encodeBE;
    p_utf->codec->decodeLE = &CmlUTF16_decodeLE;
    p_utf->codec->decodeBE = &CmlUTF16_decodeBE;
    p_utf->codec->getOctetsLengthBE = &CmlUTF16_getOctetsLengthBE;
    p_utf->codec->getOctetsLengthLE = &CmlUTF16_getOctetsLengthLE;
}
