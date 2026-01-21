#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Trail/LayerStack.h"
#include "Trail/Events/ApplicationEvent.h"

#include "Trail/ImGui/ImGuiLayer.h"
#include "Trail/Renderer/Buffer.h"
#include "Trail/Renderer/Shader.h"
#include "Trail/Renderer/Camera.h"
#include "Trail/Core/DeltaTime.h"
namespace Trail {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();//run loop

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack; //the layer stack shares it's lifetime with the Application class
		static Application* s_Instance;

		//uint32_t vertexArray, vertexBuffer, indexBuffer;
		float m_LastFrameTime = 0.0f;
	};

	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();
}

