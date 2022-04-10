#include "GameTimer.h"
using namespace std::chrono;
namespace crystal
{
	double GameTimer::GetCurrentTime()
	{
		return duration_cast<duration<double>>(steady_clock::now().time_since_epoch()).count();
	}
	GameTimer::GameTimer()
	{
		m_totalStopDuration = m_physDeltaTime = nanoseconds(0LL);
	}

	double GameTimer::GetLogicTime() const
	{
		return duration_cast<duration<double>>(m_logicalCurTime - m_physStartTime).count();
	}

	double GameTimer::GetPhysicalTime() const
	{
		return duration_cast<duration<double>>(m_physCurTime - m_physStartTime).count();
	}

	double GameTimer::GetLogicalDeltaTime() const
	{
		return duration_cast<duration<double>>(m_logicalCurTime - m_logicalPrevTime).count();
	}

	double GameTimer::GetPhysicalDeltaTime() const
	{
		return duration_cast<duration<double>>(m_physDeltaTime).count();
	}

	void GameTimer::Sample()
	{
		m_physCurTime = steady_clock::now();
		m_physDeltaTime = m_physCurTime - m_physPrevTime;
		m_physPrevTime = m_physCurTime;
		if (m_physDeltaTime.count() < 0)
		{
			m_physDeltaTime = nanoseconds(0LL);
		}
	}

	void GameTimer::Tick()
	{
		if (m_stopped)
		{
			return;
		}
		m_logicalPrevTime = m_logicalCurTime;
		m_logicalCurTime += m_physDeltaTime;
	}

	void GameTimer::Stop()
	{
		if (m_stopped) return;
		m_physStopTime = m_physCurTime;
		m_stopped = true;
	}

	void GameTimer::Resume()
	{
		if (!m_stopped) return;
		m_totalStopDuration += m_physCurTime - m_physStopTime;
		m_stopped = false;
	}

	void GameTimer::Start()
	{
		m_physPrevTime = m_physCurTime = m_logicalCurTime = 
			m_logicalPrevTime = m_physStartTime = m_physStopTime = steady_clock::now();
	}
}