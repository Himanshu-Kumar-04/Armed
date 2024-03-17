#pragma once
#include"Armed/event/event.h"

namespace Arm {
	class Layer {
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}