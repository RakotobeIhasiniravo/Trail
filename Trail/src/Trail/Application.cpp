#include "trlpch.h"


#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include "Input.h"
#include "Renderer/Renderer.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
namespace Trail {


	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		TRL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());//why do we need this unique pointer ? ( because we will only have a single window)
		//m_Window->SetEventCallback(TRL_BIND_EVENT_FN(Application::OnEvent)); //this is set up so that when the EventCallback function from the 
		//window class gets called, it automatically calls the OnEvent function, so the only thing we need to do is to set up GLFW call backs from the window
		//and call EventCallback for every type of handled event by GLFW
		m_Window->SetEventCallback([this](Event& e) {Application::OnEvent(e); });
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e)-> bool {return Application::OnWindowClosed(e);});
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e)-> bool {return Application::OnWindowResized(e); });
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) {
				TRL_TRACE("EVENT HANDLED");
				break;
			}
		}
	}
	bool Application::OnWindowClosed(WindowCloseEvent& e) {
		m_Running = false; //since our run loop is dependent on m_Running being true, this function stops the application
		return true;
	}
	bool Application::OnWindowResized(WindowResizeEvent& e) {
		
		TRL_CORE_INFO("win size: {0}, {1}", e.GetWidth(), e.GetHeight());
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());
		return false;
	}
	Application::~Application()
	{
	}
	
	
	void Application::Run() {
		while (m_Running) {
			//accès au temps et calcul du deltatime 
			float time = (float)glfwGetTime();
			DeltaTime deltatime = time - m_LastFrameTime; 
			m_LastFrameTime = time; 
			
			if (!m_Minimized) {
				//mise à jour de chaque Layer du LayerStack
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(deltatime); 
				}
			}
			//mise à jour de la fenêtre et récupération des évènements
			m_Window->OnUpdate();
		}
	}


}

