#pragma once
#include "Event.h"
#include <sstream>
namespace Trail {
	class WindowResizeEvent : public Event {
	public: 
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}
			 
		inline unsigned int GetWidth() const { return m_Width; }//search about the use of const in the signature of the method
		inline unsigned int GetHeight() const { return m_Height; }

		std::string GetString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height; //resized to m_Width x m_Height
			return ss.str(); //why use stringstream in ToString method instead of just using a string
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private: 
		unsigned int m_Width, m_Height;
	};
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};



	//to be decided later because Ticks, Updates and Renders are the staple of the execution of an engine, 
	//so it's to be decided if those 3 should be implemented directly into the codebase without the need to call them as events 
	//and dispatch them 
	/*class   AppTickEvent : public Event {
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class   AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class   AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};*/
}