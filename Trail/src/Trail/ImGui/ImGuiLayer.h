#pragma once
#include "Trail/Layer.h"
#include "Trail/Events/ApplicationEvent.h"
#include "Trail/Events/KeyEvent.h"
#include "Trail/Events/MouseEvent.h"
#include "Trail/Core.h"
namespace  Trail {
	class ImGuiLayer : public Layer {
	public: 
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}