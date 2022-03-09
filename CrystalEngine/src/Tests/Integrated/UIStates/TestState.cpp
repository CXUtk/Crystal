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
        widget->SetSize(SizeLayout(400, 400));
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

        m_slider = std::make_shared<UIValueSlider>();
        m_slider->SetName("Slider");
        m_slider->SetPivot(Vector2f(0.5f, 0.5f));
        m_slider->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        m_slider->SetSize(SizeLayout(200, 20));
        m_slider->SetValue(0.f);
        m_slider->SetPosition(Vector2f(0.f, -80.f));

        auto progressBar = std::make_shared<UIProgressBar>();
        progressBar->SetName("Slider Label");
        progressBar->SetPivot(Vector2f(0.5f, 0.5f));
        progressBar->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        progressBar->SetSize(SizeLayout(200, 20));
        progressBar->SetValue(0.5f);
        progressBar->SetPosition(Vector2f(0.f, -130.f));

        auto uiValueLabel = std::make_shared<UILabel>();
        uiValueLabel->SetName("Slider Label");
        uiValueLabel->SetPivot(Vector2f(0, 0.5f));
        uiValueLabel->SetAnchorPoint(Vector2f(0.5f));
        uiValueLabel->SetPosition(Vector2f(100.f, -80.f));
        uiValueLabel->SetText("0");

        auto ptrLabel = ptr(uiValueLabel);
        m_slider->AddOnValueChangedEventListener([ptrLabel](UIValueChangeEventArgs<double> args) {
            ptrLabel->SetText(string_format("%.2lf", args.Value));
        });

        auto scrollBar = std::make_shared<UIScrollBarV>();
        scrollBar->SetName("Scroll Bar V");
        scrollBar->SetPivot(Vector2f(0, 0.5f));
        scrollBar->SetAnchorPoint(Vector2f(0.5f));
        scrollBar->SetPosition(Vector2f(100.f, -120.f));
        scrollBar->SetSize(SizeLayout(16, 140));
        scrollBar->SetViewRange(0.1f);

        auto scrollBarH = std::make_shared<UIScrollBarH>();
        scrollBarH->SetName("Scroll Bar H");
        scrollBarH->SetPivot(Vector2f(0.5f, 0.5f));
        scrollBarH->SetAnchorPoint(Vector2f(0.5f));
        scrollBarH->SetPosition(Vector2f(0.f, -140.f));
        scrollBarH->SetSize(SizeLayout(140, 16));
        scrollBarH->SetViewRange(0.1f);


        auto uilist = std::make_shared<UIList>();
        uilist->SetPivot(Vector2f(0.5f));
        uilist->SetAnchorPoint(Vector2f(0.5f));
        uilist->SetPosition(Vector2f(0.f, 0.f));
        uilist->SetSize(SizeLayout(200, 200));

        for (int i = 0; i < 111111; i++)
        {
            auto text = std::make_shared<UILabel>();
            text->SetPivot(Vector2f(0.5f));
            text->SetAnchorPoint(Vector2f(0.5f));
            text->SetPosition(Vector2f(0.f, 0.f));
            text->SetSize(SizeLayout(200, 200));
            text->SetText("ItemXXXX");

            uilist->Add(text);
        }

        //widget->AppendChild(button);
        //widget->AppendChild(m_slider);
        //widget->AppendChild(uiValueLabel);
        //widget->AppendChild(progressBar);
        //widget->AppendChild(scrollBar);
        //widget->AppendChild(scrollBarH);
        widget->AppendChild(uilist);

        AppendElement(widget);
    }

    TestState::~TestState()
    {
    }
    void TestState::Update(const GameTimer& gameTimer)
    {
        //m_slider->SetValue(std::sin(gameTimer.GetCurrentTime()) * 0.5 + 0.5);
        UIState::Update(gameTimer);
    }
}