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

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "utf16.h"
#include "types.h"

static inline void CmlUTF16_encode32bits(u_int32_t code, u_int16_t *w1, u_int16_t *w2)
{
    u_int32_t u = code - 0x10000;
    *w1 = (u >> 10) + 0xD800;
    *w2 = (u & 0x3FF) + 0xDC00;
}

static inline u_int32_t CmlUTF16_decode32bits(u_int16_t w1, u_int16_t w2)
{
    return (((w1 - 0xD800) << 10) | (w2 - 0xDC00)) + 0x10000;
}

inline size_t CmlUTF16_getOctetsLengthBE(u_int8_t *buff, size_t len)
{
    return len >= 4 && (buff[0] & 0xFC) == 0xD8 && (buff[2] & 0xFC) == 0xDC
        ? 4
        : 2;
}

inline size_t CmlUTF16_getOctetsLengthLE(u_int8_t *buff, size_t len)
{
    return len >= 4 && (buff[1] & 0xFC) == 0xD8 && (buff[3] & 0xFC) == 0xDC
        ? 4
        : 2;
}

void CmlUTF16_encodeBE(u_int32_t code, u_int8_t *buff, size_t len)
{
    if (code <= 0xFFFF) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        buff[0] = code >> 8;
        buff[1] = code & 0xFF;
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        u_int16_t w1, w2;
        CmlUTF16_encode32bits(code, &w1, &w2);
        buff[0] = w1 >> 8;
        buff[1] = w1 & 0xFF;
        buff[2] = w2 >> 8;
        buff[3] = w2 & 0xFF;
    }

    return;
    bufferTooSmallErr:
    errno = EINVAL;
}

u_int32_t CmlUTF16_decodeBE(u_int8_t *buff, size_t len)
{
    if (CmlUTF16_getOctetsLengthBE(buff, len) == 2) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        return (buff[0] << 8) | buff[1];
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        return CmlUTF16_decode32bits((buff[0] << 8) | buff[1], (buff[2] << 8) | buff[3]);
    }

    bufferTooSmallErr:
    errno = EINVAL;
    return -1;
}

void CmlUTF16_encodeLE(u_int32_t code, u_int8_t *buff, size_t len)
{
    if (code <= 0xFFFF) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        buff[0] = code & 0xFF;
        buff[1] = code >> 8;
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        u_int16_t w1, w2;
        CmlUTF16_encode32bits(code, &w1, &w2);
        buff[0] = w1 & 0xFF;
        buff[1] = w1 >> 8;
        buff[2] = w2 & 0xFF;
        buff[3] = w2 >> 8;
    }

    return;
    bufferTooSmallErr:
    errno = EINVAL;
}

u_int32_t CmlUTF16_decodeLE(u_int8_t *buff, size_t len)
{
    if (CmlUTF16_getOctetsLengthLE(buff, len) == 2) {
        if (len < 2) {
            goto bufferTooSmallErr;
        }

        return (buff[1] << 8) | buff[0];
    } else {
        if (len < 4) {
            goto bufferTooSmallErr;
        }

        return CmlUTF16_decode32bits((buff[1] << 8) | buff[0], (buff[3] << 8) | buff[2]);
    }

    bufferTooSmallErr:
    errno = EINVAL;
    return -1;
}

enum Cml_endianness CmlUTF16_detectEndianness(u_int8_t *buff, size_t len)
{
    if (len < 2) {
        goto defaultEndian;
    }

    if (buff[0] == 0xFE && buff[1] == 0xFF) {
        return Cml_endianness_BE;
    } else if (buff[0] == 0xFF && buff[1] == 0xFE) {
        return Cml_endianness_LE;
    } else {
        defaultEndian:
        #ifdef _WIN32
            return Cml_endianness_LE;
        #else
            return Cml_endianness_BE;
        #endif
    }

}

void CmlUTF16_new(struct CmlUTF_buffer *utf, u_int8_t *buff, size_t offset, size_t len, enum Cml_endianness endian)
{
    if (buff == NULL) {
        errno = EINVAL;
        return;
    }

    utf->buff = buff;
    utf->currIndex = 0;
    utf->offset = offset;
    utf->endian = endian == 0 ? CmlUTF16_detectEndianness(buff, len) : endian;
    utf->len = len;

    utf->codec = malloc(sizeof(struct CmlUTF_codec));
    utf->codec->encodeLE = &CmlUTF16_encodeLE;
    utf->codec->encodeBE = &CmlUTF16_encodeBE;
    utf->codec->decodeLE = &CmlUTF16_decodeLE;
    utf->codec->decodeBE = &CmlUTF16_decodeBE;
    utf->codec->getOctetsLengthBE = &CmlUTF16_getOctetsLengthBE;
    utf->codec->getOctetsLengthLE = &CmlUTF16_getOctetsLengthLE;
}
