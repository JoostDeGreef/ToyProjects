#pragma once

namespace OpenGL
{
    class Font
    {
    public:
        Font(Data::Bytes&& bytes, const int size = 12);

        Font& PixelSize(const double pixelSize);
        Font& Color(const Color& color);

        void Draw(const double x, const double y, const std::string& text);
        Size GetSize(const std::string& text);

    protected:
        CharacterMap& GetCharacterMap(const unsigned int size);

    private:
        std::unordered_map<int, CharacterMap> m_characterMaps;
        Data::Bytes m_bytes;
        int m_size;
        double m_pixelSize;
        OpenGL::Color m_color;
    };
}; // namespace Viewer

