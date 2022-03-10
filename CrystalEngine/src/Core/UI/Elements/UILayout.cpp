#include "UILayout.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

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

    void UILayout::RecalculateChildren()
    {
        int size = m_layouts.size();
        auto bound = m_calculatedInnerBound;
        auto boundSize = bound.GetSize();
        for (int i = 0; i < size; i++)
        {
            auto start = m_layouts[i].GetMinPos();
            m_pChildren[i]->SetPosition(start * boundSize);
        }
        UIElement::RecalculateChildren();
    }
}
