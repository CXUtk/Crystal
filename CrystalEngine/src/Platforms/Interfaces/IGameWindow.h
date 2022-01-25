#pragma once
#include <Platforms/Platforms.h>
#include <Core/Input/InputCode.h>
#include <Utils/Event.h>
#include <Utils/Geometry.h>


namespace crystal
{
	class IGameWindow
	{
	public:
		using OnResizeEvent =				Event<Vector2i>;
		using OnMouseScrollEvent =			Event<Vector2f>;
		using OnMouseButtonChangeEvent =	Event<int, int, int>;
		using OnKeyChangeEvent =			Event<int, int, int, int>;

		virtual ~IGameWindow() = 0 {};

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual bool ShouldClose() const = 0;
		virtual void PollEvents() = 0;

		virtual Point2i GetMousePos() const = 0;
		virtual Vector2i GetWindowSize() const = 0;

		virtual void AppendOnResizeEvent(OnResizeEvent::Func eventHandler) = 0;
		virtual void AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler) = 0;
		virtual void AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler) = 0;
		virtual void AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler) = 0;
	};
}