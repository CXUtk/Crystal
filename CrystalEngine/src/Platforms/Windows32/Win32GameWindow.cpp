#include "Win32GameWindow.h"
#include <Core/InitArgs.h>
#include <Core/Utils/Logger.h>

#include <array>
#include <windowsx.h>

namespace crystal
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	Win32GameWindow* g_win32Window = nullptr;

	LRESULT CALLBACK
		MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// Forward hwnd on because we can get messages (e.g., WM_CREATE)
		// before CreateWindow returns, and thus before m_hMainWnd is valid.
		return g_win32Window->_msgProc(hwnd, msg, wParam, lParam);
	}

	Win32GameWindow::Win32GameWindow(const InitArgs& args, HINSTANCE hInstance)
		: m_hWindowInstance(hInstance)
	{
		g_win32Window = this;

		m_windowSize = Vector2i(args.WindowWidth, args.WindowHeight);

		if (!m_InitMainWindow())
		{
			throw std::exception("[Win32GameWindow::Win32GameWindow] Unable to start Win32 window");
		}
	}

	Win32GameWindow::~Win32GameWindow()
	{}

	void Win32GameWindow::BeginFrame()
	{}

	void Win32GameWindow::EndFrame()
	{}

	bool Win32GameWindow::ShouldClose() const
	{
		return m_shouldClose;
	}

	void Win32GameWindow::PollEvents()
	{
		MSG msg{};

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

    Vector2i Win32GameWindow::GetMousePos() const
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            if (ScreenToClient(m_hMainWnd, &p))
            {
                return Vector2i(p.x, m_windowSize.y - p.y - 1);
            }
        }
        return Vector2i();
    }

	void Win32GameWindow::AppendOnResizeEvent(OnResizeEvent::Func eventHandler)
	{
		m_eventOnWindowResize += eventHandler;
	}

	void Win32GameWindow::AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler)
	{
		m_eventOnMouseScroll += eventHandler;
	}

	void Win32GameWindow::AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler)
	{
		m_eventOnMouseButtonChange += eventHandler;
	}

	void Win32GameWindow::AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler)
	{
		m_eventOnKeyChange += eventHandler;
	}

	bool Win32GameWindow::m_InitMainWindow()
	{
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hWindowInstance;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = "Win32WndClassName";

		if (!RegisterClass(&wc))
		{
			MessageBox(0, "RegisterClass Failed.", 0, 0);
			return false;
		}

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, m_windowSize.x, m_windowSize.y };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

		int width = R.right - R.left;
		int height = R.bottom - R.top;

		m_hMainWnd = CreateWindow("Win32WndClassName", m_windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			0, 0, m_hWindowInstance, 0);
		if (!m_hMainWnd)
		{
			MessageBox(0, "CreateWindow Failed.", 0, 0);
			return false;
		}

		ShowWindow(m_hMainWnd, SW_SHOW);
		UpdateWindow(m_hMainWnd);

		printf("DPI: %d\n", GetDpiForWindow(m_hMainWnd));
		return true;
	}

	void Win32GameWindow::m_Resize(Vector2i size)
	{
		m_windowSize = size;
		m_eventOnWindowResize.Invoke(m_windowSize);
	}

	void Win32GameWindow::m_MouseScroll(Vector2f offset)
	{
		m_eventOnMouseScroll.Invoke(offset);
	}

	constexpr size_t MAX_KEYS = 512;
	template<size_t N>
	constexpr std::array<KeyCode, N> generateKeyCodeMapper()
	{
		std::array<KeyCode, N> M{};
		M[0x00B] = KeyCode::CRYSTAL_0_KEY;
		M[0x002] = KeyCode::CRYSTAL_1_KEY;
		M[0x003] = KeyCode::CRYSTAL_2_KEY;
		M[0x004] = KeyCode::CRYSTAL_3_KEY;
		M[0x005] = KeyCode::CRYSTAL_4_KEY;
		M[0x006] = KeyCode::CRYSTAL_5_KEY;
		M[0x007] = KeyCode::CRYSTAL_6_KEY;
		M[0x008] = KeyCode::CRYSTAL_7_KEY;
		M[0x009] = KeyCode::CRYSTAL_8_KEY;
		M[0x00A] = KeyCode::CRYSTAL_9_KEY;

		M[0x01E] = KeyCode::CRYSTAL_A_KEY;
		M[0x030] = KeyCode::CRYSTAL_B_KEY;
		M[0x02E] = KeyCode::CRYSTAL_C_KEY;
		M[0x020] = KeyCode::CRYSTAL_D_KEY;
		M[0x012] = KeyCode::CRYSTAL_E_KEY;
		M[0x021] = KeyCode::CRYSTAL_F_KEY;
		M[0x022] = KeyCode::CRYSTAL_G_KEY;
		M[0x023] = KeyCode::CRYSTAL_H_KEY;
		M[0x017] = KeyCode::CRYSTAL_I_KEY;
		M[0x024] = KeyCode::CRYSTAL_J_KEY;
		M[0x025] = KeyCode::CRYSTAL_K_KEY;
		M[0x026] = KeyCode::CRYSTAL_L_KEY;
		M[0x032] = KeyCode::CRYSTAL_M_KEY;
		M[0x031] = KeyCode::CRYSTAL_N_KEY;
		M[0x018] = KeyCode::CRYSTAL_O_KEY;
		M[0x019] = KeyCode::CRYSTAL_P_KEY;
		M[0x010] = KeyCode::CRYSTAL_Q_KEY;
		M[0x013] = KeyCode::CRYSTAL_R_KEY;
		M[0x01F] = KeyCode::CRYSTAL_S_KEY;
		M[0x014] = KeyCode::CRYSTAL_T_KEY;
		M[0x016] = KeyCode::CRYSTAL_U_KEY;
		M[0x02F] = KeyCode::CRYSTAL_V_KEY;
		M[0x011] = KeyCode::CRYSTAL_W_KEY;
		M[0x02D] = KeyCode::CRYSTAL_X_KEY;
		M[0x015] = KeyCode::CRYSTAL_Y_KEY;
		M[0x02C] = KeyCode::CRYSTAL_Z_KEY;

		M[0x148] = KeyCode::CRYSTAL_UP_KEY;
		M[0x150] = KeyCode::CRYSTAL_DOWN_KEY;
		M[0x14B] = KeyCode::CRYSTAL_LEFT_KEY;
		M[0x14D] = KeyCode::CRYSTAL_RIGHT_KEY;

		M[0x01C] = KeyCode::CRYSTAL_ENTER_KEY;
		M[0x01D] = KeyCode::CRYSTAL_LEFT_CTRL_KEY;
		M[0x11D] = KeyCode::CRYSTAL_RIGHT_CTRL_KEY;
		M[0x038] = KeyCode::CRYSTAL_LEFT_ALT_KEY;
		M[0x138] = KeyCode::CRYSTAL_RIGHT_ALT_KEY;

		M[0x00D] = KeyCode::CRYSTAL_PLUS_KEY;
		M[0x00C] = KeyCode::CRYSTAL_MINUS_KEY;
		M[0x029] = KeyCode::CRYSTAL_TILDE_KEY;

		M[0x039] = KeyCode::CRYSTAL_SPACE_KEY;
		M[0x001] = KeyCode::CRYSTAL_ESC_KEY;
		return M;
	}

	constexpr auto keyCodeMap = generateKeyCodeMapper<MAX_KEYS>();

	static KeyMODState getKeyMods(void)
	{
		int mods = 0;

		if (GetKeyState(VK_SHIFT) & 0x8000)
			mods |= KeyMODState::CRYSTAL_MOD_SHIFT;
		if (GetKeyState(VK_CONTROL) & 0x8000)
			mods |= KeyMODState::CRYSTAL_MOD_CTRL;
		if (GetKeyState(VK_MENU) & 0x8000)
			mods |= KeyMODState::CRYSTAL_MOD_SHIFT;
		if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
			mods |= KeyMODState::CRYSTAL_MOD_SHIFT;
		if (GetKeyState(VK_CAPITAL) & 1)
			mods |= KeyMODState::CRYSTAL_MOD_SHIFT;
		if (GetKeyState(VK_NUMLOCK) & 1)
			mods |= KeyMODState::CRYSTAL_MOD_SHIFT;
		return (KeyMODState)mods;
	}

	void Win32GameWindow::m_MouseButtonChange(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		MouseButtonCode button;
		InputAction action;

		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
			button = MouseButtonCode::LEFT_BUTTON;
		else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
			button = MouseButtonCode::RIGHT_BUTTON;
		else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
			button = MouseButtonCode::MIDDLE_BUTTON;
		else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			button = MouseButtonCode::X_BUTTON;
		else
			button = MouseButtonCode::X_BUTTON;

		if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN ||
			msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN)
		{
			action = InputAction::PRESS;
		}
		else
		{
			action = InputAction::RELEASE;
		}

		// 鼠标按下后就一直捕获窗口
		if (!m_mouseDowned.any() && action == InputAction::PRESS)
		{
			SetCapture(m_hMainWnd);
		}

		m_mouseDowned[(int)button] = (action == InputAction::PRESS);
		MouseButtonEventArgs args;
		args.Action = action;
		args.ButtonCode = button;
		args.Mods = getKeyMods();
		m_eventOnMouseButtonChange.Invoke(args);

		if (!m_mouseDowned.any() && action == InputAction::RELEASE)
		{
			ReleaseCapture();
		}
	}

	void Win32GameWindow::m_KeyChange(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		const InputAction action = (HIWORD(lParam) & KF_UP) ? InputAction::RELEASE : InputAction::PRESS;
		const KeyMODState mods = getKeyMods();

		int scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
		if (!scancode)
		{
			// NOTE: Some synthetic key messages have a scancode of zero
			// HACK: Map the virtual key back to a usable scancode
			scancode = MapVirtualKeyW((UINT)wParam, MAPVK_VK_TO_VSC);
		}

		//printf("0x%.4X\n", scancode);
		KeyCode keyCode = keyCodeMap[scancode];

		KeyEventArgs args;
		args.KeyCode = keyCode;
		args.Action = action;
		args.Mods = mods;

		m_eventOnKeyChange.Invoke(args);
	}

	LRESULT Win32GameWindow::_msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			// WM_ACTIVATE is sent when the window is activated or deactivated.  
			// We pause the game when the window is deactivated and unpause it 
			// when it becomes active.  
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				m_AppPaused = true;
			}
			else
			{
				m_AppPaused = false;
			}
			return 0;

			// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			// Save the new client area dimensions.
			m_tmpWindowSize.x = LOWORD(lParam);
			m_tmpWindowSize.y = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				m_Resize(m_tmpWindowSize);
			}
			else if (wParam == SIZE_RESTORED)
			{
				// Restoring from minimized state?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					m_Resize(m_tmpWindowSize);
				}

				// Restoring from maximized state?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					m_Resize(m_tmpWindowSize);
				}
				else if (m_Resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or m_pSwapChain->SetFullscreenState.
				{
					m_Resize(m_tmpWindowSize);
				}

			}
			return 0;

			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		case WM_ENTERSIZEMOVE:
			m_AppPaused = true;
			m_Resizing = true;
			// m_Timer.Stop();
			return 0;

			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			m_AppPaused = false;
			m_Resizing = false;
			// m_Timer.Start();
			m_Resize(m_tmpWindowSize);
			return 0;

			// WM_DESTROY is sent when the window is being destroyed.
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			m_shouldClose = true;
			PostQuitMessage(0);
			return 0;

			// The WM_MENUCHAR message is sent when a menu is active and the user presses 
			// a key that does not correspond to any mnemonic or accelerator key. 
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);

			// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			m_MouseButtonChange(msg, wParam, lParam);
		}
			return 0;
		case WM_MOUSEMOVE:
		{
			const int x = GET_X_LPARAM(lParam);
			const int y = GET_Y_LPARAM(lParam);
			m_mousePos.x = x;
			m_mousePos.y = m_windowSize.y - y - 1;
			return 0;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			m_KeyChange(msg, wParam, lParam);
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			m_MouseScroll(Vector2f(0.0, (SHORT)HIWORD(wParam) / (double)WHEEL_DELTA));
			return 0;
		}

		case WM_MOUSEHWHEEL:
		{
			m_MouseScroll(Vector2f(-(SHORT)HIWORD(wParam) / (double)WHEEL_DELTA, 0.0));
			return 0;
		}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

}
