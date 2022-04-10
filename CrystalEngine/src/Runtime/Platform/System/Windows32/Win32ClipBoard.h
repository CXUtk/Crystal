#pragma once
#include "Platform/RHI/Interfaces.h"

namespace crystal
{
    class Win32GameWindow;
    class Win32ClipBoard : public IClipBoard
    {
    public:
        Win32ClipBoard(Win32GameWindow* window);
        ~Win32ClipBoard() override;

        virtual void Copy(const std::u32string& text) override;
        virtual std::u32string Paste() override;

    private:
        Win32GameWindow*    m_pWindow;
    };
}