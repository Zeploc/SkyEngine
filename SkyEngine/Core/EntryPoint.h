#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SkyEngine::Application* SkyEngine::CreateApplication();

int main(int argc, char** argv)
{
	const auto App = SkyEngine::CreateApplication();
	int AppExitState = App->Run();
	delete App;
	return AppExitState;
}
#else
	#error SkyEngine only supports Windows!
#endif
		

