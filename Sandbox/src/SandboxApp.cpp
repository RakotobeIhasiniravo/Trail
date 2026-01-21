#include <Trail.h>
#include "Trail/EntryPoint.h"
#include "imgui/imgui.h"
#include "SandBox2D.h"
//class ExampleLayer : public Trail::Layer {
//public:
//	ExampleLayer()
//		: Layer("Example"), m_CameraController(1920.0f/1080.0f)
//	{
//		float positions[4 * 5] = {
//		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
//			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
//		};
//		unsigned int indeces[] = {
//			0,1,2,2,3,0
//		};
//
//		m_VertexArray.reset(new Trail::VertexArray());
//
//		std::shared_ptr<Trail::VertexBuffer> vertexBuffer;
//		vertexBuffer.reset(new Trail::VertexBuffer(positions, sizeof(positions)));
//		std::shared_ptr<Trail::IndexBuffer> indexBuffer;
//		indexBuffer.reset(new Trail::IndexBuffer(indeces, sizeof(indeces) / sizeof(uint32_t)));
//
//
//		Trail::BufferLayout layout = {
//			{ Trail::ShaderDataType::Float3, "a_Position"},
//			{ Trail::ShaderDataType::Float2, "a_TextureCoords"},
//		};
//		vertexBuffer->SetLayout(layout);
//
//		m_VertexArray->AddVertexBuffer(vertexBuffer);
//		m_VertexArray->SetIndexBuffer(indexBuffer);
//
//		std::string vertexSrc = R"(
//			#version 330 core
//
//			layout(location = 0) in vec3 a_Position;
//			layout(location = 1) in vec2 a_TextureCoords;
//
//			uniform mat4 u_ViewProjection;
//			
//			out vec2 v_TextureCoords;
//
//			void main(){
//				v_TextureCoords = a_TextureCoords;
//				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
//			}	
//		)";
//		std::string fragmentSrc = R"(
//			#version 330 core
//
//			layout(location = 0) out vec4 color;
//
//			in vec2 v_TextureCoords;
//
//			uniform sampler2D u_Texture;
//
//			void main(){
//				
//				color = texture(u_Texture,v_TextureCoords);
//			}	
//		)";
//		m_Shader.reset(new Trail::Shader("assets/Shaders/baseShader.glsl"));
//
//		m_Texture.reset(new Trail::Texture("assets/Textures/KCLogoB.png"));
//		m_RainbowTexture.reset(new Trail::Texture("assets/Textures/rainbow.png"));
//		m_Shader->Bind();
//		m_Shader->SetUniformInt("u_Texture", 0);
//	}
//	void OnUpdate(Trail::DeltaTime deltatime) override {
//		//glClearColor();
//		//glClear(GL_COLOR_BUFFER_BIT);
//		//TRL_TRACE("Delta time: {0}s ({1}ms)", deltatime.GetSeconds(), deltatime.GetMilliseconds());
//		m_CameraController.OnUpdate(deltatime);
//
//		Trail::RenderCommand::SetClearColor({ 0.9f, 0.1f, 0.1f, 1 });
//		Trail::RenderCommand::Clear();
//
//		
//
//		Trail::Renderer::BeginScene(m_CameraController.GetCamera());
//
//		//m_Shader->Bind();//bind and setUniform will both go into Renderer submit as a material: Shader + Uniform
//		////will be stored in mesh
//		//m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
//		
//		m_RainbowTexture->Bind();
//		Trail::Renderer::Submit(m_Shader, m_VertexArray);
//		m_Texture->Bind();
//		Trail::Renderer::Submit(m_Shader, m_VertexArray);
//		
//		Trail::Renderer::EndScene();
//
//
//		//m_VertexArray->Bind();//will get submited
//		//glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
//
//		/*if (Trail::Input::isKeyPressed(TRL_KEY_SPACE))
//			TRL_INFO("SPACE IS PRESSED");*/
//	}
//	void OnEvent(Trail::Event& event) override {
//		m_CameraController.OnEvent(event);
//
//		//TRL_TRACE("{0}", event.GetString());
//		//Trail::EventDispatcher dispatcher(event);
//		//dispatcher.Dispatch<Trail::KeyPressedEvent>([this](Trail::KeyPressedEvent& event)-> bool {return OnKeyPressed(event); });
//	}
//	virtual void OnImGuiRender() override {
//		
//	}
//private:
//	std::shared_ptr<Trail::VertexArray> m_VertexArray;
//	std::shared_ptr<Trail::Shader> m_Shader;
//
//	std::shared_ptr<Trail::Texture> m_Texture; 
//	std::shared_ptr<Trail::Texture> m_RainbowTexture;
//	Trail::CameraController m_CameraController;
//	glm::vec3 m_CameraPosition; 
//};




class Sandbox : public Trail::Application {
private:
public:	
	Sandbox() {
		PushLayer(new ExempleLayer());
	}
	~Sandbox() {

	}
};

Trail::Application* Trail::CreateApplication() {//declaration of the function located in dll/Application.h because of the include
	return new Sandbox();
}