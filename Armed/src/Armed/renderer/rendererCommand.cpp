#include "ArmPCH.h"
#include "rendererCommand.h"
#include "Armed/renderer/OpenGL/openglRendererAPI.h"

namespace Arm {
    Scope<RendererAPI> RendererCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}
