#pragma once
#include "Armed/event/event.h"
#include "Armed/core/timestep.h"

namespace Arm {
	class Layer {
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}