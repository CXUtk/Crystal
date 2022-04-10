#include "UIInputComponent.h"
#include <utf8.h>

#include <Engine.h>

#include "Resource/Asset/AssetManager.h"
#include "Function/Render/RenderExports.h"
#include "Function/Input/InputController.h"

namespace crystal
{
    UIInputComponent::UIInputComponent(const std::string& initText)
    {
        m_inputController = Engine::GetInstance()->GetInputController();
        m_gameWindow = Engine::GetInstance()->GetWindow();
    }

    UIInputComponent::~UIInputComponent()
    {}

    void UIInputComponent::MoveCarrot(int pos)
    {
        assert(pos >= 0 && pos <= m_text.size());
        m_carrotPos = pos;
    }


    void UIInputComponent::SampleInput()
    {
        auto sequence = m_inputController->GetKeyDownSequence();
        auto charInputs = m_inputController->GetInputCharSequence();

        m_carrotChanged = !sequence.empty() || !charInputs.empty();


        for (auto& keyArgs : sequence)
        {
            if (HandleControls(keyArgs))
            {
                continue;
            }
            auto keyCode = keyArgs.KeyCode;
            if (keyCode == KeyCode::CRYSTAL_LEFT_KEY)
            {
                if (keyArgs.Mods & KeyMODState::CRYSTAL_MOD_SHIFT)
                {
                    if (m_selectionLeft == m_selectionRight)
                    {
                        m_selectionLeft = m_selectionRight = m_carrotPos;
                    }
                    if (m_carrotPos > 0)
                    {
                        m_carrotPos--;
                    }
                    if (keyArgs.Mods & KeyMODState::CRYSTAL_MOD_SHIFT)
                    {
                        m_selectionLeft = m_carrotPos;
                    }
                }
                else
                {
                    if (m_selectionLeft == m_selectionRight)
                    {
                        if (m_carrotPos > 0)
                        {
                            m_carrotPos--;
                        }
                    }
                    else
                    {
                        m_selectionLeft = m_selectionRight = m_carrotPos;
                    }
                }
            }
            else if (keyCode == KeyCode::CRYSTAL_RIGHT_KEY)
            {
                if (keyArgs.Mods & KeyMODState::CRYSTAL_MOD_SHIFT)
                {
                    if (m_selectionLeft == m_selectionRight)
                    {
                        m_selectionLeft = m_selectionRight = m_carrotPos;
                    }
                    if (m_carrotPos < m_text.size())
                    {
                        m_carrotPos++;
                    }
                    if (keyArgs.Mods & KeyMODState::CRYSTAL_MOD_SHIFT)
                    {
                        m_selectionRight = m_carrotPos;
                    }
                }
                else
                {
                    if (m_selectionLeft == m_selectionRight)
                    {
                        if (m_carrotPos < m_text.size())
                        {
                            m_carrotPos++;
                        }
                    }
                    else
                    {
                        m_selectionLeft = m_selectionRight = m_carrotPos;
                    }
                }
            }
        }

        for (auto c : charInputs)
        {
            if (c == '\b')
            {
                BackValue(1);
            }
            else if(c >= ' ')
            {
                InsertValue(c);
            }
        }
    }

    void UIInputComponent::SetSelectionRange(int l, int r)
    {
        m_selectionLeft = l;
        m_selectionRight = r;
    }

    void UIInputComponent::GetSelectionRange(int& l, int& r)
    {
        l = m_selectionLeft, r = m_selectionRight;
    }

    std::string UIInputComponent::GetText() const
    {
        return utf8::utf32to8(m_text);
    }

    void UIInputComponent::SetText(const std::string& text)
    {
        auto&& text32 = utf8::utf8to32(text);
        m_text = std::move(text32);
        MoveCarrot(m_text.size());
        m_selectionLeft = m_selectionRight = m_carrotPos;
    }

    bool UIInputComponent::HandleControls(const KeyEventArgs& args)
    {
        if (args.Mods & KeyMODState::CRYSTAL_MOD_CTRL)
        {
            auto clipboard = m_gameWindow->GetClipBoard();
            if (args.KeyCode == KeyCode::CRYSTAL_C_KEY)
            {
                // Ctrl + C
                clipboard->Copy(m_text.substr(m_selectionLeft, m_selectionRight - m_selectionLeft));
            }
            else if (args.KeyCode == KeyCode::CRYSTAL_V_KEY)
            {
                // Ctrl + V
                auto str = clipboard->Paste();
                for (auto c : str)
                {
                    InsertValue(c);
                }
            }
            else if (args.KeyCode == KeyCode::CRYSTAL_X_KEY)
            {
                // Ctrl + X
                clipboard->Copy(m_text.substr(m_selectionLeft, m_selectionRight - m_selectionLeft));
                EraseSelection();
            }
            return true;
        }
        return false;
    }

    void UIInputComponent::EraseSelection()
    {
        m_text.erase(m_text.begin() + m_selectionLeft, m_text.begin() + m_selectionRight);
        m_carrotPos = m_selectionLeft;
        m_selectionRight = m_selectionLeft;
    }

    void UIInputComponent::InsertValue(char32_t c)
    {
        if (m_selectionLeft != m_selectionRight)
        {
            EraseSelection();
        }

        if (m_carrotPos == m_text.size())
        {
            m_text.push_back(c);
        }
        else
        {
            m_text.insert(m_text.begin() + m_carrotPos, c);
        }
        m_carrotPos++;
    }

    void UIInputComponent::BackValue(int count)
    {
        if (m_selectionLeft == m_selectionRight)
        {
            if (m_carrotPos > 0)
            {
                if (m_carrotPos == m_text.size())
                {
                    m_text.pop_back();
                }
                else
                {
                    m_text.erase(m_text.begin() + m_carrotPos - 1);
                }
                m_carrotPos--;
            }
        }
        else
        {
            EraseSelection();
        }
    }
}