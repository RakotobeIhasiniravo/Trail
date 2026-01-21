#pragma once
#include "../Core.h"
#include <string>
#include <functional>

namespace Trail {
	//Events for now are "blocking" that means that as soon as an event occurs, the execution of the whole progam stops
	//and the event has to be dealt with before the execution of the program continues
	enum class EventType { //holds all the types of events the engine can manage
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, //WindowEvents
		AppTick, AppUpdate, AppRender, //Application Events
		KeyPressed, KeyReleased, KeyTyped, //Key events
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled //Mouse events
	};
	enum EventCategory {//holds in the different categories of events managed by the engine, let's us group up and seperate the events. Uses a bit switch system to hold the values
		//simple enums get directly converted to the underlying type (here int)
		
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput		 = BIT(1),
		EventCategoryKeyboard	 = BIT(2),
		EventCategoryMouse		 = BIT(3),
		EventCategoryMouseButton = BIT(4) 
	};
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;} \
								virtual EventType GetEventType() const override { return GetStaticType();}\
								virtual const char* GetName() const override { return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class   Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string GetString() const { return GetName(); }
		inline bool IsInCategory(EventCategory category) {//uses the AND operator to compare the bits from the checked category and the fethed category from the event
			//if it's not in the category, no bits will be 1 at the same positions so the result will be 0 = False
			return GetCategoryFlags() & static_cast<int>(category);
		}
		bool Handled = false; //way to check if the event has already been handled
	};
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_Event(event) {
		}
		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(*(T*)&m_Event); 
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
	
	inline std::ostream& operator <<(std::ostream& os, const Event& e) { //why inline functions ? what is ostream ?
		return os << e.GetString();
	}
}