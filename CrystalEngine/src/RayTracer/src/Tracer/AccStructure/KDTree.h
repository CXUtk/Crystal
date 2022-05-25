#pragma once
#include "IAccStructure.h"

namespace tracer
{
    struct KDTreeNode;

    class KDTree : public IAccStructure
    {
    public:
        KDTree();
        ~KDTree();
        void Build(const std::vector<std::shared_ptr<crystal::IRayPrimitive>>& objects) override;
        bool Intersect(const Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        bool IntersectTest(const Ray3f& ray, const crystal::IRayPrimitive* ignoreShape,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        GPUDataPackage GetGPUData() const override;

    private:

        int m_tot, m_root;
        KDTreeNode* m_nodes;
        std::vector<const crystal::IRayPrimitive*> m_objects;
        Bound3f m_masterBox;

        int newNode(const std::vector<const crystal::IRayPrimitive*>& objs,
            const Bound3f& box, int split, float splitPos);
        void push_up(int p);
        void _build(int& p, const Bound3f& outerBox,
            std::vector<const crystal::IRayPrimitive*>& objs, int depth);
        bool ray_test(int p, const Ray3f& ray, SurfaceInteraction* info, float tMin, float tMax) const;
        bool ray_test_p(int p, const Ray3f& ray, const crystal::IRayPrimitive* ignoreShape,
            float tMin, float tMax) const;
    };

}