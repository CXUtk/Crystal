#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"
#include <Core/Asset/Content/Font.h>

namespace crystal
{
    class UILabel : public UIElement
    {
    public:
        UILabel();
        ~UILabel() override;

        Color4f GetTextColor() const { return m_textColor; }
        void SetTextColor(const Color4f& color) { m_textColor = color; }

        Color4f GetBorderColor() const { return m_borderColor; }
        void SetBorderColor(const Color4f& color) { m_borderColor = color; }

        std::string GetText() const { return m_text; }
        void SetText(const std::string& text)
        {
            if (m_text != text)
            {
                m_shouldRecalculateText = true;
            }
            m_text = text;
        }

        std::shared_ptr<Font> GetFont() const { return m_pFont; }
        void SetFont(std::shared_ptr<Font> font)
        {
            if (m_pFont != font)
            {
                m_shouldRecalculateText = true;
            }
            m_pFont = font;
        }

        virtual void UpdateSelf(const GameTimer& gameTimer) override;
        virtual void RecalculateSelf() override;

    protected:
        Color4f             m_textColor{};
        Color4f             m_borderColor{};
        bool                m_enableBorder = false;
        std::string         m_text{};
        bool                m_shouldRecalculateText = true;
        Vector2f            m_originOffset{};

        std::shared_ptr<Font>   m_pFont = nullptr;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}