#pragma once

#include "../UIElement.h"
#include "../UILabel.h"
#include "../Input/UIValueInputBox.h"

namespace crystal
{
    template<typename T>
    class UIKeyValuePair : public UIElement
    {
    public:
        UIKeyValuePair(const std::string& key)
        {
            m_keyLabel = std::make_shared<UILabel>(key);
            m_keyLabel->SetPivot(Vector2f(1.f, 0.f));
            m_keyLabel->SetAnchorPoint(Vector2f(0.5f, 0.f));
            m_keyLabel->SetPosition(Vector2f(-KEY_PADDING, 0.f));
            AppendChild(m_keyLabel);

            m_valueInput = std::make_shared<UIValueInputBox<T>>();
            m_valueInput->SetPivot(Vector2f(0.f, 0.f));
            m_valueInput->SetAnchorPoint(Vector2f(0.5f, 0.f));
            m_valueInput->SetPosition(Vector2f(VALUE_PADDING, 0.f));
            m_valueInput->SetSize(SizeLayout(-2 * VALUE_PADDING, 0.5f, 0.f, 1.f));
            AppendChild(m_valueInput);
        }
        ~UIKeyValuePair() override {}

        void SetDividerRatio(float ratio) { m_divideRatio = ratio; m_isStateDirty = true; }

        std::shared_ptr<Font> GetFont() const { return m_keyLabel->GetFont(); }
        void SetFont(std::shared_ptr<Font> font)
        {
            m_keyLabel->SetFont(font);
            m_valueInput->SetFont(font);
        }

        virtual int GetPredictedHeight(UIElement* fakeParent) const override
        {
            return m_keyLabel->GetPredictedHeight(fakeParent) + INPUT_PADDING_TOP * 2;
        }

        void SetCheckFunction(std::function<bool(const std::string& val, T& output)> func)
        {
            m_valueInput->SetCheckFunction(func);
        }

    protected:
        std::shared_ptr<UILabel>                    m_keyLabel = nullptr;
        std::shared_ptr<UIValueInputBox<T>>         m_valueInput = nullptr;
        float                                       m_divideRatio = 0.f;

        virtual void UpdateSelf(const GameTimer& gameTimer) override
        {
            m_keyLabel->SetAnchorPoint(Vector2f(m_divideRatio, 0.f));
            m_valueInput->SetAnchorPoint(Vector2f(m_divideRatio, 0.f));

            int maxHeight = m_keyLabel->GetPredictedHeight(this) + INPUT_PADDING_TOP * 2;
            m_valueInput->SetSize(SizeLayout(-2 * VALUE_PADDING, 1.f - m_divideRatio,
                maxHeight, 0.f));

            auto layout = GetSize();
            SetSize(SizeLayout(layout.Width.Absolute, layout.Width.Relative, maxHeight, 0.f));
        }

    private:
        static constexpr int KEY_PADDING = 5;
        static constexpr int VALUE_PADDING = 5;
        static constexpr int INPUT_PADDING_TOP = 2;
    };
}