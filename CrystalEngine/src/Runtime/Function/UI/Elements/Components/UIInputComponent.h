#pragma once
#include "../../Common/UICommon.h"
#include "../../Common/UIEventArgs.h"

#include <Core/Asset/Content/Font.h>

namespace crystal
{
    class UIInputComponent
    {
    public:
        UIInputComponent(const std::string& initText);
        ~UIInputComponent();

        void MoveCarrot(int pos);
        void SampleInput();
        void SetSelectionRange(int l, int r);
        void GetSelectionRange(int& l, int& r);

        bool IsCarrotChanged() const { return m_carrotChanged; }
        std::string GetText() const;
        std::u32string GetText32() const { return m_text; }
        size_t GetLength() const { return m_text.size(); }
        int GetCarrotPos() const { return m_carrotPos; }

        void SetText(const std::string& text);
    private:
        InputController*                    m_inputController;
        IGameWindow*                        m_gameWindow;
        std::u32string                      m_text{};
        int                                 m_carrotPos = 0;
        bool                                m_carrotChanged = false;
        int                                 m_selectionLeft = 0;
        int                                 m_selectionRight = 0;

        bool HandleControls(const KeyEventArgs& args);
        void EraseSelection();
        void InsertValue(char32_t c);
        void BackValue(int count);
    };
}