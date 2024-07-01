#include "ArmPCH.h"
#include "RenderCommand.h"
#include "Armed/renderer/OpenGL/openglRendererAPI.h"

namespace Arm {
    Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}
