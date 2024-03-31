#include<armed.h>

class ExampleLayer : public Arm::Layer{
public:
	ExampleLayer()
		:Layer("")
	{
	}
	void onUpdate() override
	{
	}
	void onEvent(Arm::Event& event) override
	{
		if (event.getEventType() == Arm::EventType::MouseButtonPressed) {
			Arm::KeyPressedEvent& e = (Arm::KeyPressedEvent&)event;
		std::cout <<  (char)e.GetKeyCode() << '\n';
		}
	}
};

class Sandbox : public Arm::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		pushOverlay(new Arm::ImGuiLayer());
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}