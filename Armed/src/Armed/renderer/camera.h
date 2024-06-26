#pragma once
#include <glm/glm.hpp>

namespace Arm {
    class Camera {
    public:
        enum class ProjectionType : uint8_t {
            orthographic, perspective
        };
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : m_Projection(projection) {}
        const glm::mat4& getProjection() const { return m_Projection; }
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };
}