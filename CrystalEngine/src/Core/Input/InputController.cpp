#include "InputController.h"
#include <Core/Platform/Platforms.h>
#include <Core/Utils/Logger.h>

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

		window->AppendOnKeyChangeEvent([this](KeyEventArgs args) {
			bool downed = (args.Action != InputAction::RELEASE);
			_curKeysDown[(int)args.KeyCode] = downed;
		});

		GlobalLogger::Log(SeverityLevel::Debug, "InputController construct");
	}

	InputController::~InputController()
	{
		GlobalLogger::Log(SeverityLevel::Debug, "InputController destruct");
	}

	void InputController::SampleNewInput()
	{
		_wasKeysDown = _curKeysDown;
		_wasMouseButtonDown = _curMouseButtonDown;
		_scrollWheel = Vector2f(0);
	}
	Point2i InputController::GetMousePos() const
	{
		return _gameWindow->GetMousePos();
	}
}