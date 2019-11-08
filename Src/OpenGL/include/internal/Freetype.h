#pragma once

namespace OpenGL
{
    class FreeType
    {
    public:
        FreeType()
        {
            if (FT_Init_FreeType(&m_ft) != 0)
            {
                // todo: exception?
                //            fprintf(stderr, "Could not init freetype library\n");
                //            return 1;
            }
        }

        ~FreeType()
        {
            FT_Done_FreeType(m_ft);
        }

        operator FT_Library& () { return m_ft; }

    private:
        FT_Library m_ft;
    };

    class FreeTypeFont
    {
    public:
        FreeTypeFont(std::shared_ptr<Data>& data, const std::string& fontName, const unsigned int size)
        {
            m_bytes = data->operator["fonts"].GetFont(fontName);
            if (FT_New_Memory_Face(m_ft, m_bytes.data(), (FT_Long)m_bytes.size(), (FT_Long)0, &m_face) != 0)
            {
                // todo: exception?
                //            fprintf(stderr, "Could not open font\n");
                //            return 1;
            }
            FT_Set_Pixel_Sizes(m_face, 0, size);
        }
        ~FreeTypeFont()
        {
            FT_Done_Face(m_face);
        }

        operator FT_Face& () { return m_face; }

    private:
        static FreeType m_ft;
        FT_Face m_face;
        std::vector<unsigned char> m_bytes;
    };
}
