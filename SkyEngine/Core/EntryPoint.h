#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SkyEngine::Application* SkyEngine::CreateApplication();

void main(int argc, char** argv)
{
	auto App = SkyEngine::CreateApplication();
	App->Run();
	delete App;
}
#else
	#error SkyEngine only supports Windows!
#endif
		

