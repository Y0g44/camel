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

size_t CmlTokenizer_preprocess(u_int32_t c1, u_int32_t c2, u_int32_t *p_code)
{
    *p_code = c1;
    c1 = CmlTokenizer_convertToLowerCase(c1);
    if (c1 == 'x')
        c1 = 0x00E9;

    if (c1 == CmlTokenizer_TRANSLITERATION_AS_IS_START_SYMBOL) {
        *p_code = 0xF0005;
        goto skipOneChar;
    } else if (c1 == CmlTokenizer_TRANSLITERATION_AS_IS_END_SYMBOL) {
        *p_code = 0xF0006;
        goto skipOneChar;
    }

    if (c2 == CmlTokenizer_RETROFLEX_SYMBOL) {
        switch (c1) {
            case 'n': *p_code = 0x1E47;
            break;
            case 'd': *p_code = 0x1E0D;
            break;
            case 't': *p_code = 0x1E6D;
            break;
            case 's': *p_code = 0x1E63;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CmlTokenizer_SYLLABIC_CONSONANT_SYMBOL) {
        switch (c1) {
            case 'l': *p_code = 0x1E37;
            break;
            case 'r': *p_code = 0x1E5B;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CmlTokenizer_LONG_SYLLABIC_CONSONANT_SYMBOL) {
        switch (c1) {
            case 'l': *p_code = 0x1E39;
            break;
            case 'r': *p_code = 0x1E5D;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CmlTokenizer_LONG_VOCAL_SYMBOL) {
        switch (c1) {
            case 'a': *p_code = 0x0101;
            break;
            case 'i': *p_code = 0x012B;
            break;
            case 'u': *p_code = 0x016B;
            break;
            case 'e': *p_code = 0x0113;
            break;
            case 0x00E9: *p_code = 0x1E17;
            break;
            case 'o': *p_code = 0x014D;
            break;
            default: goto skipOneChar;
        }

        goto skipTwoChars;
    } else if (c2 == CmlTokenizer_PALATAL_SYMBOL && c1 == 's') {
        *p_code = 0x015B;
        goto skipTwoChars;
    } else if (c1 == c2) {
        switch (c1) {
            case '+': *p_code = 0xF0000;
            break;
            case '-': *p_code = 0xF0001;
            break;
            case '#': *p_code = 0xF0002;
            break;
            case '/': *p_code = 0xF0003;
            break;
            case '=': *p_code = 0xF0004;
            break;
            default: goto skipOneChar;
        }
        goto skipTwoChars;
    }

    skipOneChar: return 1;
    skipTwoChars: return 2;
}

CmlTokenizer_TokenStream CmlTokenizer_tokenizationUTF(struct CmlUTF_Buffer *p_utf)
{
    size_t utfLen = CmlUTF_len(p_utf);
    size_t tokenStreamLen = 0;
    CmlTokenizer_TokenStream tokenStream = malloc(sizeof(enum CmlTokenizer_Token) * (utfLen + 1));

    for (size_t i = 0; ; i++) {
        u_int32_t c1 = CmlUTF_read(p_utf);
        if (c1 == -1 && errno == ERANGE)
            break;

        u_int32_t c2 = (CmlUTF_next(p_utf, 1), CmlUTF_read(p_utf));
        unsigned short isUseTwoChars = CmlTokenizer_preprocess(c1, c2, &c1) == 2;
        enum CmlTokenizer_Token token = CmlTokenizer_RAW_TOKEN(c1);
        if (c1 == CmlTokenizer_ESCAPE_SYMBOL) {
            token = CmlTokenizer_RAW_TOKEN(c2);
            isUseTwoChars = 2;
            goto pushToken;
        }

        if (c1 >= '0' && c1 <= '9') {
            token = CmlTokenizer_NUMBER_0_TOKEN + c1 - '0';
        } else {
            switch (c1) {
                case ' ': token = CmlTokenizer_SPACE_TOKEN;
                break;
                case 'a': token = CmlTokenizer_VOCAL_A_TOKEN;
                break;
                case 'i': token = CmlTokenizer_VOCAL_I_TOKEN;
                break;
                case 'u': token = CmlTokenizer_VOCAL_U_TOKEN;
                break;
                case 'e': token = CmlTokenizer_VOCAL_SCHWA_TOKEN;
                break;
                case 0x00E9: token = CmlTokenizer_VOCAL_E_TOKEN;
                break;
                case 'o': token = CmlTokenizer_VOCAL_O_TOKEN;
                break;
                case 0x1E37: token = CmlTokenizer_SYLLABIC_CONSONANT_L_TOKEN;
                break;
                case 0x1E5B: token = CmlTokenizer_SYLLABIC_CONSONANT_R_TOKEN;
                break;
                case 0x0101: token = CmlTokenizer_LONG_VOCAL_A_TOKEN;
                break;
                case 0x012B: token = CmlTokenizer_LONG_VOCAL_I_TOKEN;
                break;
                case 0x016B: token = CmlTokenizer_LONG_VOCAL_U_TOKEN;
                break;
                case 0x0113: token = CmlTokenizer_LONG_VOCAL_SCHWA_TOKEN;
                break;
                case 0x1E17: token = CmlTokenizer_LONG_VOCAL_E_TOKEN;
                break;
                case 0x014D: token = CmlTokenizer_LONG_VOCAL_O_TOKEN;
                break;
                case 0x1E39: token = CmlTokenizer_LONG_SYLLABIC_CONSONANT_L_TOKEN;
                break;
                case 0x1E5D: token = CmlTokenizer_LONG_SYLLABIC_CONSONANT_R_TOKEN;
                break;
                case 'h': token = CmlTokenizer_CONSONANT_H_TOKEN;
                break;
                case 'n': token = CmlTokenizer_CONSONANT_N_TOKEN;
                break;
                case 'c': token = CmlTokenizer_CONSONANT_C_TOKEN;
                break;
                case 'r': token = CmlTokenizer_CONSONANT_R_TOKEN;
                break;
                case 'k': token = CmlTokenizer_CONSONANT_K_TOKEN;
                break;
                case 'd': token = CmlTokenizer_CONSONANT_D_TOKEN;
                break;
                case 't': token = CmlTokenizer_CONSONANT_T_TOKEN;
                break;
                case 's': token = CmlTokenizer_CONSONANT_S_TOKEN;
                break;
                case 'w': token = CmlTokenizer_CONSONANT_W_TOKEN;
                break;
                case 'l': token = CmlTokenizer_CONSONANT_L_TOKEN;
                break;
                case 'm': token = CmlTokenizer_CONSONANT_M_TOKEN;
                break;
                case 'g': token = CmlTokenizer_CONSONANT_G_TOKEN;
                break;
                case 'b': token = CmlTokenizer_CONSONANT_B_TOKEN;
                break;
                case 'p': token = CmlTokenizer_CONSONANT_P_TOKEN;
                break;
                case 'j': token = CmlTokenizer_CONSONANT_J_TOKEN;
                break;
                case 'y': token = CmlTokenizer_CONSONANT_Y_TOKEN;
                break;
                case 0x1E47: token = CmlTokenizer_RETROFLEX_CONSONANT_N_TOKEN;
                break;
                case 0x1E0D: token = CmlTokenizer_RETROFLEX_CONSONANT_D_TOKEN;
                break;
                case 0x1E6D: token = CmlTokenizer_RETROFLEX_CONSONANT_T_TOKEN;
                break;
                case 0x1E63: token = CmlTokenizer_RETROFLEX_CONSONANT_S_TOKEN;
                break;
                case 0x015B: token = CmlTokenizer_PALATAL_CONSONANT_S_TOKEN;
                break;
                case ',': token = CmlTokenizer_PUNCTUATION_CARIK_SIKI_TOKEN;
                break;
                case '.': token = CmlTokenizer_PUNCTUATION_CARIK_KALIH_TOKEN;
                break;
                case ':': token = CmlTokenizer_PUNCTUATION_CARIK_PAMUNGKAH_TOKEN;
                break;
                case 0xF0000: token = CmlTokenizer_PUNCTUATION_PANTEN_TOKEN;
                break;
                case 0xF0001: token = CmlTokenizer_PUNCTUATION_PASALINAN_TOKEN;
                break;
                case 0xF0002: token = CmlTokenizer_PUNCTUATION_PAMADA_TOKEN;
                break;
                case 0xF0003: token = CmlTokenizer_PUNCTUATION_CARIK_AGUNG_TOKEN;
                break;
                case 0xF0004: token = CmlTokenizer_PUNCTUATION_IDEM_TOKEN;
                break;
                case 0xF0005: token = CmlTokenizer_TRANSLITERATION_AS_IS_START_TOKEN;
                break;
                case 0xF0006: token = CmlTokenizer_TRANSLITERATION_AS_IS_END_TOKEN;
                break;
            }
        }

        pushToken:
        tokenStream[i] = token;
        tokenStreamLen++;
        CmlUTF_next(p_utf, isUseTwoChars ? 1 : 0);
    }

    tokenStream[tokenStreamLen] = CmlTokenizer_END_OF_TOKEN;
    return utfLen != tokenStreamLen
        ? realloc(tokenStream, sizeof(enum CmlTokenizer_Token) * (tokenStreamLen + 1))
        : tokenStream;
}
