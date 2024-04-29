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
	}
};

class Sandbox : public Arm::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}