#pragma once
#include <stdint.h>

namespace crystal
{
	enum KeyMODState : int
	{
		CRYSTAL_MOD_SHIFT = 1 << 0, 
		CRYSTAL_MOD_CTRL = 1 << 1,
		CRYSTAL_MOD_ALT = 1 << 2,
		CRYSTAL_MOD_CAPS_LOCK = 1 << 3,
		CRYSTAL_MOD_NUM_LOCK = 1 << 3,
	};

	enum class KeyCode
	{
		UNKNOWN,

		CRYSTAL_A_KEY,
		CRYSTAL_B_KEY,
		CRYSTAL_C_KEY, 
		CRYSTAL_D_KEY,
		CRYSTAL_E_KEY,
		CRYSTAL_F_KEY,
		CRYSTAL_G_KEY,
		CRYSTAL_H_KEY,
		CRYSTAL_I_KEY,
		CRYSTAL_J_KEY,
		CRYSTAL_K_KEY,
		CRYSTAL_L_KEY,
		CRYSTAL_M_KEY,
		CRYSTAL_N_KEY,
		CRYSTAL_O_KEY,
		CRYSTAL_P_KEY,
		CRYSTAL_Q_KEY,
		CRYSTAL_R_KEY,
		CRYSTAL_S_KEY,
		CRYSTAL_T_KEY,
		CRYSTAL_U_KEY,
		CRYSTAL_V_KEY,
		CRYSTAL_W_KEY,
		CRYSTAL_X_KEY,
		CRYSTAL_Y_KEY,
		CRYSTAL_Z_KEY,

        CRYSTAL_0_KEY,
        CRYSTAL_1_KEY,
        CRYSTAL_2_KEY,
        CRYSTAL_3_KEY,
        CRYSTAL_4_KEY,
        CRYSTAL_5_KEY,
        CRYSTAL_6_KEY,
        CRYSTAL_7_KEY,
        CRYSTAL_8_KEY,
        CRYSTAL_9_KEY,

        CRYSTAL_PLUS_KEY,
        CRYSTAL_MINUS_KEY,
        CRYSTAL_TILDE_KEY,
        CRYSTAL_LEFT_BRACKET_KEY,
        CRYSTAL_RIGHT_BRACKET_KEY,
        CRYSTAL_LEFT_SLASH_KEY,
        CRYSTAL_SEMICOLON_KEY,
        CRYSTAL_QUOTE_KEY,
        CRYSTAL_COMMA_KEY,
        CRYSTAL_DOT_KEY,
        CRYSTAL_RIGHT_SLASH_KEY,

        CRYSTAL_SPACE_KEY,

		CRYSTAL_UP_KEY,
		CRYSTAL_DOWN_KEY,
		CRYSTAL_LEFT_KEY,
		CRYSTAL_RIGHT_KEY,

        CRYSTAL_ENTER_KEY,
		CRYSTAL_LEFT_CTRL_KEY,
		CRYSTAL_LEFT_ALT_KEY,
		CRYSTAL_RIGHT_CTRL_KEY,
		CRYSTAL_RIGHT_ALT_KEY,

		CRYSTAL_ESC_KEY,
        CRYSTAL_BACK_KEY,

		__COUNT,
	};

	enum class MouseButtonCode
	{
		EMPTY,

		LEFT_BUTTON,
		RIGHT_BUTTON,
		MIDDLE_BUTTON,
		X_BUTTON,

		__COUNT,
	};

	enum class InputAction
	{
		RELEASE,
		PRESS,
		REPEAT,

		__COUNT,
	};

	struct MouseButtonEventArgs
	{
		MouseButtonCode ButtonCode;
		InputAction Action;
		int Mods;
	};

	struct KeyEventArgs
	{
		KeyCode KeyCode;
		InputAction Action;
		KeyMODState Mods;
	};

    struct CharInputArgs
    {
        char32_t Code;
    };

    class InputCommon
    {
    public:
        static char KeyCodeToCharMapLower(KeyCode code);
        static char KeyCodeToCharMapUpper(KeyCode code);
    };
}