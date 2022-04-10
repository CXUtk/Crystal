#include "InputCommon.h"

namespace crystal
{
    char InputCommon::KeyCodeToCharMapLower(KeyCode code)
    {
        switch (code)
        {
        case crystal::KeyCode::CRYSTAL_A_KEY:
        case crystal::KeyCode::CRYSTAL_B_KEY:
        case crystal::KeyCode::CRYSTAL_C_KEY:
        case crystal::KeyCode::CRYSTAL_D_KEY:
        case crystal::KeyCode::CRYSTAL_E_KEY:
        case crystal::KeyCode::CRYSTAL_F_KEY:
        case crystal::KeyCode::CRYSTAL_G_KEY:
        case crystal::KeyCode::CRYSTAL_H_KEY:
        case crystal::KeyCode::CRYSTAL_I_KEY:
        case crystal::KeyCode::CRYSTAL_J_KEY:
        case crystal::KeyCode::CRYSTAL_K_KEY:
        case crystal::KeyCode::CRYSTAL_L_KEY:
        case crystal::KeyCode::CRYSTAL_M_KEY:
        case crystal::KeyCode::CRYSTAL_N_KEY:
        case crystal::KeyCode::CRYSTAL_O_KEY:
        case crystal::KeyCode::CRYSTAL_P_KEY:
        case crystal::KeyCode::CRYSTAL_Q_KEY:
        case crystal::KeyCode::CRYSTAL_R_KEY:
        case crystal::KeyCode::CRYSTAL_S_KEY:
        case crystal::KeyCode::CRYSTAL_T_KEY:
        case crystal::KeyCode::CRYSTAL_U_KEY:
        case crystal::KeyCode::CRYSTAL_V_KEY:
        case crystal::KeyCode::CRYSTAL_W_KEY:
        case crystal::KeyCode::CRYSTAL_X_KEY:
        case crystal::KeyCode::CRYSTAL_Y_KEY:
        case crystal::KeyCode::CRYSTAL_Z_KEY:
            return 'a' + ((int)code - (int)KeyCode::CRYSTAL_A_KEY);
        case crystal::KeyCode::CRYSTAL_0_KEY:
        case crystal::KeyCode::CRYSTAL_1_KEY:
        case crystal::KeyCode::CRYSTAL_2_KEY:
        case crystal::KeyCode::CRYSTAL_3_KEY:
        case crystal::KeyCode::CRYSTAL_4_KEY:
        case crystal::KeyCode::CRYSTAL_5_KEY:
        case crystal::KeyCode::CRYSTAL_6_KEY:
        case crystal::KeyCode::CRYSTAL_7_KEY:
        case crystal::KeyCode::CRYSTAL_8_KEY:
        case crystal::KeyCode::CRYSTAL_9_KEY:
            return '0' + ((int)code - (int)KeyCode::CRYSTAL_0_KEY);
        case crystal::KeyCode::CRYSTAL_PLUS_KEY:
            return '=';
        case crystal::KeyCode::CRYSTAL_MINUS_KEY:
            return '-';
        case crystal::KeyCode::CRYSTAL_TILDE_KEY:
            return '`';
        case crystal::KeyCode::CRYSTAL_LEFT_BRACKET_KEY:
            return '[';
        case crystal::KeyCode::CRYSTAL_RIGHT_BRACKET_KEY:
            return ']';
        case crystal::KeyCode::CRYSTAL_LEFT_SLASH_KEY:
            return '\\';
        case crystal::KeyCode::CRYSTAL_SEMICOLON_KEY:
            return ';';
        case crystal::KeyCode::CRYSTAL_QUOTE_KEY:
            return '\'';
        case crystal::KeyCode::CRYSTAL_COMMA_KEY:
            return ',';
        case crystal::KeyCode::CRYSTAL_DOT_KEY:
            return '.';
        case crystal::KeyCode::CRYSTAL_RIGHT_SLASH_KEY:
            return '/';
        case crystal::KeyCode::CRYSTAL_SPACE_KEY:
            return ' ';
        default:
            break;
        }
        return '\0';
    }

