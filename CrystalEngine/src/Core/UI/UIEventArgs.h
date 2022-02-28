#pragma once
#include "UICommon.h"
#include <Core/Utils/Event.h>

namespace crystal
{
    struct UIEventArgs
    {
        UIElement*  Element;
        double      TimeStamp;
    };


    struct UIMouseEventArgs : public UIEventArgs
    {
        Vector2i    MousePosScreen;
    };

    using UIMouseEvent = Event<UIMouseEventArgs>;

    enum class UIEventType
    {
        OnMouseClicked,
        OnMouseDown,
        OnMouseUp
    };

    template<UIEventType E>
    struct UIEventDelegate {};

    template<>
    struct UIEventDelegate<UIEventType::OnMouseClicked>
    {
        using Func_Type = typename Event<UIMouseEventArgs>::Func;
        using Event_Type = UIMouseEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseDown>
    {
        using Func_Type = typename Event<UIMouseEventArgs>::Func;
        using Event_Type = UIMouseEvent;
    };

    template<>
    struct UIEventDelegate<UIEventType::OnMouseUp>
    {
        using Func_Type = typename Event<UIMouseEventArgs>::Func;
        using Event_Type = UIMouseEvent;
    };

    template<UIEventType E>
    using UIEvent = UIEventDelegate<E>::Event_Type;

    template<UIEventType E>
    using UIEventListener = UIEventDelegate<E>::Func_Type;
}