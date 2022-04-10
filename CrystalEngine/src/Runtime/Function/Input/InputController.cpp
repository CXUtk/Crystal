#include "InputController.h"
#include "Platform/RHI/Interfaces.h"
#include <Core/Utils/Logger.h>
#include <iostream>
#include <utf8.h>

namespace crystal
{
	InputController::InputController(IGameWindow* window)
	{
		window->AppendOnMouseScrollEvent([this](Vector2f offset){
			_scrollWheel += offset;
		});

		window->AppendOnMouseButtonChangeEvent([this](MouseButtonEventArgs args) {
			bool downed = (args.Action == InputAction::PRESS);
			_curMouseButtonDown[(int)args.ButtonCode] = downed;
		});

        auto& keySeq = m_keySequence;
		window->AppendOnKeyChangeEvent([this, &keySeq](KeyEventArgs args) {
			bool downed = (args.Action != InputAction::RELEASE);
			_curKeysDown[(int)args.KeyCode] = downed;
            if (downed)
            {
                keySeq.push_back(args);
            }
		});

        auto& charSeq = m_inputSequence;
        window->AddCharInputEventListener([this, &charSeq](CharInputArgs args) {
            charSeq.push_back(args.Code);
        });

		GlobalLogger::Log(SeverityLevel::Debug, "InputController construct");
	}

	InputController::~InputController()
	{
		GlobalLogger::Log(SeverityLevel::Debug, "InputController destruct");
	}

	void InputController::SampleNewInput()
	{
        m_keySequence.clear();
        m_inputSequence.clear();
		_wasKeysDown = _curKeysDown;
		_wasMouseButtonDown = _curMouseButtonDown;
		_scrollWheel = Vector2f(0);
	}

    Point2i InputController::GetMousePos() const
	{
		return _gameWindow->GetMousePos();
	}

    std::u32string InputController::GetInputCharSequence() const
    {
        return m_inputSequence;
    }
}