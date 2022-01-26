#include "GameTimer.h"
using namespace std::chrono;
namespace crystal
{
	GameTimer::GameTimer()
	{
		_startTime = steady_clock::now();
	}
	double GameTimer::GetTimeFromGameStartInSeconds() const
	{
		return duration_cast<duration<double>>(steady_clock::now() - _startTime).count();
	}
}