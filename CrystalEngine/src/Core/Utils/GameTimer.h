#pragma once
#include <chrono>
#include <Crystal.h>

namespace crystal
{
	class GameTimer
	{
	public:
		static double GetCurrentTime();

		GameTimer();
		double GetLogicTime() const;
		double GetPhysicalTime() const;
		double GetLogicalDeltaTime() const;
		double GetPhysicalDeltaTime() const;
		bool isStopped() const { return m_stopped; }

		void Sample();
		void Tick();
		void Stop();
		void Resume();
		void Start();

	private:
		// Time since last frame
		std::chrono::nanoseconds m_physDeltaTime;
		// Current time stamp
		std::chrono::steady_clock::time_point m_logicalCurTime{};
		// Logical time stamp of last frame
		std::chrono::steady_clock::time_point m_logicalPrevTime{};
		// Logical time stamp at initialization
		std::chrono::steady_clock::time_point m_physStartTime{};
		// Physical time stamp at pause
		std::chrono::steady_clock::time_point m_physStopTime{};
		// Physical time stamp at sampling stage
		std::chrono::steady_clock::time_point m_physCurTime{};
		// Physical time stamp of last frame
		std::chrono::steady_clock::time_point m_physPrevTime{};
		// Total time at pause
		std::chrono::nanoseconds m_totalStopDuration;

		
		bool m_stopped = false;		// Is the timer paused?
	};
}