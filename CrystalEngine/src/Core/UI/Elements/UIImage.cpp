#include "UIImage.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UIImage::UIImage()
    {
        m_drawColor = Color4f(1.f);
    }

    UIImage::~UIImage()
    {}

    void UIImage::SetTexture(std::shared_ptr<ITexture2D> texture)
    {
        m_texture = texture;
        if (m_inline)
        {
            m_isStateDirty = true;
        }
    }

    void UIImage::SetInline(bool value)
    {
        m_inline = value;
        if (m_inline)
        {
            m_isStateDirty = true;
        }
    }

    void UIImage::UpdateSelf(const GameTimer& gameTimer)
    {
    }

    void UIImage::RecalculateSelf()
    {
        if (m_inline)
        {
            auto size = m_texture->GetSize();
            SetSize(SizeLayout(size.x, size.y));
        }
    }

    void UIImage::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        assert(m_texture != nullptr);

        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        auto spriteBatch = payload.SpriteBatch;

        if (m_drawType == ImageType::Full)
        {
            spriteBatch->Draw(m_texture, BoundingBoxConvert<int>(m_calculatedInnerBound), m_drawColor);
        }
        else
        {
            spriteBatch->DrawSlicedTexture(m_texture, m_sliceInfo,
                BoundingBoxConvert<int>(m_calculatedInnerBound), m_drawColor);
        }
    }
}
