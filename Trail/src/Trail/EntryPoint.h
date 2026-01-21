#pragma once

#ifdef TRL_PLATFORM_WINDOWS

extern Trail::Application* Trail::CreateApplication();

int main(int argc, char** argv) {

	Trail::Log::Init(); //initailization of the logger to be used, will be moved to a seperate initialiaztion system
	TRL_CORE_WARN("initialize log!");
	printf("Trail Engine");
	auto app = Trail::CreateApplication(); //this just returns a new appplication
	app->Run();//can be ran because Sandbox class inherits Application class from the dll
	delete app;
}
#endif