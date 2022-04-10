#pragma once
#include <Crystal.h>
#include <wrl/client.h>
#include <string>
#include <bitset>

#include "Platform/RHI/IGameWindow.h"
#include "Win32ClipBoard.h"

namespace crystal
{
	class Win32GameWindow : public IGameWindow
	{
	public:
		Win32GameWindow(const InitArgs& args, HINSTANCE hInstance);
		~Win32GameWindow();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual bool ShouldClose() const override;
		virtual void PollEvents() override;


        Vector2i GetMousePos() const override;
		Vector2i GetWindowSize() const override { return m_windowSize; }
		bool IsPaused() const override { return m_AppPaused; }

		virtual void AppendOnResizeEvent(OnResizeEvent::Func eventHandler) override;
		virtual void AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler) override;
		virtual void AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler) override;
		virtual void AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler) override;
        virtual void AddCharInputEventListener(CharInputEvent::Func eventHandler) override;

        virtual IClipBoard* GetClipBoard() const override { return ptr(m_pClipBoard); }

		HINSTANCE GetAppInst() const { return m_hWindowInstance; }
		HWND GetHWND() const { return m_hMainWnd; }

	private:
		friend LRESULT CALLBACK
			MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


		// 使用模板别名(C++11)简化类型名
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		HINSTANCE	m_hWindowInstance = nullptr;	// 应用实例句柄
		HWND		m_hMainWnd = nullptr;			// 主窗口句柄
		bool		m_AppPaused = false;			// 应用是否暂停
		bool		m_Minimized = false;			// 应用是否最小化
		bool		m_Maximized = false;			// 应用是否最大化
		bool		m_Resizing = false;				// 窗口大小是否变化
		bool		m_shouldClose = false;			// Window should close

		Vector2i	m_windowSize{};					// 窗体大小
		Vector2i	m_tmpWindowSize{};
		std::string m_windowTitle{};				// 窗体标题
		Vector2i	m_mousePos{};

		OnResizeEvent				m_eventOnWindowResize;		// Event when framebuffer size needs to change
		OnKeyChangeEvent			m_eventOnKeyChange;			// Keyboard event
		OnMouseScrollEvent			m_eventOnMouseScroll;		// Mouse scroll event
		OnMouseButtonChangeEvent	m_eventOnMouseButtonChange;	// Mouse button press event
        CharInputEvent              m_eventOnCharInput;

		std::bitset<(int)MouseButtonCode::__COUNT>				m_mouseDowned{};

        std::unique_ptr<Win32ClipBoard>     m_pClipBoard = nullptr;


		bool m_InitMainWindow(const InitArgs& args);

		LRESULT _msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void m_Resize(Vector2i size);
		void m_MouseScroll(Vector2f offset);
		void m_MouseButtonChange(UINT msg, WPARAM wParam, LPARAM lParam);
		void m_KeyChange(UINT msg, WPARAM wParam, LPARAM lParam);
	};
}