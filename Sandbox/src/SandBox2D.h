#pragma once
#include <Trail.h>
#include "../TetriBoard.h"
class SandBox2D : public Trail::Layer
{
public:
	SandBox2D();
	void OnUpdate(Trail::DeltaTime dt) override;
	void OnEvent(Trail::Event& e) override;
	void OnAttach() override;
	void OnImGuiRender() override;
private:
	Trail::CameraController m_CameraController;
	std::shared_ptr<Trail::Texture> m_BackgroundTexture;
	Trail::SoundDevice* m_SoundDevice;
	uint32_t m_Sound;
	TetriBoard m_Board;
	std::unique_ptr<Trail::SoundSource> m_AudioSource;
};