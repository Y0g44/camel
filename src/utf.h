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

#include <sys/types.h>
#include "types.h"

struct CmlUTF_buffer {
    u_int8_t *buff;
    size_t currIndex;
    enum Cml_endianness endian;
    size_t len;
    size_t offset;

    void (*encodeLE)(u_int32_t code, u_int8_t *buff, size_t len);
    void (*encodeBE)(u_int32_t code, u_int8_t *buff, size_t len);
    u_int32_t (*decodeLE)(u_int8_t *buff, size_t len);
    u_int32_t (*decodeBE)(u_int8_t *buff, size_t len);
    size_t (*getOctetsLengthBE)(u_int8_t *buff, size_t len);
    size_t (*getOctetsLengthLE)(u_int8_t *buff, size_t len);
};

size_t CmlUTF_len(struct CmlUTF_buffer *utf);
size_t CmlUTF_next(struct CmlUTF_buffer *utf, size_t n);
u_int32_t CmlUTF_iter(struct CmlUTF_buffer *utf);
u_int32_t CmlUTF_read(struct CmlUTF_buffer *utf);
size_t CmlUTF_write(struct CmlUTF_buffer *utf, u_int32_t code);

#endif
