#pragma once

namespace OpenGL
{
    class FreeType
    {
    public:
        FreeType();
        ~FreeType();

        operator FT_Library& ();

    private:
        FT_Library m_ft;
    };

    class FreeTypeFont
    {
    public:
        FreeTypeFont(const Data::Bytes& bytes, const unsigned int size);
        ~FreeTypeFont();

        operator FT_Face& ();

    private:
        static FreeType m_ft;
        FT_Face m_face;
    };
}
