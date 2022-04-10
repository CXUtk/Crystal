#pragma once
#include "../../Common/UICommon.h"
#include "../../Common/UIEventArgs.h"

#include "../UIElement.h"
#include <Engine.h>

namespace crystal
{
    template<typename T>
    class IUIValueElement : public UIElement
    {
    public:
        virtual ~IUIValueElement() = 0 {};
        virtual T GetValue() const { return m_value; }
        virtual void SetValue(T value)
        {
            if (m_value != value)
            {
                InvokeValueChange(value);
            }
            m_value = value;
        }

        virtual void AddOnValueChangedEventListener(typename UIValueChangeEvent<T>::Func listener)
        {
            m_eventOnValueChanged += listener;
        }

    protected:
        T                           m_value = 0.f;
        UIValueChangeEvent<T>       m_eventOnValueChanged{};

        void InvokeValueChange(T newValue)
        {
            UIValueChangeEventArgs<T> args = {};
            args.Element = this;
            args.TimeStamp = Engine::GetInstance()->GetCurrentTime();
            args.Value = newValue;
            m_eventOnValueChanged.Invoke(args);
        }
    };
}