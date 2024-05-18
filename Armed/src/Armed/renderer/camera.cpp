#include "ArmPCH.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Armed/core/input.h"
#include "Armed/core/keyCodes.h"
#include "Armed/core/mouseCodes.h"

namespace Arm
{
    Camera::Camera(float fieldOfView, float aspectRatio, float nearPoint, float farPoint)
        :m_ProjectionMatrix(glm::perspective(fieldOfView, aspectRatio, nearPoint, farPoint)), m_ViewMatrix(glm::mat4(1.0f)),
        m_NearPoint(nearPoint), m_FarPoint(farPoint),m_AspectRatio(aspectRatio),m_FOV(fieldOfView)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::setPosition(const glm::vec3& position)
    {
        m_Position = position;
        recalculateViewMatrix();
    }

    void Camera::setRotation(float rotation)
    {
        m_Rotation = rotation;
        recalculateViewMatrix();
    }

    void Camera::setProjection(float fieldOfView, float aspectRatio, float nearPoint, float farPoint)
    {
        m_ProjectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearPoint, farPoint);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void Camera::onUpdate(Timestep ts)
    {
        if (Input::isKeyPressed(Key::A)) {
            m_Position += m_TranslationSpeed * ts * -glm::normalize(glm::cross(m_Orientation, m_Up));
        }
        else if (Input::isKeyPressed(Key::D)) {
            m_Position += m_TranslationSpeed * ts * glm::normalize(glm::cross(m_Orientation, m_Up));
        }
        if (Input::isKeyPressed(Key::W)) {
            m_Position += m_TranslationSpeed * ts * m_Orientation;
        }
        else if (Input::isKeyPressed(Key::S)) {
            m_Position += m_TranslationSpeed * ts * -m_Orientation;
        }
        if (Input::isKeyPressed(Key::Enter)) {
            m_Position += m_TranslationSpeed * ts * m_Up;
        }
        else if (Input::isKeyPressed(Key::RightShift)) {
            m_Position += m_TranslationSpeed * ts * -m_Up;
        }

        //TODO: Rotation

        //m_TranslationSpeed = m_FOV;
    }

    void Camera::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(ARM_BIND_EVENT_FN(Camera::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(ARM_BIND_EVENT_FN(Camera::onWindowResize));
    }

    bool Camera::onMouseScrolled(MouseScrolledEvent& event)
    {
        m_FOV += event.GetYOffset() * 0.05f;
        if (m_FOV < 0.209f) m_FOV = 0.209f;
        if (m_FOV > 1.466f) m_FOV = 1.466f;
        setProjection(m_FOV, m_AspectRatio, m_NearPoint, m_FarPoint);
        return false;
    }

    bool Camera::onWindowResize(WindowResizeEvent& event)
    {
        m_AspectRatio = (float)event.getWidth() / (float)event.getHeight();
        setProjection(m_FOV, m_AspectRatio, m_NearPoint, m_FarPoint);
        return false;
    }

    void Camera::recalculateViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_Position, {0.0f,0.0f,-1.0f}, { 0.0f,1.0f,0.0f }) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 1, 0));
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }


}