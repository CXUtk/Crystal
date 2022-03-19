#pragma once

#include "../UIElement.h"
#include "../UILabel.h"
#include "../UIInputBox.h"

namespace crystal
{
    class UIKeyValuePair : public UIElement
    {
    public:
        UIKeyValuePair(const std::string& key);
        ~UIKeyValuePair() override;

        void SetDividerRatio(float ratio) { m_divideRatio = ratio; m_isStateDirty = true; }

        std::shared_ptr<Font> GetFont() const { return m_keyLabel->GetFont(); }
        void SetFont(std::shared_ptr<Font> font)
        {
            m_keyLabel->SetFont(font);
            m_valueInput->SetFont(font);
        }

    protected:
        std::shared_ptr<UILabel>        m_keyLabel = nullptr;
        std::shared_ptr<UIInputBox>     m_valueInput = nullptr;
        float                           m_divideRatio = 0.f;

        virtual void UpdateSelf(const GameTimer& gameTimer) override;
    };
}