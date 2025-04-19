/*
tokenizer.h - Tokenizer

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

#ifndef __TOKENIZER_H
#define __TOKENIZER_H

#include "utf.h"

#define CmlTokenizer_RAW_TOKEN(c) 57 + c
#define CmlTokenizer_IS_RAW_TOKEN(c) c >= 57
#define CmlTokenizer_RETROFLEX_SYMBOL '^'
#define CmlTokenizer_SYLLABIC_CONSONANT_SYMBOL '_'
#define CmlTokenizer_LONG_SYLLABIC_CONSONANT_SYMBOL '*'
#define CmlTokenizer_LONG_VOCAL_SYMBOL '~'
#define CmlTokenizer_PALATAL_SYMBOL '\''
#define CmlTokenizer_ESCAPE_SYMBOL '$'
#define CmlTokenizer_TRANSLITERATION_AS_IS_START_SYMBOL '['
#define CmlTokenizer_TRANSLITERATION_AS_IS_END_SYMBOL ']'

typedef u_int32_t *CmlTokenizer_TokenStream;

enum CmlTokenizer_Token {
    CmlTokenizer_END_OF_TOKEN,
    CmlTokenizer_SPACE_TOKEN,
    CmlTokenizer_VOCAL_A_TOKEN,
    CmlTokenizer_VOCAL_I_TOKEN,
    CmlTokenizer_VOCAL_U_TOKEN,
    CmlTokenizer_VOCAL_E_TOKEN,
    CmlTokenizer_VOCAL_SCHWA_TOKEN,
    CmlTokenizer_VOCAL_O_TOKEN,
    CmlTokenizer_SYLLABIC_CONSONANT_L_TOKEN,
    CmlTokenizer_SYLLABIC_CONSONANT_R_TOKEN,
    CmlTokenizer_LONG_VOCAL_A_TOKEN,
    CmlTokenizer_LONG_VOCAL_I_TOKEN,
    CmlTokenizer_LONG_VOCAL_U_TOKEN,
    CmlTokenizer_LONG_VOCAL_E_TOKEN,
    CmlTokenizer_LONG_VOCAL_SCHWA_TOKEN,
    CmlTokenizer_LONG_VOCAL_O_TOKEN,
    CmlTokenizer_LONG_SYLLABIC_CONSONANT_L_TOKEN,
    CmlTokenizer_LONG_SYLLABIC_CONSONANT_R_TOKEN,
    CmlTokenizer_CONSONANT_H_TOKEN,
    CmlTokenizer_CONSONANT_N_TOKEN,
    CmlTokenizer_CONSONANT_C_TOKEN,
    CmlTokenizer_CONSONANT_R_TOKEN,
    CmlTokenizer_CONSONANT_K_TOKEN,
    CmlTokenizer_CONSONANT_D_TOKEN,
    CmlTokenizer_CONSONANT_T_TOKEN,
    CmlTokenizer_CONSONANT_S_TOKEN,
    CmlTokenizer_CONSONANT_W_TOKEN,
    CmlTokenizer_CONSONANT_L_TOKEN,
    CmlTokenizer_CONSONANT_M_TOKEN,
    CmlTokenizer_CONSONANT_G_TOKEN,
    CmlTokenizer_CONSONANT_B_TOKEN,
    CmlTokenizer_CONSONANT_P_TOKEN,
    CmlTokenizer_CONSONANT_J_TOKEN,
    CmlTokenizer_CONSONANT_Y_TOKEN,
    CmlTokenizer_RETROFLEX_CONSONANT_N_TOKEN,
    CmlTokenizer_RETROFLEX_CONSONANT_D_TOKEN,
    CmlTokenizer_RETROFLEX_CONSONANT_T_TOKEN,
    CmlTokenizer_RETROFLEX_CONSONANT_S_TOKEN,
    CmlTokenizer_PALATAL_CONSONANT_S_TOKEN,
    CmlTokenizer_NUMBER_0_TOKEN,
    CmlTokenizer_NUMBER_1_TOKEN,
    CmlTokenizer_NUMBER_2_TOKEN,
    CmlTokenizer_NUMBER_3_TOKEN,
    CmlTokenizer_NUMBER_4_TOKEN,
    CmlTokenizer_NUMBER_5_TOKEN,
    CmlTokenizer_NUMBER_6_TOKEN,
    CmlTokenizer_NUMBER_7_TOKEN,
    CmlTokenizer_NUMBER_8_TOKEN,
    CmlTokenizer_NUMBER_9_TOKEN,
    CmlTokenizer_PUNCTUATION_CARIK_SIKI_TOKEN,
    CmlTokenizer_PUNCTUATION_CARIK_KALIH_TOKEN,
    CmlTokenizer_PUNCTUATION_CARIK_PAMUNGKAH_TOKEN,
    CmlTokenizer_PUNCTUATION_PANTEN_TOKEN,
    CmlTokenizer_PUNCTUATION_PASALINAN_TOKEN,
    CmlTokenizer_PUNCTUATION_PAMADA_TOKEN,
    CmlTokenizer_PUNCTUATION_CARIK_AGUNG_TOKEN,
    CmlTokenizer_PUNCTUATION_IDEM_TOKEN,
    CmlTokenizer_TRANSLITERATION_AS_IS_START_TOKEN,
    CmlTokenizer_TRANSLITERATION_AS_IS_END_TOKEN
};

size_t CmlTokenizer_preprocess(u_int32_t c1, u_int32_t c2, u_int32_t *p_code);
CmlTokenizer_TokenStream CmlTokenizer_tokenizationUTF(struct CmlUTF_Buffer *p_utf);

#endif
