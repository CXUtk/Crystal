#include "UIPanel.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>

namespace crystal
{
    UIPanel::UIPanel() : UIElement()
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        m_panelTexture = assetManager->LoadAsset<ITexture2D>("package1:BoxBlack");
        m_cornerSize = Vector2i(10);
        m_drawColor = Color4f(1.f);
    }

    UIPanel::~UIPanel()
    {}

    void UIPanel::DrawSelf(const RenderPayload & payload, const GameTimer & gameTimer)
    {
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto spriteBatch = payload.SpriteBatch;
        spriteBatch->Begin(SpriteSortMode::Deferred, device->GetCommonSamplerState(SamplerStates::PointClamp),
            device->GetCommonBlendState(BlendStates::AlphaBlend));
        SpriteBatchUtils::DrawNineSquareTexture(payload.SpriteBatch, m_panelTexture,
            m_cornerSize, BoundingBoxConvert<int>(m_calculatedInnerBound), m_drawColor);
        spriteBatch->End();
    }
}
