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

    struct UIUpdateEventArgs : public UIEventArgs
    {
        const GameTimer*          GameTimer;
    };

    struct UIDrawEventArgs : public UIEventArgs
    {
        const GameTimer*          GameTimer;
        const RenderPayload*      Payload;
    };

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
        Vector2f            Value;
    };

    template<typename T>
    struct UIValueChangeEventArgs
    {
        UIElement*  Element;
        double      TimeStamp;
        T           Value;
    };

    using UIActionEvent = Event< UIEventArgs>;
    using UIMouseButtonEvent = Event<UIMouseButtonEventArgs>;
    using UIMouseEvent = Event<UIMouseEventArgs>;
    using UIMouseScrollEvent = Event<UIMouseScrollEventArgs>;
    using UIDrawEvent = Event<UIDrawEventArgs>;
    using UIUpdateEvent = Event<UIUpdateEventArgs>;
    template<typename T>
    using UIValueChangeEvent = Event<UIValueChangeEventArgs<T>>;

    enum class UIEventType
    {
        OnMouseJustPressed,
        OnMouseJustReleased,
        OnMouseClicked,
        OnMouseDoubleClicked,
        OnMouseEnter,
        OnMouseLeave,
        OnMouseHover,
        OnMouseScroll,
        PostUpdate,
        PostDraw,
        OnFocused,
        OnUnFocused
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
    struct UIEventDelegate<UIEventType::OnMouseClicked>
    {
        using Func_Type = typename UIMouseButtonEvent::Func;
        using Event_Type = UIMouseButtonEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseDoubleClicked>
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

    template<>
    struct UIEventDelegate<UIEventType::PostUpdate>
    {
        using Func_Type = typename UIUpdateEvent::Func;
        using Event_Type = UIUpdateEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::PostDraw>
    {
        using Func_Type = typename UIDrawEvent::Func;
        using Event_Type = UIDrawEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnFocused>
    {
        using Func_Type = typename UIActionEvent::Func;
        using Event_Type = UIActionEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnUnFocused>
    {
        using Func_Type = typename UIActionEvent::Func;
        using Event_Type = UIActionEvent;
    };

    template<UIEventType E>
    using UIEvent = typename UIEventDelegate<E>::Event_Type;

    template<UIEventType E>
    using UIEventListener = typename UIEventDelegate<E>::Func_Type;
}