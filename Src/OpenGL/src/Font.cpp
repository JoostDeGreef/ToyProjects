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
#include "Freetype.h"
#include "CharacterMap.h"
using namespace OpenGL;

CharacterMap& GetCharacterMap(const std::string& fontName, const unsigned int size)
{
    static std::unordered_map<std::string, CharacterMap> characterMaps;
    std::string id = std::to_string(size) + ";" + fontName;
    auto iter = characterMaps.find(id);
    if (iter == characterMaps.end())
    {
        iter = characterMaps.emplace(id, CharacterMap(fontName, size)).first;
    }
    return iter->second;
}


Font::Font(std::shared_ptr<Data> & data,std::string&& fontName, const int size)
    : m_fontName(std::move(fontName))
    , m_size(size)
    , m_pixelSize(1 / 800)
    , m_color(1, 1, 1, 1)
    , m_data(std::move(data))
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
    CharacterMap& characterMap = GetCharacterMap(m_fontName, m_size);
    characterMap.RenderText(std::move(text), x, y, m_pixelSize, m_pixelSize, m_color);
}

Size Font::GetSize(const std::string& text)
{
    CharacterMap& characterMap = GetCharacterMap(m_fontName, m_size);
    return characterMap.MeasureText(std::move(text), m_pixelSize, m_pixelSize);
}
