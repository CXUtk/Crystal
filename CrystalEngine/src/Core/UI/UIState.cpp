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
            if (element->IsActive() && element->IsEnabled())
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
        if (m_pAttachedElement)
        {
            // If focused on an element then respond directly to that element
            eventElement = m_pAttachedElement->GetResponseElement(mousePos);
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
            mouseArgs.TimeStamp = mouseButtonArgs.TimeStamp;

            // Mouse Just Press Events
            MouseButtonFlags mouseButtonsFlags = MouseButtonFlags::None;
            if (m_pInputController->IsMouseJustPressed(MouseButtonCode::LEFT_BUTTON))
            {
                mouseButtonsFlags = mouseButtonsFlags | MouseButtonFlags::LeftButton;
                m_pAttachedElement = eventElement;

                UIEventArgs args = {};
                args.Element = mouseArgs.Element;
                args.TimeStamp = mouseArgs.TimeStamp;
                if (m_pFocusedElement != nullptr && m_pFocusedElement != eventElement)
                {
                    m_pFocusedElement->OnUnFocused(args);
                }
                m_pFocusedElement = eventElement;
                m_pFocusedElement->OnFocused(args);
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

                // If up position is in the event bound, then it is a click event
                if ((mouseButtonsFlags & MouseButtonFlags::LeftButton)
                    && m_pAttachedElement != nullptr
                    && m_pAttachedElement == eventElement
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

                if (mouseButtonsFlags & MouseButtonFlags::LeftButton)
                {
                    m_pAttachedElement = nullptr;
                }
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
