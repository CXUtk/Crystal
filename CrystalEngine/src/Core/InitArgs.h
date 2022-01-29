#pragma once
#include <string>

namespace crystal
{
	constexpr int MAX_WINDOW_TITLE_LENGTH = 128;

	struct InitArgs
	{
		uint32_t WindowWidth, WindowHeight;
		uint32_t FPSLimit;
		bool WindowResizable;
		char WindowTitle[MAX_WINDOW_TITLE_LENGTH];
		bool Enable4xMSAA;
	};
}