    char InputCommon::KeyCodeToCharMapUpper(KeyCode code)
    {
        switch (code)
        {
        case crystal::KeyCode::CRYSTAL_A_KEY:
        case crystal::KeyCode::CRYSTAL_B_KEY:
        case crystal::KeyCode::CRYSTAL_C_KEY:
        case crystal::KeyCode::CRYSTAL_D_KEY:
        case crystal::KeyCode::CRYSTAL_E_KEY:
        case crystal::KeyCode::CRYSTAL_F_KEY:
        case crystal::KeyCode::CRYSTAL_G_KEY:
        case crystal::KeyCode::CRYSTAL_H_KEY:
        case crystal::KeyCode::CRYSTAL_I_KEY:
        case crystal::KeyCode::CRYSTAL_J_KEY:
        case crystal::KeyCode::CRYSTAL_K_KEY:
        case crystal::KeyCode::CRYSTAL_L_KEY:
        case crystal::KeyCode::CRYSTAL_M_KEY:
        case crystal::KeyCode::CRYSTAL_N_KEY:
        case crystal::KeyCode::CRYSTAL_O_KEY:
        case crystal::KeyCode::CRYSTAL_P_KEY:
        case crystal::KeyCode::CRYSTAL_Q_KEY:
        case crystal::KeyCode::CRYSTAL_R_KEY:
        case crystal::KeyCode::CRYSTAL_S_KEY:
        case crystal::KeyCode::CRYSTAL_T_KEY:
        case crystal::KeyCode::CRYSTAL_U_KEY:
        case crystal::KeyCode::CRYSTAL_V_KEY:
        case crystal::KeyCode::CRYSTAL_W_KEY:
        case crystal::KeyCode::CRYSTAL_X_KEY:
        case crystal::KeyCode::CRYSTAL_Y_KEY:
        case crystal::KeyCode::CRYSTAL_Z_KEY:
            return 'A' + ((int)code - (int)KeyCode::CRYSTAL_A_KEY);
        case crystal::KeyCode::CRYSTAL_0_KEY:
            return ')';
        case crystal::KeyCode::CRYSTAL_1_KEY:
            return '!';
        case crystal::KeyCode::CRYSTAL_2_KEY:
            return '@';
        case crystal::KeyCode::CRYSTAL_3_KEY:
            return '#';
        case crystal::KeyCode::CRYSTAL_4_KEY:
            return '$';
        case crystal::KeyCode::CRYSTAL_5_KEY:
            return '%';
        case crystal::KeyCode::CRYSTAL_6_KEY:
            return '^';
        case crystal::KeyCode::CRYSTAL_7_KEY:
            return '&';
        case crystal::KeyCode::CRYSTAL_8_KEY:
            return '*';
        case crystal::KeyCode::CRYSTAL_9_KEY:
            return '(';
        case crystal::KeyCode::CRYSTAL_PLUS_KEY:
            return '+';
        case crystal::KeyCode::CRYSTAL_MINUS_KEY:
            return '_';
        case crystal::KeyCode::CRYSTAL_TILDE_KEY:
            return '~';
        case crystal::KeyCode::CRYSTAL_LEFT_BRACKET_KEY:
            return '{';
        case crystal::KeyCode::CRYSTAL_RIGHT_BRACKET_KEY:
            return '}';
        case crystal::KeyCode::CRYSTAL_LEFT_SLASH_KEY:
            return '|';
        case crystal::KeyCode::CRYSTAL_SEMICOLON_KEY:
            return ':';
        case crystal::KeyCode::CRYSTAL_QUOTE_KEY:
            return '"';
        case crystal::KeyCode::CRYSTAL_COMMA_KEY:
            return '<';
        case crystal::KeyCode::CRYSTAL_DOT_KEY:
            return '>';
        case crystal::KeyCode::CRYSTAL_RIGHT_SLASH_KEY:
            return '?';
        case crystal::KeyCode::CRYSTAL_SPACE_KEY:
            return ' ';
        default:
            break;
        }
        return '\0';
    }
}
