#include "UIWidget.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

#include "UIIconButton.h"

namespace crystal
{
    UIWidget::UIWidget() : UIElement()
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        m_panelTexture = assetManager->LoadAsset<ITexture2D>("package1:BoxBlack");
        m_cornerSize = Vector2i(10);

        m_closeButton = std::make_shared<UIIconButton>();
        m_closeButton->SetIconTexture(assetManager->LoadAsset<ITexture2D>("package1:Cross"));
        m_closeButton->SetAnchorPoint(Vector2f(1.f));
        m_closeButton->SetPivot(Vector2f(.5f));
        m_closeButton->SetSize(SizeLayout(22, 22));
        m_closeButton->SetPosition(Vector2f(-16.f));
        m_closeButton->AddEventListener<UIEventType::OnMouseClicked>([this](UIMouseButtonEventArgs args) {
            UIEventArgs closeArg = {};
            closeArg.Element = this;
            closeArg.TimeStamp = args.TimeStamp;
            m_eventOnClose.Invoke(closeArg);
        });
        AppendChild(m_closeButton);

        m_gameWindow = Engine::GetInstance()->GetWindow();
    }

    UIWidget::~UIWidget()
    {}

    void UIWidget::UpdateSelf(const GameTimer& gameTimer)
    {
        if (m_isDragging)
        {
            UpdateDragPos();
            RecalculateSelf();
        }
        m_lastMousePos = m_gameWindow->GetMousePos();
    }

    void UIWidget::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        m_isDragging = true;
    }

    void UIWidget::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_isDragging = false;
    }

    void UIWidget::AddOnCloseEventListener(UIWidgetCloseEvent::Func listener)
    {
        m_eventOnClose += listener;
    }

    void UIWidget::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto geometryRenderer = payload.GeometryRenderer;
        auto spriteBatch = payload.SpriteBatch;
        spriteBatch->Begin(SpriteSortMode::Deferred, device->GetCommonSamplerState(SamplerStates::PointClamp),
            device->GetCommonBlendState(BlendStates::AlphaBlend));
        SpriteBatchUtils::DrawNineSquareTexture(payload.SpriteBatch, m_panelTexture,
            m_cornerSize, BoundingBoxConvert<int>(m_calculatedInnerBound), Color4f(1.f));
        spriteBatch->End();
    }

    void UIWidget::UpdateDragPos()
    {
        auto mousePos = m_gameWindow->GetMousePos();
        auto offset = mousePos - m_lastMousePos;

        auto screenPos = GetLocalPositionToScreenPos(m_position);
        m_position = GetScreenPositionToLocalPos(screenPos + Vector2f(offset));
    }
}
