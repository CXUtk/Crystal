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
		double GetCurrentTime() const;
		double GetDeltaTime() const;

		void Tick();
		void Stop();
		void Start();

	private:
		// Time since last frame
		std::chrono::nanoseconds m_deltaTime;
		// Current time stamp
		std::chrono::steady_clock::time_point m_curTime;
		// Time stamp of last frame
		std::chrono::steady_clock::time_point m_prevTime;
		// Time stamp at initialization
		std::chrono::steady_clock::time_point m_initTime;
		// Time stamp at pause
		std::chrono::steady_clock::time_point m_stopTime;
		// Total time at pause
		std::chrono::nanoseconds m_totalStopTime;

		
		bool m_stopped = false;		// Is the timer paused?
	};
}