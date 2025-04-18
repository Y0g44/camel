/*
tokenizer.c - Tokenizer

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
#include "def.h"
#include "utf.h"
#include "tokenizer.h"

static u_int32_t CmlTokenizer_convertToLowerCase(u_int32_t code)
{
    if (code >= 0x0041 && code <= 0x005A)
        return code + 32;
    else if (code == 0x0130)
        return 'i';
    else if (code == 0x00C9)
        return 0x00E9;
    else if (code == 0x0178)
        return 0x00FF;
    else if ((code >= 0x0100 && code <= 0x017E && code != 0x0131) || (code >= 0x01E00 && code <= 0x1E95))
        return code % 2 == 0 ? code + 1 : code;

    return code;
}

static size_t CmlTokenizer_normalizeUTF(u_int32_t c1, u_int32_t c2, u_int32_t *code)
{
    *code = c1;
    c1 = CmlTokenizer_convertToLowerCase(c1);
    if (c1 == 'x') {
        c1 = 0x00E9;
    }

    if (c2 == CML_RETROFLEX_SYMBOL) {
        switch (c1) {
            case 'n': *code = 0x1E47;
            break;
            case 'd': *code = 0x1E0D;
            break;
            case 't': *code = 0x1E6D;
            break;
            case 's': *code = 0x1E63;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CML_SYLLABIC_CONSONANT_SYMBOL) {
        switch (c1) {
            case 'l': *code = 0x1E37;
            break;
            case 'r': *code = 0x1E5B;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CML_LONG_SYLLABIC_CONSONANT_SYMBOL) {
        switch (c1) {
            case 'l': *code = 0x1E39;
            break;
            case 'r': *code = 0x1E5D;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CML_LONG_VOCAL_SYMBOL) {
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
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CML_PALATAL_SYMBOL && c1 == 's') {
        *code = 0x015B;
        goto skipTwoChars;
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
            case '=': *code = 0xF0004;
            break;
            default: goto skipOneChar;
        }
        goto skipTwoChars;
    }

    skipOneChar: return 1;
    skipTwoChars: return 2;
}

Cml_tokenStream CmlTokenizer_tokenizationUTF(struct CmlUTF_buffer *utf)
{
    size_t utfLen = CmlUTF_len(utf);
    size_t tokenStreamLen = 0;
    Cml_tokenStream tokenStream = malloc(sizeof(enum Cml_tokens) * (utfLen + 1));

    for (size_t i = 0; ; i++) {
        u_int32_t c1 = CmlUTF_read(utf);
        if (c1 == -1 && errno == ERANGE) {
            break;
        }

        u_int32_t c2 = (CmlUTF_next(utf, 1), CmlUTF_read(utf));
        unsigned short isUseTwoChars = CmlTokenizer_normalizeUTF(c1, c2, &c1) == 2;
        enum Cml_tokens token = Cml_RAW_TOKEN(c1);
        if (c1 == CML_ESCAPE_SYMBOL && !(c2 == -1 && errno == ERANGE)) {
            token = Cml_RAW_TOKEN(c2);
            isUseTwoChars = 2;
            goto pushToken;
        }

        if (c1 >= '0' && c1 <= '9') {
            token = Cml_NUMBER_0_TOKEN + c1 - '0';
        } else {
            switch (c1) {
                case ' ': token = Cml_SPACE_TOKEN;
                break;
                case 'a': token = Cml_VOCAL_A_TOKEN;
                break;
                case 'i': token = Cml_VOCAL_I_TOKEN;
                break;
                case 'u': token = Cml_VOCAL_U_TOKEN;
                break;
                case 'e': token = Cml_VOCAL_SCHWA_TOKEN;
                break;
                case 0x00E9: token = Cml_VOCAL_E_TOKEN;
                break;
                case 'o': token = Cml_VOCAL_O_TOKEN;
                break;
                case 0x1E37: token = Cml_SYLLABIC_CONSONANT_L_TOKEN;
                break;
                case 0x1E5B: token = Cml_SYLLABIC_CONSONANT_R_TOKEN;
                break;
                case 0x0101: token = Cml_LONG_VOCAL_A_TOKEN;
                break;
                case 0x012B: token = Cml_LONG_VOCAL_I_TOKEN;
                break;
                case 0x016B: token = Cml_LONG_VOCAL_U_TOKEN;
                break;
                case 0x0113: token = Cml_LONG_VOCAL_SCHWA_TOKEN;
                break;
                case 0x1E17: token = Cml_LONG_VOCAL_E_TOKEN;
                break;
                case 0x014D: token = Cml_LONG_VOCAL_O_TOKEN;
                break;
                case 0x1E39: token = Cml_LONG_SYLLABIC_CONSONANT_L_TOKEN;
                break;
                case 0x1E5D: token = Cml_LONG_SYLLABIC_CONSONANT_R_TOKEN;
                break;
                case 'h': token = Cml_CONSONANT_H_TOKEN;
                break;
                case 'n': token = Cml_CONSONANT_N_TOKEN;
                break;
                case 'c': token = Cml_CONSONANT_C_TOKEN;
                break;
                case 'r': token = Cml_CONSONANT_R_TOKEN;
                break;
                case 'k': token = Cml_CONSONANT_K_TOKEN;
                break;
                case 'd': token = Cml_CONSONANT_D_TOKEN;
                break;
                case 't': token = Cml_CONSONANT_T_TOKEN;
                break;
                case 's': token = Cml_CONSONANT_S_TOKEN;
                break;
                case 'w': token = Cml_CONSONANT_W_TOKEN;
                break;
                case 'l': token = Cml_CONSONANT_L_TOKEN;
                break;
                case 'm': token = Cml_CONSONANT_M_TOKEN;
                break;
                case 'g': token = Cml_CONSONANT_G_TOKEN;
                break;
                case 'b': token = Cml_CONSONANT_B_TOKEN;
                break;
                case 'p': token = Cml_CONSONANT_P_TOKEN;
                break;
                case 'j': token = Cml_CONSONANT_J_TOKEN;
                break;
                case 'y': token = Cml_CONSONANT_Y_TOKEN;
                break;
                case 0x1E47: token = Cml_RETROFLEX_CONSONANT_N_TOKEN;
                break;
                case 0x1E0D: token = Cml_RETROFLEX_CONSONANT_D_TOKEN;
                break;
                case 0x1E6D: token = Cml_RETROFLEX_CONSONANT_T_TOKEN;
                break;
                case 0x1E63: token = Cml_RETROFLEX_CONSONANT_S_TOKEN;
                break;
                case 0x015B: token = Cml_PALATAL_CONSONANT_S_TOKEN;
                break;
                case ',': token = Cml_PUNCTUATION_CARIK_SIKI_TOKEN;
                break;
                case '.': token = Cml_PUNCTUATION_CARIK_KALIH_TOKEN;
                break;
                case ':': token = Cml_PUNCTUATION_CARIK_PAMUNGKAH_TOKEN;
                break;
                case 0xF0000: token = Cml_PUNCTUATION_PANTEN_TOKEN;
                break;
                case 0xF0001: token = Cml_PUNCTUATION_PASALINAN_TOKEN;
                break;
                case 0xF0002: token = Cml_PUNCTUATION_PAMADA_TOKEN;
                break;
                case 0xF0003: token = Cml_PUNCTUATION_CARIK_AGUNG_TOKEN;
                break;
                case 0xF0004: token = Cml_PUNCTUATION_IDEM_TOKEN;
                break;

            }
        }

        pushToken:
        tokenStream[i] = token;
        tokenStreamLen++;
        CmlUTF_next(utf, isUseTwoChars ? 1 : 0);
    }

    tokenStream[tokenStreamLen] = Cml_END_OF_TOKEN;
    if (utfLen != tokenStreamLen)
        tokenStream = realloc(tokenStream, sizeof(enum Cml_tokens) * (tokenStreamLen + 1));
    return tokenStream;
}
