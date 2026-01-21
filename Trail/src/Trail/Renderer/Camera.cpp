#include "trlpch.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Trail/Input.h"
#include "Trail/KeyCodes.h"
namespace Trail {
	Camera::Camera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom,top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::setProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0,0,1));
		//get the position matrix by translating an identity matrix by the position
		//then multiply that by another
		// identity matrix that we rotate by the rotation on the z-axis (x,y,Z)
		m_ViewMatrix = glm::inverse(transform); 
		//since it's a camera, to mimic the movement of the camera
		//when we're supposed to move the camera in one direction, we instead move everything else in the opposite direction
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		//the multiplication of the MVP matrix goes in the opposite side because the effects of matrix multiplication go from right to left
	}
	CameraController::CameraController(float width, float height, bool rotation)
		:m_AspectRatio(width/height), m_Width(width),m_Height(height), m_Camera(-m_Width * m_ZoomLevel / 2, m_Width* m_ZoomLevel / 2, -m_Height * m_ZoomLevel / 2, m_Height* m_ZoomLevel / 2), m_Rotation(rotation)
	{
	}
	void CameraController::OnUpdate(DeltaTime deltatime)
	{/*
		if (Input::isKeyPressed(TRL_KEY_LEFT))
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltatime;
		if (Input::isKeyPressed(TRL_KEY_RIGHT))
			m_CameraPosition.x += m_CameraTranslationSpeed * deltatime;
		if (Input::isKeyPressed(TRL_KEY_DOWN))
			m_CameraPosition.y -= m_CameraTranslationSpeed * deltatime;
		if (Input::isKeyPressed(TRL_KEY_UP))
			m_CameraPosition.y += m_CameraTranslationSpeed * deltatime;

		if (m_Rotation) {
			if (Input::isKeyPressed(TRL_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * deltatime;
			if (Input::isKeyPressed(TRL_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * deltatime;
			m_Camera.SetRotation(m_CameraRotation);
		}
		
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;*/
	}
	void CameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([this](Trail::MouseScrolledEvent& event)-> bool {return OnMouseScrolled(event); });
		dispatcher.Dispatch<WindowResizeEvent>([this](Trail::WindowResizeEvent& event)-> bool {return OnWindowResize(event); });
	}
	bool CameraController::OnMouseScrolled(MouseScrolledEvent event)
	{
		/*m_ZoomLevel -= event.GetYOffset()*0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjection(-m_Width * m_ZoomLevel / 2, m_Width * m_ZoomLevel / 2, -m_Height * m_ZoomLevel / 2, m_Height * m_ZoomLevel / 2);
		*/
		return false;
	}
	bool CameraController::OnWindowResize(WindowResizeEvent event)
	{

		//m_Width = (float)event.GetWidth();
		//m_Height = (float)event.GetHeight();
		//m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		TRL_CORE_INFO("aspect ratio: {0}" , m_AspectRatio);
		m_Camera.setProjection(-m_Width * m_ZoomLevel/2, m_Width * m_ZoomLevel/2, -m_Height * m_ZoomLevel/2, m_Height * m_ZoomLevel/2);
		return false;
	}
	bool CameraController::Zoom(float zoomFactor)
	{
		m_ZoomLevel -= zoomFactor;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjection(
			-m_Width  * m_ZoomLevel / 2,
			 m_Width  * m_ZoomLevel / 2,
			-m_Height * m_ZoomLevel / 2,
			 m_Height * m_ZoomLevel / 2
		);

		return false;
	}
	bool CameraController::Move(float xFactor, float yFactor, float deltatime) {
		m_CameraPosition.x += xFactor * deltatime;
		m_CameraPosition.y += yFactor * deltatime;
		return false;
	}
	bool CameraController::SetPosition(float xPos, float yPos) {
		m_CameraPosition.x = xPos;
		m_CameraPosition.y = yPos;
		return false;
	}
}