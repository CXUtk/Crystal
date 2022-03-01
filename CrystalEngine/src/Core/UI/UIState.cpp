#include "UIState.h"
#include "Elements/UIElement.h"

#include <Engine.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    void UIState::Preprocess()
    {}

    void UIState::Update(const GameTimer& gameTimer)
    {
        auto engine = Engine::GetInstance();
        m_pInputController = engine->GetInputController();
        m_pGameWindow = engine->GetWindow();

        ReorderElements();

        MouseEvent(gameTimer);
        for (auto& element : m_pUIElements)
        {
            if (element->IsActive())
            {
                element->Update(gameTimer);
            }
        }

        RecalculateAll();
    }

    void UIState::Draw(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        for (auto it = m_pUIElements.rbegin(); it != m_pUIElements.rend(); ++it)
        {
            auto& child = (*it);
            if (!child->IsActive() || !child->IsVisible()) continue;
            (*it)->Draw(payload, gameTimer);
        }

        DrawTooltip();
    }

    void UIState::AppendElement(std::shared_ptr<UIElement> element)
    {
        m_pUIElements.push_back(element);
    }

    void UIState::ReorderElements()
    {}

    void UIState::RecalculateAll()
    {
        for (auto& element : m_pUIElements)
        {
            if (element->IsActive())
            {
                element->Recalculate();
            }
        }
    }
    void UIState::MouseEvent(const GameTimer& gameTimer)
    {
        m_pHoverElement = nullptr;
        auto mousePos = m_pGameWindow->GetMousePos();

        for (auto& element : m_pUIElements)
        {
            if (element->CanResponseEvent()
                && element->GetEventBound().Contains(mousePos))
            {
                m_pHoverElement = element->GetResponseElement(mousePos);
                break;
            }
        }

        if (m_pHoverElement)
        {
            m_toolTip = m_pHoverElement->GetTooltip();

            UIMouseButtonEventArgs mouseButtonArgs = {};
            mouseButtonArgs.Element = ptr(m_pHoverElement);
            mouseButtonArgs.MousePosScreen = mousePos;
            mouseButtonArgs.TimeStamp = gameTimer.GetPhysicalTime();

            UIMouseEventArgs mouseArgs = {};
            mouseArgs.MousePosScreen = mousePos;
            mouseArgs.TimeStamp = gameTimer.GetPhysicalTime();

            // Mouse Just Press Events
            MouseButtonFlags mouseButtonsFlags = MouseButtonFlags::None;
            if (m_pInputController->IsMouseJustPressed(MouseButtonCode::LEFT_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::LeftButton;
            }
            if (m_pInputController->IsMouseJustPressed(MouseButtonCode::RIGHT_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::RightButton;
            }
            if (m_pInputController->IsMouseJustPressed(MouseButtonCode::MIDDLE_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::MiddleButton;
            }
            mouseButtonArgs.ButtonFlags = mouseButtonsFlags;
            m_pHoverElement->MouseJustPressed(mouseButtonArgs);

            // Mouse Just Release Events
            mouseButtonsFlags = MouseButtonFlags::None;
            if (m_pInputController->IsMouseReleased(MouseButtonCode::LEFT_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::LeftButton;
            }
            if (m_pInputController->IsMouseReleased(MouseButtonCode::RIGHT_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::RightButton;
            }
            if (m_pInputController->IsMouseReleased(MouseButtonCode::MIDDLE_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::MiddleButton;
            }
            mouseButtonArgs.ButtonFlags = mouseButtonsFlags;
            m_pHoverElement->MouseJustReleased(mouseButtonArgs);

            if (m_pPrevHoverElement != m_pHoverElement)
            {
                if (m_pPrevHoverElement)
                {
                    mouseArgs.Element = m_pPrevHoverElement.get();
                    m_pPrevHoverElement->MouseLeave(mouseArgs);
                }
                mouseArgs.Element = m_pHoverElement.get();
                m_pHoverElement->MouseEnter(mouseArgs);
            }
        }
        else
        {
            if (m_pPrevHoverElement != m_pHoverElement)
            {
                UIMouseEventArgs mouseArgs = {};
                mouseArgs.Element = m_pPrevHoverElement.get();
                mouseArgs.MousePosScreen = mousePos;
                mouseArgs.TimeStamp = gameTimer.GetPhysicalTime();

                if (m_pPrevHoverElement)
                {
                    m_pPrevHoverElement->MouseLeave(mouseArgs);
                }
            }
        }
        m_pPrevHoverElement = m_pHoverElement;
    }
    void UIState::DrawTooltip()
    {}
}
