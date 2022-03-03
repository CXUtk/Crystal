#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"

namespace crystal
{
    class UIPanel : public UIElement
    {
    public:
        UIPanel();
        ~UIPanel() override;

        Color4f GetDrawColor() const { return m_drawColor; }
        void SetDrawColor(const Color4f& color) { m_drawColor = color; }

        Vector2i GetCornerSize() const { return m_cornerSize; }
        void SetDrawColor(const Vector2i& size) { m_cornerSize = size; }

        std::shared_ptr<ITexture2D> GetPanelTexture() const { return m_panelTexture; }
        void SetDrawColor(std::shared_ptr<ITexture2D> texture) { m_panelTexture = texture; }

    protected:
        Color4f     m_drawColor{};
        Vector2i    m_cornerSize{};

        std::shared_ptr<ITexture2D>     m_panelTexture = nullptr;


        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}