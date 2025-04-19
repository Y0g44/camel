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

#include "def.h"
#include "utf.h"

size_t CmlUTF16_getOctetsLengthBE(unsigned char *p_buff, size_t len);
size_t CmlUTF16_getOctetsLengthLE(unsigned char *p_buff, size_t len);
void CmlUTF16_encodeBE(CmlUTF_Code code, unsigned char *p_buff, size_t len);
CmlUTF_Code CmlUTF16_decodeBE(unsigned char *p_buff, size_t len);
void CmlUTF16_encodeLE(CmlUTF_Code code, unsigned char *p_buff, size_t len);
CmlUTF_Code CmlUTF16_decodeLE(unsigned char *p_buff, size_t len);
enum Cml_Endianness CmlUTF16_detectEndianness(unsigned char *p_buff, size_t len);
void CmlUTF16_new(struct CmlUTF_Buffer *p_utf, unsigned char *p_buff, size_t offset, size_t len, enum Cml_Endianness endian);

#endif
