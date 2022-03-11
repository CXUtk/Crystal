#include "UIInputBox.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>
#include <Core/UI/UIStateMachine.h>

namespace crystal
{
    static constexpr int TEXT_PADDING = 2;
    UIInputBox::UIInputBox()
    {
        m_backgroundColor = UIStyle::GetButtonColor();
        m_borderColor = UIStyle::GetPanelBorderColor();
        m_focusBorderColor = UIStyle::GetPanelBorderColorHighlight();
        m_propagationFlags = PropagationFlags::MouseScroll;
    }

    UIInputBox::~UIInputBox()
    {}

    void UIInputBox::OnFocused(UIEventArgs args)
    {
        m_isFocused = true;
    }

    void UIInputBox::OnUnFocused(UIEventArgs args)
    {
        m_isFocused = false;
    }

    void UIInputBox::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
        auto outerBound = BoundingBoxConvert<int>(m_calculatedInnerBound);
        auto spriteBatch = payload.SpriteBatch;

        // Background
        spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(outerBound),
            m_backgroundColor);

        // Border
        SliceInfo slice = {};
        slice.Left = 1;
        slice.Right = 1;
        slice.Top = 1;
        slice.Bot = 1;
        slice.DrawFlags = Slice_Nine;
        spriteBatch->DrawSlicedTexture(stateMachine->GetFrameTexture(), slice,
            BoundingBoxConvert<int>(outerBound),
            m_isFocused ? m_focusBorderColor : m_borderColor);
    }
}