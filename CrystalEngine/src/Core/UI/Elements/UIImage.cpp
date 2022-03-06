#include "UIImage.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UIImage::UIImage()
    {
        m_imageDesc.DrawColor = Color4f(1.f);
        m_propagationFlags = PropagationFlags::All;
    }

    UIImage::~UIImage()
    {}

    void UIImage::SetTexture(std::shared_ptr<ITexture2D> texture)
    {
        m_imageDesc.Texture = texture;
    }

    void UIImage::SetInline(bool value)
    {
        m_inline = value;
    }

    void UIImage::UpdateSelf(const GameTimer& gameTimer)
    {}

    void UIImage::RecalculateSelf()
    {
        if (m_inline)
        {
            auto size = m_imageDesc.Texture->GetSize();
            SetSize(SizeLayout(size.x, size.y));
        }
        UIElement::RecalculateSelf();
    }

    void UIImage::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        assert(m_imageDesc.Texture != nullptr);
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        auto geometryRenderer = payload.GeometryRenderer;
        auto spriteBatch = payload.SpriteBatch;

        spriteBatch->Begin(SpriteSortMode::Deferred, payload.PSO);
        switch (m_imageDesc.DrawType)
        {
        case crystal::ImageType::Full:
        {
            spriteBatch->Draw(m_imageDesc.Texture, BoundingBoxConvert<int>(m_calculatedInnerBound), m_imageDesc.DrawColor);
        }
            break;
        case crystal::ImageType::Sliced6:
        {
            SpriteBatchUtils::DrawSixSquareUpTexture(payload.SpriteBatch, m_imageDesc.Texture,
                m_imageDesc.CornerSize, BoundingBoxConvert<int>(m_calculatedInnerBound), m_imageDesc.DrawColor);
        }
            break;
        case crystal::ImageType::Sliced8NoCenter:
        {

        }
            break;
        case crystal::ImageType::Sliced9:
        {
            SpriteBatchUtils::DrawNineSquareTexture(payload.SpriteBatch, m_imageDesc.Texture,
                m_imageDesc.CornerSize, BoundingBoxConvert<int>(m_calculatedInnerBound), m_imageDesc.DrawColor);
        }
            break;
        default:
            break;
        }
        spriteBatch->End();
    }
}
