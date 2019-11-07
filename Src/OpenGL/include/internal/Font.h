#pragma once

namespace OpenGL
{
    class Font
    {
    public:
        Font(std::string&& fontName, const int size = 12);

        Font& PixelSize(const double pixelSize);
        Font& Color(const Color& color);

        const std::string& GetName() const { return m_fontName; }

        void Draw(const double x, const double y, const std::string& text);
        Size GetSize(const std::string& text);

    private:
        std::string m_fontName;
        int m_size;
        double m_pixelSize;
        OpenGL::Color m_color;
    };
}; // namespace Viewer

