#include "Win32ClipBoard.h"
#include <Windows.h>
#include <utf8.h>

#include "Utils/Encoding.h"
#include "Win32GameWindow.h"

namespace crystal
{
    Win32ClipBoard::Win32ClipBoard(Win32GameWindow* window)
        : m_pWindow(window)
    {}

    Win32ClipBoard::~Win32ClipBoard()
    {}

    void Win32ClipBoard::Copy(const std::u32string& text)
    {
        if (!OpenClipboard(m_pWindow->GetHWND()))
            return;

        EmptyClipboard();

        auto wstr = Win32Encoding::Utf8StringToWString(utf8::utf32to8(text));

        HANDLE hglbCopy = GlobalAlloc(GMEM_MOVEABLE,
            (wstr.size() + 1) * sizeof(wchar_t));
        if (hglbCopy == NULL)
        {
            CloseClipboard();
            return;
        }

        // Lock the handle and copy the text to the buffer. 

        void* memptr = GlobalLock(hglbCopy);
        if (!memptr)
        {
            return;
        }
        memcpy(memptr, wstr.data(),
            wstr.size() * sizeof(wchar_t));

        auto p = (wchar_t*)memptr;
        p[wstr.size()] = 0;    // null character 
        GlobalUnlock(hglbCopy);

        SetClipboardData(CF_UNICODETEXT, hglbCopy);
        CloseClipboard();
    }

    std::u32string Win32ClipBoard::Paste()
    {
        std::u32string result;
        if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
            return result;
        if (!OpenClipboard(m_pWindow->GetHWND()))
            return result;

        HANDLE hglb = GetClipboardData(CF_UNICODETEXT);

        std::wstring text;
        if (hglb != NULL)
        {
            wchar_t* memptr = (wchar_t*)GlobalLock(hglb);
            if (memptr != NULL)
            {
                // Call the application-defined ReplaceSelection 
                // function to insert the text and repaint the 
                // window. 
                text.assign(memptr);
                GlobalUnlock(hglb);
            }
        }
        CloseClipboard();
        return utf8::utf8to32(Win32Encoding::WStringToUtf8String(text));
    }
}
