/*
utf32.h - Encode and decode UTF-32

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

#ifndef __UTF_32_H
#define __UTF_32_H

#include "def.h"
#include "utf.h"

size_t CmlUTF32_getOctetsLength(unsigned char *p_buff, size_t len);
void CmlUTF32_LE_encode(CmlUTF_Code code, unsigned char *p_buff, size_t len);
CmlUTF_Code CmlUTF32_LE_decode(unsigned char *p_buff, size_t len);
void CmlUTF32_BE_encode(CmlUTF_Code code, unsigned char *p_buff, size_t len);
CmlUTF_Code CmlUTF32_BE_decode(unsigned char *p_buff, size_t len);
enum Cml_Endianness CmlUTF32_detectEndianness(unsigned char *p_buff, size_t len);
void CmlUTF32_new(struct CmlUTF_Buffer *p_utf, unsigned char *p_buff, size_t offset, size_t len, enum Cml_Endianness endian);

#endif
