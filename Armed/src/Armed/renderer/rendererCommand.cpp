#include "ArmPCH.h"
#include "rendererCommand.h"
#include "Armed/renderer/OpenGL/openglRendererAPI.h"

namespace Arm {
    RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}
