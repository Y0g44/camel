/*
encoder.c - Bridge to various encode formats and the output will be the input of transliteration

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
#include <sys/types.h>
#include <stdlib.h>
#include "encoder.h"
#include "utf.h"
#define CML_RETROFLEX_NOTATION '`'
#define CML_SYLLABIC_CONSONANT_NOTATION '!'
#define CML_SYLLABIC_CONSONANT_LONG_NOTATION '*'
#define CML_VOCAL_LONG_NOTATION '~'
#define CML_PALATAL_NOTATION '\''

static u_int32_t CmlEncoder_convertToLowerCase(u_int32_t code)
{
    if (code >= 0x0041 && code <= 0x005A)
        return code + 32;
    else if (code == 0x0130)
        return 'i';
    else if (code == 0x0178)
        return 0x00FF;
    else if ((code >= 0x0100 && code <= 0x017E && code != 0x0131) || (code >= 0x01E00 && code <= 0x1E95))
        return code % 2 == 0 ? code + 1 : code;

    return code;
}

static size_t CmlEncoder_normalizeCurrUTF(struct CmlUTF_buffer *utf, u_int32_t *code)
{
    CmlUTF_mark(utf);
    u_int32_t c1 = CmlEncoder_convertToLowerCase(CmlUTF_read(utf));
    u_int32_t c2 = (CmlUTF_next(utf, 1), CmlUTF_read(utf));
    if (c1 == 'x') {
        c1 = 0x00E9;
    }
    *code = c1;

    if (c2 == CML_RETROFLEX_NOTATION) {
        switch (c1) {
            case 'n': *code = 0x1E47;
            break;
            case 'd': *code = 0x1E0D;
            break;
            case 't': *code = 0x1E6D;
            break;
            case 's': *code = 0x1E63;
            break;
            default: goto readOneChar;
        }

        goto readTwoChars;
    } else if (c2 == CML_SYLLABIC_CONSONANT_NOTATION) {
        switch (c1) {
            case 'l': *code = 0x1E37;
            break;
            case 'r': *code = 0x1E5B;
            break;
            default: goto readOneChar;
        }

        goto readTwoChars;
    } else if (c2 == CML_SYLLABIC_CONSONANT_LONG_NOTATION) {
        switch (c1) {
            case 'l': *code = 0x1E39;
            break;
            case 'r': *code = 0x1E5D;
            break;
            default: goto readOneChar;
        }

        goto readTwoChars;
    } else if (c2 == CML_VOCAL_LONG_NOTATION) {
        switch (c1) {
            case 'a': *code = 0x0101;
            break;
            case 'i': *code = 0x012B;
            break;
            case 'u': *code = 0x016B;
            break;
            case 'e': *code = 0x0113;
            break;
            case 0x00E9: *code = 0x1E17;
            break;
            case 'o': *code = 0x014D;
            break;
            default: goto readOneChar;
        }

        goto readTwoChars;
    } else if (c2 == CML_PALATAL_NOTATION && c1 == 's') {
        *code = 0x015B;
        goto readTwoChars;
    } else if (c1 == c2) {
        switch (c1) {
            case '+': *code = 0xF0000;
            break;
            case '-': *code = 0xF0001;
            break;
            case '#': *code = 0xF0002;
            break;
            case '/': *code = 0xF0003;
            break;
            default: goto readOneChar;
        }

        goto readTwoChars;
    }

    readOneChar:
        CmlUTF_ret(utf);
        return 1;
    readTwoChars:
        CmlUTF_ret(utf);
        return 2;
}

enum Cml_codePoints *CmlEncoder_encodeUTF(struct CmlUTF_buffer *utf)
{
    size_t utfLen = CmlUTF_len(utf);
    enum Cml_codePoints *buff = malloc(sizeof(enum Cml_codePoints) * utfLen);

    for (size_t i = 0; i < utfLen; i++) {
        enum Cml_codePoints code = CmlCodePoint_REPLACEMENT_CHAR;
        u_int32_t currCode = 0;
        size_t isUseTwoChars = CmlEncoder_normalizeCurrUTF(utf, &currCode) == 2;
        if (currCode == -1 && errno == ERANGE) {
            buff[i] = CmlCodePoint_END_OF_BUFF;
            break;
        }

        if (currCode == ' ') {
            code = CmlCodePoint_SPACE;
        } else if (currCode >= '0' && currCode <= '9') {
            code = CmlCodePoint_NUMBER_0 + currCode - '0';
        } else {
            switch (currCode) {
                case 'a': code = CmlCodePoint_VOCAL_A;
                break;
                case 'i': code = CmlCodePoint_VOCAL_I;
                break;
                case 'u': code = CmlCodePoint_VOCAL_U;
                break;
                case 'e': code = CmlCodePoint_VOCAL_SCHWA;
                break;
                case 0x00E9: code = CmlCodePoint_VOCAL_E;
                break;
                case 'o': code = CmlCodePoint_VOCAL_O;
                break;
                case 0x1E37: code = CmlCodePoint_SYLLABIC_CONSONANT_L;
                break;
                case 0x1E5B: code = CmlCodePoint_SYLLABIC_CONSONANT_R;
                break;
                case 0x0101: code = CmlCodePoint_LONG_VOCAL_A;
                break;
                case 0x012B: code = CmlCodePoint_LONG_VOCAL_I;
                break;
                case 0x016B: code = CmlCodePoint_LONG_VOCAL_U;
                break;
                case 0x0113: code = CmlCodePoint_LONG_VOCAL_SCHWA;
                break;
                case 0x1E17: code = CmlCodePoint_LONG_VOCAL_E;
                break;
                case 0x014D: code = CmlCodePoint_LONG_VOCAL_O;
                break;
                case 0x1E39: code = CmlCodePoint_LONG_SYLLABIC_CONSONANT_L;
                break;
                case 0x1E5D: code = CmlCodePoint_LONG_SYLLABIC_CONSONANT_R;
                break;
                case 'h': code = CmlCodePoint_CONSONANT_H;
                break;
                case 'n': code = CmlCodePoint_CONSONANT_N;
                break;
                case 'c': code = CmlCodePoint_CONSONANT_C;
                break;
                case 'r': code = CmlCodePoint_CONSONANT_R;
                break;
                case 'k': code = CmlCodePoint_CONSONANT_K;
                break;
                case 'd': code = CmlCodePoint_CONSONANT_D;
                break;
                case 't': code = CmlCodePoint_CONSONANT_T;
                break;
                case 's': code = CmlCodePoint_CONSONANT_S;
                break;
                case 'w': code = CmlCodePoint_CONSONANT_W;
                break;
                case 'l': code = CmlCodePoint_CONSONANT_L;
                break;
                case 'm': code = CmlCodePoint_CONSONANT_M;
                break;
                case 'g': code = CmlCodePoint_CONSONANT_G;
                break;
                case 'b': code = CmlCodePoint_CONSONANT_B;
                break;
                case 'p': code = CmlCodePoint_CONSONANT_P;
                break;
                case 'j': code = CmlCodePoint_CONSONANT_J;
                break;
                case 'y': code = CmlCodePoint_CONSONANT_Y;
                break;
                case 0x1E47: code = CmlCodePoint_RETROFLEX_CONSONANT_N;
                break;
                case 0x1E0D: code = CmlCodePoint_RETROFLEX_CONSONANT_D;
                break;
                case 0x1E6D: code = CmlCodePoint_RETROFLEX_CONSONANT_T;
                break;
                case 0x1E63: code = CmlCodePoint_RETROFLEX_CONSONANT_S;
                break;
                case 0x015B: code = CmlCodePoint_PALATAL_CONSONANT_S;
                break;
                case ',': code = CmlCodePoint_PUNCTUATION_CARIK_SIKI;
                break;
                case '.': code = CmlCodePoint_PUNCTUATION_CARIK_KALIH;
                break;
                case ':': code = CmlCodePoint_PUNCTUATION_CARIK_PAMUNGKAH;
                break;
                case 0xF0000: code = CmlCodePoint_PUNCTUATION_PANTEN;
                break;
                case 0xF0001: code = CmlCodePoint_PUNCTUATION_PASALINAN;
                break;
                case 0xF0002: code = CmlCodePoint_PUNCTUATION_PAMADA;
                break;
                case 0xF0003: code = CmlCodePoint_PUNCTUATION_CARIK_AGUNG;
                break;
            }
        }

        buff[i] = code;
        CmlUTF_next(utf, isUseTwoChars ? 2 : 1);
    }

    return buff;
}
