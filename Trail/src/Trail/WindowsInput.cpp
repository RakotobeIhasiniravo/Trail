#include "trlpch.h"
#include "Trail/WindowsInput.h"
#include <GLFW/glfw3.h>
#include "Trail/Application.h"
namespace Trail {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::isKeyPressedImpl(int keycode)
	{
		//accès à la fenêtre
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//récupération de l'état d'une touche spécifique
		auto state = glfwGetKey(window, keycode);
		//retourne true si la touche est appuyée
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::isMouseButtonPressedImpl(int button)
	{
		//accès à la fenêtre 
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//récupération de l'état d'un bouton spécifique de la souris
		auto state = glfwGetMouseButton(window, button);
		//retourne true si la touche est appuyée
		return state == GLFW_PRESS;
	}
	float WindowsInput::getMouseXImpl()
	{
		//accès à la fenêtre 
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		//récupération des coordonnées de la souris
		glfwGetCursorPos(window, &xPos, &yPos);
		//retourne la position sur l’axe x
		return (float)xPos;
	}
	float WindowsInput::getMouseYImpl()
	{
		//accès à la fenêtre 
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		//récupération des coordonnées de la souris
		glfwGetCursorPos(window, &xPos, &yPos);
		//retourne la position sur l’axe y
		return  (float)yPos;
	}
}

