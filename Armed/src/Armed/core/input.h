#pragma once
#include "keyCodes.h"
#include "mouseCodes.h"

#include "glm/glm.hpp"

namespace Arm {

	class Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}