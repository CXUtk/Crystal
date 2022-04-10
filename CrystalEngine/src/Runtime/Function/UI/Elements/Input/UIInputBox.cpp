#include "UIInputBox.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>
#include <Core/UI/UIStateMachine.h>


namespace crystal
{
    static constexpr int TEXT_PADDING = 2;
    static constexpr int TEXT_SPACING_OFFSET = 4;
    UIInputBox::UIInputBox()
    {
        m_backgroundColor = UIStyle::GetButtonColor();
        m_borderColor = UIStyle::GetPanelBorderColor();
        m_focusBorderColor = UIStyle::GetPanelBorderColorHighlight();
        m_propagationFlags = PropagationFlags::MouseScroll;

        auto assetManager = Engine::GetInstance()->GetAssetManager();
        auto font = assetManager->LoadAsset<Font>("Crystal:Consolas18");
        m_textDrawComponent = std::make_shared<UITextComponent>(font, false);

        m_textDrawComponent->SetTextColor(Color4f(1.f));

        m_inputComponent = std::make_shared<UIInputComponent>("");

        m_drawXOffset = -TEXT_SPACING_OFFSET;
    }

    UIInputBox::~UIInputBox()
    {}

    std::string UIInputBox::GetText() const
    {
        return m_inputComponent->GetText();
    }

    void UIInputBox::SetText(const std::string& text)
    {
        m_inputComponent->SetText(text);
        RespondToChange();
    }

    void UIInputBox::OnFocused(UIEventArgs args)
    {
        m_isFocused = true;
        m_blinkTimer = 0.f;
    }

    void UIInputBox::OnUnFocused(UIEventArgs args)
    {
        m_isFocused = false;
    }

    void UIInputBox::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
        auto outerBound = BoundingBoxConvert<int>(GetInnerBound());
        auto spriteBatch = payload.SpriteBatch;

        // Input
        auto text32 = m_inputComponent->GetText32();
        m_textDrawComponent->SetText32(text32);

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

        spriteBatch->End();

        auto RSState = payload.PSO->GetRasterState();
        auto oldScissorState = RSState->GetScissorState();
        auto oldScissorBound = RSState->GetScissorBound();

        RSState->SetScissorState(true);
        auto newBound = Bound2f(outerBound.GetMinPos() + Vector2i(TEXT_PADDING),
            outerBound.GetMaxPos() - Vector2i(TEXT_PADDING));
        auto scissorBound = BoundingBoxConvert<int>(newBound)
            .IntersectWith(oldScissorBound);
        RSState->SetScissorBound(scissorBound);

