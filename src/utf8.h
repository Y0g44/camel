/*
utf8.h - Encode and decode UTF-8

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

#ifndef __UTF8_H
#define __UTF8_H

#include <sys/types.h>
#include "def.h"
#include "utf.h"

size_t CmlUTF8_getOctetsLength(u_int8_t *p_buff, size_t len);
void CmlUTF8_encode(u_int32_t code, u_int8_t *p_buff, size_t len);
u_int32_t CmlUTF8_decode(u_int8_t *p_buff, size_t len);
void CmlUTF8_new(struct CmlUTF_Buffer *p_utf, u_int8_t *p_buff, size_t offset, size_t len);

#endif
