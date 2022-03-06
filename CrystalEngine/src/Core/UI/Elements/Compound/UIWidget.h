#pragma once

#include "../UIElement.h"
#include "../UILabel.h"
#include "../UIImage.h"
#include "../UIIconButton.h"

namespace crystal
{
    using UIWidgetCloseEvent = Event<UIEventArgs>;

    class UIWidget : public UIElement
    {
    public:
        UIWidget(const std::string& title);
        ~UIWidget() override;

        virtual void UpdateSelf(const GameTimer& gameTimer) override;

        virtual void MouseJustPressed(UIMouseButtonEventArgs args) override;
        virtual void MouseJustReleased(UIMouseButtonEventArgs args) override;

        void AddOnCloseEventListener(UIWidgetCloseEvent::Func listener);

    protected:
        IGameWindow*                    m_gameWindow = nullptr;
        std::shared_ptr<UIIconButton>   m_closeButton = nullptr;
        std::shared_ptr<UILabel>        m_windowTitle = nullptr;

        bool                            m_isDragging = false;
        Vector2i                        m_lastMousePos{};
        UIWidgetCloseEvent              m_eventOnClose{};

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;


    private:
        void UpdateDragPos();
    };
}