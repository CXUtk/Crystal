﻿#pragma once
#include "InputCode.h"

#include <bitset>
#include <Crystal.h>
#include <Core/Utils/Geometry.h>
#include <Platforms/Platforms.h>


namespace crystal
{
	class InputController
	{
	public:
		InputController(IGameWindow* window);
		~InputController();

		/**
		 * @brief Clear current input states and swap them to old state
		*/
		void ClearInput();

		/**
		 * @brief Calculate input state from poll events in this frame
		*/
		void SampleNewInput();

		/**
		 * @brief Get the change of mouse wheel value in this frame
		 * @return
		*/
		Vector2f GetScrollValue() const { return _scrollWheel; }

		/**
		 * @brief Returns true if the given key is just pressed
		 * @param key Keycode of the key
		 * @return
		*/
		bool IsKeyJustPressed(KeyCode key) const { return !_wasKeysDown[(int)key] && _curKeysDown[(int)key]; }

		/**
		 * @brief Returns true if the given key is down
		 * @param key Keycode of the key
		 * @return
		*/
		bool IsKeyDowned(KeyCode key) const { return _curKeysDown[(int)key]; }

		/**
		 * @brief Returns true if the given key is released from pressed state
		 * @param key Keycode of the key
		 * @return
		*/
		bool IsKeyJustReleased(KeyCode key) const { return _wasKeysDown[(int)key] && !_curKeysDown[(int)key]; }

		/**
		 * @brief Returns true if the given mouse button is just pressed
		 * @param mouse Mouse button code
		 * @return
		*/
		bool IsMouseJustPressed(MouseButtonCode mouse) const { return !_wasMouseButtonDown[(int)mouse] && _curMouseButtonDown[(int)mouse]; }

		/**
		 * @brief Returns true if the given mouse button is pressing
		 * @param mouse
		 * @return
		*/
		bool IsMouseDowned(MouseButtonCode mouse) const { return _curMouseButtonDown[(int)mouse]; }

		/**
		 * @brief Returns true if the given mouse button is released from pressed state
		 * @param mouse
		 * @return
		*/
		bool IsMouseReleased(MouseButtonCode mouse) const { return _wasMouseButtonDown[(int)mouse] && !_curMouseButtonDown[(int)mouse]; }

		/**
		 * @brief Get current mouse position related to the bottom left point of the window
		 * @return
		*/
		Vector2i GetMousePos() const;

	private:

		// Keys
		std::bitset<(int)KeyCode::__COUNT> _curKeysDown;
		std::bitset<(int)KeyCode::__COUNT> _wasKeysDown;

		// Mouse wheel
		glm::vec2 _scrollWheel;

		// Mouse button
		std::bitset<(int)MouseButtonCode::__COUNT> _curMouseButtonDown;
		std::bitset<(int)MouseButtonCode::__COUNT> _wasMouseButtonDown;
	};
}