#pragma once

namespace Trail {
	class DeltaTime {
	public:
		DeltaTime(float time = 0.0f) : m_Time(time)
		{

		}

		operator float() const { return m_Time; } //overrides the "float()" operator so that it just returns time when it's cast implicitly or explicitly

		inline float GetSeconds() const { return m_Time;  }
		inline float GetMilliseconds() const { return m_Time * 1000; }
	private:
		float m_Time;
	};
}