#include<armed.h>
#include "sandbox2D.h"
#include "exampleLayer.h"

//---EntryPoint---------
#include "Armed/core/entryPoint.h"


class Sandbox : public Arm::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		//pushLayer(new Sandbox2D());
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}