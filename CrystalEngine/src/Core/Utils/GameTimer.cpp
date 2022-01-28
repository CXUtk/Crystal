#include "GameTimer.h"
using namespace std::chrono;
namespace crystal
{
	GameTimer::GameTimer()
	{
		m_totalStopTime = m_deltaTime = nanoseconds(0LL);
		auto now = steady_clock::now();
		m_initTime = m_prevTime = m_curTime = now;
	}
	double GameTimer::GetTimeFromGameStartInSeconds() const
	{
		return duration_cast<duration<double>>(steady_clock::now() - m_initTime - m_totalStopTime).count();
	}
	double GameTimer::GetCurrentTime() const
	{
		return duration_cast<duration<double>>(steady_clock::now().time_since_epoch()).count();
	}
	double GameTimer::GetDeltaTime() const
	{
		return duration_cast<duration<double>>(m_deltaTime).count();
	}
	void GameTimer::Tick()
	{
		if (m_stopped)
		{
			m_deltaTime = nanoseconds(0LL);
			return;
		}
		m_curTime = steady_clock::now();
		m_deltaTime = m_curTime - m_prevTime;
		m_prevTime = m_curTime;

		if (m_deltaTime.count() < 0)
		{
			m_deltaTime = nanoseconds(0LL);
		}
	}
	void GameTimer::Stop()
	{
		if (m_stopped) return;
		m_stopTime = steady_clock::now();
		m_stopped = true;
	}
	void GameTimer::Start()
	{
		if (!m_stopped) return;
		m_stopped = false;
		auto stopTime = m_curTime - m_stopTime;
		m_totalStopTime += stopTime;
		m_prevTime += stopTime;
	}
}