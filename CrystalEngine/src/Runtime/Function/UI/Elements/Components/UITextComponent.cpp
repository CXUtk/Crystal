#include "UITextComponent.h"
#include <codecvt>

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>


namespace crystal
{
    static int dx[5] = { 1, -1, 0, 0, 0 };
    static int dy[5] = { 0, 0, 1, -1, 0 };

    UITextComponent::UITextComponent(std::shared_ptr<Font> font, bool hasBorder)
        :m_pFont(font), m_enableBorder(hasBorder)
    {}

    UITextComponent::~UITextComponent()
    {}

    void UITextComponent::DrawWithBot(const Vector2f& botOrigin, const RenderPayload& payload)
    {
        auto bbox = m_pFont->GetBoundingBox();
        auto spriteBatch = payload.SpriteBatch;
        if (m_enableBorder)
        {
            for (int i = 0; i < 5; i++)
            {
                spriteBatch->DrawString(m_pFont, m_textView, botOrigin + Vector2f(dx[i], dy[i] - bbox.GetMinPos().y), i == 4 ? m_textBaseColor : m_textBorderColor);
            }
        }
        else
        {
            spriteBatch->DrawString(m_pFont, m_textView, botOrigin + Vector2f(0.f, -bbox.GetMinPos().y), m_textBaseColor);
        }
    }

    void UITextComponent::DrawWithBase(const Vector2f& baseOrigin, const RenderPayload& payload)
    {
        auto spriteBatch = payload.SpriteBatch;
        if (m_enableBorder)
        {
            for (int i = 0; i < 5; i++)
            {
                spriteBatch->DrawString(m_pFont, m_textView, baseOrigin + Vector2f(dx[i], dy[i]), i == 4 ? m_textBaseColor : m_textBorderColor);
            }
        }
        else
        {
            spriteBatch->DrawString(m_pFont, m_textView, baseOrigin, m_textBaseColor);
        }
    }

    TextMetric UITextComponent::MeasureSize() const
    {
        auto bbox = m_pFont->GetBoundingBox();
        auto metric = m_pFont->MeasureString(m_textView);
        metric.yMax = bbox.GetMaxPos().y;
        metric.yMin = bbox.GetMinPos().y;
        return metric;
    }

    std::vector<float> UITextComponent::GetWidthsForAllChars() const
    {
        return m_pFont->GetWidthsForAllChars(m_textView);
    }

}