#include "ArmPCH.h"
#include "orthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Armed/core/input.h"
#include "Armed/core/keyCodes.h"
#include "Armed/core/mouseCodes.h"

namespace Arm {
    OrthographicCamera::OrthographicCamera(float aspectRatio, bool rotation)
    {
        m_Properties.aspectRatio = aspectRatio;
        m_Properties.projectionMatrix = glm::ortho(-m_Properties.aspectRatio * m_Properties.zoomLevel, m_Properties.aspectRatio * m_Properties.zoomLevel, -m_Properties.zoomLevel, m_Properties.zoomLevel);
        m_Properties.viewProjectionMatrix = m_Properties.projectionMatrix * m_Properties.viewMatrix;
    }
    
    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_Properties.projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_Properties.viewProjectionMatrix = m_Properties.projectionMatrix * m_Properties.viewMatrix;
    }

    const glm::vec3& OrthographicCamera::getPosition() const {
        return m_Properties.position;
    }
    void OrthographicCamera::setPosition(const glm::vec3& position) {
        m_Properties.position = position;
        recalculateViewMatrix();
    }

    float OrthographicCamera::getRotation() const {
        return m_Properties.rotation;
    }
    void OrthographicCamera::setRotation(float rotation) {
        m_Properties.rotation = rotation;
        recalculateViewMatrix();
    }
    void OrthographicCamera::onResize(float width, float height)
    {
        m_Properties.aspectRatio = width / height;
        SetProjection(-m_Properties.aspectRatio * m_Properties.zoomLevel, m_Properties.aspectRatio * m_Properties.zoomLevel, -m_Properties.zoomLevel, m_Properties.zoomLevel);
    }
    const glm::mat4& OrthographicCamera::getViewMatrix() const
    {
        return m_Properties.viewMatrix;
    }
    const glm::mat4& OrthographicCamera::getProjectionMatrix() const
    {
        return m_Properties.projectionMatrix;
    }
    const glm::mat4& OrthographicCamera::getViewProjectionMatrix() const
    {
        return m_Properties.viewProjectionMatrix;
    }
    void OrthographicCamera::onUpdate(Timestep ts)
    {
        if (Input::isKeyPressed(Key::A)) {
            m_Properties.position.x -= cos(m_Properties.rotation) * m_Properties.translationSpeed * ts;
            m_Properties.position.y -= sin(m_Properties.rotation) * m_Properties.translationSpeed * ts;
        }
        else if (Input::isKeyPressed(Key::D)) {
            m_Properties.position.x += cos(m_Properties.rotation) * m_Properties.translationSpeed * ts;
            m_Properties.position.y += sin(m_Properties.rotation) * m_Properties.translationSpeed * ts;
        }

        if (Input::isKeyPressed(Key::W)) {
            m_Properties.position.x += -sin(m_Properties.rotation) * m_Properties.translationSpeed * ts;
            m_Properties.position.y +=  cos(m_Properties.rotation) * m_Properties.translationSpeed * ts;
        }
        else if (Input::isKeyPressed(Key::S)) {
            m_Properties.position.x -= -sin(m_Properties.rotation) * m_Properties.translationSpeed * ts;
            m_Properties.position.y -=  cos(m_Properties.rotation) * m_Properties.translationSpeed * ts;
        }

        if (m_Properties.rotationFlag) {
            if (Input::isKeyPressed(Key::Q))
                m_Properties.rotation -= m_Properties.rotationSpeed * ts;
            else if (Input::isKeyPressed(Key::E))
                m_Properties.rotation += m_Properties.rotationSpeed * ts;

            setRotation(m_Properties.rotation);
        }

        setPosition(m_Properties.position);
        m_Properties.translationSpeed = m_Properties.zoomLevel;
    }
    void OrthographicCamera::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseScrolledEvent>(ARM_BIND_EVENT_FN(OrthographicCamera::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(ARM_BIND_EVENT_FN(OrthographicCamera::onWindowResize));
    }
    const void OrthographicCamera::setZoomLevel(float zoom)
    {
        m_Properties.zoomLevel = zoom;
        SetProjection(-m_Properties.aspectRatio * m_Properties.zoomLevel, m_Properties.aspectRatio * m_Properties.zoomLevel, -m_Properties.zoomLevel, m_Properties.zoomLevel);
    }
    bool OrthographicCamera::onMouseScrolled(MouseScrolledEvent& event)
    {
        m_Properties.zoomLevel -= event.GetYOffset() * 0.25f;
        m_Properties.zoomLevel = std::max(m_Properties.zoomLevel, 0.25f);
        SetProjection(-m_Properties.aspectRatio * m_Properties.zoomLevel, m_Properties.aspectRatio * m_Properties.zoomLevel, -m_Properties.zoomLevel, m_Properties.zoomLevel);
        return false;
    }
    bool OrthographicCamera::onWindowResize(WindowResizeEvent& event)
    {
        onResize((float)event.getWidth(), (float)event.getHeight());
        return false;
    }
    void OrthographicCamera::recalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Properties.position) *
            glm::rotate(glm::mat4(1.0f), m_Properties.rotation, glm::vec3(0,0,1));

        m_Properties.viewMatrix = glm::inverse(transform);
        m_Properties.viewProjectionMatrix = m_Properties.projectionMatrix * m_Properties.viewMatrix;
    }
}