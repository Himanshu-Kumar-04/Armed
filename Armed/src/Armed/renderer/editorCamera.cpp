#include "ArmPCH.h"
#include "editorCamera.h"

#include "Armed/core/input.h"
#include "Armed/core/keyCodes.h"
#include "Armed/core/mouseCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Arm {
    EditorCamera::EditorCamera(float fov, float nearClip, float farClip)
        : m_FOV(fov), m_NearClip(nearClip), m_FarClip(farClip)
    {
        recalculateProjection();
    }

    void EditorCamera::recalculateProjection()
    {
        m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    float EditorCamera::zoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }

    void EditorCamera::onUpdate(Timestep ts)
    {
        const glm::vec2& mouse{ Input::getMouseX(), Input::getMouseY() };
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;

        if (Input::isMouseButtonPressed(Mouse::ButtonMiddle))
            mousePan(delta);

        if (Input::isKeyPressed(Key::LeftAlt))
        {
            if (Input::isMouseButtonPressed(Mouse::ButtonLeft))
                mouseRotate(delta);
            else if (Input::isMouseButtonPressed(Mouse::ButtonRight))
                mouseZoom(delta.y);
        }

        m_Position = m_FocalPoint - getForwardDirection() * m_Distance;
    }

    void EditorCamera::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrolledEvent>(ARM_BIND_EVENT_FN(EditorCamera::onMouseScroll));
    }

    bool EditorCamera::onMouseScroll(MouseScrolledEvent& e)
    {
        mouseZoom(e.GetYOffset() * 0.1f);
        m_Position = m_FocalPoint - getForwardDirection() * m_Distance;
        return false;
    }

    void EditorCamera::mousePan(const glm::vec2& delta)
    {
        m_FocalPoint += -getRightDirection() * delta.x * std::min(0.3f, m_AspectRatio * 0.3f) * m_Distance;
        m_FocalPoint += getUpDirection() * delta.y * std::min(0.3f, m_AspectRatio * 0.2f) * m_Distance;
    }

    void EditorCamera::mouseRotate(const glm::vec2& delta)
    {
        float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * 0.8f;
        m_Pitch += delta.y * 0.8f;
    }

    void EditorCamera::mouseZoom(float delta)
    {
        m_Distance -= delta * zoomSpeed();
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += getForwardDirection();
            m_Distance = 1.0f;
        }
    }

    const glm::mat4 EditorCamera::getTransform() const
    {
        return glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(getOrientation());
    }

    glm::vec3 EditorCamera::getUpDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getRightDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getForwardDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::getOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }
}