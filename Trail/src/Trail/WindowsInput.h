#pragma once
#include "Trail/Input.h"
namespace Trail {
	class WindowsInput : public Input{
	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};
}