#pragma once
#include "Common/UICommon.h"
#include <Interfaces/Interfaces.h>

namespace crystal
{
	class UIState
	{
	public:
        virtual ~UIState() = 0 {};

        virtual void Preprocess();
        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(SpriteBatch* spriteBatch, const GameTimer& gameTimer);

    protected:
        std::vector<std::shared_ptr<UIElement>> m_pUIElements{};

        InputController*        m_pInputController = nullptr;
        IGameWindow*            m_pGameWindow = nullptr;

        std::shared_ptr<UIElement>  m_pHoverElement = nullptr;
        std::shared_ptr<UIElement>  m_pPrevHoverElement = nullptr;
        std::string                 m_toolTip{};

        void AppendElement(std::shared_ptr<UIElement> element);

        virtual void ReorderElements();
        virtual void RecalculateAll();

    private:
        void MouseEvent(const GameTimer& gameTimer);
        void DrawTooltip();
	};
}