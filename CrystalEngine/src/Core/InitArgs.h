#pragma once
#include <string>

namespace crystal
{
	constexpr int MAX_WINDOW_TITLE_LENGTH = 128;

	struct InitArgs
	{
		uint32_t WindowWidth, WindowHeight;
		bool WindowResizable;
		char WindowTitle[MAX_WINDOW_TITLE_LENGTH];
	};
}