#include "PropertiesPanel.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>
#include <Core/UI/UIStateMachine.h>

using namespace crystal;

namespace tracer
{
    constexpr int FRAME_PADDING = 5;
    constexpr int INNER_LIST_PADDING = 2;
    constexpr int INPUT_AREA_PADDING = 4;
    PropertiesPanel::PropertiesPanel()
    {

        auto outerFrame = std::make_shared<UIPanel>();
        outerFrame->SetName("Outer Frame");
        outerFrame->SetPivot(Vector2f(0.5f));
        outerFrame->SetAnchorPoint(Vector2f(0.5f));
        outerFrame->SetSize(SizeLayout(-2* FRAME_PADDING, 1.f, -2 * FRAME_PADDING, 1.f));
        outerFrame->SetFillColor(UIStyle::GetContainerInnerColor());
        AppendChild(outerFrame);


        auto innerList = std::make_shared<UIList>();
        innerList->SetName("Props List");
        innerList->SetPivot(Vector2f(0.5f));
        innerList->SetAnchorPoint(Vector2f(0.5f));
        innerList->SetSize(SizeLayout(-2 * INNER_LIST_PADDING, 1.f, -2 * INNER_LIST_PADDING, 1.f));
        innerList->SetGap(10.f);
        outerFrame->AppendChild(innerList);

        auto engine = Engine::GetInstance();
        auto assetManager = engine->GetAssetManager();
        auto font18 = assetManager->LoadAsset<Font>("Crystal:Consolas18");
        auto font14 = assetManager->LoadAsset<Font>("Crystal:Consolas14");

        auto frameBufferSizeList = std::make_shared<UIList>();
        frameBufferSizeList->SetPivot(Vector2f(0.f, 1.f));
        frameBufferSizeList->SetAnchorPoint(Vector2f(0.f, 1.f));
        frameBufferSizeList->SetPosition(Vector2f(0.f, -16.f));
        frameBufferSizeList->SetSize(SizeLayout(0, 1.f, 55, 0.f));
        frameBufferSizeList->SetGap(2.f);
        innerList->Add(frameBufferSizeList);


        auto frameBufferWidthKV = std::make_shared<UIKeyValuePair>("X");
        frameBufferWidthKV->SetFont(font14);
        frameBufferWidthKV->SetSize(SizeLayout(0, 1.f, 24, 0.f));
        frameBufferWidthKV->SetDividerRatio(0.4f);

        auto frameBufferHeightKV = std::make_shared<UIKeyValuePair>("Y");
        frameBufferHeightKV->SetFont(font14);
        frameBufferHeightKV->SetSize(SizeLayout(0, 1.f, 24, 0.f));
        frameBufferHeightKV->SetDividerRatio(0.4f);

        frameBufferSizeList->Add(frameBufferWidthKV);
        frameBufferSizeList->Add(frameBufferHeightKV);


        auto frameBufferSizeLabel = std::make_shared<UILabel>("Resolution");
        frameBufferSizeLabel->SetPivot(Vector2f(0.f, 0.f));
        frameBufferSizeLabel->SetAnchorPoint(Vector2f(0.f, 0.f));
        frameBufferSizeLabel->SetPosition(Vector2f(8.f, 0.f));
        frameBufferSizeLabel->SetFont(font14);
        frameBufferWidthKV->AppendChild(frameBufferSizeLabel);



        auto SPPKV = std::make_shared<UIKeyValuePair>("Samples");
        SPPKV->SetFont(font14);
        SPPKV->SetSize(SizeLayout(0, 1.f, 24, 0.f));
        SPPKV->SetDividerRatio(0.4f);
        innerList->Add(SPPKV);

        auto CoresKV = std::make_shared<UIKeyValuePair>("# Cores");
        CoresKV->SetFont(font14);
        CoresKV->SetSize(SizeLayout(0, 1.f, 24, 0.f));
        CoresKV->SetDividerRatio(0.4f);
        innerList->Add(CoresKV);

        auto AccKV = std::make_shared<UIKeyValuePair>("Acc. Structure");
        AccKV->SetFont(font14);
        AccKV->SetSize(SizeLayout(0, 1.f, 24, 0.f));
        AccKV->SetDividerRatio(0.4f);
        innerList->Add(AccKV);
        //auto button2 = std::make_shared<UITextButton>();
        //button2->SetPivot(Vector2f(0.5f, 1.f));
        //button2->SetAnchorPoint(Vector2f(0.5f, 1.f));
        //button2->SetPosition(Vector2f(0.f, -64.f));
        //button2->SetSize(SizeLayout(72, 32));
        //button2->SetText("Button");
        //button2->SetEnabled(false);
        //m_properties->AppendChild(button2);

        //auto slider1 = std::make_shared<UIValueSlider>();
        //slider1->SetPivot(Vector2f(0.5f, 1.f));
        //slider1->SetAnchorPoint(Vector2f(0.5f, 1.f));
        //slider1->SetPosition(Vector2f(0.f, -96.f));
        //slider1->SetSize(SizeLayout(108, 24));
        //m_properties->AppendChild(slider1);

        //auto list1 = std::make_shared<UIList>();
        //list1->SetPivot(Vector2f(0.5f, 1.f));
        //list1->SetAnchorPoint(Vector2f(0.5f, 1.f));
        //list1->SetPosition(Vector2f(0.f, -136.f));
        //list1->SetSize(SizeLayout(135, 135));
        //m_properties->AppendChild(list1);

        //{
        //    auto text1 = std::make_shared<UILabel>("ItemXXXX");
        //    text1->SetPivot(Vector2f(0.5f));
        //    text1->SetAnchorPoint(Vector2f(0.5f));
        //    text1->SetPosition(Vector2f(0.f, 0.f));
        //    text1->SetSize(SizeLayout(200, 200));

        //    auto text2 = std::make_shared<UILabel>("ItemXXXX--2");
        //    text2->SetPivot(Vector2f(0.5f));
        //    text2->SetAnchorPoint(Vector2f(0.5f));
        //    text2->SetPosition(Vector2f(0.f, 0.f));
        //    text2->SetSize(SizeLayout(200, 200));

        //    list1->Add(text1);
        //    list1->Add(text2);
        //}
    }

    PropertiesPanel::~PropertiesPanel()
    {}
    void PropertiesPanel::DrawSelf(const crystal::RenderPayload & payload, const crystal::GameTimer & gameTimer)
    {}
}