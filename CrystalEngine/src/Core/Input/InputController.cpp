#include "InputController.h"
#include <Platforms/Interfaces/IGameWindow.h>

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
	}
	InputController::~InputController()
	{}
}