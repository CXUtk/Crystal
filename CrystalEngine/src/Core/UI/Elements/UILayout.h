#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"

namespace crystal
{
    class UILayout : public UIElement
    {
    public:
        UILayout();
        ~UILayout() override;

        void AppendElement(std::shared_ptr<UIElement> element, const Bound2f& targetLayout);
        
    protected:
        std::vector<Bound2f>    m_layouts;

        virtual void RecalculateChildren() override;
    };
}