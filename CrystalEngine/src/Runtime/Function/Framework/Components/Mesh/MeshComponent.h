#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "MeshData.h"

namespace crystal
{
    class MeshComponent : public Component
    {
    public:
        MeshComponent();
        virtual ~MeshComponent() override;

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

        const MeshData& GetTransform() const { return m_mesh; }
        MeshData& GetTransform() { return m_mesh; }

    private:
        MeshData   m_mesh{};
    };
}