#pragma once
#include "Common/UICommon.h"
#include "Platform/RHI/Interfaces.h"


namespace crystal
{
	class UIState
	{
	public:
        virtual ~UIState() = 0 {};

        virtual void Activate();
        virtual void DeActivate();
        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const RenderPayload& payload, const GameTimer& gameTimer);
        virtual void RecalculateAll();

    protected:
        std::vector<std::shared_ptr<UIElement>> m_pUIElements{};

        InputController*        m_pInputController = nullptr;
        IGameWindow*            m_pGameWindow = nullptr;

        std::shared_ptr<UIElement>  m_pPrevHoverElement = nullptr;
        std::shared_ptr<UIElement>  m_pAttachedElement = nullptr;
        std::shared_ptr<UIElement>  m_pFocusedElement = nullptr;
        double                      m_lastLeftMouseClickTime = 0.0;
        std::string                 m_toolTip{};

        void AppendElement(std::shared_ptr<UIElement> element);

        virtual void ReorderElements();
    private:
        void MouseEvent(const GameTimer& gameTimer);
        void DrawTooltip();
	};
}