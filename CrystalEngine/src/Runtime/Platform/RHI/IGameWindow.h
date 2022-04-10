#pragma once
#include <Core/Input/InputCommon.h>
#include <Core/Utils/Event.h>
#include <Core/Utils/Geometry.h>
#include "IClipBoard.h"

namespace crystal
{
	class IGameWindow
	{
	public:
		using OnResizeEvent =				Event<Vector2i>;
		using OnMouseScrollEvent =			Event<Vector2f>;
		using OnMouseButtonChangeEvent =	Event<MouseButtonEventArgs>;
		using OnKeyChangeEvent =			Event<KeyEventArgs>;
        using CharInputEvent =              Event<CharInputArgs>;

		virtual ~IGameWindow() = 0 {};

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual bool ShouldClose() const = 0;
		virtual bool IsPaused() const = 0;
		virtual void PollEvents() = 0;

		virtual Point2i GetMousePos() const = 0;
		virtual Vector2i GetWindowSize() const = 0;

		virtual void AppendOnResizeEvent(OnResizeEvent::Func eventHandler) = 0;
		virtual void AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler) = 0;
		virtual void AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler) = 0;
		virtual void AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler) = 0;
        virtual void AddCharInputEventListener(CharInputEvent::Func eventHandler) = 0;

        virtual IClipBoard* GetClipBoard() const = 0;
	};
}