#pragma once
#include "Armed/renderer/camera.h"

namespace Arm {

    class SceneCamera : public Camera {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setOrthographic(float size, float nearClip, float farClip);
        void setPerspective(float verticalFOV, float nearClip, float farClip);
        
        void setViewportSize(uint32_t width, uint32_t height);

        float getPerspectiveVerticalFOV() { return m_PerspectiveVerticalFOV; }
        void setPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveVerticalFOV = verticalFOV; recalculateProjection(); }

        float getOrthographicSize() { return m_OrthographicSize; }
        void setOrthographicSize(float size) { m_OrthographicSize = size; recalculateProjection(); }

        ProjectionType getProjectionType() const { return m_ProjectionType; }
        void setProjectionType(ProjectionType projectionType) { m_ProjectionType = projectionType; recalculateProjection(); }

        float getNearClip();
        float getNearClip(ProjectionType projectionType);
        void setNearClip(float nearClip);

        float getFarClip();
        float getFarClip(ProjectionType projectionType);
        void setFarClip(float farClip);
    private:
        void recalculateProjection();
    private:
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_PerspectiveVerticalFOV = 0.25f;
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_AspectRatio = 0.0f;
        ProjectionType m_ProjectionType;
    };
}