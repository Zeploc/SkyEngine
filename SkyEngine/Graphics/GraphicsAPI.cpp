#include "GraphicsAPI.h"

#include "GLFW/GLFWAPI.h"
#include "System/LogManager.h"

Pointer<IGraphicsAPI> IGraphicsAPI::CreateGraphicsAPI(EGraphicsAPI APIType)
{
	switch (APIType)
	{
	case EGraphicsAPI::GLFW:
		return std::make_shared<IGLFWAPI>();
	case EGraphicsAPI::VULKAN:
		break;
	default: ;
	}
	LogManager::GetInstance()->DisplayLogMessage("No valid Graphics API Set!");
	return nullptr;
}
