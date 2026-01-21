#include "trlpch.h"
#include "WindowsWindow.h"
#include "Trail/Events/ApplicationEvent.h"
#include "Trail/Events/KeyEvent.h"
#include "Trail/Events/MouseEvent.h"
#include <glad/glad.h>
namespace Trail {
	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description) {
		TRL_CORE_ERROR("GLFW ERROR ({0}), {1}", error, description);
	}
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);//returns a pointer that'll be saved as a uniue pointer ?
	}
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}
	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		TRL_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized) {
			//TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			TRL_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TRL_CORE_ASSERT(status, "Failed to initialize Glad");
		glfwSetWindowUserPointer(m_Window, &m_Data); //we need to reference the WindowData struct because it's the only way for us to access that window data inside callbacks
		TRL_INFO("OpenGl GPU : {0} , {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));
		



		//SET GLFW CALLBACKS
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
			
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window,unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event((int)keycode);
			data.EventCallback(event);
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void WindowsWindow::SetVSync(bool enabled) {
		if(enabled) 
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled; 
	}
	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync; 
	}

}