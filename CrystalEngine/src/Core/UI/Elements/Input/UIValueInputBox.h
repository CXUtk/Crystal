#pragma once
#include "UIInputBox.h"
#include "../Interfaces/IUIValueElement.h"

namespace crystal
{
    template<typename T>
    class UIValueInputBox : public UIInputBox
    {
    public:
        using value_type = T;

        virtual T GetValue() const { return m_value; }
        virtual void SetValue(T value)
        {
            if (m_value != value)
            {
                InvokeValueChange(value);
            }
            m_value = value;
        }

        UIValueInputBox(T defaultValue)
            : m_value(defaultValue)
        {
            SetText(std::to_string(defaultValue));
        }
        ~UIValueInputBox() override
        {

        }

        void SetCheckFunction(std::function<bool(const std::string& val, T& output)> func)
        {
            m_checkValue = func;
        }

        virtual void OnUnFocused(UIEventArgs args) override
        {
            auto newText = this->m_inputComponent->GetText();
            T newValue;
            if (m_checkValue(newText, newValue))
            {
                this->SetValue(newValue);
            }
            SetText(std::to_string(m_value));
            UIInputBox::OnUnFocused(args);
        }
    private:
        T                           m_value{};
        UIValueChangeEvent<T>       m_eventOnValueChanged{};

        std::function<bool(const std::string& val, T& output)> m_checkValue{};

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