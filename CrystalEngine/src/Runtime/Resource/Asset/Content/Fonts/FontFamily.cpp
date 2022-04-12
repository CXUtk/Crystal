#include "FontFamily.h"
#include "Font.h"
#include "Platform/RHI/Interfaces.h"

namespace crystal
{
    FontFamily::FontFamily(FT_Face face)
        : m_fontFace(face)
    {

    }

    FontFamily::~FontFamily()
    {}

    std::shared_ptr<Font> FontFamily::GetFont(int fontSize)
    {
        std::map<int, std::shared_ptr<Font>>::iterator p;
        if ((p = m_fontMap.find(fontSize)) == m_fontMap.end())
        {
            auto font = std::make_shared<Font>(this, fontSize);
            m_fontMap[fontSize] = font;
            return font;
        }
        return p->second;
    }
}
