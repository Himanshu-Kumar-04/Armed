#pragma once
#pragma once

#include "Armed/Core/Layer.h"

#include "Armed/event/applicationEvent.h"
#include "Armed/event/keyEvent.h"
#include "Armed/event/mouseEvent.h"

namespace Arm {
	enum Fonts : uint8_t {
		openSans_bold = 0, openSans_regular,
	};

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;

		void Begin();
		void End();
		void onImGuiRender() override;
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = false;
	};

}