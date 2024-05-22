#pragma once

#ifdef ARMED_PLATFORM_WINDOWS

extern Arm::Application* Arm::createApplication();

int main(int argc, char** argv) {
	ARM_PROFILE_BEGIN_SESSION("Startup", "Armed_Profile_Startup.json");
	Arm::Application* app = Arm::createApplication();
	ARM_PROFILE_END_SESSION();

	ARM_PROFILE_BEGIN_SESSION("Startup", "Armed_Profile_Runtime.json");
	app->run();
	ARM_PROFILE_END_SESSION();
	
	ARM_PROFILE_BEGIN_SESSION("Startup", "Armed_Profile_Shutdown.json");
	delete app;
	ARM_PROFILE_END_SESSION();
}

#endif // ARMED_PLATFORM_WINDOWS
