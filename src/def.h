/*
def.h - Type definitions

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

#include <sys/types.h>

#define Cml_RAW_TOKEN(c) 57 + c
#define Cml_IS_RAW_TOKEN(c) c >= 57
#define CML_RETROFLEX_SYMBOL '`'
#define CML_SYLLABIC_CONSONANT_SYMBOL '!'
#define CML_LONG_SYLLABIC_CONSONANT_SYMBOL '*'
#define CML_LONG_VOCAL_SYMBOL '~'
#define CML_PALATAL_SYMBOL '\''
#define CML_ESCAPE_SYMBOL '$'

typedef u_int32_t *Cml_tokenStream;

enum Cml_tokens {
    Cml_SPACE_TOKEN,
    Cml_VOCAL_A_TOKEN,
    Cml_VOCAL_I_TOKEN,
    Cml_VOCAL_U_TOKEN,
    Cml_VOCAL_E_TOKEN,
    Cml_VOCAL_SCHWA_TOKEN,
    Cml_VOCAL_O_TOKEN,
    Cml_SYLLABIC_CONSONANT_L_TOKEN,
    Cml_SYLLABIC_CONSONANT_R_TOKEN,
    Cml_LONG_VOCAL_A_TOKEN,
    Cml_LONG_VOCAL_I_TOKEN,
    Cml_LONG_VOCAL_U_TOKEN,
    Cml_LONG_VOCAL_E_TOKEN,
    Cml_LONG_VOCAL_SCHWA_TOKEN,
    Cml_LONG_VOCAL_O_TOKEN,
    Cml_LONG_SYLLABIC_CONSONANT_L_TOKEN,
    Cml_LONG_SYLLABIC_CONSONANT_R_TOKEN,
    Cml_CONSONANT_H_TOKEN,
    Cml_CONSONANT_N_TOKEN,
    Cml_CONSONANT_C_TOKEN,
    Cml_CONSONANT_R_TOKEN,
    Cml_CONSONANT_K_TOKEN,
    Cml_CONSONANT_D_TOKEN,
    Cml_CONSONANT_T_TOKEN,
    Cml_CONSONANT_S_TOKEN,
    Cml_CONSONANT_W_TOKEN,
    Cml_CONSONANT_L_TOKEN,
    Cml_CONSONANT_M_TOKEN,
    Cml_CONSONANT_G_TOKEN,
    Cml_CONSONANT_B_TOKEN,
    Cml_CONSONANT_P_TOKEN,
    Cml_CONSONANT_J_TOKEN,
    Cml_CONSONANT_Y_TOKEN,
    Cml_RETROFLEX_CONSONANT_N_TOKEN,
    Cml_RETROFLEX_CONSONANT_D_TOKEN,
    Cml_RETROFLEX_CONSONANT_T_TOKEN,
    Cml_RETROFLEX_CONSONANT_S_TOKEN,
    Cml_PALATAL_CONSONANT_S_TOKEN,
    Cml_NUMBER_0_TOKEN,
    Cml_NUMBER_1_TOKEN,
    Cml_NUMBER_2_TOKEN,
    Cml_NUMBER_3_TOKEN,
    Cml_NUMBER_4_TOKEN,
    Cml_NUMBER_5_TOKEN,
    Cml_NUMBER_6_TOKEN,
    Cml_NUMBER_7_TOKEN,
    Cml_NUMBER_8_TOKEN,
    Cml_NUMBER_9_TOKEN,
    Cml_PUNCTUATION_CARIK_SIKI_TOKEN,
    Cml_PUNCTUATION_CARIK_KALIH_TOKEN,
    Cml_PUNCTUATION_CARIK_PAMUNGKAH_TOKEN,
    Cml_PUNCTUATION_PANTEN_TOKEN,
    Cml_PUNCTUATION_PASALINAN_TOKEN,
    Cml_PUNCTUATION_PAMADA_TOKEN,
    Cml_PUNCTUATION_CARIK_AGUNG_TOKEN,
    Cml_PUNCTUATION_IDE_TOKENM
};

enum Cml_endianness {
    Cml_endianness_BE,
    Cml_endianness_LE,
};

#endif
