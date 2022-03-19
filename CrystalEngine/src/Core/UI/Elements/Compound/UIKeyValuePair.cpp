#include "UIKeyValuePair.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    constexpr int KEY_PADDING = 5;
    constexpr int VALUE_PADDING = 5;
    UIKeyValuePair::UIKeyValuePair(const std::string& key)
    {
        m_keyLabel = std::make_shared<UILabel>(key);
        m_keyLabel->SetPivot(Vector2f(1.f, 0.f));
        m_keyLabel->SetAnchorPoint(Vector2f(0.5f, 0.f));
        m_keyLabel->SetPosition(Vector2f(-KEY_PADDING, 0.f));
        AppendChild(m_keyLabel);

        m_valueInput = std::make_shared<UIInputBox>();
        m_valueInput->SetPivot(Vector2f(0.f, 0.f));
        m_valueInput->SetAnchorPoint(Vector2f(0.5f, 0.f));
        m_valueInput->SetPosition(Vector2f(VALUE_PADDING, 0.f));
        m_valueInput->SetSize(SizeLayout(-2 * VALUE_PADDING, 0.5f, 0.f, 1.f));
        AppendChild(m_valueInput);
    }

    UIKeyValuePair::~UIKeyValuePair()
    {}
    void UIKeyValuePair::UpdateSelf(const GameTimer & gameTimer)
    {
        m_keyLabel->SetAnchorPoint(Vector2f(m_divideRatio, 0.f));
        m_valueInput->SetAnchorPoint(Vector2f(m_divideRatio, 0.f));
        m_valueInput->SetSize(SizeLayout(-2 * VALUE_PADDING, 1.f - m_divideRatio, 0.f, 1.f));
    }
}
