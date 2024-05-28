#include<armed.h>
#include "gameOfLife.h"
#include "exampleLayer.h"

//---EntryPoint---------
#include "Armed/core/entryPoint.h"


class Sandbox : public Arm::Application {
public:
	Sandbox()
	{
#if 0
		pushLayer(new Sandbox2D());
#else
		pushLayer(new GameOfLife());
#endif
	}
	~Sandbox() {

	}
};

Arm::Application* Arm::createApplication() {
	return new Sandbox();
}