/*
utf8.c - Encode and decode UTF-8

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

#include "utf8.h"
#include "types.h"
#include <errno.h>
#include <stdio.h>

size_t CmlUTF8_getOctetsLength(u_int8_t *buff, size_t len)
{
    size_t octetsLength = 0;
    if (!(buff[0] & 0x80)) {
        octetsLength = 1;
    } else if ((buff[0] & 0xE0) == 0xC0) {
        octetsLength = 2;
    } else if ((buff[0] & 0xF0) == 0xE0) {
        octetsLength = 3;
    } else if ((buff[0] & 0xF8) == 0xF0) {
        octetsLength = 4;
    }

    return len < octetsLength ? 0 : octetsLength;
}

void CmlUTF8_encode(u_int32_t code, u_int8_t *buff, size_t len)
{
    if (len == 0) {
        goto bufferTooSmallError;
    }

    u_int8_t b1 = 0;
    size_t octetsLength = 1;

    if (code < 0x80) {
        b1 = code & 0x7F;
        octetsLength = 1;
    } else if (code < 0x800) {
        b1 = 0xC0 | ((code >> 6) & 0x1F);
        octetsLength = 2;
    } else if (code < 0x10000) {
        b1 = 0xE0 | ((code >> 12) & 0x0F);
        octetsLength = 3;
    } else if (code <= 0x10FFFF) {
        b1 = 0xF0 | ((code >> 18) & 0x07);
        octetsLength = 4;
    } else {
        errno = EINVAL;
        return;
    }

    if (len < octetsLength) {
        goto bufferTooSmallError;
    }

    buff[0] = b1;
    for (size_t i = 1; i < octetsLength; i++) {
        buff[i] = 0x80 | ((code >> (6 * (octetsLength - 1 - i))) & 0x3F);
    }

    return;

    bufferTooSmallError:
    errno = ERANGE;
    return;
}

u_int32_t CmlUTF8_decode(u_int8_t *buff, size_t len)
{
    if (len == 0) {
        goto bufferTooSmallError;
    }

    u_int32_t code = 0;
    size_t octetsLength = CmlUTF8_getOctetsLength(buff, len);
    switch (octetsLength) {
        case 1: code = buff[0];
        break;
        case 2: code = buff[0] & 0x1F;
        break;
        case 3: code = buff[0] & 0xF;
        break;
        case 4: code = buff[0] & 0x7;
        break;
        default: goto invalidUTF8Error;
    }

    if (len < octetsLength) {
        goto bufferTooSmallError;
    }

    for (size_t i = 1; i < octetsLength; i++) {
        if ((buff[i] & 0xC0) != 0x80) {
            goto invalidUTF8Error;
        }
        code = (code << 6) | (buff[i] & 0x3F);
    }

    return code;

    bufferTooSmallError:
    errno = ERANGE;
    return -1;

    invalidUTF8Error:
    errno = EINVAL;
    return -1;
}

inline int CmlUTF8_detectBOM(u_int8_t *buff, size_t len)
{
    return len >= 3 && buff[0] == 0xEF && buff[1] == 0xBB && buff[2] == 0xBF;
}

void CmlUTF8_new(struct CmlUTF_buffer *utf, u_int8_t *buff, size_t offset, size_t len)
{
    if (buff == NULL) {
        errno = EINVAL;
        return;
    }

    utf->buff = buff;
    utf->currIndex = 0;
    utf->offset = offset;
    utf->endian = Cml_endianness_BE;
    utf->len = len;

    utf->encodeLE = &CmlUTF8_encode;
    utf->encodeBE = &CmlUTF8_encode;
    utf->decodeLE = &CmlUTF8_decode;
    utf->decodeBE = &CmlUTF8_decode;
    utf->getOctetsLengthBE = &CmlUTF8_getOctetsLength;
    utf->getOctetsLengthLE = &CmlUTF8_getOctetsLength;
}
