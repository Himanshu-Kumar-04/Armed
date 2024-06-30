#include "ArmPCH.h"
#include "openGLRendererAPI.h"
#include <glad.h>

void Arm::OpenGLRendererAPI::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
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

void Arm::OpenGLRendererAPI::enableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void Arm::OpenGLRendererAPI::disableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

void Arm::OpenGLRendererAPI::drawIndexed(const Ref<VertexBuffer>& vertexBuffer, uint32_t indexCount)
{
    glBindVertexArray(vertexBuffer->getID());
    uint32_t count = indexCount ? indexCount : vertexBuffer->getIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
