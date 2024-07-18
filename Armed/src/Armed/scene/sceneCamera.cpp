#include "ArmPCH.h"
#include "sceneCamera.h"
#include<glm/gtc/matrix_transform.hpp>

Arm::SceneCamera::SceneCamera()
    :m_ProjectionType(ProjectionType::perspective)
{
    recalculateProjection();
}

void Arm::SceneCamera::setOrthographic(float size, float nearClip, float farClip)
{
    m_ProjectionType = ProjectionType::orthographic;
    m_OrthographicSize = size;
    m_OrthographicNear = nearClip;
    m_OrthographicFar = farClip;

    recalculateProjection();
}

void Arm::SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
{
    m_ProjectionType = ProjectionType::perspective;
    m_PerspectiveVerticalFOV = verticalFOV;
    m_PerspectiveNear = nearClip;
    m_PerspectiveFar = farClip;

    recalculateProjection();
}

void Arm::SceneCamera::setViewportSize(uint32_t width, uint32_t height)
{
    m_AspectRatio = (float)width / (float)height;
    recalculateProjection();
}

float Arm::SceneCamera::getNearClip()
{
    switch (m_ProjectionType)
    {
    case Arm::Camera::ProjectionType::orthographic: return m_OrthographicNear;
    case Arm::Camera::ProjectionType::perspective:  return m_PerspectiveNear;
    }
}

float Arm::SceneCamera::getNearClip(ProjectionType projectionType)
{
    switch (projectionType)
    {
    case Arm::Camera::ProjectionType::orthographic: return m_OrthographicNear;
    case Arm::Camera::ProjectionType::perspective:  return m_PerspectiveNear;
    }
}

float Arm::SceneCamera::getFarClip()
{
    switch (m_ProjectionType)
    {
    case Arm::Camera::ProjectionType::orthographic: return m_OrthographicFar;
    case Arm::Camera::ProjectionType::perspective:  return m_PerspectiveFar;
    }
}

float Arm::SceneCamera::getFarClip(ProjectionType projectionType)
{
    switch (projectionType)
    {
    case Arm::Camera::ProjectionType::orthographic: return m_OrthographicFar;
    case Arm::Camera::ProjectionType::perspective:  return m_PerspectiveFar;
    }
}

void Arm::SceneCamera::setNearClip(float nearClip)
{
    switch (m_ProjectionType)
    {
    case Arm::Camera::ProjectionType::orthographic:
        m_OrthographicNear = nearClip;
        recalculateProjection();
        break;
    case Arm::Camera::ProjectionType::perspective:
        m_PerspectiveNear = nearClip;
        recalculateProjection();
        break;
    }
}

void Arm::SceneCamera::setFarClip(float farClip)
{
    switch (m_ProjectionType)
    {
    case Arm::Camera::ProjectionType::orthographic:
        m_OrthographicFar = farClip;
        recalculateProjection();
        break;
    case Arm::Camera::ProjectionType::perspective:
        m_PerspectiveFar = farClip;
        recalculateProjection();
        break;
    }
}

void Arm::SceneCamera::recalculateProjection()
{
    switch (m_ProjectionType)
    {
    case ProjectionType::orthographic:
        m_Projection = glm::ortho(
            -m_OrthographicSize * m_AspectRatio * 0.5f,
            m_OrthographicSize * m_AspectRatio * 0.5f,
            -m_OrthographicSize * 0.5f,
            m_OrthographicSize * 0.5f,
            m_OrthographicNear,
            m_OrthographicFar);
        break;
    case ProjectionType::perspective:
        m_Projection = glm::perspective(m_PerspectiveVerticalFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        break;
    }
}
