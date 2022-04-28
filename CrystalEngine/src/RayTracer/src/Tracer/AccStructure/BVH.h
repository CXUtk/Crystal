#pragma once
#include "IAccStructure.h"

namespace tracer
{
    struct BVHNode;

    class BVH : public IAccStructure
    {
    public:
        BVH();
        ~BVH() override;
        void Build(const std::vector<std::shared_ptr<IRayHiter>>& objects) override;
        bool Intersect(const Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        bool IntersectTest(const Ray3f& ray, const IRayHiter* ignoreShape,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

    private:
        BVHNode* _nodes;
        int _tot, _root;
        std::vector<const IRayHiter*> _objects;

        void _build(int& p, int l, int r);
        //int newNode(int startOffset, int cnt, int splitAxis, const BoundingBox& box);

        //bool _intersect(int p, const Ray& ray, SurfaceInteraction* info, float tMin, float tMax) const;
        //bool _intersectP(int p, const Ray& ray, const crystal::IIntersectable* ignoreShape, 
        //	float tMin, float tMax) const;


        int splitByEqualCount(int l, int r, const Bound3f& box, int& splitPos);
        bool splitBySAH(int l, int r, const Bound3f& box, int& splitAxis, int& splitPos);

        int createLeaf(int& p, int l, int r);
        int createInternal(int splitAxis, const Bound3f& box);
    };
}