#pragma once
#include <cstdio>
#include <mutex>

namespace crystal
{
	enum class SeverityLevel
	{
		Info,
		Warning,
		Error,
		Debug,

		__COUNT
	};

	class GlobalLogger
	{
	public:
		static void Log(SeverityLevel slevel, const char* format, ...);

	private:
		static void pushLog(SeverityLevel slevel, const char* badge, const char* format, va_list ap);
	};
}