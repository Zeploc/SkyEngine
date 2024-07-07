// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "GraphicsAPI.h"

#include "GL/GLAPI.h"
#include "System/LogManager.h"
#include "Vulkan/VulkanAPI.h"

THardPointer<IGraphicsAPI> IGraphicsAPI::CreateGraphicsAPI(EGraphicsAPI APIType)
{
	THardPointer<IGraphicsAPI> NewGraphicsApi = nullptr;
	switch (APIType)
	{
	case EGraphicsAPI::OPENGL:
		NewGraphicsApi = std::make_shared<CGLAPI>();
		break;
	case EGraphicsAPI::VULKAN:
		NewGraphicsApi = std::make_shared<CVulkanAPI>();
		break;
	case EGraphicsAPI::DIRECTX:
	default:
		// Not implemented
		break;
	}
	
	if (NewGraphicsApi)
	{
		NewGraphicsApi->Init();
	}
	else
	{
		CLogManager::Get()->DisplayError("No valid Graphics API Set!");		
	}
	
	return NewGraphicsApi;
}
