#include "UITextComponent.h"
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
        float desc = m_pFont->GetDescender();
        auto spriteBatch = payload.SpriteBatch;
        if (m_enableBorder)
        {
            for (int i = 0; i < 5; i++)
            {
                spriteBatch->DrawString(m_pFont, m_text, botOrigin + Vector2f(dx[i], dy[i] - desc), i == 4 ? m_textBaseColor : m_textBorderColor);
            }
        }
        else
        {
            spriteBatch->DrawString(m_pFont, m_text, botOrigin + Vector2f(0.f, -desc), m_textBaseColor);
        }
    }

    void UITextComponent::DrawWithBase(const Vector2f& baseOrigin, const RenderPayload& payload)
    {
        auto spriteBatch = payload.SpriteBatch;
        if (m_enableBorder)
        {
            for (int i = 0; i < 5; i++)
            {
                spriteBatch->DrawString(m_pFont, m_text, baseOrigin + Vector2f(dx[i], dy[i]), i == 4 ? m_textBaseColor : m_textBorderColor);
            }
        }
        else
        {
            spriteBatch->DrawString(m_pFont, m_text, baseOrigin, m_textBaseColor);
        }
    }

    TextMetric UITextComponent::MeasureSize() const
    {
        auto metric = m_pFont->MeasureString(m_text);
        metric.yMin = m_pFont->GetDescender();
        return metric;
    }
}