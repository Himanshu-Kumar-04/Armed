#pragma once
#include<glm/glm.hpp>

#include"vertexArray.h"

namespace Arm {
    class RendererAPI {
    public:
        enum class API {
            None =0,
            OpenGL
        };
    public:
        virtual void init() = 0;
        virtual void setclearColor(const glm::vec4& color) = 0;
        virtual void clearColor() = 0;

        virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        static API getAPI() { return s_API; }
    private:
        static API s_API;
    };
}