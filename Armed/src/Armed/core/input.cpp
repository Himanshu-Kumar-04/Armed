#include "ArmPCH.h"
#include "input.h"

#include<GLFW/glfw3.h>
#include"Application.h"

namespace Arm {
    bool Input::isKeyPressed(const KeyCode key)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS;
    }

    bool Input::isMouseButtonPressed(const MouseCode button)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::getMousePosition()
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { (float)xPos, (float)yPos };
    }

    float Input::getMouseX()
    {
        return getMousePosition().x;
    }

    float Input::getMouseY()
    {
        return getMousePosition().y;
    }
}
