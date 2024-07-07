#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SkyEngine::Application* SkyEngine::CreateApplication();

int main(int argc, char** argv)
{
	const auto App = SkyEngine::CreateApplication();
	// THardPointer<SkyEngine::Application> ApplicationLifetimePointer(App);
	std::string ExecutablePath;
	if (argv)
	{
		ExecutablePath = std::string(*argv);
	}
	int AppExitState = App->Run(ExecutablePath);
	delete App;
	return AppExitState;
}
#else
	#error SkyEngine only supports Windows!
#endif
		

