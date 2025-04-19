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

#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include "def.h"
#include "utf.h"
#include "utf8.h"

__Cml_INLINE int CmlUTF8_detectBOM(unsigned char *p_buff, size_t len) {
    return len >= 3 && p_buff[0] == 0xEF && p_buff[1] == 0xBB && p_buff[2] == 0xBF;
}

size_t CmlUTF8_getOctetsLength(unsigned char *p_buff, size_t len)
{
    size_t octetsLength = 0;
    if (!(p_buff[0] & 0x80)) {
        octetsLength = 1;
    } else if ((p_buff[0] & 0xE0) == 0xC0) {
        octetsLength = 2;
    } else if ((p_buff[0] & 0xF0) == 0xE0) {
        octetsLength = 3;
    } else if ((p_buff[0] & 0xF8) == 0xF0) {
        octetsLength = 4;
    }

    return len < octetsLength ? 0 : octetsLength;
}

void CmlUTF8_encode(CmlUTF_Code code, unsigned char *p_buff, size_t len)
{
    if (len == 0) {
        goto bufferTooSmallError;
    }

    unsigned char b1 = 0;
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

    p_buff[0] = b1;
    size_t i = 1;
    for (; i < octetsLength; i++) {
        p_buff[i] = 0x80 | ((code >> (6 * (octetsLength - 1 - i))) & 0x3F);
    }

    return;

    bufferTooSmallError:
    errno = ERANGE;
    return;
}

CmlUTF_Code CmlUTF8_decode(unsigned char *p_buff, size_t len)
{
    if (len == 0) {
        goto bufferTooSmallError;
    }

    CmlUTF_Code code = 0;
    size_t octetsLength = CmlUTF8_getOctetsLength(p_buff, len);
    switch (octetsLength) {
        case 1: code = p_buff[0];
        break;
        case 2: code = p_buff[0] & 0x1F;
        break;
        case 3: code = p_buff[0] & 0xF;
        break;
        case 4: code = p_buff[0] & 0x7;
        break;
        default: goto invalidUTF8Error;
    }

    if (len < octetsLength) {
        goto bufferTooSmallError;
    }

    size_t i = 1;
    for (; i < octetsLength; i++) {
        if ((p_buff[i] & 0xC0) != 0x80) {
            goto invalidUTF8Error;
        }
        code = (code << 6) | (p_buff[i] & 0x3F);
    }

    return code;

    bufferTooSmallError:
    errno = ERANGE;
    return -1;

    invalidUTF8Error:
    errno = EINVAL;
    return -1;
}

void CmlUTF8_new(struct CmlUTF_Buffer *p_utf, unsigned char *p_buff, size_t offset, size_t len)
{
    if (p_buff == NULL) {
        errno = EINVAL;
        return;
    }

    p_utf->buff = p_buff;
    p_utf->currIndex = 0;
    p_utf->offset = offset;
    p_utf->endian = Cml_BE;
    p_utf->len = len;

    p_utf->codec = malloc(sizeof(struct CmlUTF_Codec));
    p_utf->codec->encodeLE = &CmlUTF8_encode;
    p_utf->codec->encodeBE = &CmlUTF8_encode;
    p_utf->codec->decodeLE = &CmlUTF8_decode;
    p_utf->codec->decodeBE = &CmlUTF8_decode;
    p_utf->codec->getOctetsLengthBE = &CmlUTF8_getOctetsLength;
    p_utf->codec->getOctetsLengthLE = &CmlUTF8_getOctetsLength;
}
