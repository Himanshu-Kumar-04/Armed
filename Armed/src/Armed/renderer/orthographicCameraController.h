#pragma once
#include "orthographicCamera.h"
#include "Armed/core/timestep.h"
#include "Armed/event/mouseEvent.h"
#include "Armed/event/applicationEvent.h"

namespace Arm {
    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void onUpdate(Timestep ts);
        void onEvent(Event& event);

        OrthographicCamera& getCamera() { return m_Camera; }
        const OrthographicCamera& getCamera() const { return m_Camera; }
        const float getZoomLevel() { return m_ZoomLevel; }
        const void setZoomLevel(float zoom) { m_ZoomLevel = zoom; calculateProjection(); }

    private:
        bool onMouseScrolled(MouseScrolledEvent& event);
        bool onWindowResize(WindowResizeEvent& event);

        void calculateProjection();
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_Rotation;
        OrthographicCamera m_Camera;

        glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraMoveSpeed = 5.0f, m_CameraRotationSpeed = 3.1416f;
    };
}