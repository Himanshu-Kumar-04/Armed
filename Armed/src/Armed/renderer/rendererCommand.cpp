#include "ArmPCH.h"
#include "rendererCommand.h"
#include "Armed/renderer/OpenGL/openglRendererAPI.h"

namespace Arm {
    RendererAPI* RendererCommand::m_RendererAPI = new OpenGLRendererAPI;
}
