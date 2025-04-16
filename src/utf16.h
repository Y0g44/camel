/*
utf16.h - Encode and decode UTF-16

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

#ifndef __UTF16_H
#define __UTF16_H

#include <sys/types.h>
#include "types.h"
#include "utf.h"

size_t CmlUTF16_getOctetsLengthBE(u_int8_t *buff, size_t len);
size_t CmlUTF16_getOctetsLengthLE(u_int8_t *buff, size_t len);
void CmlUTF16_encodeBE(u_int32_t code, u_int8_t *buff, size_t len);
u_int32_t CmlUTF16_decodeBE(u_int8_t *buff, size_t len);
void CmlUTF16_encodeLE(u_int32_t code, u_int8_t *buff, size_t len);
u_int32_t CmlUTF16_decodeLE(u_int8_t *buff, size_t len);
enum Cml_endianness CmlUTF16_detectEndianness(u_int8_t *buff, size_t len);
void CmlUTF16_new(struct CmlUTF_buffer *utf, u_int8_t *buff, size_t offset, size_t len, enum Cml_endianness endian);

#endif
