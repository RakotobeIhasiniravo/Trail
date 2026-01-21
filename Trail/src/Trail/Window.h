#pragma once
#include "trlpch.h"
#include "Trail/Core.h"
#include "Trail/Events/Event.h"


namespace Trail {
	struct WindowProps{
		std::string Title;
		unsigned int Height;
		unsigned int Width;

		WindowProps(const std::string& title = "Trail Engine", unsigned int height = 720, unsigned int width = 1280)
			: Title(title), Height(height), Width(width) {}
	};

	class Window{//Interface representing a desktop system based window
	public: 
		using EventCallbackFn = std::function<void(Event&)>;//explain this

		virtual ~Window() {} //destructor
		virtual void OnUpdate() = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps()); // the equal signs specifies the default, we can change the properties
	};
}
