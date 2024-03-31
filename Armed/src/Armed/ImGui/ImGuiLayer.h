#pragma once
#include"Armed/core/layer.h"

#include"Armed/event/applicationEvent.h"
#include"Armed/event/keyEvent.h"
#include"Armed/event/mouseEvent.h"

namespace Arm {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void begin();
		void end();

		void onAttach();
		void onDetach();
		void onUpdate();

		void getImGuiEvent(bool event) { m_ImGuiEvent = event; }

		void onEvent(Event& event);
	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);
		bool m_ImGuiEvent = true;
	};
}