#pragma once
#include "UICommon.h"
#include <Core/Utils/Event.h>
#include <Core/Input/InputCode.h>

namespace crystal
{
    struct UIEventArgs
    {
        UIElement*  Element;
        double      TimeStamp;
    };

    BIT_FLAGS_BEGIN(MouseButtonFlags, uint32_t)
    {
        None = 0,
        LeftButton = (1 << 0),
        RightButton = (1 << 1),
        MiddleButton = (1 << 2),
    };
    BIT_FLAGS_END(MouseButtonFlags);

    struct UIMouseEventArgs : public UIEventArgs
    {
        Vector2i            MousePosScreen;
    };

    struct UIMouseButtonEventArgs : public UIMouseEventArgs
    {
        MouseButtonFlags    ButtonFlags;
    };

    struct UIMouseScrollEventArgs : public UIEventArgs
    {
        Vector2i            MousePosScreen;
    };

    using UIMouseButtonEvent = Event<UIMouseButtonEventArgs>;
    using UIMouseEvent = Event<UIMouseEventArgs>;
    using UIMouseScrollEvent = Event<UIMouseScrollEventArgs>;

    enum class UIEventType
    {
        OnMouseJustPressed,
        OnMouseJustReleased,
        OnMouseEnter,
        OnMouseLeave,
        OnMouseHover,
        OnMouseScroll
    };

    template<UIEventType E>
    struct UIEventDelegate {};

    template<>
    struct UIEventDelegate<UIEventType::OnMouseJustPressed>
    {
        using Func_Type = typename UIMouseButtonEvent::Func;
        using Event_Type = UIMouseButtonEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseJustReleased>
    {
        using Func_Type = typename UIMouseButtonEvent::Func;
        using Event_Type = UIMouseButtonEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseEnter>
    {
        using Func_Type = typename UIMouseEvent::Func;
        using Event_Type = UIMouseEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseLeave>
    {
        using Func_Type = typename UIMouseEvent::Func;
        using Event_Type = UIMouseEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseScroll>
    {
        using Func_Type = typename UIMouseScrollEvent::Func;
        using Event_Type = UIMouseScrollEvent;
    };

    template<UIEventType E>
    using UIEvent = typename UIEventDelegate<E>::Event_Type;

    template<UIEventType E>
    using UIEventListener = typename UIEventDelegate<E>::Func_Type;
}