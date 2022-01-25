#pragma once
#include <Platforms/Platforms.h>
#include <Utils/Event.h>
#include <Utils/Geometry.h>

namespace crystal
{
	class IGameWindow
	{
	public:
		virtual ~IGameWindow() = 0 {};

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual bool ShouldClose() const = 0;
		virtual void PollEvents() = 0;

		virtual Point2i GetMousePos() const = 0;
		virtual Vector2i GetWindowSize() const = 0;

		virtual void AppendOnResizeEvent(Event<Vector2i>::Func eventHandler) = 0;
	};
}