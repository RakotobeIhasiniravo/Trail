#include "trlpch.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Trail {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	std::shared_ptr<Trail::TextRenderer> Trail::Renderer::m_TextRenderer = nullptr;
	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->m_TexShader->Bind();
		m_SceneData->m_TexShader->SetUniformMat4f("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		if ((float)width / height >= 1920.0f / 1080.0f) {
			uint32_t viewportWidth = (int)(height * (1920.0f / 1080.0f));
			RenderCommand::SetViewport((width - viewportWidth) / 2, 0, viewportWidth, height);
		}
		else {
			uint32_t viewportHeight = (int)(width / (1920.0f / 1080.0f));
			RenderCommand::SetViewport(0, (height - viewportHeight) / 2, width, viewportHeight);
		}
		
	}
	void Renderer::Init()
	{
		float positions[4 * 5] = {
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		unsigned int indeces[] = {
			0,1,2,2,3,0
		};

		m_SceneData->m_QuadVertexArray.reset(new VertexArray());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(new VertexBuffer(positions, sizeof(positions)));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(new IndexBuffer(indeces, sizeof(indeces) / sizeof(uint32_t)));


		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float2, "a_TexCoords"}
		};
		vertexBuffer->SetLayout(layout);

		m_SceneData->m_QuadVertexArray->AddVertexBuffer(vertexBuffer);
		m_SceneData->m_QuadVertexArray->SetIndexBuffer(indexBuffer);

		m_SceneData->m_TexShader.reset(
			new Shader("Assets/Shaders/baseShader.glsl")
		);
		m_SceneData->m_TexShader->SetUniformInt("u_Texture", 0);

	/*	m_Texture.reset(new Trail::Texture("assets/Textures/KCLogoB.png"));
		m_RainbowTexture.reset(new Trail::Texture("assets/Textures/rainbow.png"));*/
		//m_SceneData->m_QuadShader->SetUniformMat4f("u_ViewProjection", m_SceneData->ProjectionViewMatrix);
		/*m_TextRenderer = std::make_shared<TextRenderer>("assets/Textures/Roboto.ttf");*/
		
		RenderCommand::Init();
	}
	void Renderer::Shutdown()
	{
		delete m_SceneData;
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{	//DRAW QUAD x,y position, flat color, no texture
		DrawQuad({position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{	//DRAW QUAD x,y,z position, flat color, no texture
		std::shared_ptr<Texture> defaultTexture; 
		defaultTexture.reset(new Texture("Assets/Shaders/WhitePixel.png"));
		DrawQuad(position, size, color, defaultTexture);
	}
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Trail::Texture>& texture) 
	{	//DRAW QUAD x,y position, no color, with texture
		DrawQuad({ position.x, position.y, 0.0f }, size, { 1.0f,1.0f,1.0f,1.0f }, texture);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Trail::Texture>& texture)
	{	//DRAW QUAD x,y,z position, no color, with texture
		DrawQuad(position, size, {1.0f,1.0f,1.0f,1.0f}, texture);
	}
	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color , const std::shared_ptr<Trail::Texture>& texture)
	{	//DRAW QUAD x,y position, flat color, with texture
		DrawQuad({ position.x, position.y, 0.0f }, size, color, texture);
	}

	void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
		const std::shared_ptr<Trail::Texture>& texture)
	{	//activer le shader
		m_SceneData->m_TexShader->Bind(); 
		
		//enregistrer les données à dessiner dans le shader
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f),
			{ size.x, size.y, 1.0f });
		m_SceneData->m_TexShader->SetUniformMat4f("u_Transform", transform);
		m_SceneData->m_TexShader->SetUniformFloat4("u_Color", color);

		//activer la texture
		texture->Bind();
		m_SceneData->m_QuadVertexArray->Bind();

		//rendu final
		RenderCommand::DrawIndexed(m_SceneData->m_QuadVertexArray);
	}

	void Renderer::SetTextRenderer(const std::shared_ptr<TextRenderer>& textRenderer)
	{
		m_TextRenderer = textRenderer;
	}

	void Renderer::RenderText(glm::vec2 position, std::string text, unsigned int fontSize, const glm::vec4& color) {
		fontSize = fontSize * 10;
		
		if (!m_TextRenderer)
		{
			TRL_CORE_ERROR("Renderer::RenderText called but no TextRenderer is set!");
			return;
		}
		
		auto& fontMap = m_TextRenderer->m_FontSizes;

		// Find the closest available size if the requested one isn't loaded
		if (fontMap.find(fontSize) == fontMap.end())
		{
			//TRL_CORE_WARN("Font size {} not loaded, using closest available size", fontSize);
			auto it = fontMap.lower_bound(fontSize);
			if (it == fontMap.end())
				it = std::prev(fontMap.end());
			fontSize = it->first;
		}

		const auto& characters = fontMap.at(fontSize);

		for (auto c : text)
		{
			const Character& ch = characters.at(c);
			float xpos = (position.x + ch.Bearing.x);
			float ypos = (position.y + (ch.Bearing.y)-(ch.Size.y / 2));//TODO: check if this is not inverted
			float w = ch.Size.x;
			float h = -ch.Size.y;

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { xpos,ypos, 0.0f }) * glm::scale(glm::mat4(1.0f), { w, h, 1.0f });

			m_SceneData->m_TexShader->Bind();
			m_SceneData->m_TexShader->SetUniformMat4f("u_Transform", transform);
			m_SceneData->m_TexShader->SetUniformFloat4("u_Color", color);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			m_SceneData->m_QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(m_SceneData->m_QuadVertexArray);

			position.x += (ch.Advance >> 6);
		}
	}

	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjection", m_SceneData->ProjectionViewMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}