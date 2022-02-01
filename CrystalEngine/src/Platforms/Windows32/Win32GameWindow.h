#pragma once
#include <Core/Platform/Platforms.h>
#include <Crystal.h>

#include <wrl/client.h>
#include <string>
#include <bitset>

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


		Vector2i GetMousePos() const override { return m_mousePos; }
		Vector2i GetWindowSize() const override { return m_windowSize; }
		bool IsPaused() const override { return m_AppPaused; }

		virtual void AppendOnResizeEvent(OnResizeEvent::Func eventHandler) override;
		virtual void AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler) override;
		virtual void AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler) override;
		virtual void AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler) override;

		HINSTANCE GetAppInst() const { return m_hWindowInstance; }
		HWND GetHWND() const { return m_hMainWnd; }

	private:
		friend LRESULT CALLBACK
			MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


		// ʹ��ģ�����(C++11)��������
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		HINSTANCE	m_hWindowInstance = nullptr;	// Ӧ��ʵ�����
		HWND		m_hMainWnd = nullptr;			// �����ھ��
		bool		m_AppPaused = false;			// Ӧ���Ƿ���ͣ
		bool		m_Minimized = false;			// Ӧ���Ƿ���С��
		bool		m_Maximized = false;			// Ӧ���Ƿ����
		bool		m_Resizing = false;				// ���ڴ�С�Ƿ�仯
		bool		m_shouldClose = false;			// Window should close

		Vector2i	m_windowSize{};					// �����С
		std::string m_windowTitle{};				// �������
		Vector2i	m_mousePos{};

		OnResizeEvent				m_eventOnWindowResize;		// Event when framebuffer size needs to change
		OnKeyChangeEvent			m_eventOnKeyChange;			// Keyboard event
		OnMouseScrollEvent			m_eventOnMouseScroll;		// Mouse scroll event
		OnMouseButtonChangeEvent	m_eventOnMouseButtonChange;	// Mouse button press event

		std::bitset<10>				m_mouseDowned{};


		bool initMainWindow();

		LRESULT _msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void resize();
		void mouseScroll(Vector2f offset);
		void mouseButtonChange(UINT msg, WPARAM wParam, LPARAM lParam);
		void keyChange(UINT msg, WPARAM wParam, LPARAM lParam);
	};
}