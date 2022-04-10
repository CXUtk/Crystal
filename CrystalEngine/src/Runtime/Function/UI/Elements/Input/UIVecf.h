#pragma once

#include "../UIElement.h"
#include "../UILabel.h"
#include "UIKeyValuePair.h"

namespace crystal
{
    template<typename T, size_t N>
    class UIVecf : public UIElement
    {
    public:
        UIVecf(const char* keys[N], const char* legend = nullptr)
        {
            static_assert(N > 0, "UIVecf<T, N>: Components number is invalid");

            SetSize(SizeLayout(0, 1.f, 0, 0.f));
            m_dependOnChildrenHeight = true;

            for (int i = 0; i < N; i++)
            {
                m_valueInputs[i] = std::make_shared<UIKeyValuePair<T>>(keys[i]);
                m_valueInputs[i]->SetPivot(Vector2f(0.f, 1.f));
                m_valueInputs[i]->SetAnchorPoint(Vector2f(0.f, 1.f));
                m_valueInputs[i]->SetSize(SizeLayout(0.f, 1.f, 0.f, 0.f));
                AppendChild(m_valueInputs[i]);
            }

            if (legend)
            {
                m_legend = std::make_shared<UILabel>(legend);
                m_legend->SetPivot(Vector2f(0.f, 0.f));
                m_legend->SetAnchorPoint(Vector2f(0.f, 0.f));
                m_legend->SetPosition(Vector2f(8.f, 0.f));
                m_valueInputs[0]->AppendChild(m_legend);
            }
        }
        ~UIVecf() override {}

        void SetDividerRatio(float ratio)
        {
            for (int i = 0; i < N; i++)
            {
                m_valueInputs[i]->SetDividerRatio(ratio);
            }
            m_isStateDirty = true;
        }

        std::shared_ptr<Font> GetFont() const { return m_valueInputs[0]->GetFont(); }
        void SetFont(std::shared_ptr<Font> font)
        {
            for (int i = 0; i < N; i++)
            {
                m_valueInputs[i]->SetFont(font);
            }
            if (m_legend)
            {
                m_legend->SetFont(font);
            }
        }

        void SetCheckFunction(std::function<bool(const std::string& val, T& output)> func)
        {
            for (int i = 0; i < N; i++)
            {
                m_valueInputs[i]->SetCheckFunction(func);
            }
        }

        template<size_t Idx>
        T Get() const
        {
            static_assert(Idx >= 0 && Idx < N);
            return m_valueInputs[Idx]->GetValue();
        }

        virtual void RecalculateHeight() override
        {
            UIElement::RecalculateHeight();
            float offsetY = 0.f;
            for (int i = 0; i < N; i++)
            {
                m_valueInputs[i]->SetPosition(Vector2f(0.f, -offsetY));
                auto height = m_valueInputs[i]->GetHeight();
                offsetY += height;
                if (i != N - 1)
                {
                    offsetY += m_componentsGap;
                }
            }
            m_calculatedHeight = offsetY;
        }

    protected:
        std::shared_ptr<UIKeyValuePair<T>>  m_valueInputs[N]{};
        std::shared_ptr<UILabel>                m_legend = nullptr;
        float                                   m_componentsGap = 2.f;
        float                                   m_totalHeight = 0.f;


        virtual void UpdateSelf(const GameTimer& gameTimer) override
        {

        }
    };
}