#pragma once
#include <vector>

namespace crystal
{
    struct TreeNode;
    struct ContourNode
    {
        TreeNode* node = nullptr;
        ContourNode* next = nullptr;
    };
    struct TreeNode
    {
        float Left, Right;
        float LazyTag;

        std::vector<TreeNode*> Children{};

        TreeNode* LeftContour = nullptr;
        TreeNode* RightContour = nullptr;

        TreeNode()
        {
            Left = Right = 0;
            LazyTag = 0;
        }

        float Center() const { return (Left + Right) / 2.f; }

        void MoveToLeftBeginAt(float destLeft)
        {
            float diff = destLeft - Left;
            Left += diff;
            Right += diff;
            LazyTag += diff;

            PushDown();
        }

        void PushDown()
        {
            for (auto child : Children)
            {
                child->Left += LazyTag;
                child->Right += LazyTag;
                child->LazyTag += LazyTag;

                child->PushDown();
            }
            LazyTag = 0;
        }
    };

    void ClearTree(TreeNode* node);

}