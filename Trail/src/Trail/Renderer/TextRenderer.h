#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <glm/ext/vector_int2.hpp>
#include <glad/glad.h>
namespace Trail {
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	
	class TextRenderer {
	public:
		std::map<unsigned int, std::map<char, Character>> m_FontSizes;
		FT_Face m_Face;

		TextRenderer(const char* fontfilename);
		~TextRenderer();
		
		void LoadFontSize(unsigned int fontSize);
	private:
		FT_Library m_Ft;
		
		
	};
}