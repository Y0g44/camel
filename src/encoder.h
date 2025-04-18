/*
encoder.h - Bridge to various encode formats and the output will be the input of transliteration

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

#ifndef __ENCODER_H
#define __ENCODER_H

#include "utf.h"

enum Cml_codePoints {
    CmlCodePoint_END_OF_BUFF,
    CmlCodePoint_REPLACEMENT_CHAR,
    CmlCodePoint_SPACE,
    CmlCodePoint_VOCAL_A,
    CmlCodePoint_VOCAL_I,
    CmlCodePoint_VOCAL_U,
    CmlCodePoint_VOCAL_E,
    CmlCodePoint_VOCAL_SCHWA,
    CmlCodePoint_VOCAL_O,
    CmlCodePoint_SYLLABIC_CONSONANT_L,
    CmlCodePoint_SYLLABIC_CONSONANT_R,
    CmlCodePoint_LONG_VOCAL_A,
    CmlCodePoint_LONG_VOCAL_I,
    CmlCodePoint_LONG_VOCAL_U,
    CmlCodePoint_LONG_VOCAL_E,
    CmlCodePoint_LONG_VOCAL_SCHWA,
    CmlCodePoint_LONG_VOCAL_O,
    CmlCodePoint_LONG_SYLLABIC_CONSONANT_L,
    CmlCodePoint_LONG_SYLLABIC_CONSONANT_R,
    CmlCodePoint_CONSONANT_H,
    CmlCodePoint_CONSONANT_N,
    CmlCodePoint_CONSONANT_C,
    CmlCodePoint_CONSONANT_R,
    CmlCodePoint_CONSONANT_K,
    CmlCodePoint_CONSONANT_D,
    CmlCodePoint_CONSONANT_T,
    CmlCodePoint_CONSONANT_S,
    CmlCodePoint_CONSONANT_W,
    CmlCodePoint_CONSONANT_L,
    CmlCodePoint_CONSONANT_M,
    CmlCodePoint_CONSONANT_G,
    CmlCodePoint_CONSONANT_B,
    CmlCodePoint_CONSONANT_P,
    CmlCodePoint_CONSONANT_J,
    CmlCodePoint_CONSONANT_Y,
    CmlCodePoint_RETROFLEX_CONSONANT_N,
    CmlCodePoint_RETROFLEX_CONSONANT_D,
    CmlCodePoint_RETROFLEX_CONSONANT_T,
    CmlCodePoint_RETROFLEX_CONSONANT_S,
    CmlCodePoint_PALATAL_CONSONANT_S,
    CmlCodePoint_NUMBER_0,
    CmlCodePoint_NUMBER_1,
    CmlCodePoint_NUMBER_2,
    CmlCodePoint_NUMBER_3,
    CmlCodePoint_NUMBER_4,
    CmlCodePoint_NUMBER_5,
    CmlCodePoint_NUMBER_6,
    CmlCodePoint_NUMBER_7,
    CmlCodePoint_NUMBER_8,
    CmlCodePoint_NUMBER_9,
    CmlCodePoint_PUNCTUATION_CARIK_SIKI,
    CmlCodePoint_PUNCTUATION_CARIK_KALIH,
    CmlCodePoint_PUNCTUATION_CARIK_PAMUNGKAH,
    CmlCodePoint_PUNCTUATION_PANTEN,
    CmlCodePoint_PUNCTUATION_PASALINAN,
    CmlCodePoint_PUNCTUATION_PAMADA,
    CmlCodePoint_PUNCTUATION_CARIK_AGUNG,
    CmlCodePoint_PUNCTUATION_IDEM
};

enum Cml_codePoints *CmlEncoder_encodeUTF(struct CmlUTF_buffer *utf);

#endif
