#pragma once
#include<glm/glm.hpp>
#include "Armed/core/timestep.h"
#include "Armed/event/mouseEvent.h"
#include "Armed/event/applicationEvent.h"

namespace Arm {
    class Camera {
    public:
        Camera(float fieldOfView, float aspectRatio, float nearPoint, float farPoint);
        ~Camera() = default;

        const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        const glm::vec3& getPosition() const { return m_Position; }
        float getRotation() const { return m_Rotation; }

        void setPosition(const glm::vec3& position);
        void setRotation(float rotation);
        void setProjection(float fieldOfView, float aspectRatio, float nearPoint, float farPoint);

        //Controller Functions

        void onUpdate(Timestep ts);
        void onEvent(Event& event);

        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

    private:
        void recalculateViewMatrix();
    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = glm::vec3(0.0f);
        float m_TranslationSpeed = 5.0f;
        float m_Rotation = 0.0f, m_RotationSpeed = 3.1416f;
        float m_FOV;
        float m_AspectRatio;
        float m_NearPoint;
        float m_FarPoint;
        glm::vec3 m_Up = { 0.0f,1.0f,0.0f };
        glm::vec3 m_Orientation = { 0.0f,0.0f,-1.0f };
    };
}