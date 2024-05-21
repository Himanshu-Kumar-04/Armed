#include<armed.h>
#include "sandbox2D.h"
#include "exampleLayer.h"

//---EntryPoint---------
#include "Armed/core/entryPoint.h"


class Sandbox : public Arm::Application {
public:
	Sandbox() {
#if 0
		pushLayer(new ExampleLayer());
#else
		pushLayer(new Sandbox2D());
#endif
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}