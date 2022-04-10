#include "UILayout.h"

#include <Engine.h>

#include "Resource/Asset/AssetManager.h"
#include "Function/Render/RenderExports.h"
#include "Function/Input/InputController.h"

namespace crystal
{
    UILayout::UILayout()
    {}

    UILayout::~UILayout()
    {}

    void UILayout::AppendElement(std::shared_ptr<UIElement> element, const Bound2f& targetLayout)
    {
        element->SetPivot(Vector2f(0.f));
        element->SetAnchorPoint(Vector2f(0.f));
        auto layoutSize = targetLayout.GetSize();
        element->SetSize(SizeLayout(0, layoutSize.x, 0, layoutSize.y));
        AppendChild(element);
        m_layouts.push_back(targetLayout);
    }

    void UILayout::RecalculateChildren(RecalculateMask mask)
    {
        int size = m_layouts.size();
        auto bound = GetInnerBound();
        auto boundSize = bound.GetSize();
        for (int i = 0; i < size; i++)
        {
            auto start = m_layouts[i].GetMinPos();
            m_pChildren[i]->SetPosition(start * boundSize);
        }
        UIElement::RecalculateChildren(mask);
    }

}
