#pragma once
#include <chrono>
#include <Crystal.h>

namespace crystal
{
	class GameTimer
	{
	public:
		/**
		 * @brief Get current physical time_since_epoch immediately
		 * @return Time in seconds
		*/
		static double GetCurrentTime();

		GameTimer();

		/**
		 * @brief Get the logical time from the start of the game to this frame
		 * @return Time in seconds
		*/
		double GetLogicTime() const;
		
		/**
		 * @brief Get the physical time from the start of the game to this frame
		 * @return Time in seconds
		*/
		double GetPhysicalTime() const;

		/**
		 * @brief Get the logical delta time from last frame to this frame
		 * @return Time in seconds
		*/
		double GetLogicalDeltaTime() const;

		/**
		 * @brief Get the physical delta time from last frame to this frame
		 * @return 
		*/
		double GetPhysicalDeltaTime() const;

		/**
		 * @brief Sample game time for this frame
		*/
		void Sample();

		/**
		 * @brief Update logical time
		*/
		void Tick();

		/**
		 * @brief Stop the timer
		*/
		void Stop();

		/**
		 * @brief Resume the timer
		*/
		void Resume();

		/**
		 * @brief Start the timer
		*/
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