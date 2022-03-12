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

        SliceInfo slice = {};
        slice.Left = 10;
        slice.Right = 10;
        slice.Top = 10;
        slice.Bot = 10;
        slice.DrawFlags = Slice_Nine;

        SliceInfo slice6 = {};
        slice6.Left = 10;
        slice6.Right = 10;
        slice6.Top = 10;
        slice6.Bot = 10;
        slice6.DrawFlags = Slice_Six;

        std::shared_ptr<UIImage> panel = std::make_shared<UIImage>();
        panel->SetInline(false);
        panel->SetImageType(ImageType::Sliced);
        panel->SetPivot(Vector2f(0.f));
        panel->SetAnchorPoint(Vector2f(0.f));
        panel->SetSize(SizeLayout(0, 1.f, 0, 1.f));
        panel->SetTexture(assetManager->LoadAsset<ITexture2D>("package1:BoxBlack"));
        panel->SetSliceInfo(slice);

        std::shared_ptr<UIImage> panelBar = std::make_shared<UIImage>();
        panelBar->SetInline(false);
        panelBar->SetImageType(ImageType::Sliced);
        panelBar->SetPivot(Vector2f(0.f, 1.f));
        panelBar->SetAnchorPoint(Vector2f(0.f, 1.f));
        panelBar->SetSize(SizeLayout(0, 1.f, 40, 0.f));
        panelBar->SetColor(UIStyle::GetButtonColor());
        panelBar->SetTexture(assetManager->LoadAsset<ITexture2D>("package1:BoxNoB"));
        panelBar->SetSliceInfo(slice6);


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

        m_windowTitle = std::make_shared<UILabel>(title);
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
        auto mousePos = m_gameWindow->GetMousePos();
        if (m_isDragging)
        {
            UpdateDragPos(mousePos);
            Recalculate();
        }
        m_lastMousePos = mousePos;
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

    void UIWidget::UpdateDragPos(Point2i mousePos)
    {
        auto offset = mousePos - m_lastMousePos;

        auto screenPos = GetLocalPositionToScreenPos(m_position);
        m_position = GetScreenPositionToLocalPos(screenPos + Vector2f(offset));
    }
}
