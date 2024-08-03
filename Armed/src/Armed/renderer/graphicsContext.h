#pragma once
#include "Armed/core/core.h"

namespace Arm {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
        virtual void init() = 0;
        virtual void swapBuffers() = 0;
        static Scope<GraphicsContext> Create(void* window);
    };
}