#include "ArmPCH.h"
#include "openGLRendererAPI.h"
#include <glad.h>

void Arm::OpenGLRendererAPI::setclearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Arm::OpenGLRendererAPI::clearColor()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Arm::OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
