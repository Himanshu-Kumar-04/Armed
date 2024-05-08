#include "ArmPCH.h"
#include "openGLRendererAPI.h"
#include <glad.h>

void Arm::OpenGLRendererAPI::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Arm::OpenGLRendererAPI::setclearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Arm::OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void Arm::OpenGLRendererAPI::clearColor()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Arm::OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
