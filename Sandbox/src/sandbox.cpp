#include<armed.h>
#include"logicSim.h"

//---EntryPoint---------
#include "Armed/core/entryPoint.h"


class Sandbox : public Arm::Application {
public:
	Sandbox()
	{
#if 0
		pushLayer(new Arm::ExampleLayer());
#else
		pushLayer(new LOGIC_SIM());
#endif
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}