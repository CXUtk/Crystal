#include "TestState.h"

#include <Engine.h>
#include <Core/Asset/AssetManager.h>

namespace crystal
{
    TestState::TestState()
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();

        m_pUIStyle = std::make_shared<UIStyle>();

        //m_uiElement = std::make_shared<UIElement>();
        //m_uiElement->SetPivot(Vector2f(0.5f, 0.5f));
        //m_uiElement->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        //m_uiElement->SetSize(SizeLayout(100, 100));
        //m_uiElement->AddEventListener<UIEventType::OnMouseJustPressed>([](UIMouseButtonEventArgs args) {
        //    printf("Down\n");
        //});
        //m_uiElement->AddEventListener<UIEventType::OnMouseJustReleased>([](UIMouseButtonEventArgs args) {
        //    printf("Up\n");
        //});
        //m_uiElement->AddEventListener<UIEventType::OnMouseClicked>([](UIMouseButtonEventArgs args) {
        //    printf("Clicked\n");
        //});
        //m_uiElement->AddEventListener<UIEventType::OnMouseDoubleClicked>([](UIMouseButtonEventArgs args) {
        //    printf("Double Clicked\n");
        //});
        //m_uiElement->AddEventListener<UIEventType::OnMouseEnter>([](UIMouseEventArgs args) {
        //    printf("Enter\n");
        //});
        //m_uiElement->AddEventListener<UIEventType::OnMouseLeave>([](UIMouseEventArgs args) {
        //    printf("Leave\n");
        //});
        //m_uiElement->AddEventListener<UIEventType::OnMouseScroll>([](UIMouseScrollEventArgs args) {
        //    printf("Scroll %f, %f\n", args.Value.x, args.Value.y);
        //});
        //m_uiElement->Recalculate();

        //AppendElement(m_uiElement);


        //auto uiPanel = std::make_shared<UIPanel>();
        //uiPanel->SetPivot(Vector2f(0.5f, 0.5f));
        //uiPanel->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        //uiPanel->SetSize(SizeLayout(100, 100));
        //uiPanel->SetPosition(Vector2f(-200, -200));
        //uiPanel->Recalculate();

        //AppendElement(uiPanel);

        //auto uibutton = std::make_shared<UIIconButton>();
        //uibutton->SetPivot(Vector2f(0.5f, 0.5f));
        //uibutton->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        //uibutton->SetSize(SizeLayout(100, 100));
        //uibutton->SetPosition(Vector2f(200, 200));
        //uibutton->SetIconTexture(assetManager->LoadAsset<ITexture2D>("package1:Box"));
        //uibutton->Recalculate();

        //AppendElement(uibutton);

        auto widget = std::make_shared<UIWidget>("Window");
        widget->SetName("Window");
        widget->SetPivot(Vector2f(0.5f, 0.5f));
        widget->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        widget->SetSize(SizeLayout(200, 200));
        widget->SetPosition(Vector2f(0, 0));
        widget->AddOnCloseEventListener([](UIEventArgs args) {
            args.Element->SetActive(false);
        });

        auto button = std::make_shared<UITextButton>();
        button->SetText("Test");
        button->SetName("Button");
        button->SetPivot(Vector2f(0.5f, 0.5f));
        button->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        button->SetSize(SizeLayout(100, 50));

        widget->AppendChild(button);

        AppendElement(widget);
    }

    TestState::~TestState()
    {
    }
}