#include "InputController.h"
#include <Interfaces/Interfaces.h>
#include <Core/Utils/Logger.h>
#include <iostream>
#include <codecvt>

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

        auto& charSeq = m_wString;
        window->AddCharInputEventListener([this, &charSeq](CharInputArgs args) {
            charSeq.push_back((wchar_t)args.Code);
            auto c = (wchar_t)args.Code;
            printf("%08X\n", (wchar_t)args.Code);
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
        m_wString.clear();
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
        if (m_wString.empty()) return std::u32string();
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert32;
        auto u8String = convert.to_bytes(m_wString);
        return convert32.from_bytes(u8String);
    }
}