        spriteBatch->Begin(SpriteSortMode::Deferred, payload.PSO);
        {
            auto drawStringOrigin = Vector2f(outerBound.GetMinPos())
                + Vector2f(TEXT_PADDING - m_drawXOffset, TEXT_PADDING);
            m_textDrawComponent->DrawWithBot(drawStringOrigin, payload);

            auto font = m_textDrawComponent->GetFont();
            auto textLength = m_inputComponent->GetLength();
            auto bbox = font->GetBoundingBox();
            if ((m_blinkTimer < 0.5 && m_isFocused) || m_isDragging)
            {
                float xOffset = 0.f;
                int carrot = m_inputComponent->GetCarrotPos();
                if (carrot != 0)
                {
                    xOffset = GetXOffsetByCarrot(carrot);
                }
                Bound2f carrotBound = Bound2f(drawStringOrigin + Vector2f(xOffset, 0.f),
                    drawStringOrigin + Vector2f(xOffset + 1, bbox.GetSize().y));
                spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(carrotBound),
                    Color4f(1.f));
            }
            if (m_isFocused)
            {
                if (m_dragStartIndex != m_dragEndIndex)
                {
                    int left = m_dragStartIndex, right = m_dragEndIndex;
                    if (left > right) std::swap(left, right);

                    float leftX = GetXOffsetByCarrot(left);
                    float rightX = GetXOffsetByCarrot(right);

                    Vector2f leftPos = Vector2f(leftX, 0.f) + drawStringOrigin;
                    Vector2f rightPos = Vector2f(rightX, bbox.GetSize().y) + drawStringOrigin;
                    Bound2f selectionBound = Bound2f(leftPos, rightPos);

                    spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(selectionBound),
                        Color4f(1.f, 1.f, 1.f, 0.5f));
                }
            }
        }
        spriteBatch->End();

        RSState->SetScissorState(oldScissorState);
        RSState->SetScissorBound(oldScissorBound);

        spriteBatch->Begin(SpriteSortMode::Deferred, payload.PSO);
    }

    void UIInputBox::UpdateSelf(const GameTimer& gameTimer)
    {
        auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
        if (m_isFocused)
        {
            if (m_isDragging)
            {
                m_dragEndIndex = GetCarrotByPos(mousePos);
                m_inputComponent->MoveCarrot(m_dragEndIndex);

                int left = m_dragStartIndex, right = m_dragEndIndex;
                if (left > right) std::swap(left, right);
                m_inputComponent->SetSelectionRange(left, right);

                UpdateCarrotShift();
            }

            m_inputComponent->SampleInput();
            m_blinkTimer += gameTimer.GetPhysicalDeltaTime();
            if (m_blinkTimer > 1.f)
            {
                m_blinkTimer -= 1.f;
            }

            if (m_inputComponent->IsCarrotChanged())
            {
                RespondToChange();
                UpdateCarrotShift();
            }
        }
    }


    int UIInputBox::GetCarrotByPos(const Vector2i& screenPos)
    {
        float xPos = screenPos.x - GetInnerBound().GetMinPos().x - TEXT_PADDING + m_drawXOffset;
        auto p = std::lower_bound(m_charWidths.begin(), m_charWidths.end(), xPos);
        if (p == m_charWidths.end()) return m_inputComponent->GetLength();
        int carrot = p - m_charWidths.begin();
        if (xPos - GetXOffsetByCarrot(carrot) >= GetXOffsetByCarrot(carrot + 1) - xPos)
        {
            return carrot + 1;
        }
        else
        {
            return carrot;
        }
        return carrot;
    }

    float UIInputBox::GetXOffsetByCarrot(int carrot)
    {
        return carrot ? m_charWidths[carrot - 1] : 0;
    }

    void UIInputBox::UpdateCarrotShift()
    {
        int carrot = m_inputComponent->GetCarrotPos();

        auto bound = GetInnerBound();
        auto minPos = bound.GetMinPos();
        auto maxPos = bound.GetMaxPos();
        float actualX = GetXOffsetByCarrot(carrot) + minPos.x + TEXT_PADDING - m_drawXOffset;
        if (isnan(actualX)) return;
        if (actualX > maxPos.x - TEXT_PADDING - TEXT_SPACING_OFFSET)
        {
            m_drawXOffset += actualX - (maxPos.x - TEXT_PADDING - TEXT_SPACING_OFFSET);
        }
        else if (actualX < minPos.x + TEXT_PADDING + TEXT_SPACING_OFFSET)
        {
            m_drawXOffset -= minPos.x + TEXT_PADDING + TEXT_SPACING_OFFSET - actualX;
        }
    }

    void UIInputBox::RespondToChange()
    {
        auto font = m_textDrawComponent->GetFont();
        m_charWidths = font->GetWidthsForAllChars(m_inputComponent->GetText32());

        m_blinkTimer = 0.f;

        m_inputComponent->GetSelectionRange(m_dragStartIndex, m_dragEndIndex);
    }

    void UIInputBox::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        m_dragStartIndex = GetCarrotByPos(args.MousePosScreen);
        m_dragEndIndex = m_dragStartIndex;
        m_inputComponent->MoveCarrot(m_dragStartIndex);
        m_isDragging = true;
        m_blinkTimer = 0.f;
    }

    void UIInputBox::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_isDragging = false;
        m_blinkTimer = 0.f;
    }
}