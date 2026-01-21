#pragma once
#include <glm/glm.hpp>
#include "Trail/Core/DeltaTime.h"
#include "Trail/Events/Event.h"
#include "Trail/Events/MouseEvent.h"
#include "Trail/Events/ApplicationEvent.h"
namespace Trail {
	class Camera {
	public:
		Camera(float left, float right, float bottom, float top);
		void setProjection(float left, float right, float bottom, float top);


		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(glm::vec3& position) { m_Position = position; RecalculateViewMatrix();
		}

		const float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix();}

		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position = {0.0f,0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};


	class CameraController {
	public:
		CameraController(float width, float height, bool rotation = false);

		void OnUpdate(DeltaTime deltatime);
		void OnEvent(Event& event);
		inline Camera& GetCamera() { return m_Camera; }
		inline const Camera& GetCamera() const { return m_Camera; }
		bool Zoom(float zoomFactor);
		bool Move(float xFactor, float yFactor, float deltatime);
		bool SetPosition(float xFactor, float yFactor);
	private:
		bool OnMouseScrolled(MouseScrolledEvent event);
		bool OnWindowResize(WindowResizeEvent event);
		
	private:
		float m_AspectRatio = 1920.0f / 1080.0f;//this is a hack to force the window at 1.77778 aspect ratio
		float m_Width, m_Height;
		float m_ZoomLevel = 1.0f;
		Camera m_Camera; 
		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;
		bool m_Rotation;
		float m_CameraTranslationSpeed = 3.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}