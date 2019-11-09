#pragma once

namespace OpenGL
{
    class Character
    {
    public:
        Character(const char c,
            const int width, const int height,
            const int left, const int top,
            const int advance_x, const int advance_y,
            const int bearing_x, const int bearing_y)
            : m_char(c)
            , m_width(width)
            , m_height(height)
            , m_left(left)
            , m_top(top)
            , m_advance_x(advance_x)
            , m_advance_y(advance_y)
            , m_bearing_x(bearing_x)
            , m_bearing_y(bearing_y)
        {}

        operator char() const { return m_char; }

        int GetHeight() const { return m_height; }
        int GetWidth() const { return m_width; }

        int GetTextureX() const { return m_texture_x; }
        int GetTextureY() const { return m_texture_y; }

        int GetAdvanceX() const { return m_advance_x; }
        int GetAdvanceY() const { return m_advance_y; }

        int GetBearingX() const { return m_bearing_x; }
        int GetBearingY() const { return m_bearing_y; }

        void SetTexturePos(const int x, const int y) { m_texture_x = x; m_texture_y = y; }
    private:
        char m_char;
        int m_width;
        int m_height;
        int m_left;
        int m_top;
        int m_advance_x;
        int m_advance_y;
        int m_bearing_x;
        int m_bearing_y;
        int m_texture_x;
        int m_texture_y;
    };

    class CharacterMap
    {
    public:
        CharacterMap(const Data::Bytes& bytes, const unsigned int size);
        void RenderText(const std::string& text, double x, double y, double scale_x, double scale_y, OpenGL::Color& color);

        // this ignores whatever falls below the baseline of the glyph
        Size MeasureText(const std::string& text, double scale_x, double scale_y);

    protected:
        void LoadCharacters(FreeTypeFont& font, const std::string& chars);

        Character& GetCharacter(char c);

    private:
        std::unordered_map<char, Character> m_characters;
        unsigned int m_textureId;
        double m_scale;
    };
}

