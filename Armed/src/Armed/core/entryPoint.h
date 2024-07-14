#pragma once

#ifdef ARMED_PLATFORM_WINDOWS

extern Arm::Application* Arm::createApplication();

int main(int argc, char** argv) {
	Arm::Application* app = Arm::createApplication();

	app->run();

	delete app;
}

#endif // ARMED_PLATFORM_WINDOWS
