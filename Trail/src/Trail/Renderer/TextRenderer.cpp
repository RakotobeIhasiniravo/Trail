#include "trlpch.h"
#include "TextRenderer.h"


namespace Trail {
	TextRenderer::TextRenderer(const char* fontfilename)
	{
        if (FT_Init_FreeType(&m_Ft))
            TRL_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");

        if (FT_New_Face(m_Ft, fontfilename, 0, &m_Face))
            TRL_CORE_ERROR("ERROR::FREETYPE: Failed to load font");

        if (FT_Select_Charmap(m_Face, FT_ENCODING_UNICODE))
            TRL_CORE_ERROR("ERROR::FREETYPE: Failed to set Unicode charmap");

        // Preload a range of sizes
        for (unsigned int size = 10; size <= 500; size += 10)
            LoadFontSize(size);

        TRL_CORE_INFO("Font loaded with sizes 10,20,...,100");
	}
    void TextRenderer::LoadFontSize(unsigned int fontSize)
    {
        if (m_FontSizes.find(fontSize) != m_FontSizes.end())
            return; // already loaded

        FT_Set_Pixel_Sizes(m_Face, 0, fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        std::map<char, Character> characters;
        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
            {
                TRL_CORE_ERROR("ERROR::FREETYPE: Failed to load Glyph for char {}", c);
                continue;
            }

            unsigned int texture;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GLenum internalFormat = GL_R8, dataFormat = GL_RED;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
            glGenTextures(1, &texture); glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                internalFormat,
                m_Face->glyph->bitmap.width,
                m_Face->glyph->bitmap.rows,
                0,
                dataFormat,
                GL_UNSIGNED_BYTE,
                m_Face->glyph->bitmap.buffer
            );

            Character character = {
                texture,
                glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
                glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
                m_Face->glyph->advance.x
            };
            characters.insert(std::pair<char, Character>(c, character));
        }

        m_FontSizes[fontSize] = characters;
    }


	TextRenderer::~TextRenderer()
	{
        for (auto& [size, chars] : m_FontSizes)
            for (auto& [c, ch] : chars)
                glDeleteTextures(1, &ch.TextureID);

        FT_Done_Face(m_Face);
        FT_Done_FreeType(m_Ft);
	}
}

