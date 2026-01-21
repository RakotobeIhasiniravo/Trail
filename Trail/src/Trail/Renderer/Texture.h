#pragma once
namespace Trail {
	class Texture {
	public:
		Texture(const std::string& path);
		~Texture();

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Width; }
		void Bind(uint32_t slot = 0) const;
		void BindCustom(uint32_t textureID) const;
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
	};
}