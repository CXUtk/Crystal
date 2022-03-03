#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"

namespace crystal
{
    using UIWidgetCloseEvent = Event<UIEventArgs>;

    class UIWidget : public UIElement
    {
    public:
        UIWidget();
        ~UIWidget() override;

        virtual void UpdateSelf(const GameTimer& gameTimer);

        virtual void MouseJustPressed(UIMouseButtonEventArgs args);
        virtual void MouseJustReleased(UIMouseButtonEventArgs args);

        void AddOnCloseEventListener(UIWidgetCloseEvent::Func listener);

    protected:
        IGameWindow*                    m_gameWindow = nullptr;
        std::shared_ptr<UIIconButton>   m_closeButton = nullptr;

        Vector2i                        m_cornerSize{};
        std::shared_ptr<ITexture2D>     m_panelTexture = nullptr;
        bool                            m_isDragging = false;
        Vector2i                        m_dragStartMousePos{};
        Vector2f                        m_dragStartPos{};
        UIWidgetCloseEvent              m_eventOnClose{};

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;


    private:
        void UpdateDragPos();
    };
}