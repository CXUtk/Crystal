#include "BVH.h"
#include <algorithm>

namespace tracer
{
    enum class SplitMethod
    {
        EQUAL_COUNTS,
        SAH
    };
    constexpr int MAX_OBJ_IN_NODE = 1;
    constexpr SplitMethod SLILT_METHOD = SplitMethod::EQUAL_COUNTS;
    constexpr float TRAV_COST = 0.5f;
    constexpr float INTERSECT_COST = 1.0f;

    struct BVHNode
    {
        Bound3f bound;
        union
        {
            // Leaf
            struct
            {
                int entitiesStartOffset;
                int count;
            };
            // Internal
            int ch[2]{ 0, 0 };
        };
        int8_t splitAxis = -1;
    };
    constexpr int SizeBVHNode = sizeof(BVHNode);

    BVH::BVH() : m_nodes(nullptr), m_tot(0), m_root(0)
    {}

    BVH::~BVH()
    {
        if (m_nodes)
            delete[] m_nodes;
    }

    void BVH::Build(const std::vector<std::shared_ptr<IRayPrimitive>>& objects)
    {
        for (auto& obj : objects)
        {
            m_primitives.push_back(cptr(obj));
        }
        m_nodes = new BVHNode[objects.size() * 2 + 1];
        m_tot = 0;
        _build(m_root, 0, objects.size() - 1);
    }

    bool BVH::Intersect(const Ray3f& ray, SurfaceInteraction* info, float tMin, float tMax) const
    {
        constexpr int MAX_STACK_SIZE = 64;

        Vector3f invDir(1.f / ray.Dir().x, 1.f / ray.Dir().y, 1.f / ray.Dir().z);
        bool dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };
        int nodesStack[MAX_STACK_SIZE]{}, top = 0;
        int currentNode = m_root;

        RayHitPayload payload = {};
        while (true)
        {
            auto& current = m_nodes[currentNode];
            float t1 = tMin, t2 = std::min(tMax, payload.Distance);
            if (RayBoxTest(ray, dirIsNeg, invDir, current.bound, t1, t2))
            {
                // 如果是叶子节点就是暴力判定一下
                if (current.splitAxis == -1)
                {
                    const IRayPrimitive* const* startP = &m_primitives[current.entitiesStartOffset];
                    int objCnt = current.count;
                    for (int i = 0; i < objCnt; i++)
                    {
                        t1 = tMin, t2 = std::min(tMax, payload.Distance);
                        if (!RayBoxTest(ray, dirIsNeg, invDir, startP[i]->GetBoundingBox(), t1, t2)) continue;
                        Float tCurrent = std::numeric_limits<Float>::infinity();
                        if (!startP[i]->IntersectTest(ray, &tCurrent, tMin, tMax)) continue;
                        if (tCurrent < payload.Distance)
                        {
                            payload.Distance = tCurrent;
                            payload.HitPrimitive = startP[i];
                        }
                    }
                    if (!top) break;
                    currentNode = nodesStack[--top];
                }
                else
                {
                    int initDir = dirIsNeg[current.splitAxis] ? 1 : 0;
                    nodesStack[top++] = current.ch[!initDir];
                    currentNode = current.ch[initDir];
                    assert(top < MAX_STACK_SIZE && top >= 0);
                }
            }
            else
            {
                if (!top) break;
                currentNode = nodesStack[--top];
            }
        }

