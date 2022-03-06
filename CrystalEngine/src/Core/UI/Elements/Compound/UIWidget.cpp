#include "UIWidget.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>



namespace crystal
{
    UIWidget::UIWidget(const std::string& title) : UIElement()
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        m_overflowStyle = OverflowStyle::Hidden;

        std::shared_ptr<UIImage> panel = std::make_shared<UIImage>();
        panel->SetInline(false);
        panel->SetImageType(ImageType::Sliced9);
        panel->SetPivot(Vector2f(0.f));
        panel->SetAnchorPoint(Vector2f(0.f));
        panel->SetSize(SizeLayout(0, 1.f, 0, 1.f));
        panel->SetTexture(assetManager->LoadAsset<ITexture2D>("package1:BoxBlack"));
        panel->SetCornerSize(Vector2i(10));

        std::shared_ptr<UIImage> panelBar = std::make_shared<UIImage>();
        panelBar->SetInline(false);
        panelBar->SetImageType(ImageType::Sliced6);
        panelBar->SetPivot(Vector2f(0.f, 1.f));
        panelBar->SetAnchorPoint(Vector2f(0.f, 1.f));
        panelBar->SetSize(SizeLayout(0, 1.f, 32, 0.f));
        panelBar->SetColor(UIStyle::GetButtonColor());
        panelBar->SetTexture(assetManager->LoadAsset<ITexture2D>("package1:BoxNoB"));
        panelBar->SetCornerSize(Vector2i(10));


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

        m_windowTitle = std::make_shared<UILabel>();
        m_windowTitle->SetText(title);
        m_windowTitle->SetAnchorPoint(Vector2f(0.f, 1.f));
        m_windowTitle->SetPivot(Vector2f(0.f, 1.f));
        m_windowTitle->SetPosition(Vector2f(10, -10));

        AppendChild(panel);
        AppendChild(panelBar);
        AppendChild(m_windowTitle);
        AppendChild(m_closeButton);

        m_gameWindow = Engine::GetInstance()->GetWindow();
    }

    UIWidget::~UIWidget()
    {
    }

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
        UIElement::MouseJustPressed(args);
    }

    void UIWidget::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_isDragging = false;
        UIElement::MouseJustPressed(args);
    }

    void UIWidget::AddOnCloseEventListener(UIWidgetCloseEvent::Func listener)
    {
        m_eventOnClose += listener;
    }

    void UIWidget::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
    }

    void UIWidget::UpdateDragPos()
    {
        auto mousePos = m_gameWindow->GetMousePos();
        auto offset = mousePos - m_lastMousePos;

        auto screenPos = GetLocalPositionToScreenPos(m_position);
        m_position = GetScreenPositionToLocalPos(screenPos + Vector2f(offset));
    }
}
