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

        Color4f GetColor() const { return m_drawColor; }
        void SetColor(const Color4f& color) { m_drawColor = color; }

        std::shared_ptr<ITexture2D> GetTexture() const { return m_texture; }
        void SetTexture(std::shared_ptr<ITexture2D> texture);

        bool IsInline() const { return m_inline; }
        void SetInline(bool value);

        SliceInfo GetSliceInfo() const { return m_sliceInfo; }
        void SetSliceInfo(const SliceInfo& slice) { m_sliceInfo = slice; }

        ImageType GetImageType() const { return m_drawType; }
        void SetImageType(ImageType value) { m_drawType = value; }

        virtual void UpdateSelf(const GameTimer& gameTimer) override;
        virtual void RecalculateSelf() override;

    protected:
        Color4f                         m_drawColor{};
        std::shared_ptr<ITexture2D>     m_texture = nullptr;
        SliceInfo                       m_sliceInfo{};
        ImageType                       m_drawType = ImageType::Full;
        bool                            m_inline = true;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}