#pragma once
#include<glm/glm.hpp>

namespace Arm {
    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& getPosition(const glm::vec3& position) const;
        void setPosition(const glm::vec3& position);

        float getRotation(float rotation) const;
        void setRotation(float rotation);

        glm::mat4& getViewMatrix();
        glm::mat4& getProjectionMatrix();
        glm::mat4& getViewProjectionMatrix();
    private:
        void recalculateViewMatrix();
    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
        float m_Rotation = 0.0f;
    };
}