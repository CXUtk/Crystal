#pragma once
#include "RenderCommon.h"

namespace crystal
{
    class GeometryRenderer
    {
    public:
        explicit GeometryRenderer(IGraphicsDevice* graphicsDevice, IGraphicsContext* context);
        ~GeometryRenderer();

        void Begin();
        void End();

        void DrawLineStrip(const std::vector<Vector2f>& points,
            const Color4f& color);
        void DrawLine(const Vector2f& start,
            const Vector2f& end,
            const Color4f& color);
        void DrawLine(const Vector3f& start,
            const Vector3f& end,
            const Color4f& color);
        void DrawTriangle(const Vector3f& V1,
            const Vector3f& V2,
            const Vector3f& V3,
            const Color4f& color);

        void DrawBound2D(const Bound2i& bound,
            const Color4f& color);
        void DrawBound2DFill(const Bound2i& bound,
            const Color4f& fillColor, const Color4f& borderColor);

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}