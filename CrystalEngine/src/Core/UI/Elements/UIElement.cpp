#include "UIElement.h"
#include <Engine.h>
#include <Core/Render/SpriteBatch.h>

namespace crystal
{
    UIElement::UIElement()
    {}
    UIElement::~UIElement()
    {}

    void UIElement::Update(const GameTimer& gameTimer)
    {
        UpdateSelf(gameTimer);
        UpdateChildren(gameTimer);
    }

    void UIElement::Draw(SpriteBatch* spriteBatch, const GameTimer& gameTimer)
    {
        DrawSelf(spriteBatch, gameTimer);
        DrawChildren(spriteBatch, gameTimer);
    }

    void UIElement::ReCalculate()
    {
        CalculateBounds();
    }

    void UIElement::AppendChild(std::shared_ptr<UIElement> element)
    {
        if (element->m_pParent)
        {
            element->m_pParent->RemoveChild(element);
        }
        element->m_pParent = this;
        m_pChildren.push_back(element);
    }

    void UIElement::RemoveChild(std::shared_ptr<UIElement> element)
    {
        auto p = std::find(m_pChildren.begin(), m_pChildren.end(), element);
        if (p == m_pChildren.end())
        {
            throw std::invalid_argument("Given UIElement does not exist in children set");
        }
        m_pChildren.erase(p);
        element->m_pParent = nullptr;
    }

    std::shared_ptr<UIElement> UIElement::GetChildByName(const std::string& name)
    {
        auto p = std::find_if(m_pChildren.begin(), m_pChildren.end(),
            [&](const std::shared_ptr<UIElement>& element) {
                return name == element->GetName();
            });
        if (p == m_pChildren.end())
        {
            return nullptr;
        }
        return *p;
    }

    void UIElement::UpdateSelf(const GameTimer& gameTimer)
    {}

    void UIElement::UpdateChildren(const GameTimer& gameTimer)
    {
        for (auto& child : m_pChildren)
        {
            if (!child->m_isActive) continue;
            child->Update(gameTimer);
        }
    }

    void UIElement::DrawSelf(SpriteBatch* spriteBatch, const GameTimer& gameTimer)
    {
        if constexpr (EnableDebugDraw)
        {
            auto graphicsDevice = Engine::GetInstance()->GetGraphicsDevice();
            spriteBatch->Begin(SpriteSortMode::Deferred,
                graphicsDevice->GetCommonSamplerState(SamplerStates::PointClamp),
                graphicsDevice->GetCommonBlendState(BlendStates::AlphaBlend));
            spriteBatch->Draw()
            spriteBatch->End();
        }
    }

    void UIElement::DrawChildren(SpriteBatch* spriteBatch, const GameTimer& gameTimer)
    {
        for (auto& child : m_pChildren)
        {
            if (!child->m_isActive || !child->m_isVisible) continue;
            child->Draw(spriteBatch, gameTimer);
        }
    }

    void UIElement::CalculateBounds()
    {
        Bound2f parentBound = {};
        if (m_pParent)
        {
            parentBound = m_pParent->m_calculatedInnerBound;
        }
        else
        {
            auto size = Engine::GetInstance()->GetWindow()->GetWindowSize();
            parentBound = Bound2f(Vector2f(0.f), Vector2f(size));
        }
        auto parentSize = parentBound.GetMaxPos() - parentBound.GetMinPos();
        auto pivotPosWorld = parentBound.GetMinPos() + parentSize * m_anchorPoint;

        auto selfSize = Vector2f(m_size.Width.Relative * parentSize.x + m_size.Width.Absolute,
            m_size.Height.Relative * parentSize.y + m_size.Height.Absolute);

        m_calculatedInnerBound = Bound2f(pivotPosWorld - selfSize * m_pivot,
            pivotPosWorld + selfSize * (Vector2f(1.f) - m_pivot));
    }
}
