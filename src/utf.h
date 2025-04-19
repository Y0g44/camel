/*
utf.h - Encode and decode UTF

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

#ifndef __UTF_H
#define __UTF_H

#include <stddef.h>
#include "def.h"

typedef unsigned int CmlUTF_Code;

struct CmlUTF_Codec {
    void (*encodeLE)(CmlUTF_Code code, unsigned char *p_buff, size_t len);
    void (*encodeBE)(CmlUTF_Code code, unsigned char *p_buff, size_t len);
    CmlUTF_Code (*decodeLE)(unsigned char *p_buff, size_t len);
    CmlUTF_Code (*decodeBE)(unsigned char *p_buff, size_t len);
    size_t (*getOctetsLengthBE)(unsigned char *p_buff, size_t len);
    size_t (*getOctetsLengthLE)(unsigned char *p_buff, size_t len);
};

struct CmlUTF_Buffer {
    unsigned char *buff;
    size_t currIndex;
    enum Cml_Endianness endian;
    size_t len;
    size_t offset;
    size_t moffset;
    size_t mcurrIndex;
    struct CmlUTF_Codec *codec;
};

size_t CmlUTF_len(struct CmlUTF_Buffer *p_utf);
size_t CmlUTF_next(struct CmlUTF_Buffer *p_utf, size_t n);
CmlUTF_Code CmlUTF_iter(struct CmlUTF_Buffer *p_utf);
CmlUTF_Code CmlUTF_read(struct CmlUTF_Buffer *p_utf);
size_t CmlUTF_write(struct CmlUTF_Buffer *p_utf, CmlUTF_Code code);

#endif
