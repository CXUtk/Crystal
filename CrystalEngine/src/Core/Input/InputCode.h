#pragma once

namespace crystal
{
	enum class KeyCode
	{
		EMPTY,

		CRYSTAL_W_KEY,
		CRYSTAL_A_KEY,
		CRYSTAL_S_KEY,
		CRYSTAL_D_KEY,
		CRYSTAL_M_KEY,

		CRYSTAL_UP_KEY,
		CRYSTAL_DOWN_KEY,
		CRYSTAL_LEFT_KEY,
		CRYSTAL_RIGHT_KEY,

		CRYSTAL_ENTER_KEY,
		CRYSTAL_LEFT_CTRL_KEY,
		CRYSTAL_LEFT_ALT_KEY,
		CRYSTAL_RIGHT_CTRL_KEY,
		CRYSTAL_RIGHT_ALT_KEY,

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

		CRYSTAL_SPACE_KEY,
		CRYSTAL_ESC_KEY,

		__COUNT,
	};

	enum class MouseButtonCode
	{
		EMPTY,

		LEFT_BUTTON,
		RIGHT_BUTTON,
		MIDDLE_BUTTON,

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
		int mods;
	};

	struct KeyEventArgs
	{
		KeyCode KeyCode;
		InputAction Action;
		int mods;
	};
}