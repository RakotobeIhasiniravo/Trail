#pragma once

#include "Buffer.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "TextRenderer.h"
namespace Trail {
	
	class Renderer {
	public:
		static void BeginScene(Camera& m_Camera);
		static void OnWindowResized(uint32_t width, uint32_t height);
		static void Init();
		static void Shutdown();
		static void EndScene();
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Trail::Texture>& m_Texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Trail::Texture>& m_Texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,const std::shared_ptr<Trail::Texture>& m_Texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Trail::Texture>& m_Texture);
		static void SetTextRenderer(const std::shared_ptr<TextRenderer>& textRenderer);
		static void RenderText(glm::vec2 position, std::string text, unsigned int fontSize, const glm::vec4& color);

		static void Submit(const std::shared_ptr<Shader> shader ,const std::shared_ptr<VertexArray>& vertexArray);
	private:
		struct SceneData {
			glm::mat4 ProjectionViewMatrix;
			std::shared_ptr<VertexArray> m_QuadVertexArray;
			std::shared_ptr<Shader> m_TexShader;
		};
		static SceneData* m_SceneData;
		static std::shared_ptr<TextRenderer> m_TextRenderer;
	}; 
	class RenderCommand {
	public:
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			// RENDER COMMANDS ONLY DO ONE THING, DO NOT PUT MORE STUFF HERE, THIS GETS CALLED EVERY FRAME
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		inline static void  Init() {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
		}
		inline static void  SetClearColor(const glm::vec4& color) {
			glClearColor(color.r, color.g, color.b, color.a);
		}
		inline static void Clear() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			TRL_CORE_TRACE("render command viewport {0}, {1}, {2}, {3}", x, y, width, height);
			glViewport(x, y, width, height);
		}
	};
}