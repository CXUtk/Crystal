#pragma once
#include "../../Common/UICommon.h"
#include "../../Common/UIEventArgs.h"

#include <Resource/Asset/Content/Fonts/FontFamily.h>
namespace crystal
{
    class UITextComponent
    {
    public:
        UITextComponent(std::shared_ptr<Font> font, bool hasBorder);
        ~UITextComponent();

        Color4f GetTextColor() const { return m_textBaseColor; }
        void SetTextColor(const Color4f& color) { m_textBaseColor = color; }

        Color4f GetBorderColor() const { return m_textBorderColor; }
        void SetBorderColor(const Color4f& color) { m_textBorderColor = color; }

        void SetText32(const std::u32string_view& text) { m_textView = text; }

        std::shared_ptr<Font> GetFont() const { return m_pFont; }
        void SetFont(std::shared_ptr<Font> font) { m_pFont = font; }

        void DrawWithBot(const Vector2f& botOrigin, const RenderPayload& payload);
        void DrawWithBase(const Vector2f& baseOrigin, const RenderPayload& payload);

        TextMetric MeasureSize() const;
        std::vector<float> GetWidthsForAllChars() const;

        Vector2f GetPosAt(int charIndex) const;
    private:
        std::shared_ptr<Font>   m_pFont = nullptr;
        std::u32string_view     m_textView{};
        Color4f                 m_textBaseColor{};
        Color4f                 m_textBorderColor{};
        bool                    m_enableBorder = false;
    };
}