/*
utf.c - Encode and decode UTF

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
#include <stdio.h>
#include <sys/types.h>
#include "utf.h"
#include "types.h"

void CmlUTF_mark(struct CmlUTF_buffer *utf)
{
    utf->moffset = utf->offset;
    utf->mcurrIndex = utf->currIndex;
}

void CmlUTF_ret(struct CmlUTF_buffer *utf)
{
    utf->offset = utf->moffset;
    utf->currIndex = utf->mcurrIndex;
}

size_t CmlUTF_len(struct CmlUTF_buffer *utf)
{
    size_t len = 0;
    size_t currOffset = utf->offset;
    u_int8_t currIndex = utf->currIndex;
    utf->offset = 0;
    utf->currIndex = 0;

    while (1) {
        if (CmlUTF_next(utf, 1) == -1 && errno == ERANGE) {
            utf->offset = currOffset;
            utf->currIndex = currIndex;
            return len;
        }
        len++;
    }
}

size_t CmlUTF_next(struct CmlUTF_buffer *utf, size_t n)
{
    size_t offset = utf->offset;

    while (n != 0) {
        if (utf->endian == Cml_endianness_BE) {
            utf->currIndex += utf->codec->getOctetsLengthBE(utf->buff + utf->currIndex, utf->len - utf->currIndex);
        } else {
            utf->currIndex += utf->codec->getOctetsLengthLE(utf->buff + utf->currIndex, utf->len - utf->currIndex);
        }

        if (utf->currIndex > utf->len) {
            errno = ERANGE;
            utf->currIndex = utf->len;
            return -1;
        }

        offset++;
        n--;
    }

    utf->currIndex = utf->currIndex;
    utf->offset = offset;
    return offset;
}

u_int32_t CmlUTF_iter(struct CmlUTF_buffer *utf)
{
    u_int32_t code = CmlUTF_read(utf);
    return CmlUTF_next(utf, 1) == -1 && errno == ERANGE ? -1 : code;
}

u_int32_t CmlUTF_read(struct CmlUTF_buffer *utf)
{
    if (utf->currIndex >= utf->len) {
        errno = ERANGE;
        return -1;
    }

    switch (utf->endian) {
        case Cml_endianness_BE:
            return utf->codec->decodeBE(utf->buff + utf->currIndex, utf->len - utf->currIndex);
        case Cml_endianness_LE:
            return utf->codec->decodeLE(utf->buff + utf->currIndex, utf->len - utf->currIndex);
    }

    return -1;
}

size_t CmlUTF_write(struct CmlUTF_buffer *utf, u_int32_t code)
{
    if (utf->currIndex >= utf->len) {
        errno = ERANGE;
        return -1;
    }

    switch (utf->endian) {
        case Cml_endianness_BE:
            utf->codec->encodeBE(code, utf->buff + utf->currIndex, utf->len - utf->currIndex);
            break;
        case Cml_endianness_LE:
            utf->codec->encodeLE(code, utf->buff + utf->currIndex, utf->len - utf->currIndex);
            break;
    }

    return utf->offset;
}
