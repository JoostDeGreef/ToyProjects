#include <string>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

#include "Data.h"
#include "OpenGL.h"
using namespace OpenGL;

CharacterMap::CharacterMap(const Data::Bytes& bytes, const unsigned int size)
    : m_characters()
    , m_textureId(0)
{
    FreeTypeFont font(bytes, size);
    LoadCharacters(font,
        " "
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "!@#$%^&*()-_=+`~[]{};:'\"\\|,.<>/?");
}

void CharacterMap::RenderText(const std::string& text, double x, double y, double scale_x, double scale_y, OpenGL::Color& color)
{
    Shaders::Type prevShader = Shaders::Select(Shaders::Type::Text);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int inputColorLocation = Shaders::GetUniformLocation("inputColor");
    int textureLocation = Shaders::GetUniformLocation("texture");

    glUniform1i(textureLocation, 0); // sampler, not texture!
    glUniform4fv(inputColorLocation, 1, color.GetRGBA());

    glBegin(GL_TRIANGLES);
    for (const char c : text)
    {
        const Character& character = GetCharacter(c);

        double tx0 = character.GetTextureX() * m_scale;
        double tx1 = (character.GetTextureX() + character.GetWidth()) * m_scale;
        double ty0 = character.GetTextureY() * m_scale;
        double ty1 = (character.GetTextureY() + character.GetHeight()) * m_scale;

        double w = character.GetWidth() * scale_x;
        double h = character.GetHeight() * scale_y;
        double bx = character.GetBearingX() * scale_x / 64;
        double by = character.GetBearingY() * scale_y / 64;

        /* Skip glyphs that have no pixels */
        if (w && h)
        {
            glTexCoord2d(tx0, ty1);
            glVertex2d(x + bx, y - h + by);
            glTexCoord2d(tx1, ty0);
            glVertex2d(x + w + bx, y + by);
            glTexCoord2d(tx0, ty0);
            glVertex2d(x + bx, y + by);

            glTexCoord2d(tx1, ty1);
            glVertex2d(x + w + bx, y - h + by);
            glTexCoord2d(tx1, ty0);
            glVertex2d(x + w + bx, y + by);
            glTexCoord2d(tx0, ty1);
            glVertex2d(x + bx, y - h + by);
        }

        /* Advance the cursor to the start of the next character */
        x += character.GetAdvanceX() * scale_x / 64;
        y += character.GetAdvanceY() * scale_y / 64;
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    Shaders::Select(prevShader);
}

// this ignores whatever falls below the baseline of the glyph
Size CharacterMap::MeasureText(const std::string& text, double scale_x, double scale_y)
{
    double x = 0;
    double y = 0;
    for (const char c : text)
    {
        const Character& character = GetCharacter(c);

        double h = character.GetHeight() * scale_y;
        x += character.GetAdvanceX() * scale_x / 64;

        y = std::max(y, h);
    }
    return Size(x, y);
}

void CharacterMap::LoadCharacters(FreeTypeFont& font, const std::string& chars)
{
    FT_Face face = font;
    FT_GlyphSlot g = face->glyph;
    int64_t surface = 0;
    std::vector<Character> characters;

    for (char c : chars)
    {
        if (0 != FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            //                fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }
        surface += g->bitmap.width * g->bitmap.rows;
        characters.emplace_back(c, g->bitmap.width, g->bitmap.rows, g->bitmap_left, g->bitmap_top, g->advance.x, g->advance.y, g->metrics.horiBearingX, g->metrics.horiBearingY);
    }

    // sort the glyps with the tallest ones in front
    sort(characters.begin(), characters.end(), [](Character& a, Character& b)
        {
            return a.GetHeight() == b.GetHeight() ? a.GetWidth() > b.GetWidth() : a.GetHeight() > b.GetHeight();
        });

    // see how big the (square) texture should be to contain all glyphs.
    int side = (int)pow(2, ceil(log2(sqrt(surface) / 2)));
    int height;
    do
    {
        side *= 2;
        height = 0;
        int x = 0;
        int y = 0;
        for (Character& c : characters)
        {
            if (c.GetWidth() + x + 2 > side)
            {
                x = 0;
            }
            if (x == 0)
            {
                y = height;
                height += c.GetHeight() + 2;
            }
            c.SetTexturePos(x + 1, y + 1);
            x += c.GetWidth() + 2;
        }
    } while (height > side);

    // create the texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, side, side, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    for (const Character& c : characters)
    {
        if (0 == FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, c.GetTextureX(), c.GetTextureY(),
                g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
        }
        else
        {
            int dummy = 0;
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // store the characters
    for (Character& c : characters)
    {
        m_characters.emplace(c, c);
    }

    // store 1/side for texture coord normalization
    m_scale = 1.0 / side;
}

Character& CharacterMap::GetCharacter(char c)
{
    auto iter = m_characters.find(c);
    if (iter == m_characters.end())
    {
        iter = m_characters.find('?');
        assert(iter != m_characters.end());
    }
    return iter->second;
}

