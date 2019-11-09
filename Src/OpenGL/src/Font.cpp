#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Data.h"
#include "OpenGL.h"
using namespace OpenGL;

CharacterMap& Font::GetCharacterMap(const unsigned int size)
{
    auto iter = m_characterMaps.find(size);
    if (iter == m_characterMaps.end())
    {
        iter = m_characterMaps.emplace(size, CharacterMap(m_bytes, size)).first;
    }
    return iter->second;
}

Font::Font(Data::Bytes&& bytes, const int size)
    : m_size(size)
    , m_pixelSize(1 / 800)
    , m_color(1, 1, 1, 1)
    , m_bytes(std::move(bytes))
{}

Font& Font::PixelSize(const double pixelSize)
{
    m_pixelSize = pixelSize;
    return *this;
}

Font& Font::Color(const OpenGL::Color& color)
{
    m_color = color;
    return *this;
}

void Font::Draw(const double x, const double y, const std::string& text)
{
    CharacterMap& characterMap = GetCharacterMap(m_size);
    characterMap.RenderText(text, x, y, m_pixelSize, m_pixelSize, m_color);
}

Size Font::GetSize(const std::string& text)
{
    CharacterMap& characterMap = GetCharacterMap(m_size);
    return characterMap.MeasureText(text, m_pixelSize, m_pixelSize);
}
