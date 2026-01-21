#include "SandBox2D.h"

SandBox2D::SandBox2D() : m_CameraController(1920.0f , 1080.0f)
{
}

void SandBox2D::OnAttach()
{

	auto font = std::make_shared<Trail::TextRenderer>("Assets/Textures/Roboto.ttf");
	Trail::Renderer::SetTextRenderer(font);
	m_BackgroundTexture.reset(new Trail::Texture("Assets/Textures/tetris.png"));
	m_SoundDevice = Trail::SoundDevice::get();
	m_Sound = Trail::SoundBuffer::get()->addSoundEffect("Assets/Sounds/start.mp3");
	m_AudioSource = std::make_unique<Trail::SoundSource>(32); 

}

void SandBox2D::OnUpdate(Trail::DeltaTime deltatime)
{
	m_CameraController.OnUpdate(deltatime);

	Trail::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Trail::RenderCommand::Clear();

	
	Trail::Renderer::BeginScene(m_CameraController.GetCamera());

	Trail::Renderer::DrawQuad(
		{ 0.0f, 0.0f, -0.1f },
		{ 1980.0f, 1080.0f },
		m_BackgroundTexture
	);
	
	

	/*Trail::Renderer::DrawQuad(
		{ 0.0f, 0.0f, 1.0f},
		{ 1000.0f, 2000.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f }
	);*/
	/*Trail::Renderer::DrawQuad(
		{ 0.0f, 0.0f, -0.1f },
		{ 500.0f, 500.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f }
	);*/

	/*Trail::Renderer::DrawQuad(
		{ 0.0f, 0.0f },
		{ 5.0f, 2000.0f },
		{ 1.0f, 0.3f, 0.1f, 1.0f }
	);*/

	/*Trail::Renderer::RenderText(
		{ 0.0f, 0.0f},
		"MattDevLog",
		500,
		{ 1.0f,1.0f,1.0f,1.0f }
	);*/
	/*Trail::Renderer::DrawQuad(
		{ 0.0f, 0.0f, -0.1f },
		{ 1080.0f, 1080.0f },
		m_Texture
	);*/
	m_Board.OnUpdate(deltatime);

	Trail::Renderer::EndScene();

	if (Trail::Input::isKeyPressed(TRL_KEY_F))
		m_CameraController.Zoom(0.1f);
	if (Trail::Input::isKeyPressed(TRL_KEY_G))
		m_CameraController.Zoom(-0.1f);
	//if (Trail::Input::isKeyPressed(TRL_KEY_H))
	//	m_CameraController.Move(0.0f , -40.0f, deltatime);
	//if(Trail::Input::isKeyPressed(TRL_KEY_ENTER))
	//	m_CameraController.SetPosition(0.0f, 0.0f);



	//music.UpdateBufferStream();
}

void SandBox2D::OnEvent(Trail::Event& e)
{
	
	m_CameraController.OnEvent(e);
	Trail::EventDispatcher dispatcher(e);
	//dispatcher.Dispatch<Trail::MouseScrolledEvent>([this](Trail::MouseScrolledEvent& event)-> bool {return m_CameraController.Zoom(-0.01f); });

}

void SandBox2D::OnImGuiRender()
{
}
