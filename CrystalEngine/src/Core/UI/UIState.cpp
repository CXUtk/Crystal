#include "UIState.h"
#include "Elements/UIElement.h"

#include <Engine.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    void UIState::Activate()
    {
        auto engine = Engine::GetInstance();
        m_pInputController = engine->GetInputController();
        m_pGameWindow = engine->GetWindow();

        m_pGameWindow->AppendOnResizeEvent([this](Vector2i size) {
            this->RecalculateAll();
        });
        RecalculateAll();
    }

    void UIState::DeActivate()
    {}

    void UIState::Update(const GameTimer& gameTimer)
    {
        ReorderElements();
        MouseEvent(gameTimer);

        for (auto& element : m_pUIElements)
        {
            if (element->IsActive())
            {
                element->Update(gameTimer);
            }
        }
    }

    void UIState::Draw(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        for (auto& child : m_pUIElements)
        {
            if (!child->IsActive() || !child->IsVisible()) continue;
            child->Draw(payload, gameTimer);
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
        std::shared_ptr<UIElement> eventElement = nullptr;

        auto mousePos = m_pGameWindow->GetMousePos();
        if (m_pFocusedElement)
        {
            // If focused on an element then respond directly to that element
            eventElement = m_pFocusedElement->GetResponseElement(mousePos);
        }
        else
        {
            for (auto it = m_pUIElements.rbegin(); it != m_pUIElements.rend(); ++it)
            {
                auto& child = (*it);
                if (child->CanResponseEvent() && child->GetEventBound().Contains(mousePos))
                {
                    eventElement = child->GetResponseElement(mousePos);
                    break;
                }
            }
        }

        if (eventElement)
        {
            m_toolTip = eventElement->GetTooltip();

            UIMouseButtonEventArgs mouseButtonArgs = {};
            mouseButtonArgs.Element = ptr(eventElement);
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
                m_pLastLeftMouseDownElement = eventElement;
                m_pFocusedElement = eventElement;
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
            if (mouseButtonsFlags != MouseButtonFlags::None)
            {
                eventElement->MouseJustPressed(mouseButtonArgs);
            }

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
            if (mouseButtonsFlags != MouseButtonFlags::None)
            {
                eventElement->MouseJustReleased(mouseButtonArgs);
                if ((mouseButtonsFlags & MouseButtonFlags::LeftButton) && m_pFocusedElement)
                {
                    m_pFocusedElement = nullptr;
                }

                // If up position is in the event bound, then it is a click event
                if (m_pLastLeftMouseDownElement != nullptr && m_pLastLeftMouseDownElement == eventElement
                    && eventElement->GetEventBound().Contains(mousePos))
                {
                    if (mouseArgs.TimeStamp - m_lastLeftMouseClickTime > 0.25)
                    {
                        eventElement->MouseClicked(mouseButtonArgs);
                    }
                    else
                    {
                        eventElement->MouseDoubleClicked(mouseButtonArgs);
                    }
                    m_lastLeftMouseClickTime = mouseArgs.TimeStamp;
                }
                m_pLastLeftMouseDownElement = nullptr;
            }

            if (m_pPrevHoverElement != eventElement)
            {
                if (m_pPrevHoverElement)
                {
                    mouseArgs.Element = m_pPrevHoverElement.get();
                    m_pPrevHoverElement->MouseLeave(mouseArgs);
                }
                mouseArgs.Element = ptr(eventElement);
                eventElement->MouseEnter(mouseArgs);
            }

            auto scrollValue = m_pInputController->GetScrollValue();
            if (scrollValue != Vector2f(0.f))
            {
                UIMouseScrollEventArgs mouseScrollArgs = {};
                mouseScrollArgs.Element = ptr(eventElement);
                mouseScrollArgs.TimeStamp = mouseArgs.TimeStamp;
                mouseScrollArgs.Value = scrollValue;

                eventElement->MouseScroll(mouseScrollArgs);
            }
        }
        else
        {
            if (m_pPrevHoverElement != eventElement)
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
        m_pPrevHoverElement = eventElement;
    }
    void UIState::DrawTooltip()
    {}
}
