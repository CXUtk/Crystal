#pragma once

#include <vector>
#include <functional>

namespace crystal
{
	template<typename ... EventArgs>
	class Event
	{
	public:
		using Func = std::function<void(EventArgs ...args)>;

		Event() {}
		~Event() {}

		// Not allowed to return *self
		void operator+=(const Func& handler)
		{
			_eventHandlers.push_back(std::move(handler));
		}

		// Not allowed to return *self
		void operator-=(const Func& handler)
		{
			std::remove(_eventHandlers.begin(), _eventHandlers.end(), handler);
		}

		void Invoke(EventArgs ... args)
		{
			for (auto& handler : _eventHandlers)
			{
				handler(args ...);
			}
		}

	private:
        std::vector<Func> _eventHandlers{};
	};
}