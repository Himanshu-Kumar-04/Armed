#pragma once

#include "Camera.h"
#include "Armed/core/Timestep.h"
#include "Armed/event/Event.h"
#include "Armed/event/MouseEvent.h"

#include <glm/glm.hpp>

namespace Arm {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float nearClip, float farClip);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		inline float getDistance() const { return m_Distance; }
		inline void setDistance(float distance) { m_Distance = distance; }

		inline void setViewportSize(float width, float height) { m_AspectRatio = width / height; recalculateProjection(); }

		const glm::mat4 getTransform() const;

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;

		const glm::vec3& getPosition() const { return m_Position; }
		glm::quat getOrientation() const;

		float getPitch() const { return m_Pitch; }
		float getYaw() const { return m_Yaw; }
	private:
		void recalculateProjection();

		bool onMouseScroll(MouseScrolledEvent& e);
		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		float zoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;
	};

}