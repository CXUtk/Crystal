#pragma once
#include <chrono>
#include <Crystal.h>

namespace crystal
{
	class GameTimer
	{
	public:
		GameTimer();
		double GetTimeFromGameStartInSeconds() const;
	private:
		std::chrono::steady_clock::time_point _startTime;
	};
}