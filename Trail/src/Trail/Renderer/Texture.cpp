#include "trlpch.h"
#include "Texture.h"
#include <glad/glad.h>
#include "stb_image.h"
Trail::Texture::Texture(const std::string& path)
	: m_Path(path)
{
	int width, height, channels;

	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data =  stbi_load(path.c_str(),&width, &height, &channels, 0);
	if(data)
		//TRL_ERROR("DATA EXISTS ");
	TRL_CORE_ASSERT(data, "failed to load image, returned null from data");
	m_Width = width;
	m_Height = height;




	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
		//TRL_ERROR("CHANNELS SUPPORTED! CHANNELS {0}", channels);
	}
	else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
		//TRL_ERROR("CHANNELS SUPPORTED! CHANNELS {0}", channels);
	}
	else if (channels == 2) {
		internalFormat = GL_RG8;
		dataFormat = GL_RG;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);   // Use grayscale value as Red
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);   // Use grayscale value as Green
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);   // Use grayscale value as Blue
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
		//TRL_ERROR("CHANNELS SUPPORTED! CHANNELS {0}", channels);
	}
	else if (channels == 1) {
		internalFormat = GL_R8;
		dataFormat = GL_RED;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
		//TRL_ERROR("CHANNELS SUPPORTED! CHANNELS {0}", channels);
	}
	else {
		TRL_ERROR("CHANNELS NOT SUPPORTED! CHANNELS {0}", channels);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
	
	stbi_image_free(data);
}

Trail::Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Trail::Texture::Bind( uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
