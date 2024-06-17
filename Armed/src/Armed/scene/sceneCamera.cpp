#include "ArmPCH.h"
#include "sceneCamera.h"
#include<glm/gtc/matrix_transform.hpp>

Arm::SceneCamera::SceneCamera()
{
    recalculateProjection();
}

void Arm::SceneCamera::setOrthographic(float size, float nearClip, float farClip)
{
    m_OrthographicSize = size;
    m_OrthographicNear = nearClip;
    m_OrthographicFar = farClip;

    recalculateProjection();
}

void Arm::SceneCamera::setViewportSize(uint32_t width, uint32_t height)
{
    m_AspectRatio = (float)width / (float)height;
    recalculateProjection();
}

void Arm::SceneCamera::recalculateProjection()
{
    m_Projection = glm::ortho(
        -m_OrthographicSize * m_AspectRatio * 0.5f,
        m_OrthographicSize * m_AspectRatio * 0.5f,
        -m_OrthographicSize * 0.5f,
        m_OrthographicSize * 0.5f,
        m_OrthographicNear,
        m_OrthographicFar);
}