        if (payload.HitPrimitive)
        {
            payload.HitPrimitive->Intersect(ray, info);
            info->SetHitPrimitive(payload.HitPrimitive);
            return true;
        }
        return false;
    }

    bool BVH::IntersectTest(const Ray3f& ray, const IRayPrimitive* ignoreShape,
        float tMin, float tMax) const
    {
        constexpr int MAX_STACK_SIZE = 64;

        Vector3f invDir(1.f / ray.Dir().x, 1.f / ray.Dir().y, 1.f / ray.Dir().z);
        bool dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };

        int nodesStack[MAX_STACK_SIZE]{}, top = 0;
        int currentNode = m_root;

        // Unused
        Float t;
        while (true)
        {
            auto& current = m_nodes[currentNode];
            float t1 = tMin, t2 = tMax;
            if (RayBoxTest(ray, dirIsNeg, invDir, current.bound, t1, t2))
            {
                // 如果是叶子节点就是暴力判定一下
                if (current.splitAxis == -1)
                {
                    const IRayPrimitive* const* startP = &m_primitives[current.entitiesStartOffset];
                    int objCnt = current.count;
                    for (int i = 0; i < objCnt; i++)
                    {
                        if (startP[i] == ignoreShape) continue;
                        t1 = tMin, t2 = tMax;
                        if (!RayBoxTest(ray, dirIsNeg, invDir, startP[i]->GetBoundingBox(), t1, t2)) continue;
                        if (!startP[i]->IntersectTest(ray, &t, t1, t2)) continue;
                        return true;
                    }
                    if (!top) break;
                    currentNode = nodesStack[--top];
                }
                else
                {
                    int initDir = dirIsNeg[current.splitAxis] ? 1 : 0;
                    nodesStack[top++] = current.ch[!initDir];
                    assert(top < MAX_STACK_SIZE && top >= 0);
                    currentNode = current.ch[initDir];
                }
            }
            else
            {
                if (!top) break;
                currentNode = nodesStack[--top];
                assert(top < MAX_STACK_SIZE && top >= 0);
            }
        }
        return false;
    }

    void BVH::WriteGPUSceneData(GPUDataPackage* package) const
    {
        dfs(&m_nodes[m_root], package);
    }

    void BVH::_build(int& p, int l, int r)
    {
        if (r < l) return;
        if (r - l + 1 <= MAX_OBJ_IN_NODE)
        {
            createLeaf(p, l, r);
            return;
        }
        // 获取[l, r]的碰撞箱
        BoundingBox box = m_primitives[l]->GetBoundingBox();
        for (int i = l + 1; i <= r; i++)
        {
            box = box.Union(m_primitives[i]->GetBoundingBox());
        }

        int splitAxis, splitPos;
        if constexpr (SLILT_METHOD == SplitMethod::EQUAL_COUNTS)
        {
            splitAxis = splitByEqualCount(l, r, box, splitPos);
        }
        else
        {
            if (!splitBySAH(l, r, box, splitAxis, splitPos))
            {
                createLeaf(p, l, r);
                return;
            }
        }

        // 按照排序结果从中间分割物体们
        p = createInternal(splitAxis, box);
        _build(m_nodes[p].ch[0], l, splitPos);
        _build(m_nodes[p].ch[1], splitPos + 1, r);
    }

    int BVH::splitByEqualCount(int l, int r, const Bound3f& box, int& splitPos)
    {
        int splitAxis = box.MaxExtent();
        // 按照某种方式分割物体，并且排序
        auto cmp = [splitAxis](const IRayPrimitive* a, const IRayPrimitive* b) {
            return a->GetBoundingBox().GetCenter()[splitAxis] < b->GetBoundingBox().GetCenter()[splitAxis];
        };
        splitPos = l + (r - l) / 2;
        std::nth_element(m_primitives.begin() + l, m_primitives.begin() + splitPos, m_primitives.begin() + r + 1, cmp);
        return splitAxis;
    }

    bool BVH::splitBySAH(int l, int r, const Bound3f& box, int& splitAxis, int& splitPos)
    {
        splitAxis = 0, splitPos = l;
        float minCost = std::numeric_limits<float>::infinity();
        const crystal::IRayPrimitive* const* startP = &m_primitives[l];
        float totArea = box.SurfaceArea();
        float* sufArea = new float[r - l + 2];
        for (int i = 0; i < 3; i++)
        {
            // 按照中点坐标排序
            auto cmp = [i](const crystal::IRayPrimitive* a, const crystal::IRayPrimitive* b) {
                return a->GetBoundingBox().GetCenter()[i] < b->GetBoundingBox().GetCenter()[i];
            };
            std::sort(m_primitives.begin() + l, m_primitives.begin() + r + 1, cmp);

            Bound3f currentBox;
            // 求后缀包围盒表面积和
            sufArea[r - l + 1] = 0.f;
            for (int j = r - l; j >= 0; j--)
            {
                currentBox = currentBox.Union(startP[j]->GetBoundingBox());
                sufArea[j] = currentBox.SurfaceArea();
            }


            // 扫描线，动态求出最小cost
            currentBox = startP[0]->GetBoundingBox();
            for (int j = 1; j <= r - l; j++)
            {
                float cost = TRAV_COST + (currentBox.SurfaceArea() * j + sufArea[j] * (r - l + 1 - j)) / totArea * INTERSECT_COST;
                if (cost < minCost)
                {
                    splitAxis = i;
                    splitPos = l + j - 1;
                    minCost = cost;
                }
                if (j != r - l)
                {
                    currentBox = currentBox.Union(startP[j]->GetBoundingBox());
                }
            }
        }
        delete[] sufArea;
        return minCost < (r - l + 1)* INTERSECT_COST;
    }

    int BVH::createLeaf(int& p, int l, int r)
    {
        BoundingBox box = m_primitives[l]->GetBoundingBox();
        // 获取[l, r]的碰撞箱，并且把纯指针放进shapes里
        for (int i = l + 1; i <= r; i++)
        {
            box = box.Union(m_primitives[i]->GetBoundingBox());
        }
        p = ++m_tot;

        // Leaf Part
        m_nodes[m_tot].entitiesStartOffset = l;
        m_nodes[m_tot].count = r - l + 1;

        m_nodes[m_tot].bound = box;
        m_nodes[m_tot].splitAxis = -1;
        return m_tot;
    }

    int BVH::createInternal(int splitAxis, const Bound3f& box)
    {
        m_tot++;
        // Internal Part
        m_nodes[m_tot].ch[0] = m_nodes[m_tot].ch[1] = 0;

        m_nodes[m_tot].bound = box;
        m_nodes[m_tot].splitAxis = splitAxis;
        return m_tot;
    }

    size_t BVH::dfs(const BVHNode* node, GPUDataPackage* package) const
    {
        if (node->splitAxis == -1)
        {
            // Objects node
            const IRayPrimitive* const* startP = &m_primitives[node->entitiesStartOffset];
            int objCnt = node->count;
            std::vector<const IRayPrimitive*> prims;
            for (int i = 0; i < objCnt; i++)
            {
                prims.push_back(startP[i]);
            }
            return package->AddObjectsNode(node->bound, prims);
        }
        else
        {
            size_t nodeId = package->AddBVHNode(node->bound, node->splitAxis);
            if (node->ch[0])
            {
                package->SetBVHNodeLeft(nodeId, dfs(&m_nodes[node->ch[0]], package));
            }
            if (node->ch[1])
            {
                package->SetBVHNodeRight(nodeId, dfs(&m_nodes[node->ch[1]], package));
            }
            return nodeId;
        }
    }
}