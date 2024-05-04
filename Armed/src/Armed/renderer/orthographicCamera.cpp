#include "ArmPCH.h"
#include "orthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arm {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        :m_ProjectionMatrix(glm::ortho(left,right,bottom,top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    const glm::vec3& OrthographicCamera::getPosition(const glm::vec3& position) const {
        return m_Position;
    }
    void OrthographicCamera::setPosition(const glm::vec3& position) {
        m_Position = position;
        recalculateViewMatrix();
    }

    float OrthographicCamera::getRotation(float rotation) const {
        return m_Rotation;
    }
    void OrthographicCamera::setRotation(float rotation) {
        m_Rotation = rotation;
        recalculateViewMatrix();
    }
    glm::mat4& OrthographicCamera::getViewMatrix()
    {
        return m_ViewMatrix;
    }
    glm::mat4& OrthographicCamera::getProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }
    glm::mat4& OrthographicCamera::getViewProjectionMatrix()
    {
        return m_ViewProjectionMatrix;
    }
    void OrthographicCamera::recalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0,0,1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}