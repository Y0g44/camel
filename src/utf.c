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

#include <stddef.h>
#include <errno.h>
#include "def.h"
#include "utf.h"

size_t CmlUTF_len(struct CmlUTF_Buffer *p_utf)
{
    size_t len = 0;
    size_t currOffset = p_utf->offset;
    size_t currIndex = p_utf->currIndex;
    p_utf->offset = 0;
    p_utf->currIndex = 0;

    while (1) {
        len++;
        if (CmlUTF_next(p_utf, 1) == -1 && errno == ERANGE) {
            p_utf->offset = currOffset;
            p_utf->currIndex = currIndex;
            return len;
        }
    }
}

size_t CmlUTF_next(struct CmlUTF_Buffer *p_utf, size_t n)
{
    size_t offset = p_utf->offset;

    while (n != 0) {
        if (p_utf->endian == Cml_BE) {
            p_utf->currIndex += p_utf->codec->getOctetsLengthBE(p_utf->buff + p_utf->currIndex, p_utf->len - p_utf->currIndex);
        } else {
            p_utf->currIndex += p_utf->codec->getOctetsLengthLE(p_utf->buff + p_utf->currIndex, p_utf->len - p_utf->currIndex);
        }

        if (p_utf->currIndex >= p_utf->len) {
            errno = ERANGE;
            p_utf->currIndex = p_utf->len;
            return -1;
        }

        offset++;
        n--;
    }

    p_utf->currIndex = p_utf->currIndex;
    p_utf->offset = offset;
    return offset;
}

CmlUTF_Code CmlUTF_iter(struct CmlUTF_Buffer *p_utf)
{
    CmlUTF_Code code = CmlUTF_read(p_utf);
    return CmlUTF_next(p_utf, 1) == -1 && errno == ERANGE ? -1 : code;
}

CmlUTF_Code CmlUTF_read(struct CmlUTF_Buffer *p_utf)
{
    if (p_utf->currIndex >= p_utf->len) {
        errno = ERANGE;
        return -1;
    }

    switch (p_utf->endian) {
        case Cml_BE:
            return p_utf->codec->decodeBE(p_utf->buff + p_utf->currIndex, p_utf->len - p_utf->currIndex);
        case Cml_LE:
            return p_utf->codec->decodeLE(p_utf->buff + p_utf->currIndex, p_utf->len - p_utf->currIndex);
    }

    return -1;
}

size_t CmlUTF_write(struct CmlUTF_Buffer *p_utf, CmlUTF_Code code)
{
    if (p_utf->currIndex >= p_utf->len) {
        errno = ERANGE;
        return -1;
    }

    switch (p_utf->endian) {
        case Cml_BE:
            p_utf->codec->encodeBE(code, p_utf->buff + p_utf->currIndex, p_utf->len - p_utf->currIndex);
            break;
        case Cml_LE:
            p_utf->codec->encodeLE(code, p_utf->buff + p_utf->currIndex, p_utf->len - p_utf->currIndex);
            break;
    }

    return p_utf->offset;
}
