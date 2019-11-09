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

using namespace OpenGL;

FreeType::FreeType()
{
    if (FT_Init_FreeType(&m_ft) != 0)
    {
        // todo: exception?
        //            fprintf(stderr, "Could not init freetype library\n");
        //            return 1;
    }
}

FreeType::~FreeType()
{
    FT_Done_FreeType(m_ft);
}

FreeType::operator FT_Library& ()
{
    return m_ft;
}

FreeTypeFont::FreeTypeFont(const Data::Bytes& bytes, const unsigned int size)
{
    if (FT_New_Memory_Face(m_ft, bytes.data(), (FT_Long)bytes.size(), (FT_Long)0, &m_face) != 0)
    {
        // todo: exception?
        //            fprintf(stderr, "Could not open font\n");
        //            return 1;
    }
    FT_Set_Pixel_Sizes(m_face, 0, size);
}

FreeTypeFont::~FreeTypeFont()
{
    FT_Done_Face(m_face);
}

FreeTypeFont::operator FT_Face& ()
{
    return m_face;
}

FreeType FreeTypeFont::m_ft;


