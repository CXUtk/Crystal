#pragma once
#include "UICommon.h"
namespace crystal
{
	class UIState
	{
	public:
        virtual ~UIState() = 0 {};

        virtual void Update(const GameTimer& gameTimer) = 0;
        virtual void Draw(SpriteBatch* spriteBatch, const GameTimer& gameTimer) = 0;
	};
}