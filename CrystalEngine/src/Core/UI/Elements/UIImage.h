#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"
#include <Core/Asset/Content/Font.h>

namespace crystal
{
    class UIImage : public UIElement
    {
    public:
        UIImage();
        ~UIImage() override;

        const ImageDesc& GetImageDesc() const { return m_imageDesc; }
        void SetImageDesc(const ImageDesc& imageDesc) { m_imageDesc = imageDesc; }

        Color4f GetColor() const { return m_imageDesc.DrawColor; }
        void SetColor(const Color4f& color) { m_imageDesc.DrawColor = color; }

        std::shared_ptr<ITexture2D> GetTexture() const { return m_imageDesc.Texture; }
        void SetTexture(std::shared_ptr<ITexture2D> texture);

        bool IsInline() const { return m_inline; }
        void SetInline(bool value);

        Vector2i GetCornerSize() const { return m_imageDesc.CornerSize; }
        void SetCornerSize(Vector2i value) { m_imageDesc.CornerSize = value; }

        ImageType GetImageType() const { return m_imageDesc.DrawType; }
        void SetImageType(ImageType value) { m_imageDesc.DrawType = value; }

        virtual void UpdateSelf(const GameTimer& gameTimer) override;
        virtual void RecalculateSelf() override;

    protected:
        ImageDesc                       m_imageDesc{};
        bool                            m_inline = true;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}