#include "Tree.h"

void crystal::ClearTree(TreeNode* node)
{
    for (auto child : node->Children)
    {
        ClearTree(child);
    }

    delete node;
}
