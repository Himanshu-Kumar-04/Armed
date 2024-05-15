#include "ArmPCH.h"
#include "orthographicCameraController.h"
#include "Armed/core/input.h"
#include "Armed/core/keyCodes.h"
#include "Armed/core/mouseCodes.h"

Arm::OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
	:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),m_Rotation(rotation)
{
}

void Arm::OrthographicCameraController::onUpdate(Timestep ts)
{
	if (Input::isKeyPressed(Key::A)) {
		m_CameraPosition.x -= cos(m_CameraRotation) * m_CameraMoveSpeed * ts;
		m_CameraPosition.y -= sin(m_CameraRotation) * m_CameraMoveSpeed * ts;
	}
	else if (Input::isKeyPressed(Key::D)) {
		m_CameraPosition.x += cos(m_CameraRotation) * m_CameraMoveSpeed * ts;
		m_CameraPosition.y += sin(m_CameraRotation) * m_CameraMoveSpeed * ts;
	}

	if (Input::isKeyPressed(Key::W)) {
		m_CameraPosition.x += -sin(m_CameraRotation) * m_CameraMoveSpeed * ts;
		m_CameraPosition.y += cos(m_CameraRotation) * m_CameraMoveSpeed * ts;
	}
	else if (Input::isKeyPressed(Key::S)) {
		m_CameraPosition.x -= -sin(m_CameraRotation) * m_CameraMoveSpeed * ts;
		m_CameraPosition.y -= cos(m_CameraRotation) * m_CameraMoveSpeed * ts;
	}

	if (m_Rotation) {
		if (Input::isKeyPressed(Key::Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		else if (Input::isKeyPressed(Key::E))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		m_Camera.setRotation(m_CameraRotation);
	}

	m_Camera.setPosition(m_CameraPosition);
	m_CameraMoveSpeed = m_ZoomLevel;
}

void Arm::OrthographicCameraController::onEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseScrolledEvent>(ARM_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
	dispatcher.dispatch<WindowResizeEvent>(ARM_BIND_EVENT_FN(OrthographicCameraController::onWindowResize));
}

bool Arm::OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& event)
{
	m_ZoomLevel -= event.GetYOffset() * 0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

bool Arm::OrthographicCameraController::onWindowResize(WindowResizeEvent& event)
{
	m_AspectRatio = (float)event.getWidth() / (float)event.getHeight();
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
