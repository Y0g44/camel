/*
def.h - Definitions that will be shared

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

#ifndef __DEF_H
#define __DEF_H

#if __STDC__ != 1
#define __Cml_INLINE inline
#else
#define __Cml_INLINE
#endif

enum Cml_Endianness {
    Cml_BE,
    Cml_LE
};

#endif
