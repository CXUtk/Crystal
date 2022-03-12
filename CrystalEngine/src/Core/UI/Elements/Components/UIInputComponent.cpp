#include "UIInputComponent.h"
#include <codecvt>

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UIInputComponent::UIInputComponent(const std::string& initText)
    {
        m_inputController = Engine::GetInstance()->GetInputController();
    }

    UIInputComponent::~UIInputComponent()
    {}

    void UIInputComponent::MoveCarrot(int pos)
    {
        assert(m_carrotPos >= 0 && m_carrotPos <= m_text.size());
        m_carrotPos = pos;
    }


    void UIInputComponent::SampleInput()
    {
        auto sequence = m_inputController->GetKeyDownSequence();
        auto charInputs = m_inputController->GetInputCharSequence();

        for (auto c : charInputs)
        {
            InsertValue(c, m_carrotPos);
        }

        m_carrotChanged = !sequence.empty();
        for (auto& keyArgs : sequence)
        {
            if (HandleControls(keyArgs))
            {
                continue;
            }
            auto keyCode = keyArgs.KeyCode;
            if (keyCode >= KeyCode::CRYSTAL_A_KEY && keyCode <= KeyCode::CRYSTAL_SPACE_KEY)
            {
                bool holdShift = keyArgs.Mods & KeyMODState::CRYSTAL_MOD_SHIFT;
                InsertValue(holdShift ? InputCommon::KeyCodeToCharMapUpper(keyCode)
                    : InputCommon::KeyCodeToCharMapLower(keyCode), m_carrotPos);
            }
            else if (keyCode == KeyCode::CRYSTAL_BACK_KEY)
            {
                BackValue(1);
            }
            else if (keyCode == KeyCode::CRYSTAL_LEFT_KEY)
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
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        return conv.to_bytes(m_text);
    }

    bool UIInputComponent::HandleControls(const KeyEventArgs& args)
    {
        if (args.Mods & KeyMODState::CRYSTAL_MOD_CTRL)
        {
            if (args.KeyCode == KeyCode::CRYSTAL_C_KEY)
            {
                // Ctrl + C
            }
            else if (args.KeyCode == KeyCode::CRYSTAL_V_KEY)
            {
                // Ctrl + V
            }
            else if (args.KeyCode == KeyCode::CRYSTAL_X_KEY)
            {
                // Ctrl + X
            }
            return true;
        }
        return false;
    }

    void UIInputComponent::InsertValue(char32_t c, int index)
    {
        if (index == m_text.size())
        {
            m_text.push_back(c);
        }
        else
        {
            m_text.insert(m_text.begin() + index, c);
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
            m_text.erase(m_text.begin() + m_selectionLeft, m_text.begin() + m_selectionRight);
            m_carrotPos = m_selectionLeft;
            m_selectionRight = m_selectionLeft;
        }
    }
}