#pragma once
#include<glm/glm.hpp>
#include "Armed/core/timestep.h"
#include "Armed/event/mouseEvent.h"
#include "Armed/event/applicationEvent.h"

namespace Arm {
    struct OrthographicCameraProperties {
        float aspectRatio;
        float zoomLevel = 1.0f;
        
        glm::vec3 position = { 0.0f,0.0f,0.0f };
        float translationSpeed = 5.0f;
        
        bool rotationFlag = true;
        float rotation = 0.0f;
        float rotationSpeed = 3.1416f;
        
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix;
        glm::mat4 viewProjectionMatrix;
    };

    class OrthographicCamera {
    public:
        OrthographicCamera(float aspectRatio, bool rotation = true);

        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& getPosition() const;
        void setPosition(const glm::vec3& position);

        float getRotation() const;
        void setRotation(float rotation);

        const glm::mat4& getViewMatrix() const;
        const glm::mat4& getProjectionMatrix() const;
        const glm::mat4& getViewProjectionMatrix() const;

        //controller
        void onUpdate(Timestep ts);
        void onEvent(Event& event);

        const float getZoomLevel() { return m_Properties.zoomLevel; }
        const void setZoomLevel(float zoom);
    
    private:
        bool onMouseScrolled(MouseScrolledEvent& event);
        bool onWindowResize(WindowResizeEvent& event);
        void recalculateViewMatrix();
    private:
        OrthographicCameraProperties m_Properties;
    };
}